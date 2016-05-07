#include "config.h"
#include "io.h"

#include <fstream>
#include <numeric>

// Read matrix (in doubles) from a MATLAB column-major binary file in the row-major order.
void io::read_dense_matrix(const std::string file_path, MatrixXd_t & matrix, const int num_rows, const int num_cols) {
  
  // Resize the matrix.
  matrix.resize(num_rows, num_cols);
  
  // Open the input stream.
  std::ifstream input_stream(file_path.c_str(), std::ios::binary);
  if (!input_stream.is_open()) {
    std::cout << "Error! Unable to open the input file \"" << file_path << "\"." << std::endl;
  }
  
  double buffer;
  for (int j = 0; j < num_cols; ++j) {
    for (int i = 0; i < num_rows; ++i) {
      // Read each element and write to memory.
      input_stream.read((char*)(&buffer), DOUBLE_SIZE);
      matrix(i, j) = buffer;
    }
  }
  // Close the input stream.
  input_stream.close();
}

void io::find_visible_entries(const std::string weight_file_path, std::vector<std::vector<int> > & jj, std::vector<int> & nnz, int & nnz_total, const int num_rows, const int num_cols) {
  
  // Resize nnz and the outer size of jj.
  jj.resize(num_cols);
  nnz.resize(num_cols);
  
  // Open the input stream.
  std::ifstream input_stream(weight_file_path.c_str(), std::ios::binary);
  if (!input_stream.is_open()) {
    std::cout << "Error! Unable to open the input file \"" << weight_file_path << "\"." << std::endl;
  }
  
  double buffer;
  nnz_total = 0;
  for (int j = 0; j < num_cols; ++j) {
    nnz[j] = 0;
    for (int i = 0; i < num_rows; ++i) {
      // Read each element and write to memory.
      input_stream.read((char*)(&buffer), DOUBLE_SIZE);
      // Skip if the element is nan or 0.
      if (isnan(buffer) || buffer == 0) continue;
      jj[j].push_back(i);
      nnz[j] += 1;
    }
    nnz_total += nnz[j];
  }
  // Close the input stream.
  input_stream.close();
}

void io::read_visible_entries(const std::string file_path, Eigen::VectorXd & x, std::vector<std::vector<int> > jj, const std::vector<int> nnz, const int nnz_total, const int num_rows, const int num_cols) {
  // Open the input stream.
  std::ifstream input_stream(file_path.c_str(), std::ios::binary);
  if (!input_stream.is_open()) {
    std::cout << "Error! Unable to open the input file \"" << file_path << "\"." << std::endl;
  }
  
  // Resize the output vector.
  x.resize(nnz_total);
  
  double buffer;
  int l = -1;
  for (int j = 0; j < num_cols; ++j) {
    for (std::vector<int>::iterator it = jj[j].begin(); it !=jj[j].end(); ++it) {
      //    for (int k = 0; k < nnz[j]; ++k) {
      // Seek data pointer and read the corresponding value.
      input_stream.seekg((j * num_rows + (* it)) * DOUBLE_SIZE);
      input_stream.read((char *) (&buffer), DOUBLE_SIZE);
      x(++l) = buffer;
    }
  }
  
  // Close the input stream.
  input_stream.close();
}

// Write Eigen matrix to a binary file in the row-major order.
void io::write_dense_matrix(const std::string file_path, const MatrixXd_t matrix, const int num_rows, const int num_cols)
{
  std::ofstream output_stream(file_path.c_str(), std::ios::binary);
  if (!output_stream.is_open()) {
    std::cout << "Error! Unable to open the output file \"" << file_path << "\"." << std::endl;
    return;
  }
  
  std::ofstream::streampos length;
  // std::cout << "Number of elements to be written: " << nRows * nCols << std::endl;
  
  double buffer;
  for (int j = 0; j < num_cols; j++) {
    for (int i = 0; i < num_rows; i++) {
      buffer = matrix(i, j);
      output_stream.write((char*)(&buffer), DOUBLE_SIZE);
    }
  }
  output_stream.close();
}

// Write Eigen vector to a binary file.
void io::write_dense_vector(const std::string file_path, const Eigen::VectorXd vector, const int num_elements)
{
  std::ofstream output_stream(file_path.c_str(), std::ios::binary);
  if (!output_stream.is_open()) {
    std::cout << "Error! Unable to open the output file \"" << file_path << "\"." << std::endl;
    return;
  }
  
  std::ofstream::streampos length;
  // std::cout << "Number of elements to be written: " << nRows * nCols << std::endl;
  
  double buffer;
  for (int j = 0; j < num_elements; j++) {
    buffer = vector(j);
    output_stream.write((char*)(&buffer), DOUBLE_SIZE);
  }
  output_stream.close();
}