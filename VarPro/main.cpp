#include <iostream>
#include <Eigen/Dense>
#include "_everyheader.h"

int main(int argc, char* argv[]) {
  
  // Declare experiment options variable.
//  optimizer_opts_t opts;
  options_t general_opts;
  optimizer_opts_t optimizer_opts;
  
  // Parse experiment options from input arguments.
  parse_options_from_args(argc, argv, general_opts, optimizer_opts);
  
  // If user has not called for help, run the experiment.
  if (!general_opts.HELP) {
    run_experiment(general_opts, optimizer_opts);
  }
  
  return 0;
}

//
//int main(void) {
//  /* Dataset test code */
//  int m = 72;
//  int n = 319;
//  int r = 4;
//  std::string M_path = "dino_trimmed_r4_M.bin";
//  std::string W_path = "dino_trimmed_r4_W.bin";
//  std::string U_path = "dino_trimmed_r4_U.bin";
//  std::string V_path = "dino_trimmed_r4_V.bin";
//  MatrixXd_t U(m, r), V(n, r);
//  io::read_dense_matrix(U_path, U, m, r);
//  io::read_dense_matrix(V_path, V, n, r);
//  
//  // Set the dataset.
//  dataset_t data(m, n, M_path, W_path);
//  
//  // Initialize the problem.
//  problem_t problem(&data, r, &U, &V);
//  
//  // Set optimizer options.

//  opts.max_iters = 300;
//  
//  // Run the optimizer.
//  optimizer_t optimizer(&problem, &opts);
//  optimizer.solve();
//  
//  return 0;
//}
