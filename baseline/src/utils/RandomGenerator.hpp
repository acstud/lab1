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

#include <random>

/**
 * @brief A pseudo-random number generator.
 * @tparam T The type of number to generate.
 */
template<typename T>
struct RandomGenerator {
  /// @brief Construct a new RandomGenerator<T> with a seed.
  explicit RandomGenerator(int seed = 0);

  /// @brief Return a new number.
  inline T next();
};

template<>
struct RandomGenerator<float> {
  // Use the mt19937 random number engine
  std::mt19937 gen;
  // Use a uniform real destribution for floats
  std::uniform_real_distribution<float> dis;

  explicit RandomGenerator(int seed = 0) : gen(std::mt19937(seed)) {}

  inline float next() { return dis(gen); }
};

template<>
struct RandomGenerator<double> {
  // Use the mt19937 random number engine
  std::mt19937 gen;
  // Use a uniform real destribution for doubles
  std::uniform_real_distribution<double> dis;

  explicit RandomGenerator(int seed = 0) : gen(std::mt19937(seed)) {}

  inline double next() { return dis(gen); }
};

template<>
struct RandomGenerator<int> {
  // Use the mt19937 random number engine
  std::mt19937 gen;
  // Use a uniform real destribution for integers
  std::uniform_int_distribution<int> dis;

  explicit RandomGenerator(int seed = 0) : gen(std::mt19937(seed)) {}

  inline int next() { return dis(gen); }
};

template<>
struct RandomGenerator<long> {
  // Use the mt19937 random number engine
  std::mt19937 gen;
  // Use a uniform real destribution for longs
  std::uniform_int_distribution<long> dis;

  explicit RandomGenerator(int seed = 0) : gen(std::mt19937(seed)) {}

  inline long next() { return dis(gen); }
};
