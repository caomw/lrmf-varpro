CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(je_varpro_lrmf)

# FIND_PACKAGE(Ceres REQUIRED)
# INCLUDE_DIRECTORIES(${CERES_INCLUDE_DIRS})
# FIND_PACKAGE(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(/usr/local/include/eigen3)

# C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(SOURCES
  VarPro/dataset.cpp
  VarPro/io.cpp
  VarPro/problem.cpp
  VarPro/main.cpp
)

# LRMF
ADD_EXECUTABLE(je_varpro_lrmf ${SOURCES})
# TARGET_LINK_LIBRARIES(je_varpro_lrmf ${CERES_LIBRARIES})
