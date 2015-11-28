#include "optimizer.h"

#include <iomanip>
#include <cmath>

/* Destructor */
optimizer::~optimizer() {}

/* Constructor */
optimizer::optimizer(problem_t * ptr_problem, optimizer_opts_t * ptr_optimizer_opts) {
  ptr_prob = ptr_problem;
  ptr_opts = ptr_optimizer_opts;
  dx.resize(ptr_prob->num_params);
}

void optimizer::print_iter_header() {
  std::cout << std::setw(6) << "Iter";
  std::cout << std::setw(6) << "Eval";
  std::cout << std::setw(12) << "Lambda";
  std::cout << std::setw(16) << "Cost";
  std::cout << std::setw(16) << "Cost change" << std::endl;
  
  std::cout << std::setw(6) << "----";
  std::cout << std::setw(6) << "----";
  std::cout << std::setw(12) << "---------";
  std::cout << std::setw(16) << "-----------";
  std::cout << std::setw(16) << "-------------" << std::endl;
}

void optimizer::print_iter_info(int iter, int eval, double lambda, double cost, double cost_change) {
  std::cout << std::setw(6) << iter;
  std::cout << std::setw(6) << eval;
  std::cout << std::setw(12) << std::setprecision(3) << std::scientific << lambda;
  std::cout << std::setw(16) << std::setprecision(5) << cost;
  std::cout << std::setw(16) << std::setprecision(5) << cost_change << std::endl;
}

/* Compute delta update */
void optimizer::evaluate_dx(double lambda) {
  // Compute dx.
  dx = - (ptr_prob->JTJ + lambda * MatrixXd_t::Identity(ptr_prob->num_params, ptr_prob->num_params)).selfadjointView<Eigen::Upper>().ldlt().solve(ptr_prob->gradient);
}

void optimizer::solve() {
  // Initialize variables.
  double lambda = ptr_opts->lambda;
  double cost_change = 0.0;
  int trial, eval = 0;
  
  // Print iteration header.
  print_iter_header();
  
  ptr_prob->evaluate_residual();
  print_iter_info(0, 0, 0, ptr_prob->cost, cost_change);
  for (int iter = 0; iter < ptr_opts->max_iters; ++iter) {
    ptr_prob->evaluate_gradient();
    ptr_prob->evaluate_JTJ();
    for (trial = 0; trial < ptr_opts->max_trials; ++trial) {
      evaluate_dx(lambda);
      ptr_prob->evaluate_candidate_vars(dx);
      ++eval;
      cost_change = ptr_prob->candidate.cost - ptr_prob->cost;
      if (cost_change < 0) {
        // If the cost decreases, accept the update and increase the trust region size by decreasing the damping factor.
        ptr_prob->accept_candidate_vars();
        lambda /= 10;
        break;
      } else {
        // Otherwise, increase the damping factor (i.e. reduce the trusted region) and try again.
        lambda *= 10;
      }
    }

    if (trial == ptr_opts->max_trials) {
      std::cout << "MAX TRIALS REACHED!" << std::endl;
      break;
    }
    print_iter_info(iter + 1, eval, lambda, ptr_prob->cost, cost_change);
    if (std::abs(cost_change) < ptr_opts->func_tol) {
      std::cout << "FUNCTION TOLERANCE REACHED!" << std::endl;
      break;
    }
  }
}