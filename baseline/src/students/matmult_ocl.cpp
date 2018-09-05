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

// OpenCL
#ifdef __APPLE__
#include "OpenCL/opencl.h"
#else
#include "CL/cl.h"
#endif

#include "../matmult.hpp"

/* You may not remove these pragmas: */
/*************************************/
#pragma GCC push_options
#pragma GCC optimize ("O1")

/*************************************/

// Macro to check clFunction outputs.
// Throw an error if not successful, to make debugging easier.
#define CHECK(err) if (err != CL_SUCCESS) { \
  throw std::runtime_error("OpenCL error: " + std::to_string(err) + \
  " in " + __FILE__ + " line " + std::to_string(__LINE__) ); \
}

///@brief A little enum class to help us parse clDeviceInfo
enum class ClInfoType {
  CHAR, SIZE_T, //... add your own info types
};


/// @brief Function to discover OpenCL devices and print some info on stdout.
static std::vector<cl_device_id> discoverDevices(cl_platform_id platform_id) {
  std::vector<cl_device_id> devices;
  // Discover devices for each platform
  cl_uint num_devices = 0;
  // Get number of devices of this type, we will only discover GPU devices for now.
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetDeviceIDs.html
  int err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);

  std::cout << "\tDevices: " << num_devices << std::endl;

  if ((err != CL_DEVICE_NOT_FOUND) || (num_devices != 0)) {
    // Get the devices of this type and insert them into the final list
    std::vector<cl_device_id> platform_type_devices(num_devices);
    CHECK(clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, num_devices, platform_type_devices.data(), &num_devices));
    // Insert the found devices into the final result
    devices.insert(std::end(devices), std::begin(platform_type_devices), std::end(platform_type_devices));

    // Many infos exist for devices. Also see:
    // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
    //
    // DISCLAIMER: IT IS HIGHLY RECOMMENDED TO DISCOVER SOME MORE STUFF ABOUT YOUR DEVICE WHEN YOU ARE GOING TO
    // USE IT MORE INTENSELY

    for (auto platform_type_device : platform_type_devices) {
      std::vector<cl_device_info> info_queries = {CL_DEVICE_NAME, CL_DEVICE_MAX_WORK_GROUP_SIZE};
      std::vector<ClInfoType> info_types = {ClInfoType::CHAR, ClInfoType::SIZE_T};
      size_t info_size = 0;
      for (unsigned int i = 0; i < info_queries.size(); i++) {
        // Get the query size
        CHECK(clGetDeviceInfo(platform_type_device, info_queries[i], 0, nullptr, &info_size));
        auto query = new char[info_size];
        CHECK(clGetDeviceInfo(platform_type_device, info_queries[i], info_size, query, &info_size));
        switch (info_types[i]) {
          case ClInfoType::SIZE_T: std::cout << *(size_t*)query << std::endl;
            break;
          default:std::cout << query << std::endl;
            break;
        }
        delete[] query;

      }
    }
  }
  return devices;
}

/// @brief Function to discover OpenCL platforms and print some info on stdout.
static std::vector<cl_platform_id> discoverPlatforms() {
  cl_uint num_platforms = 0;

  // Obtain the number of OpenCL platforms
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformIDs.html
  CHECK(clGetPlatformIDs(0, nullptr, &num_platforms));

  // OpenCL sometimes outputs some stuff on cerr. Flush this stuff from the stream.
  std::cerr.flush();

  std::cout << "Found " << num_platforms << " OpenCL platform(s)." << std::endl;

  // Create an array to hold platform IDs.
  auto platform_ids = std::vector<cl_platform_id>(num_platforms);

  // Get the actual platform IDs
  CHECK(clGetPlatformIDs(num_platforms, platform_ids.data(), &num_platforms));

  // Identify the platform info that we would like to discover (more infos exist, but they are not interesting for us)
  const std::vector<cl_platform_info> platform_queries = {CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION};

  // Iterate over all platforms
  for (unsigned int p = 0; p < num_platforms; p++) {
    std::cout << "Platform " << p << std::endl;

    // Iterate over all platform infos we want to inquire
    for (auto platform_query : platform_queries) {
      size_t query_size = 0;

      // Get the current platform info length
      CHECK(clGetPlatformInfo(platform_ids[p], platform_query, 0, nullptr, &query_size));
      auto query = new char[query_size];

      // Get the actual info
      CHECK(clGetPlatformInfo(platform_ids[p], platform_query, query_size, query, &query_size));

      std::cout << '\t' << query << std::endl;

      delete[] query;
    }
  }

  return platform_ids;
}

Matrix<float> multiplyMatricesOCL(Matrix<float> a,
                                  Matrix<float> b) {
  /* REPLACE THE CODE IN THIS FUNCTION WITH YOUR OWN CODE */
  /* YOU MUST USE OPENCL HERE */

  /* DISCLAIMER: this example code is written using the default C interface or OpenCL. There are also C++ bindings,
   * but we choose to use the C interfaces, because the C++ bindings might not always be available for every platform
   * you might use in the future. You are free, however, to use the C++ bindings that are usually in CL/cl2.hpp.
   */

  /* Example code partially inspired by: https://www.olcf.ornl.gov/tutorials/opencl-vector-addition/ */

  std::cout << "OpenCL test." << std::endl;

  // Create a little variable to store OpenCL error codes.
  int err;

  // First, we must discover all available OpenCL platforms
  auto platforms = discoverPlatforms();

  // If there are any platforms
  if (platforms.empty()) {
    throw std::runtime_error("No OpenCL platforms detected.");
  }

  // Discover the devices on the first platform.
  // Running on the cluster node should give you only one platform.
  // Be aware that your local setup might be different.
  auto devices = discoverDevices(platforms[0]);

  // If there are any devices
  if (devices.empty()) {
    throw std::runtime_error("No OpenCL devices detected.");
  }

  // Create an OpenCL context.
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clCreateContext.html
  // We will let the implementation of this function automatically select the platform, so
  // the first argument is a nullptr. We will use the first device only. so the number of devices is 1. You should
  // not ever diverge from this during the lab. We will not use a callback function with any user data, so the next two
  // arguments are both nullptrs. Finally we let the function return any error code into err.
  auto context = clCreateContext(nullptr, 1, &devices[0], nullptr, nullptr, &err);

  // Create an OpenCL program from some source that does element wise vector multiplication.
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clCreateProgramWithSource.html
  // Obviously, it would be prettier if this came from some file.
  auto example_kernel_source =
      "__kernel void vecmult_kernel(__global float *arg0,   \n" \
          "                             __global float *arg1,   \n" \
          "                             __global float *result, \n" \
          "                             const unsigned int size)\n" \
          "{                                                    \n" \
          "  // Obtain this thread ID                           \n" \
          "  int id = get_global_id(0);                         \n" \
          "                                                     \n" \
          "  // Make sure this thread does not go out of bounds.\n" \
          "  if (id < size)                                     \n" \
          "      result[id] = arg0[id] * arg1[id];              \n" \
          "}                                                    \n" \
          "\n";

  auto program = clCreateProgramWithSource(context, 1, (const char **) &example_kernel_source, nullptr, &err);

  // And build the program
  clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);

  // Create a command queue
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clCreateCommandQueue.html
  auto queue = clCreateCommandQueue(context, devices[0], 0, &err);

  // Create the OpenCL kernel "object" that can be sent to the device.
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clCreateKernel.html
  auto kernel = clCreateKernel(program, "vecmult_kernel", &err);

  // Create some example vectors
  unsigned int vector_size = 4;
  size_t vector_bytes = sizeof(float) * vector_size;

  std::vector<float> host_arg0 = {0.1f, 0.3f, 0.3f, 0.7f};
  std::vector<float> host_arg1 = {3.1f, 4.1f, 5.9f, 2.6f};
  std::vector<float> host_result(4);

  // Create some buffers on the device to hold the data
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clCreateBuffer.html
  auto device_arg0 = clCreateBuffer(context, CL_MEM_READ_ONLY, vector_bytes, nullptr, nullptr);
  auto device_arg1 = clCreateBuffer(context, CL_MEM_READ_ONLY, vector_bytes, nullptr, nullptr);
  // And one output buffer
  auto device_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, vector_bytes, nullptr, nullptr);

  // Enqueue transfers to write to the buffers.
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clEnqueueWriteBuffer.html
  CHECK(clEnqueueWriteBuffer(queue, device_arg0, CL_TRUE, 0, vector_bytes, host_arg0.data(), 0,
                             nullptr, nullptr));
  CHECK(clEnqueueWriteBuffer(queue, device_arg1, CL_TRUE, 0, vector_bytes, host_arg1.data(), 0,
                             nullptr, nullptr));

  // Ready the kernel for computation. We must first set its arguments
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clSetKernelArg.html
  CHECK(clSetKernelArg(kernel, 0, sizeof(cl_mem), &device_arg0));
  CHECK(clSetKernelArg(kernel, 1, sizeof(cl_mem), &device_arg1));
  CHECK(clSetKernelArg(kernel, 2, sizeof(cl_mem), &device_result));
  CHECK(clSetKernelArg(kernel, 3, sizeof(unsigned int), &vector_size));

  // The number of items to work on in a local in every local work group.
  size_t local_size = 1;
  // The number of items to work on globally. Make sure that this is an integer multiple of local_size.
  size_t global_size = vector_size;

  // Enqueue the execution of the kernel.
  // DISCLAIMER: IF SO FAR YOU HAVE NOT READ ANY API DOCUMENTATION, THIS IS A GREAT TIME TO START DOING SO
  // THIS IS AN IMPORTANT FUNCTION AND ITS PARAMETERS WILL INFLUENCE THE PERFORMANCE OF YOUR IMPLEMENTATION GREATLY.
  // https://www.khronos.org/registry/OpenCL/sdk/1.0/docs/man/xhtml/clEnqueueNDRangeKernel.html
  CHECK(clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &global_size, &local_size, 0, nullptr, nullptr));

  // Wait for the queue to finish...
  clFinish(queue);

  // Read the results back, from device to host
  // https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clEnqueueReadBuffer.html
  CHECK(clEnqueueReadBuffer(queue, device_result, CL_TRUE, 0, vector_bytes, host_result.data(), 0, nullptr, nullptr));

  // Clean up all the resources we've used.
  clReleaseMemObject(device_arg0);
  clReleaseMemObject(device_arg1);
  clReleaseMemObject(device_result);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);

  // Print the results
  std::cout << "OpenCL results: " << std::endl;
  for (auto val : host_result) {
    std::cout << val << std::endl;
  }

  return Matrix<float>(1, 1);
}

Matrix<double> multiplyMatricesOCL(Matrix<double> a,
                                   Matrix<double> b) {
  /* REPLACE THE CODE IN THIS FUNCTION WITH YOUR OWN CODE */
  /* YOU MUST USE OPENCL HERE */
  return Matrix<double>(1, 1);
}

/*************************************/
#pragma GCC pop_options
/*************************************/
