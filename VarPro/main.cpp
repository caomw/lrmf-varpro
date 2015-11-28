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
  
  MatrixXd_t U(m, r), V(n, r);
  //    MatrixXd_t V(n, r);
  //  MatrixXd_t V = MatrixXd_t::Random(n, r);
  io::read_dense_matrix(U_path, U, m, r);
  io::read_dense_matrix(V_path, V, n, r);
  
  dataset_t data(m, n, M_path, W_path);
  problem_t problem(&data, r, &U, &V);
  optimizer_opts_t opts;
  optimizer_t optimizer(&problem, &opts);
  
  optimizer.solve();
  
  return 0;
}
