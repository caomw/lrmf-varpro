#include "problem.h"

#include <cmath>

/* Destructor */
problem::~problem() {}

/* Constructor */
problem::problem(dataset_t * ptr_dataset, const int rank, MatrixXd_t * ptr_U0, MatrixXd_t * ptr_V0) {
  // Set the problem rank.
  r = rank;
  num_params = ptr_dataset->m * rank;
  
  // Set pointer locations.
  ptr_data = ptr_dataset;
  ptr_U = ptr_U0;
  ptr_Vopt = ptr_V0;
  
  // Resize vectors and the JTJ matirx.
  residual.resize(ptr_dataset->nnz_total);
  U_QR.resize(ptr_dataset->n);
  gradient.resize(num_params);
  JTJ.resize(num_params, num_params);
  
  // Resize candidate vectors and the JTJ matrix.
  candidate.U.resize(ptr_dataset->m, rank);
  candidate.V.resize(ptr_dataset->n, rank);
  candidate.residual.resize(ptr_dataset->nnz_total);
  candidate.U_QR.resize(ptr_dataset->n);
  
  // Retract U if required.
  if (PERFORM_RETRACTION) {
    evaluate_U_update(* ptr_U, Eigen::VectorXd::Zero(num_params));
  }
}

// Evaluate U + dU.
void problem::evaluate_U_update(MatrixXd_t & U, const Eigen::VectorXd du) {
  for (int i = 0; i < ptr_data->m; ++i) {
    U.row(i) += du.segment(i * r, r);
  }
  if (PERFORM_RETRACTION) {
    // Take the q-factor if performing retraction.
    U = Eigen::HouseholderQR<MatrixXd_t>(U).householderQ() * MatrixXd_t::Identity(ptr_data->m, r);
  }
}

// Evaluate candidate variables.
void problem::evaluate_candidate_vars(const Eigen::VectorXd du) {
  // Evaluate U + dU.
  candidate.U = (* ptr_U);
  evaluate_U_update(candidate.U, du);
  
  // Evaluate V*(U + dU).
  evaluate_optimal_V(candidate.U, candidate.V, candidate.U_QR);
  evaluate_residual(candidate.U, candidate.V, candidate.residual, candidate.cost);
}

// Accept candidate variables.
void problem::accept_candidate_vars() {
  (* ptr_U) = candidate.U;
  (* ptr_Vopt) = candidate.V;
  U_QR = candidate.U_QR;
  residual = candidate.residual;
  cost = candidate.cost;
}

/* Evaluate the residual vector for current U and V*(U) */
void problem::evaluate_residual() {
  // If V is not optimal, evaluate optimal V first.
  if (!V_IS_OPTIMAL) {
    evaluate_optimal_V();
    V_IS_OPTIMAL = true;
  }
  problem::evaluate_residual(* ptr_U, * ptr_Vopt, residual, cost);
  RESIDUAL_IS_FOR_OPTIMAL_V = true;
}

/* Evaluate the residual vector for some U and V */
void problem::evaluate_residual(MatrixXd_t & U, MatrixXd_t & Vopt, Eigen::VectorXd & residual_, double & cost) {
  
  // Fetch some values and initialize intermediate variables.
  int n = Vopt.rows();
  int nnz_so_far = 0, cur_nnz;
  residual_ = - ptr_data->mt;
  
  for (int j = 0; j < n; ++j) {
    // Fetch the current number of nonzero entries.
    cur_nnz = ptr_data->nnz[j];
    
    // evaluate U_j.
    double _U_j[cur_nnz * r];
    for (int k = 0; k < cur_nnz; ++k) {
      for (int rr = 0; rr < r; ++rr) {
        _U_j[k * r + rr] = U(ptr_data->jj[j][k], rr);
      }
    }
    Eigen::Map<MatrixXd_t> U_j(_U_j, cur_nnz, r);
    
    // Update residuals
    residual_.segment(nnz_so_far, cur_nnz) += U_j * Vopt.row(j).transpose();
    
    // Update the nnz counter.
    nnz_so_far += cur_nnz;
  }
  cost = residual_.squaredNorm() / 2;

//  cost = sqrt(residual_.squaredNorm() / (double) ptr_data->nnz_total);
}

/* VarPro inner iteration for current U */
void problem::evaluate_optimal_V() {
  problem::evaluate_optimal_V(* ptr_U, * ptr_Vopt, U_QR);
  V_IS_OPTIMAL = true;
}

/* VarPro inner iteration */
void problem::evaluate_optimal_V(const MatrixXd_t U, MatrixXd_t & Vopt, std::vector<Eigen::HouseholderQR<MatrixXd_t>> & U_QR_) {
  int n = Vopt.rows();
  int cur_nnz, nnz_so_far = 0;
  
  for (int j = 0; j < n; ++j) {
    // Find current nnz.
    cur_nnz = ptr_data->nnz[j];
    
    // Generate U_j.
    double _U_j[cur_nnz * r];
    for (int k = 0; k < cur_nnz; ++k) {
      for (int rr = 0; rr < r; ++rr) {
        _U_j[k * r + rr] = U(ptr_data->jj[j][k], rr);
      }
    }
    
    // QR-decompose U_j.
    U_QR_[j] = Eigen::HouseholderQR<MatrixXd_t>(Eigen::Map<MatrixXd_t>(_U_j, cur_nnz, r));
    
    // Solve U_j \ mt_j.
    Vopt.row(j) = U_QR_[j].solve(ptr_data->mt.segment(nnz_so_far, cur_nnz));
    
    // Update nnz index.
    nnz_so_far += cur_nnz;
  }
}

/* evaluate gradient for current U and V*(U) */
void problem::evaluate_gradient() {
  if (!RESIDUAL_IS_FOR_OPTIMAL_V) {
    problem::evaluate_residual();
  } else if (!V_IS_OPTIMAL) {
    evaluate_optimal_V();
    V_IS_OPTIMAL = true;
  }
  
  // Initialize variables.
  int n = ptr_data->n;
  int cur_nnz, nnz_so_far = 0;
  gradient.setZero(); // reset gradient.
  
  /* Update the gradient vector which is the row-major stack of the grad matrix.
   * Essentially, G = (W .* R) * V, where R is the m-by-n residual matrix. */
  for (int j = 0; j < n; ++j) {
    cur_nnz = ptr_data->nnz[j];
    for (int k = 0; k < cur_nnz; ++k) {
      gradient.segment(ptr_data->jj[j][k] * r, r) += ptr_Vopt->row(j) * residual(nnz_so_far + k);
    }
    nnz_so_far += cur_nnz;
  }
}

/* evaluate JTJ for current U and V*(U). */
void problem::evaluate_JTJ() {
  if (!V_IS_OPTIMAL) {
    evaluate_optimal_V();
    V_IS_OPTIMAL = true;
  }
  
  // Initialize variables.
  int n = ptr_data->n;
  int block_idx, cur_nnz;
  MatrixXd_t VTV_j(r, r);
  JTJ.setZero(); // reset JTJ.
  
  if (rw == 3) {
    // evaluate RW3 approximation of JTJ.
    for (int j = 0; j < n; ++j) {
      cur_nnz = ptr_data->nnz[j];
      VTV_j.triangularView<Eigen::Upper>() = ptr_Vopt->row(j).transpose() * ptr_Vopt->row(j);
      for (int k = 0; k < cur_nnz; ++k) {
        block_idx = ptr_data->jj[j][k] * r;
        JTJ.block(block_idx, block_idx, r, r).triangularView<Eigen::Upper>() += VTV_j;
      }
    }
  } else if (rw == 2) {
    // evaluate RW2 approximation of JTJ.
    for (int j = 0; j < n; ++j) {
      cur_nnz = ptr_data->nnz[j];
      
      // evaluate I - UQ_j * UQ_j'.
      MatrixXd_t UQTUQ_j(MatrixXd_t::Identity(cur_nnz, cur_nnz)), thinQ(MatrixXd_t::Identity(cur_nnz, r));
      MatrixXd_t UQ_j = U_QR[j].householderQ() * thinQ;
      UQTUQ_j.triangularView<Eigen::Upper>() -= UQ_j * UQ_j.transpose();
      
      // evaluate v_j * v_j'.
      VTV_j.triangularView<Eigen::Upper>() = ptr_Vopt->row(j).transpose() * ptr_Vopt->row(j);
      
      // Fill the upper half of JTJ.
      for (int k = 0; k < cur_nnz; ++k) {
        block_idx = ptr_data->jj[j][k] * r;
        for (int l = k; l < cur_nnz; ++l) {
          // Fill the upper bits of each upper JTJ sub-block.
          JTJ.block(block_idx, ptr_data->jj[j][l] * r, r, r).triangularView<Eigen::Upper>() += VTV_j * UQTUQ_j(k, l);
          // Replicate to lower bits of each upper JTJ sub-block.
          JTJ.block(block_idx, ptr_data->jj[j][l] * r, r, r) = JTJ.block(block_idx, ptr_data->jj[j][l] * r, r, r).selfadjointView<Eigen::Upper>();
        }
      }
    }
  }
}
