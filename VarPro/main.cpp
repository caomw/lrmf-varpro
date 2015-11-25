#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include "_everyheader.h"

int main(void) {
  //  // double[] x;
  //  Eigen::MatrixXd m(2,2);
  //  m(0,0) = 3;
  //  m(1,0) = 2.5;
  //  m(0,1) = -1;
  //  m(1,1) = m(1,0) + m(0,1);
  //  std::cout << m << std::endl;
  //  //  Problem<2> x;
  //  int N = 3;
  //  //  double x[N];
  //  Eigen::MatrixXd *UQ = new Eigen::MatrixXd[N];
  //  for (int i = 0; i < N; ++i) {
  //    UQ[i] = Eigen::MatrixXd::Random(3,3);
  //  }
  //  for (int j = 0; j < N; ++j) {
  //    std::cout << UQ[j] << std::endl;
  //  }
  
  /* Dataset test code */
  int m = 72;
  int n = 319;
  int r = 4;
  std::string M_path = "dino_trimmed_r4_M.bin";
  std::string W_path = "dino_trimmed_r4_W.bin";
  std::string U_path = "dino_trimmed_r4_U0.bin";
  std::string V_path = "dino_trimmed_r4_V0.bin";
  
  MatrixXd_t U(m, r);
  MatrixXd_t V(n, r);
  io::read_dense_matrix(U_path, U, m, r);
  io::read_dense_matrix(V_path, V, n, r);
  
  dataset_t data(m, n, M_path, W_path);
  
  std::cout << "No. of nonzeros: " << data.nnz_total << std::endl;
  //  std::cout << "No. of elements in the first column: " << data.jj[0].size() << std::endl;
  
  problem_t problem(&data, r, &U, &V);
  
  //  std::cout << "U: " << *(problem.ptr_U) << std::endl;
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  problem.cost();
//  std::cout << "Cost: " << problem.cost() << std::endl;
  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//  std::cout << "V*: " << std::endl << problem.optimal_inner_matrix(U) << std::endl;
  problem.optimal_V(U);
  std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
  auto dur_1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  auto dur_2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
  std::cout << "Cost duration: " << dur_1 << std::endl;
  std::cout << "Cost duration: " << dur_2 << std::endl;
  //  std::cout << "Mt last: " << data.mt[5301] << std::endl;
  return 0;
}
