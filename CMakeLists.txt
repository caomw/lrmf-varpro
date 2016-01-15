CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(je_varpro_lrmf)

# Eigen3
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
FIND_PACKAGE(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

# C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(SOURCES
  VarPro/dataset.cpp
  VarPro/experiment.cpp
  VarPro/io.cpp
  VarPro/main.cpp
  VarPro/optimizer.cpp
  VarPro/options.cpp
  VarPro/parser.cpp
  VarPro/problem.cpp
)

# LRMF
ADD_EXECUTABLE(je_varpro_lrmf ${SOURCES})
# TARGET_LINK_LIBRARIES(je_varpro_lrmf ${CERES_LIBRARIES})
