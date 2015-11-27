#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_

//#include "config.h"
#include "problem.h"

// Optimizer options
typedef struct optimizer_opts {
  int max_iters = 300;
  int max_trials = 50;
  double lambda = 1.0e-1;
  double lambda_max = 1.0e+14;
  double lambda_min = 1.0e-14;
  double lambda_inc_factor = 10.0;
  double lambda_dec_factor = 10.0;
  double func_tol = 1.0e-9;
} optimizer_opts_t;

typedef struct optimizer {
  
  /* Destructor */
  ~optimizer();
  
  /* Constructors */
  optimizer(problem_t * ptr_problem, optimizer_opts_t * ptr_optimizer_opts);
  
  /* Functions */
  void evaluate_dx();
  void solve();

  /* Pointer */
  problem_t * ptr_prob;
  optimizer_opts_t * ptr_opts;
  
  // Eigen
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  /* Variables */
  Eigen::VectorXd dx;
  int eval = 0, iter = 0;
  double lambda = 0;
  
} optimizer_t;

#endif
