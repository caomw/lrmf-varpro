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
  print_iter_header_bar();
  
  std::cout << std::setw(8) << "Iter";
  std::cout << std::setw(8) << "Eval";
  std::cout << std::setw(14) << "Cost";
  std::cout << std::setw(14) << "Cost change";
  std::cout << std::setw(14) << "|Gradient|";
  std::cout << std::setw(14) << "Lambda" << std::endl;
  
  print_iter_header_bar();
}

void optimizer::print_iter_header_bar() {
  std::cout << std::setw(8) << "------";
  std::cout << std::setw(8) << "------";
  std::cout << std::setw(14) << "------------";
  std::cout << std::setw(14) << "------------";
  std::cout << std::setw(14) << "------------";
  std::cout << std::setw(14) << "------------" << std::endl;
}

void optimizer::print_iter_info(int iter, int eval, double cost, double cost_change, Eigen::VectorXd gradient, double lambda) {
  std::cout << std::setw(8) << iter;
  std::cout << std::setw(8) << eval;
  std::cout << std::setw(14) << std::scientific << std::setprecision(5) << cost;
  std::cout << std::setw(14) << std::setprecision(2) << cost_change;
  std::cout << std::setw(14) << std::setprecision(2) << gradient.norm();
  std::cout << std::setw(14) << std::setprecision(2) << std::scientific << lambda <<std::endl;
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
  
  // Measure the initial time.
  std::chrono::high_resolution_clock::time_point t_init = std::chrono::high_resolution_clock::now();
  
  ptr_prob->evaluate_residual();
  print_iter_info(0, 0, ptr_prob->cost, cost_change, ptr_prob->gradient, lambda);
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
    print_iter_info(iter + 1, eval, ptr_prob->cost, cost_change, ptr_prob->gradient, lambda);
    if (std::abs(cost_change) < ptr_opts->func_tol) {
      std::cout << "FUNCTION TOLERANCE REACHED!" << std::endl;
      break;
    }
  }
  
  // Measure the duration.
  std::chrono::high_resolution_clock::time_point t_final = std::chrono::high_resolution_clock::now();
  auto t_diff = std::chrono::duration_cast<std::chrono::milliseconds>(t_final - t_init).count();
  std::cout << "Solver duration (ms): " << t_diff << std::endl;
}