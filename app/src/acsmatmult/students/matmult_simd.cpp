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

#include "acsmatmult/matmult.h"
#include <immintrin.h>  // Intel intrinsics for SSE/AVX.

/* You may not remove these pragmas: */
/*************************************/
#pragma GCC push_options
#pragma GCC optimize ("O0")
/*************************************/

typedef union _avxd {
  __m256d val;
  double arr[4];
} avxd;

Matrix<float> multiplyMatricesSIMD(Matrix<float> a, Matrix<float> b) {
  /* REPLACE THE CODE IN THIS FUNCTION WITH YOUR OWN CODE */
  /* YOU MUST USE VECTOR EXTENSIONS HERE */

  std::cout << "AVX2 test" << std::endl;

  // Test if AVX2 works:
  __m256d x = _mm256_set_pd(0.1, 0.2, 0.3, 0.4);
  __m256d y = _mm256_set_pd(0.5, 0.6, 0.7, 0.8);

  avxd z;

  z.val = _mm256_add_pd(x, y);

  for (int i = 0; i < 4; i++) {
    std::cout << "z[" << i << "] = " << z.arr[i] << std::endl;
  }

  return Matrix<float>(1, 1);
}

Matrix<double> multiplyMatricesSIMD(Matrix<double> a,
                                  Matrix<double> b) {
  /* REPLACE THE CODE IN THIS FUNCTION WITH YOUR OWN CODE */
  /* YOU MUST USE VECTOR EXTENSIONS HERE */
  return Matrix<double>(1, 1);
}

/*************************************/
#pragma GCC pop_options
/*************************************/
