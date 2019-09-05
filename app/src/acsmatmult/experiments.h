// Copyright 2018 Delft University of Technology
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include "acsmatmult/matmult.h"

/// @brief Generate a readable header into a stringstream.
void generateHeader(std::stringstream &ss,
                    const std::vector<std::string> &headers,
                    const std::vector<std::string> &repeated_headers,
                    unsigned int repeats);

/// @brief Dump a stringstream to two output streams and clear it
void dump(std::stringstream &ss, std::ostream &s0, std::ostream &s1);

/// @brief Perform an experiment where two vectors are multiplied.
void runVectorExperiment(unsigned int from,
                         unsigned int to,
                         unsigned int repeats = 1,
                         const std::string& file_out = "vec.csv");

/* STUDENTS SHOULD IMPLEMENT THE FOLLOWING EXPERIMENTS: */
/// @brief Perform an experiment where two matrices are multiplied.
void runMatrixExperiment(unsigned int from,
                         unsigned int to,
                         unsigned int repeats = 1,
                         const std::string& file_out = "mat.csv");

/// @brief Perform an experiment where two matrices are multiplied using vector extensions
void runMatrixExperimentSIMD(unsigned int from,
                             unsigned int to,
                             unsigned int repeats = 1,
                             const std::string& file_out = "mat_simd.csv");

/// @brief Perform an experiment where two matrices are multiplied using OpenMP
void runMatrixExperimentOMP(unsigned int from,
                            unsigned int to,
                            unsigned int threads = 1,
                            unsigned int repeats = 1,
                            const std::string& file_out = "mat_omp.csv");

/// @brief Perform an experiment where two matrices are multiplied using OpenCL
void runMatrixExperimentOCL(unsigned int from,
                            unsigned int to,
                            unsigned int repeats = 1,
                            const std::string& file_out = "mat_ocl.csv");

