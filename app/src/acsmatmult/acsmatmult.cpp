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

#include <getopt.h>
#include <iostream>
#include <memory>

#include "acsmatmult/tests/test_matmult.h"
#include "acsmatmult/experiments.h"

/**
 * @brief Structure to pass program options to and run different experiments.
 */
struct BenchmarkOptions {
  bool vector = false;
  bool matrix = false;
  bool simd = false;
  bool openmp_single = false;
  bool openmp_range = false;
  bool opencl = false;
  bool tests = false;
  bool verbose_testing = true;

  // Experiment options
  unsigned int repeats = 1;
  unsigned int from = 1;
  unsigned int to = 8;
  unsigned int threads = 1;

  /// @brief Print usage information
  static void usage(char *argv[]) {
    std::cerr << "Usage: " << argv[0] << " -hrvmscta -x X -y Y -r R -o <threads>\n" <<
              "  -h    Show help.\n"
              "\n"
              "Experiment option:\n"
              "  -x X  Start the experiment with singular dimension 2^X. (default: X=1)\n"
              "  -y Y  End the experiment with dimension 2^Y. (default: Y=8)\n"
              "  -r R  Repeat each experiment/test R times.\n"
              "\n"
              "Experiment selection: \n"
              "  -v    Run app vector experiment.\n"
              "  -m    Run app matrix experiment.\n"
              "  -s    Run SIMD experiment.\n"
              "  -o N  Run OpenMP experiment using N threads.\n"
              "  -O    Run OpenMP ranging threads from 1-8\n"
              "  -c    Run OpenCL experiment.\n"
              "  -a    Run all, including tests.\n"
              "\n"
              "Other options:\n"
              "  -t    Run functional tests for all matrix multiplication functions.\n"
              "  -u    Increase verbosity of functional tests (on stderr).\n";

    std::cerr.flush();
    exit(0);
  }

  /// @brief Run matrix multiplication tests
  /// This uses all implementations that are compiled into the project.
  void runTests() {
    if (!testMatMult<float>(repeats, verbose_testing))
      std::cerr << "Single-precision floating point matrix multiplication FAILED." << std::endl;

    if (!testMatMult<double>(repeats, verbose_testing))
      std::cerr << "Double-precision floating point matrix multiplication FAILED." << std::endl;

    std::cerr.flush();
    std::cout.flush();
  }

  void run() {
    if (vector) runVectorExperiment(from, to, repeats);
    if (matrix) runMatrixExperiment(from, to, repeats);
    if (simd) runMatrixExperimentSIMD(from, to, repeats);
    if (openmp_single) runMatrixExperimentOMP(from, to, threads, repeats);
    if (openmp_range) {
      for (unsigned int t = 1; t < 9; t++) {
        runMatrixExperimentOMP(from, to, t, repeats);
      }
    }
    if (opencl) runMatrixExperimentOCL(from, to, repeats);
    if (tests) runTests();
  }
};

int main(int argc, char *argv[]) {
  BenchmarkOptions bo;

  if (argc == 1) {
    BenchmarkOptions::usage(argv);
  }

  // Use GNU getopt to parse command line options
  int opt;
  while ((opt = getopt(argc, argv, "hx:y:r:vmsOctao:u")) != -1) {
    switch (opt) {
      case 'x': {
        char *end;
        bo.from = (unsigned int) std::strtol(optarg, &end, 10);
        break;
      }

      case 'y': {
        char *end;
        bo.to = (unsigned int) std::strtol(optarg, &end, 10);
        break;
      }

      case 'r': {
        char *end;
        bo.repeats = (unsigned int) std::strtol(optarg, &end, 10);
        break;
      }

      case 'h': {
        BenchmarkOptions::usage(argv);
        break;
      }

      case 'v':bo.vector = true;
        break;

      case 'm':bo.matrix = true;
        break;

      case 's':bo.simd = true;
        break;

      case 'o': {
        char *end;
        bo.threads = (unsigned int) std::strtol(optarg, &end, 10);
        bo.openmp_single = true;
        break;
      }

      case 'O': {
        bo.openmp_range = true;
        break;
      }

      case 'c':bo.opencl = true;
        break;

      case 't':bo.tests = true;
        break;

      case 'u':bo.verbose_testing = true;
        break;

      case 'a': {
        bo.vector = true;
        bo.matrix = true;
        bo.simd = true;
        bo.openmp_single = true;
        bo.openmp_range = true;
        bo.opencl = true;
        bo.tests = true;
        break;
      }

      case '?':
        if ((optopt == 'o') || (optopt == 'r') || (optopt == 'x') || (optopt == 'y')) {
          std::cerr << "Options -o, -r, -x and -y require an argument." << std::endl;
          BenchmarkOptions::usage(argv);
        }
        break;

      default:BenchmarkOptions::usage(argv);
        break;
    }
  }

  bo.run();

  return 0;
}
