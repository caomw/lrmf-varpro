#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "config.h"
#include "dataset.h"

typedef struct problem {
  
  /* Destructor */
  ~problem();
  
  /* Constructors */
  problem(dataset_t * dataset, int rank, MatrixXd_t * ptr_U0, MatrixXd_t * ptr_V0);
  
  /* Functions */
  double cost(); // cost function
  double cost(MatrixXd_t & U, MatrixXd_t & V);
  MatrixXd_t optimal_V();  
  MatrixXd_t optimal_V(const MatrixXd_t U);
  
  // Eigen
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  /* Data */
  int r = 0;
  Eigen::VectorXd residual; // residual
  std::vector<Eigen::HouseholderQR<MatrixXd_t>> U_QR;
  
  /* Pointers */
  MatrixXd_t * ptr_U, * ptr_V;
  dataset_t * ptr_data;

} problem_t;

#endif
