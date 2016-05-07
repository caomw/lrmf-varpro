#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "config.h"
#include "dataset.h"

typedef struct problem_vars_store {
  MatrixXd_t U, V;
  double cost = 0.0;
  Eigen::VectorXd residual;
  std::vector<Eigen::HouseholderQR<MatrixXd_t> > U_QR;
} problem_vars_store_t;

typedef struct problem {
  
  /* Destructor */
  ~problem();
  
  /* Constructors */
  problem(dataset_t * ptr_dataset, const int rank, MatrixXd_t * ptr_U0, MatrixXd_t * ptr_V0);
  
  // Eigen
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  /* Functions required by the optimizer */
  void evaluate_residual(); // wrapper
  void evaluate_gradient();
  void evaluate_JTJ();
  void evaluate_candidate_vars(const Eigen::VectorXd du);
  void accept_candidate_vars();
  
  /* Other functions */
  void evaluate_U_update(MatrixXd_t & U, const Eigen::VectorXd du);
  void evaluate_residual(MatrixXd_t & U, MatrixXd_t & Vopt, Eigen::VectorXd & residual_, double & cost);
  void evaluate_optimal_V();
  void evaluate_optimal_V(const MatrixXd_t U, MatrixXd_t & Vopt, std::vector<Eigen::HouseholderQR<MatrixXd_t> > & U_QR_);
  
  /* Data */
  bool rank_fill = 0;
  bool V_IS_OPTIMAL = false;
  bool RESIDUAL_IS_FOR_OPTIMAL_V = false;
  bool PERFORM_RETRACTION = false;
  
  int r = 0;
  int rw = 2; // Ruhe and Wedin algorithm type. (1 = GN, 2 = approx. GN and 3 = ALS.)
  int num_params = 0;
  
  double cost = 0.0;
  
  Eigen::VectorXd residual; // residual
  Eigen::VectorXd gradient;
  MatrixXd_t JTJ;
  std::vector<Eigen::HouseholderQR<MatrixXd_t> > U_QR;
  
  /* Pointers */
  MatrixXd_t * ptr_U, * ptr_Vopt;
  dataset_t * ptr_data;
  problem_vars_store_t candidate;
  
} problem_t;

#endif
