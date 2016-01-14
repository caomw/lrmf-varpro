#include "dataset.h"
#include "experiment.h"
#include "io.h"
#include "problem.h"

void run_experiment(options_t general_opts, optimizer_opts_t optimizer_opts) {
  
  // Declare local variables for frequently-used variables.
  int m = general_opts.m;                                        // Number of rows
  int n = general_opts.n;                                        // Number of columns
  int r = general_opts.r;                                        // Rank
  
  // Initialize separable matrix pointers with adequate size.
  MatrixXd_t U(m, r), V(n, r);
  
  // Set sample prefix.
  std::string sample_prefix = general_opts.folder + "/" + general_opts.dataset + "_r" + std::to_string(r);
  
  // Initialize matrices.
  dataset_t data(m, n, sample_prefix + "_M.bin", sample_prefix + "_W.bin");
  
  // Read matrix from the binary files.
  io::read_dense_matrix(sample_prefix + "_U0.bin", U, m, r);
  io::read_dense_matrix(sample_prefix + "_V0.bin", V, n, r);
  
  // Initialize the problem.
  problem_t problem(&data, r, &U, &V);

  // Declare the optimizer summary.
  optimizer_summary_t optimizer_summary;
  
  //  Run the optimizer.
  optimizer_t optimizer(&problem, &optimizer_opts, &optimizer_summary);
  optimizer.solve();
  optimizer.print_summary();
  
  // Write the output files including the number of iterations.
  Eigen::VectorXd summary_output(1);
  summary_output(1) = optimizer_summary.num_iters;
  io::write_dense_matrix(sample_prefix + "_U.bin", U, m, r);
  io::write_dense_matrix(sample_prefix + "_V.bin", V, n, r);
  io::write_dense_vector(sample_prefix + "_summary.bin", summary_output, 3);
  
}
