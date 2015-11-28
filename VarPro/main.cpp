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
  std::string U_path = "dino_trimmed_r4_U.bin";
  std::string V_path = "dino_trimmed_r4_V.bin";
  MatrixXd_t U(m, r), V(n, r);
  io::read_dense_matrix(U_path, U, m, r);
  io::read_dense_matrix(V_path, V, n, r);
  
  // Set the dataset.
  dataset_t data(m, n, M_path, W_path);
  
  // Initialize the problem.
  problem_t problem(&data, r, &U, &V);
  
  // Set optimizer options.
  optimizer_opts_t opts;
  opts.max_iters = 300;
  
  // Run the optimizer.
  optimizer_t optimizer(&problem, &opts);
  optimizer.solve();
  
  return 0;
}
