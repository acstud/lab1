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

#include "acsmatmult/experiments.h"
#include <fstream>
#include "acsmatmult/utils/Timer.h"
#include "acsmatmult/utils/Matrix.h"

void generateHeader(std::stringstream &ss,
                    const std::vector<std::string> &headers,
                    const std::vector<std::string> &repeated_headers,
                    unsigned int repeats) {
  for (const auto &header : headers) {
    ss << std::setw(15) << (header + ",");
  }
  for (const auto &header : repeated_headers) {
    for (unsigned int r = 0; r < repeats; r++) {
      ss << std::setw(15) << (header + " " + std::to_string(r) + ",");
    }
  }
  ss << std::endl;
}

void dump(std::stringstream &ss, std::ostream &s0, std::ostream &s1) {
  s0 << ss.str();
  s1 << ss.str();
  ss.str("");
}

void runVectorExperiment(unsigned int min, unsigned int max, unsigned int repeats, const std::string &file_out) {
  std::cout << "Vector inner product benchmark." << std::endl;
  // Attempt to open the file for writing
  std::ofstream fos(file_out);
  if (!fos.good()) {
    std::cerr << "Could not open file " + file_out << std::endl;
    std::exit(-1);
  }
  // Generate a stringstream to write output to both stdout and a file
  std::stringstream ss;

  // Create a timer used for wall-clock time measurements
  Timer t;

  // Dump a header
  generateHeader(ss,
                 {"Experiment", "Matrix size", "Construct (s)", "Randomize (s)"},
                 {"Float", "Double"},
                 repeats);
  dump(ss, fos, std::cout);

  // Iterate over each experiment
  for (unsigned int e = min; e < max; e++) {
    // In this experiment, we use powers of 2 as the problem size.
    // Not that that is not always necessary. You may also linearly grow the problem size.
    // Shift a long value of 1 left by e, which is the same as 2^e, to obtain the matrix dimension
    auto mat_rows = 1ul << e;
    // Number of columns is 1 for now, because we just want to calculate the inner product.
    auto mat_cols = 1ul;

    // Print experiment number
    ss << std::setw(15) << (std::to_string(e) + ",");

    // Print the problem size
    ss << std::setw(15) << (std::to_string(mat_rows) + ",") << std::flush;

    // Create the matrices
    t.start();  // Start the timer.
    auto mat_a = Matrix<float>(mat_cols, mat_rows);  // Make a matrix
    auto mat_b = Matrix<float>(mat_rows, mat_cols);  // And another one, transposed.
    auto mat_c = Matrix<double>(mat_cols, mat_rows);  // And another one, doubles.
    auto mat_d = Matrix<double>(mat_rows, mat_cols);  // And another one, transposed.
    t.stop();  // Stop the timer.
    t.report(ss);  // Put interval on stdout

    // Randomize their contents
    t.start();
    mat_a.randomize();
    mat_b.randomize();
    mat_c.randomize();
    mat_d.randomize();
    t.stop();
    t.report(ss);

    // Dump the initialization output
    dump(ss, fos, std::cout);

    // Repeat the floats experiment repeats times.
    for (unsigned int r = 0; r < repeats; r++) {
      t.start();  // Start the timer.
      // Multiply the matrices
      auto mat_result = Matrix<float>::multiply(mat_a, mat_b);
      t.stop();  // Stop the timer.
      t.report(ss);

      // Dump the repeat outcome
      dump(ss, fos, std::cout);
    }

    // Repeat the doubles experiment repeats times.
    for (unsigned int r = 0; r < repeats; r++) {
      t.start();
      auto mat_result = Matrix<double>::multiply(mat_c, mat_d);
      t.stop();
      t.report(ss, r == (repeats - 1));
      dump(ss, fos, std::cout);
    }
  }
}

void runMatrixExperiment(unsigned int min, unsigned int max, unsigned int repeats, const std::string &file_out) {
  std::cout << "Baseline matrix multiplication experiment not yet implemented." << std::endl;
}

void runMatrixExperimentSIMD(unsigned int min, unsigned int max, unsigned int repeats, const std::string &file_out) {
  std::cout << "SIMD matrix multiplication experiment not yet implemented." << std::endl;
}

void runMatrixExperimentOMP(unsigned int min,
                            unsigned int max,
                            unsigned int threads,
                            unsigned int repeats,
                            const std::string &file_out) {
  std::cout << "OpenMP matrix multiplication experiment not yet implemented." << std::endl;
}

void runMatrixExperimentOCL(unsigned int min, unsigned int max, unsigned int repeats, const std::string &file_out) {
  std::cout << "OpenCL matrix multiplication experiment not yet implemented." << std::endl;
}
