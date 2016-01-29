#include "optimizer.h"

#include <iomanip>
#include <cmath>

/* Destructor */
optimizer::~optimizer() {}

/* Constructor */
optimizer::optimizer(problem_t * ptr_problem, optimizer_opts_t * ptr_optimizer_opts, optimizer_summary_t * ptr_optimizer_summary) {
  ptr_prob = ptr_problem;
  ptr_opts = ptr_optimizer_opts;
  ptr_summary = ptr_optimizer_summary;
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

void optimizer::print_summary() {
  const std::string exit_code_string[] = {"Reached function tolerance.",
    "Reached maximum number of trials.", "Reached maximum number of iterations."};
  std::cout << "------------" << std::endl;
  std::cout << std::setw(8) << "Summary" << std::endl;
  std::cout << "------------" << std::endl;
  std::cout << "- Initial cost: " << std::setw(25) << std::scientific << std::setprecision(5) << ptr_summary->initial_cost << std::endl;
  std::cout << "- Initial cost (normalized): " << std::setw(12) << ptr_summary->initial_cost_normalized << std::endl;
  std::cout << "- Final cost: " << std::setw(27) << std::scientific << std::setprecision(5) << ptr_summary->final_cost << std::endl;
  std::cout << "- Final cost (normalized): " << std::setw(14) << ptr_summary->final_cost_normalized << std::endl;
  std::cout << "- Number of iterations: " << std::setw(17) << ptr_summary->num_iters << std::endl;
  std::cout << "- Number of evaluations: " << std::setw(16) << ptr_summary->num_evals << std::endl;
  std::cout << "- Solver duration (ms): " << std::setw(17) << std::fixed << std::setprecision(0) << ptr_summary->solver_duration << std::endl;
  std::cout << "- Exit code: " << exit_code_string[ptr_summary->exit_code] << std::endl;
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
  int iter, trial, eval = 0;
  
  // Print iteration header.
  if (ptr_opts->DISPLAY) {
    print_iter_header();
  }
  
  // Measure the initial time.
  std::chrono::high_resolution_clock::time_point t_init = std::chrono::high_resolution_clock::now();
  
  // Compute the initial cost before obtaining V*(U). i.e. V = V0;
  ptr_prob->evaluate_residual(* ptr_prob->ptr_U, * ptr_prob->ptr_Vopt, ptr_prob->residual, ptr_prob->cost);
  ptr_summary->initial_cost = ptr_prob->cost; // Add the initial cost to the summary.
  cost_change = - ptr_prob->cost;
  if (ptr_opts->DISPLAY) {
    print_iter_info(-1, -1, ptr_prob->cost, 0, ptr_prob->gradient, 0);
  }
  
  // Compute the initial cost after obtaining V*(U). i.e. V*(U0).
  ptr_prob->evaluate_residual();
  cost_change += ptr_prob->cost;
  if (ptr_opts->DISPLAY) {
    print_iter_info(0, 0, ptr_prob->cost, cost_change, ptr_prob->gradient, 0);
  }
  
  for (iter = 0; iter < ptr_opts->max_iter; ++iter) {
    // For each iteration, evaluate VarPro gradient and JTJ.
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
      if (ptr_opts->DISPLAY) {
        std::cout << "Stopping! Reached the maximum number of trials." << std::endl;
      }
      ptr_summary->exit_code = 1;
      break;
    }
    if (ptr_opts->DISPLAY) {
      print_iter_info(iter + 1, eval, ptr_prob->cost, cost_change, ptr_prob->gradient, lambda / 10);
    }
    if (std::abs(cost_change) < ptr_opts->func_tol) {
      if (ptr_opts->DISPLAY) {
        std::cout << "Stopping! Reached below the function tolerance." << std::endl;
      }
      ptr_summary->exit_code = 0;
      break;
    }
    if (iter == ptr_opts->max_iter - 1) {
      if (ptr_opts->DISPLAY)
      {
      std::cout << "Stopping! Reached the maximum number of iterations." << std::endl;
      }
      ptr_summary->exit_code = 2;
    }
  }
  
  // Output summary to the summary struct.
  // Measure the duration.
  std::chrono::high_resolution_clock::time_point t_final = std::chrono::high_resolution_clock::now();
  ptr_summary->solver_duration = (int)std::chrono::duration_cast<std::chrono::milliseconds>(t_final - t_init).count();
  ptr_summary->num_iters = iter + 1;
  ptr_summary->num_evals = eval + 1;
  ptr_summary->initial_cost_normalized = sqrt(2 * ptr_summary->initial_cost / ptr_prob->ptr_data->nnz_total);
  ptr_summary->final_cost = ptr_prob->cost;
  ptr_summary->final_cost_normalized = sqrt(2 * ptr_summary->final_cost / ptr_prob->ptr_data->nnz_total);
}