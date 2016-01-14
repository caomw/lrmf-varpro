#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_

#include "config.h"
#include "problem.h"

#include <chrono>

// Optimizer options
typedef struct optimizer_opts {
  int max_iter = 300;
  int max_trials = 50;
  double lambda = 1.0e-1;
  double lambda_max = 1.0e+14;
  double lambda_min = 1.0e-14;
  double lambda_inc_factor = 10.0;
  double lambda_dec_factor = 10.0;
  double func_tol = 1.0e-9;
  bool DISPLAY = true; // display iters info.
} optimizer_opts_t;

typedef struct optimizer_summary {
  int num_iters = -1;
  int num_evals = -1;
  int exit_code = -1; // (0: function tolerance reached, 1: max number of inner trials reached)
  double final_cost = 0.0;
  double final_cost_normalized = 0.0;
  double solver_duration = 0.0;
  // Eigen
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
//  MatrixXd_t U;
//  MatrixXd_t V;
} optimizer_summary_t;

typedef struct optimizer {
  
  /* Destructor */
  ~optimizer();
  
  /* Constructors */
  optimizer(problem_t * ptr_problem, optimizer_opts_t * ptr_optimizer_opts, optimizer_summary_t * ptr_optimizer_summary);
  
  /* Functions */
  void evaluate_dx(double lambda);
  void solve();
  void print_iter_header();
  void print_iter_header_bar();
  void print_iter_info(int iter, int eval, double cost, double cost_change, Eigen::VectorXd gradient, double lambda);
  void print_summary();

  /* Pointer */
  problem_t * ptr_prob;
  optimizer_opts_t * ptr_opts;
  optimizer_summary_t * ptr_summary;
  
  // Eigen
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  /* Variables */
  Eigen::VectorXd dx;
  
} optimizer_t;

#endif
