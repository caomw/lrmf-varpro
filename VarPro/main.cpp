#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include "_everyheader.h"

int main(void) {
  /* Dataset test code */
  int m = 72;
  int n = 319;
  int r = 4;
  std::string M_path = "dino_trimmed_r4_M.bin";
  std::string W_path = "dino_trimmed_r4_W.bin";
  std::string U_path = "dino_trimmed_r4_U0.bin";
  std::string V_path = "dino_trimmed_r4_V0.bin";
  
  MatrixXd_t U(m, r);
//    MatrixXd_t V(n, r);
  MatrixXd_t V = MatrixXd_t::Random(n, r);
  io::read_dense_matrix(U_path, U, m, r);
//    io::read_dense_matrix(V_path, V, n, r);
  //  std::cout << "V0: " << V << std::endl;
  
  dataset_t data(m, n, M_path, W_path);
  
  std::cout << "No. of nonzeros: " << data.nnz_total << std::endl;
  //  std::cout << "No. of elements in the first column: " << data.jj[0].size() << std::endl;
  
  problem_t problem(&data, r, &U, &V);
  
  //  std::cout << "U: " << *(problem.ptr_U) << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  //  problem.cost();
  problem.compute_residual();
  std::cout << "Initial cost: " << problem.cost() << std::endl;
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
  problem.compute_optimal_V();
//    std::cout << "V*: " << std::endl << V << std::endl;
  std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
  problem.compute_residual();
  problem.cost();
  problem.compute_gradient();
  std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
  auto dur_1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  auto dur_2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
  auto dur_3 = std::chrono::duration_cast<std::chrono::microseconds>( t4 - t3 ).count();
  std::cout << "Final cost: " << problem.cost() << std::endl;
  std::cout << "Cost duration: " << dur_1 << std::endl;
  std::cout << "Inner iteration duration: " << dur_2 << std::endl;
  std::cout << "Gradient duration: " << dur_3 << std::endl;

  return 0;
}
