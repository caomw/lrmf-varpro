CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(lrmf_varpro)

# Eigen3
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
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

# Create the executable.
ADD_EXECUTABLE(lrmf_varpro_exec ${SOURCES})

# Install command
INSTALL(TARGETS lrmf_varpro_exec DESTINATION ${CMAKE_INSTALL_PREFIX})

