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
//  Provides tests for sycl::exception constructors
//
*******************************************************************************/

#include "../../util/sycl_exceptions.h"
#include "../common/disabled_for_test_case.h"
#include "catch2/catch_test_macros.hpp"

#include "exceptions.h"

namespace exception_constructors_test {

/**
 * @brief The function helps to verify that exception was constructed correctly.
 * This overload expects that \c .has_context() returns false and the right
 * exception is thrown after calling \c get_context().
 *
 * @param e The exception instance that needs to be verified
 * @param errcode Expected error code from \c .code() member function
 * @param errcat Expected error category from \c .category() member function
 */
inline void check_exception(sycl::exception e, const std::error_code errcode,
                            const std::error_category& errcat) {
  CHECK(e.code() == errcode);
  CHECK(e.category() == errcat);
  CHECK(e.what() != nullptr);
  CHECK(e.has_context() == false);
  CHECK_THROWS_MATCHES(e.get_context(), sycl::exception,
                       sycl_cts::util::equals_exception(sycl::errc::invalid));
}

/**
 * @brief The function helps to verify that exception was constructed correctly.
 * This overload expects that \c .has_context() returns false and the right
 * exception is thrown after calling \c get_context().
 *
 * @param e The exception instance that needs to be verified
 * @param errcode Expected error code from \c .code() member function
 * @param errcat Expected error category from \c .category() member function
 * @param what_arg Expected string from \c .what() member function
 */
inline void check_exception(sycl::exception e, const std::error_code errcode,
                            const std::error_category& errcat,
                            const std::string& what_arg) {
  CHECK(e.code() == errcode);
  CHECK(e.category() == errcat);
  CHECK(std::string(e.what()).find(what_arg) != std::string::npos);
  CHECK(e.has_context() == false);
  CHECK_THROWS_MATCHES(e.get_context(), sycl::exception,
                       sycl_cts::util::equals_exception(sycl::errc::invalid));
}

/**
 * @brief The function helps to verify that exception was constructed correctly.
 * This overload expects that \c .has_context() returns true.
 *
 * @param e The exception instance that needs to be verified
 * @param errcode Expected error code from \c .code() member function
 * @param errcat Expected error category from \c .category() member function
 * @param ctx Expected context from \c .get_context() member function
 */
inline void check_exception(sycl::exception e, const std::error_code errcode,
                            const std::error_category& errcat,
                            const sycl::context& ctx) {
  CHECK(e.code() == errcode);
  CHECK(e.category() == errcat);
  CHECK(e.what() != nullptr);
  CHECK(e.has_context() == true);
  CHECK(e.get_context() == ctx);
}

/**
 * @brief The function helps to verify that exception was constructed correctly.
 * This overload expects that \c .has_context() returns true.
 *
 * @param e The exception instance that needs to be verified
 * @param errcode Expected error code from \c .code() member function
 * @param errcat Expected error category from \c .category() member function
 * @param what_arg Expected string from \c .what() member function
 * @param ctx Expected context from \c .get_context() member function
 */
inline void check_exception(sycl::exception e, const std::error_code errcode,
                            const std::error_category& errcat,
                            const std::string& what_arg,
                            const sycl::context& ctx) {
  CHECK(e.code() == errcode);
  CHECK(e.category() == errcat);
  CHECK(std::string(e.what()).find(what_arg) != std::string::npos);
  CHECK(e.has_context() == true);
  CHECK(e.get_context() == ctx);
}

TEST_CASE("Constructors for sycl::exception with sycl::errc error codes",
          "[exception]") {
#if SYCL_CTS_ENABLE_FULL_CONFORMANCE
  std::array testing_errs = get_err_codes();
#else
  std::array testing_errs{sycl::errc::success};
#endif
  const std::string what_arg_str = "test";
  const sycl::context ctx;

  for (const auto errcode : testing_errs) {
    const std::error_code std_errc(errcode);

    SECTION("exception(std::error_code ec, const std::string& what_arg)") {
      sycl::exception e(std_errc, what_arg_str);
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str);
    }
    SECTION("exception(std::error_code ec, const char* what_arg)") {
      sycl::exception e(std_errc, what_arg_str.c_str());
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str);
    }
    SECTION("exception(std::error_code ec)") {
      sycl::exception e(std_errc);
      check_exception(e, std_errc, sycl::sycl_category());
    }

    SECTION(
        "exception(int ev, const std::error_category& ecat, const std::string& "
        "what_arg)") {
      sycl::exception e(static_cast<int>(errcode), sycl::sycl_category(),
                        what_arg_str);
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str);
    }
    SECTION(
        "exception(int ev, const std::error_category& ecat, const char* "
        "what_arg)") {
      sycl::exception e(static_cast<int>(errcode), sycl::sycl_category(),
                        what_arg_str.c_str());
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str);
    }
    SECTION("exception(int ev, const std::error_category& ecat)") {
      sycl::exception e(static_cast<int>(errcode), sycl::sycl_category());
      check_exception(e, std_errc, sycl::sycl_category());
    }

    SECTION(
        "exception(context ctx, std::error_code ec, const std::string& "
        "what_arg)") {
      sycl::exception e(ctx, std_errc, what_arg_str);
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str, ctx);
    }
    SECTION(
        "exception(context ctx, std::error_code ec, const char* what_arg)") {
      sycl::exception e(ctx, std_errc, what_arg_str.c_str());
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str, ctx);
    }
    SECTION("exception(context ctx, std::error_code ec)") {
      sycl::exception e(ctx, std_errc);
      check_exception(e, std_errc, sycl::sycl_category(), ctx);
    }

    SECTION(
        "exception(context ctx, int ev,const std::error_category& ecat, const "
        "std::string& what_arg)") {
      sycl::exception e(ctx, static_cast<int>(errcode), sycl::sycl_category(),
                        what_arg_str);
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str, ctx);
    }
    SECTION(
        "exception(context ctx, int ev,const std::error_category& ecat, const "
        "char* what_arg)") {
      sycl::exception e(ctx, static_cast<int>(errcode), sycl::sycl_category(),
                        what_arg_str.c_str());
      check_exception(e, std_errc, sycl::sycl_category(), what_arg_str, ctx);
    }
    SECTION("exception(context ctx, int ev,const std::error_category& ecat)") {
      sycl::exception e(ctx, static_cast<int>(errcode), sycl::sycl_category());
      check_exception(e, std_errc, sycl::sycl_category(), ctx);
    }

    SECTION("exception(const exception& other)") {
      sycl::exception e(std_errc);
      sycl::exception copy(e);
      check_exception(copy, errcode, sycl::sycl_category());
    }
    SECTION("operator=(const exception& other)") {
      // sycl::exception is not default-constructible, so explicitly construct
      // `copy` with an error code that is different from `errcode`
      std::error_code other_errc(errcode == sycl::errc::success
                                     ? sycl::errc::runtime
                                     : sycl::errc::success);

      sycl::exception e(std_errc);
      sycl::exception copy(other_errc);
      CHECK_NOTHROW((copy = e));
      check_exception(copy, errcode, sycl::sycl_category());
    }
  }
}

TEST_CASE("sycl::exception is derived from std::exception", "[exception]") {
  CHECK(std::is_base_of_v<std::exception, sycl::exception>);
}

}  // namespace exception_constructors_test
