#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Eigen/Dense>

// Declare global variables.
#define DOUBLE_SIZE sizeof(double)    // Size of double
#define FUNCTION_TOLERANCE_REACHED 0
#define MAX_TRIALS_REACHED 1


typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixXd_t;

#endif