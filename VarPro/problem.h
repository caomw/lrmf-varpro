#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "config.h"
#include "dataset.h"

typedef struct problem {
  
  /* Destructor */
  ~problem();
  
  /* Constructors */
  problem(dataset_t * ptr_dataset, const int rank, MatrixXd_t * ptr_U0, MatrixXd_t * ptr_V0);
  
  /* Functions */
  double cost(); // cost function
  double cost(const Eigen::VectorXd residual_);
  void compute_residual();
  void compute_residual(MatrixXd_t & U, MatrixXd_t & Vopt, Eigen::VectorXd & residual_);
  void compute_gradient();
  void compute_optimal_V();
  void compute_optimal_V(const MatrixXd_t U, MatrixXd_t & Vopt, std::vector<Eigen::HouseholderQR<MatrixXd_t>> & U_QR_);
  void compute_JTJ();
  
  
  // Eigen
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  /* Data */
  int r = 0;
  int rw = 2;
  bool rank_fill = 0;
  Eigen::VectorXd residual; // residual
  Eigen::VectorXd gradient;
  Eigen::VectorXd JTJ;
  std::vector<Eigen::HouseholderQR<MatrixXd_t>> U_QR;
  
  /* Pointers */
  MatrixXd_t * ptr_U, * ptr_Vopt;
  dataset_t * ptr_data;
  
} problem_t;

#endif
