#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Eigen/Dense>

// Declare global variables.
#define DOUBLE_SIZE sizeof(double)    // Size of double

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixXd_t;

#endif