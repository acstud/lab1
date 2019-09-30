cmake_minimum_required(VERSION 3.10)
project(acsmatmult)

# Default sources
set(DEFAULT_SOURCES
    src/acsmatmult/utils/Matrix.h
    src/acsmatmult/utils/RandomGenerator.h
    src/acsmatmult/utils/Timer.h
    src/acsmatmult/matmult.h
    src/acsmatmult/experiments.h
    src/acsmatmult/tests/test_matmult.h
    src/acsmatmult/acsmatmult.cpp)

set(STUDENT_SOURCES
        src/acsmatmult/students/experiments.cpp
        src/acsmatmult/students/matmult_simd.cpp)

set(SOLUTION_SOURCES
        src/acsmatmult/solutions/experiments.cpp
        src/acsmatmult/solutions/matmult_simd.cpp)

include_directories(src)

add_executable(${PROJECT_NAME} ${DEFAULT_SOURCES})

# Compilation options
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")    # Use C++11 (required)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")        # Enable highest compiler optimizations
# Enable all warnings except deprecated stuff / older OCL version warnings.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated -Wno-deprecated-declarations -Wno-ignored-attributes")

# GPROF profiling
OPTION(PROFILING "Profile the application using gprof" OFF)
if (PROFILING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif (PROFILING)

# Enable SIMD
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
# This attempts to use AVX. If your system does not have AVX, you may use other options such as:
# -mavx, -msse4.2, -msse4.1, -msse3, -msse2, -msse, and only use those intrinsics.
# You can run "cat /proc/cpuinfo" to see what extensions your CPU supports.

# OpenMP
include(FindOpenMP)
if (OPENMP_FOUND)
    message("[ACS LAB] OpenMP found, enabling OpenMP functions.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    add_definitions(-DUSE_OPENMP)
    list(APPEND STUDENT_SOURCES src/acsmatmult/students/matmult_omp.cpp)
    list(APPEND SOLUTION_SOURCES src/acsmatmult/solutions/matmult_omp.cpp)
else (OPENMP_FOUND)
    message("[ACS LAB] OpenMP could not be found, disabling OpenMP functions.")
endif (OPENMP_FOUND)

# OpenCL
include(FindOpenCL)
if (OPENCL_FOUND)
    message("[ACS LAB] OpenCL found, enabling OpenCL functions.")
    find_package(OpenCL REQUIRED)
    include_directories(${OpenCL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARIES})
    add_definitions(-DUSE_OPENCL)
    list(APPEND STUDENT_SOURCES src/acsmatmult/students/matmult_ocl.cpp)
    list(APPEND SOLUTION_SOURCES src/acsmatmult/solutions/matmult_ocl.cpp)
else (OPENCL_FOUND)
    message("[ACS LAB] OpenCL could not be found, disabling OpenCL functions.")
endif (OPENCL_FOUND)

# Solutions
OPTION(USE_SOLUTIONS "Build solution files instead of student files" OFF)
if (USE_SOLUTIONS)
    message("[ACS LAB] Using solution sources.")
    target_sources(${PROJECT_NAME} PUBLIC ${SOLUTION_SOURCES})
else (USE_SOLUTIONS)
    message("[ACS LAB] Using student sources.")
    target_sources(${PROJECT_NAME} PUBLIC ${STUDENT_SOURCES})
endif (USE_SOLUTIONS)

