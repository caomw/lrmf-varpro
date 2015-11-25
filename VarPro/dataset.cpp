#include <fstream>
#include <numeric>

#include "config.h"
#include "dataset.h"
#include "io.h"

/* Destructor */
dataset::~dataset() {}

/* Constructors */
dataset::dataset(int num_rows, int num_cols) {
  dataset::set_dimension(num_rows, num_cols);
}

dataset::dataset(int num_rows, int num_cols, std::string measurement_file_path, std::string weight_file_path) {
  dataset::set_dimension(num_rows, num_cols);
  dataset::read_data(measurement_file_path, weight_file_path);
}

/* Set the dimensions of the dataset. */
void dataset::set_dimension(int num_rows, int num_cols) {
  m = num_rows;
  n = num_cols;
  jj.resize(num_cols);
}

void dataset::read_data(std::string measurement_file_path, std::string weight_file_path) {
  io::find_visible_entries(weight_file_path, jj, nnz, nnz_total, m, n);
  io::read_visible_entries(measurement_file_path, mt, jj, nnz, nnz_total, m, n);
}
