#include "optimizer.h"

#include <cmath>

/* Destructor */
optimizer::~optimizer() {}

/* Constructor */
optimizer::optimizer(problem_t * ptr_problem, optimizer_opts_t * ptr_optimizer_opts) {
  ptr_prob = ptr_problem;
  ptr_opts = ptr_optimizer_opts;
  lambda = ptr_opts->lambda;
  dx.resize(ptr_prob->num_params);
}

/* Compute delta update */
void optimizer::evaluate_dx() {
  // Compute dx.
  dx = - (ptr_prob->JTJ + lambda * MatrixXd_t::Identity(ptr_prob->num_params, ptr_prob->num_params)).selfadjointView<Eigen::Upper>().ldlt().solve(ptr_prob->gradient);
}

void optimizer::solve() {
  //  ptr_prob->cost()
  std::cout << "[ITER][EVAL] <COST> <LAMBDA> <COST DECREASE>" << std::endl;
  double cost_change = 0.0;
//  ptr_opts->max_iters = 1;
//  ptr_opts->max_trials = 1;
  int trial;
  
  ptr_prob->evaluate_residual();
  std::cout << "[0][0] " << ptr_prob->cost << " " << lambda << std::endl;
  for (int iter = 0; iter < ptr_opts->max_iters; ++iter) {
    ptr_prob->evaluate_gradient();
    ptr_prob->evaluate_JTJ();
    for (trial = 0; trial < ptr_opts->max_trials; ++trial) {
      evaluate_dx();
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
    std::cout << "[" << iter + 1 << "][" << eval << "] " << ptr_prob->cost << " " << lambda << " " << - cost_change << std::endl;
    if (std::abs(cost_change) < ptr_opts->func_tol) {
      std::cout << "FUNCTION TOLERANCE REACHED!" << std::endl;
      break;
    }
  }
}