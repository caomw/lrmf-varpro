#include "problem.h"

#include <cmath>

/* Destructor */
problem::~problem() {}

problem::problem(dataset_t * ptr_dataset, int rank, MatrixXd_t * ptr_U0, MatrixXd_t * ptr_V0) {
  ptr_data = ptr_dataset;
  r = rank;
  ptr_U = ptr_U0;
  ptr_V = ptr_V0;
  residual.resize((* ptr_dataset).nnz_total);
  U_QR.resize((* ptr_dataset).n);
}

MatrixXd_t problem::optimal_V() {
  return problem::optimal_V(* ptr_U);
}

MatrixXd_t problem::optimal_V(const MatrixXd_t U) {
  int n = (* ptr_data).n;
  int cur_nnz, nnz_so_far = 0;
  
  MatrixXd_t V(n, r);
  
  for (int j = 0; j < n; ++j) {
    // Find current nnz.
    cur_nnz = (* ptr_data).nnz[j];
    
    // Generate U_j.
    double _U_j[cur_nnz * r];
    for (int k = 0; k < cur_nnz; ++k) {
      for (int rr = 0; rr < r; ++rr) {
        _U_j[k * r + rr] = (* ptr_U)((* ptr_data).jj[j][k], rr);
      }
    }
    
    // QR-decompose U_j.
    U_QR[j] = Eigen::HouseholderQR<MatrixXd_t>(Eigen::Map<MatrixXd_t>(_U_j, cur_nnz, r));
    
    // Solve U_j \ mt_j.
    V.row(j) = U_QR[j].solve((* ptr_data).mt.segment(nnz_so_far, cur_nnz));
    
    // Update nnz index.
    nnz_so_far += cur_nnz;
  }
  
  return V;
//  return qr.householderQ() * MatrixXd_t::Identity(U.rows(), U.cols());
}

/* Compute the cost */
double problem::cost() {
  return problem::cost(* ptr_U, * ptr_V);
}

double problem::cost(MatrixXd_t & U, MatrixXd_t & V) {
  
  // Fetch some values and initialize intermediate variables.
  int n = V.rows();
  int nnz_so_far = 0, cur_nnz;
  
  for (int j = 0; j < n; ++j) {
    // Fetch the current number of nonzero entries.
    cur_nnz = (* ptr_data).nnz[j];
    
    // Compute U_j.
//    MatrixXd_t Wt_j = MatrixXd_t::Zero(cur_nnz, U.rows());
    
    // Compute U_j.
    double _U_j[cur_nnz * r];
    for (int k = 0; k < cur_nnz; ++k) {
      for (int rr = 0; rr < r; ++rr) {
        _U_j[k * r + rr] = (* ptr_U)((* ptr_data).jj[j][k], rr);
      }
    }
    Eigen::Map<MatrixXd_t> U_j(_U_j, cur_nnz, r);

    // Update residuals
    residual.segment(nnz_so_far, cur_nnz) = U_j * (V.row(j).transpose()) - (* ptr_data).mt.segment(nnz_so_far, cur_nnz);
    nnz_so_far += cur_nnz;
  }
  double my_cost = residual.squaredNorm();
  my_cost = sqrt(my_cost / (double) (* ptr_data).nnz_total);
  
  return my_cost;
}