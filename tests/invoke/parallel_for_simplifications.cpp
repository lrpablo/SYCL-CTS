/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright (c) 2023 The Khronos Group Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//  Provides tests for parallel_for simplifications
//
*******************************************************************************/

#include "catch2/catch_template_test_macros.hpp"

#include "../common/get_cts_object.h"

namespace parallel_for_simplifications {

template <int dim>
struct kernel {
  using accessor_t =
      sycl::accessor<int, 1, sycl::access_mode::write, sycl::target::device>;
  accessor_t m_acc;

  kernel(accessor_t acc) : m_acc(acc) {}

  void operator()(sycl::item<dim> item) const {
    size_t index = item.get_linear_id();
    m_acc[index] = index;
  }
};

template <int N, typename ActionT>
void check(ActionT action) {
  std::array<int, N> arr;
  std::fill(arr.begin(), arr.end(), 0);
  {
    sycl::buffer<int, 1> buf(arr.data(), sycl::range<1>(N));
    sycl::queue queue = sycl_cts::util::get_cts_object::queue();

    queue.submit([&](sycl::handler& cgh) {
      auto acc = buf.get_access<sycl::access_mode::write>(cgh);
      action(cgh, acc);
    });
  }
  for (int i = 0; i < N; i++) CHECK(arr[i] == i);
}

TEST_CASE("Check parallel_for(N, some_kernel)",
          "[parallel_for_simplifications]") {
  constexpr int N = 2;
  auto action = [=](auto& cgh, auto& acc) {
    cgh.parallel_for(N, kernel<1>(acc));
  };
  check<N>(action);
}

TEST_CASE("Check parallel_for({N}, some_kernel)",
          "[parallel_for_simplifications]") {
  constexpr int N = 2;
  auto action = [=](auto& cgh, auto& acc) {
    cgh.parallel_for({N}, kernel<1>(acc));
  };
  check<N>(action);
}

TEST_CASE("Check parallel_for({N1, N2}, some_kernel)",
          "[parallel_for_simplifications]") {
  constexpr int N1 = 2;
  constexpr int N2 = 3;
  constexpr int N = N1 * N2;
  auto action = [=](auto& cgh, auto& acc) {
    cgh.parallel_for({N1, N2}, kernel<2>(acc));
  };
  check<N>(action);
}

TEST_CASE("Check parallel_for({N1, N2, N3}, some_kernel)",
          "[parallel_for_simplifications]") {
  constexpr int N1 = 2;
  constexpr int N2 = 3;
  constexpr int N3 = 5;
  constexpr int N = N1 * N2 * N3;
  auto action = [=](auto& cgh, auto& acc) {
    cgh.parallel_for({N1, N2, N3}, kernel<3>(acc));
  };
  check<N>(action);
}

}  // namespace parallel_for_simplifications
