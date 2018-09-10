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

#include <ctime>

#include "../utils/Matrix.hpp"

#include "../matmult.hpp"

/**
 * @brief Test matrix multiplication
 * @tparam T The type of the matrices.
 * @return True if test passed, false if it didn't.
 */
template<typename T>
bool testMatMult(unsigned int repeats, bool verbose) {
  if (verbose) {
    std::cerr << "Matrix multiplication functional test." << std::endl;
  }

  // Create a random number generator and seed with current UNIX time.
  RandomGenerator<long> rand((unsigned int) std::time(nullptr));

  bool pass = true;

  for (unsigned int i = 0; i < repeats; i++) {
    // Randomize the number of rows and columns to be uneven in a small range
    size_t r = 11 + ((2 * (size_t) rand.next()) % 20);
    size_t c = 11 + ((2 * (size_t) rand.next()) % 20);

    // Construct matrix operands
    auto mat_a = Matrix<T>(r, c);
    auto mat_b = Matrix<T>(c, r);

    // Randomize contents
    mat_a.randomize(1337);
    mat_b.randomize(42);

    // Calculate baseline
    auto mat_baseline = mat_a * mat_b;

    // Create a vector to store other matrix results
    std::vector<std::tuple<std::string, Matrix<T>>> results;

    // Try other functions
    results.push_back(std::make_tuple("SIMD", multiplyMatricesSIMD(mat_a, mat_b)));

#ifdef USE_OPENMP
    results.push_back(std::make_tuple("OpenMP", multiplyMatricesOMP(mat_a, mat_b, -1)));
#endif

#ifdef USE_OPENCL
    results.push_back(std::make_tuple("OpenCL", multiplyMatricesOCL(mat_a, mat_b)));
#endif

    // Check if they are similar within the accepted error.
    for (auto result : results) {
      auto approx_equal = std::get<1>(result) %= mat_baseline;
      if (!approx_equal) {
        if (verbose) {
          std::cerr << "\t" << std::get<0>(result) << " failed." << std::endl;
        }
      }
      pass = pass && approx_equal;
    }
  }
  return pass;
}
