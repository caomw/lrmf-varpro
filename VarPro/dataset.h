#ifndef _DATASET_H_
#define _DATASET_H_

#include <Eigen/Dense>
#include <iostream>
#include <vector>

typedef struct dataset {
  
  /* Destructor */
  ~dataset();
  
  /* Constructors */
  dataset(int num_rows, int num_cols);
  dataset(int num_rows, int num_cols, std::string measurement_file_path, std::string weight_file_path);
  
  /* Data read functions */
  void read_data(std::string measurement_file_path, std::string weight_file_path);
  void set_dimension(int num_rows, int num_cols);
  
  /* Dataset dimension */
  int m, n; // row, column and rank
  int nnz_total = 0; // number of nonzero elements in total
  
  /* Indices */
  std::vector<std::vector<int>> jj; // Shows which rows are visible for each column.
  std::vector<int> nnz;
  
  // Eigen
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  /* Measurements */
  Eigen::VectorXd mt; // measurements (Wt * vec(M))
} dataset_t, * ptr_dataset_t;

#endif
