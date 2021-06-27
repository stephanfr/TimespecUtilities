cmake_minimum_required(VERSION 3.14.0)
project(TimespecUtilities VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,-fuchsia-default-arguments")

# Determine if TimespecUtilities is built as a subproject (using add_subdirectory)
# or if it is the master project.
if (NOT DEFINED TIMESPECUTILITIES_IS_MASTER_PROJECT)
  set(TIMESPECUTILITIES_IS_MASTER_PROJECT OFF)
  if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(TIMESPECUTILITIES_IS_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
    message(STATUS "Timespec Utilities is master project")
  endif ()
endif ()

include(FetchContent)

enable_testing()

include_directories(include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if( TIMESPECUTILITIES_IS_MASTER_PROJECT )
  add_subdirectory("unit_tests")
endif()
