#ifndef _IO_H_
#define _IO_H_

#include <iostream>
#include <vector>
#include <Eigen/Dense>

#include "config.h"

struct io {
  
  // Destructor
  ~io();
  
  // Constructor
  io();
  
  // Read any dense matrix into the Eigen format (column-major).
  static void read_dense_matrix(const std::string file_path, MatrixXd_t & matrix, const int num_rows, const int num_cols);
  
  /* Find visible entries of a sparse matrix.
   * jj is a double array which has information about which rows are filled for each column.
   */
  static void find_visible_entries(const std::string file_path, std::vector<std::vector<int>> & jj, std::vector<int> & nnz, int & nnz_total, const int num_rows, const int cols);
  /* Read the visible entries of a sparse matrix.
   * Requires jj index and information about non-zeros.
   */
  static void read_visible_entries(const std::string file_path, Eigen::VectorXd & x, const std::vector<std::vector<int>> jj, const std::vector<int> nnz, const int nnz_total, const int num_rows, const int num_cols);
  
  static void write_dense_matrix(const std::string file_path, const MatrixXd_t matrix, const int num_rows, const int num_cols);
  static void write_dense_vector(const std::string file_path, const Eigen::VectorXd vector, const int num_elements);
};

#endif
