cmake_minimum_required(VERSION 3.20)
project(cism)

set(CPACK_PACKAGE_CONTACT "cronnosli@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "cronnosli@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Customizable Interactive Script Manager")
set(CPACK_PACKAGE_DESCRIPTION "Customizable Interactive Script Manager")
set(CPACK_PACKAGE_VENDOR "Douglas Cordeiro<cronnosli@gmail.com>")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "nlohmann-json3-dev")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/cronnosli/cism")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
include(CPack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

enable_testing()

include_directories(src/include)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)


find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(GTest REQUIRED)

#add_test(NAME ExampleTest COMMAND ExampleTest)

if(ENABLE_COVERAGE)
  # set compiler flags
  set(CMAKE_CXX_FLAGS "-O0 -coverage")

  # find required tools
  find_program(LCOV lcov REQUIRED)
  find_program(GENHTML genhtml REQUIRED)

  # add coverage target
  add_custom_target(coverage
    # gather data
    COMMAND ${LCOV} --directory . --capture --output-file coverage.info
    COMMAND ${LCOV} --remove coverage.info '/usr/*' --output-file coverage.info
    COMMAND ${LCOV} --remove coverage.info 'tests/*' '*test.cpp' '*suite.cpp' --output-file coverage.info
    # generate report
    COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(src)
add_subdirectory(tests)
