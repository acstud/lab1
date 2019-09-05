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

#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <cstring>

#include "acsmatmult/utils/RandomGenerator.h"

/**
 * @brief A matrix
 * @tparam T The type of the elements of the matrix, can be float or double
 */
template<typename T>
struct Matrix {
  size_t rows = 0;  ///< Number of rows in this matrix
  size_t columns = 0;  ///< Number of columns in this matrix
  std::shared_ptr<T> values = nullptr;  ///< Raw data buffer of this matrix

  /**
   * @brief Construct a new matrix
   * @param rows The number of rows of the matrix
   * @param columns The number of columns of the matrix
   */
  Matrix<T>(size_t rows, size_t columns) : rows(rows), columns(columns) {
    // Make sure the dimensions are valid
    if ((rows <= 0) && (columns <= 0)) {
      throw std::domain_error("Matrix dimensions must be positive integers.");
    }
    // Allocate space, zero initialize it and store matrix using a smart pointer
    values = std::shared_ptr<T>(new T[rows * columns](), std::default_delete<T[]>());
  }

  /// @brief Return a value from the 1-dimensional backing array at index \p idx.
  inline T &operator[](size_t idx) {
    return values.get()[idx];
  }

  /// @brief Return element matrix element (row+1,column+1), in the mathematical sense
  inline T &operator()(size_t row, size_t column) const {
    return values.get()[row * columns + column];
  }

#pragma GCC push_options
#pragma GCC optimize ("O0")

  /**
 * @brief Multiply two matrices.
 *
 * This is the baseline implementation without optimizations.
 * It can be used to compare throughput and verify the results.
 *
 * @tparam T The type of the elements in the matrix.
 * @param a The left matrix
 * @param b The right matrix
 * @return A matrix C where C=AxB
 */
  static Matrix<T> multiply(const Matrix<T> &a, const Matrix<T> &b) {
    // Test if matrices can be multiplied.
    if (a.columns != b.rows) {
      throw std::domain_error("Matrix dimensions do not allow matrix-multiplication.");
    }

    // Height and width
    auto rows = a.rows;
    auto columns = b.columns;

    // Create the resulting matrix
    auto result = Matrix<T>(rows, columns);

    // For each row
    for (size_t r = 0; r < rows; r++) {
      // For each column
      for (size_t c = 0; c < columns; c++) {

        T e = 0.0;
        // Accumulate the product of the elements
        for (size_t i = 0; i < b.rows; i++) {
          e += a(r, i) * b(i, c);
        }

        // Store the result
        result(r, c) = e;
      }
    }

    return result;
  }

#pragma GCC pop_options

  /// @brief Shorthand for multiplying two matrices using multiply()
  Matrix<T> operator*(const Matrix<T> &rhs) {
    return multiply(*this, rhs);
  }

  /**
   * @brief Check if this matrix is approximately equal to another matrix.
   *
   * @param rhs The other matrix.
   * @return True if it is approximately equal, false otherwise.
   */
  bool operator%=(const Matrix<T> &rhs) {
    if (rows != rhs.rows)
      return false;
    if (columns != rhs.columns)
      return false;
    for (size_t r = 0; r < rows; r++) {
      for (size_t c = 0; c < columns; c++) {
        T lv = this->operator()(r, c);
        T rv = rhs(r, c);
        T diff = lv - rv;
        T err = diff * diff;
        if (err >= 1E-6) {
          std::cerr << "Error larger than threshold at (" << r << ", " << c << ")." << std::endl;
          std::cerr << "Left hand side value : " << lv << std::endl;
          std::cerr << "Right hand side value: " << rv << std::endl;
          std::cerr << "Squared difference   : " << err << std::endl;
          return false;
        }
      }
    }
    return true;
  }

  /// @brief Randomize the elements in this matrix, with some seed.
  Matrix *randomize(int seed = 0) {

    RandomGenerator<T> gen(seed);
    // Fill the array with random values
    for (size_t i = 0; i < columns * rows; i++) {
      values.get()[i] = gen.next();
    }

    return this;
  }

  /// @brief Print the matrix
  void print() {
    std::cout << "Matrix: " << std::endl;
    for (size_t r = 0; r < rows; r++) {
      for (size_t c = 0; c < columns; c++) {
        std::cout << std::fixed << std::setprecision(4) << std::setw(6) << this->operator()(r, c) << " ";
      }
      std::cout << "\n";
    }
    std::cout << std::endl;
  }
};
