#include "problem.h"

#include <cmath>

/* Destructor */
problem::~problem() {}

/* Constructor */
problem::problem(dataset_t * ptr_dataset, const int rank, MatrixXd_t * ptr_U0, MatrixXd_t * ptr_V0) {
  // Set the problem rank.
  r = rank;
  
  // Set pointer locations.
  ptr_data = ptr_dataset;
  ptr_U = ptr_U0;
  ptr_Vopt = ptr_V0;
  
  // Resize vectors.
  residual.resize((* ptr_dataset).nnz_total);
  U_QR.resize((* ptr_dataset).n);
  gradient.resize((* ptr_dataset).m * rank);
  
}

/* Compute the objective for current residual */
double problem::cost() {
  return problem::cost(residual);
}

/* Compute the cost for some residual vector */
double problem::cost(const Eigen::VectorXd residual_) {
  // Compute the cost.
  double my_cost = residual_.squaredNorm();
  my_cost = sqrt(my_cost / (double) (* ptr_data).nnz_total);
  
  return my_cost;
}

/* Compute the residual vector for current U and V*(U) */
void problem::compute_residual() {
  problem::compute_residual(* ptr_U, * ptr_Vopt, residual);
}

/* Compute the residual vector for some U and V */
void problem::compute_residual(MatrixXd_t & U, MatrixXd_t & Vopt, Eigen::VectorXd & residual_) {
  
  // Fetch some values and initialize intermediate variables.
  int n = Vopt.rows();
  int nnz_so_far = 0, cur_nnz;
  residual_ = - (* ptr_data).mt;
  
  for (int j = 0; j < n; ++j) {
    // Fetch the current number of nonzero entries.
    cur_nnz = (* ptr_data).nnz[j];
    
    // Compute U_j.
    double _U_j[cur_nnz * r];
    for (int k = 0; k < cur_nnz; ++k) {
      for (int rr = 0; rr < r; ++rr) {
        _U_j[k * r + rr] = (* ptr_U)((* ptr_data).jj[j][k], rr);
      }
    }
    Eigen::Map<MatrixXd_t> U_j(_U_j, cur_nnz, r);
    
    // Update residuals
    residual_.segment(nnz_so_far, cur_nnz) += U_j * (Vopt.row(j).transpose());
    
    // Update the nnz counter.
    nnz_so_far += cur_nnz;
  }
}

/* VarPro inner iteration for current U */
void problem::compute_optimal_V() {
  problem::compute_optimal_V(* ptr_U, * ptr_Vopt, U_QR);
}

/* VarPro inner iteration */
void problem::compute_optimal_V(const MatrixXd_t U, MatrixXd_t & Vopt, std::vector<Eigen::HouseholderQR<MatrixXd_t>> & U_QR_) {
  int n = Vopt.rows();
  int cur_nnz, nnz_so_far = 0;
  
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
    U_QR_[j] = Eigen::HouseholderQR<MatrixXd_t>(Eigen::Map<MatrixXd_t>(_U_j, cur_nnz, r));
    
    // Solve U_j \ mt_j.
    Vopt.row(j) = U_QR_[j].solve((* ptr_data).mt.segment(nnz_so_far, cur_nnz));
    
    // Update nnz index.
    nnz_so_far += cur_nnz;
  }
}

/* Compute gradient for current U and V*(U) */
void problem::compute_gradient() {
  // Initialize variables.
  int n = (* ptr_data).n;
  int cur_nnz, nnz_so_far = 0;
  gradient.setZero(); // reset gradient.
  
  /* Update the gradient vector which is the row-major stack of the grad matrix.
   * Essentially, G = (W .* R) * V, where R is the m-by-n residual matrix. */
  for (int j = 0; j < n; ++j) {
    cur_nnz = (* ptr_data).nnz[j];
    for (int k = 0; k < cur_nnz; ++k) {
      gradient.segment((* ptr_data).jj[j][k] * r, r) += (* ptr_Vopt).row(j) * residual(nnz_so_far + k);
    }
    nnz_so_far += cur_nnz;
  }
}

/* Compute JTJ for current U and V*(U) */
void problem::compute_JTJ() {
  
}
