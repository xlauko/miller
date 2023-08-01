#
# Copyright (c) 2023-present, Trail of Bits, Inc.
# All rights reserved.
#
# This source code is licensed in accordance with the terms specified in
# the LICENSE file found in the root directory of this source tree.
#

cmake_minimum_required(VERSION 3.27)

set( CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7" )
set( CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1 )

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

if (POLICY CMP0116)
  cmake_policy(SET CMP0116 NEW)
endif ()

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
list( APPEND VCPKG_FEATURE_FLAGS "versions" )

project( miller
    LANGUAGES C CXX
    VERSION 0.0.1
    DESCRIPTION "VAST MLIR Abstract Interpreter"
    HOMEPAGE_URL "https://github.com/xlauko/miller"
)

set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

find_package( gap CONFIG REQUIRED )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# prevent in source builds check
include( prevent_in_source_builds )

include( vcpkg )

# general project options
include( project_settings )

add_library( miller_project_options INTERFACE )
target_compile_features( miller_project_options INTERFACE cxx_std_23 )
target_include_directories( miller_project_options INTERFACE include )

# link miller to use the warnings specified in compiler_warnings.cmake
add_library( miller_project_warnings INTERFACE )

# standard compiler warnings
include( compiler_warnings )
set_project_warnings( miller_project_warnings )

# sanitizer options if supported by compiler
include( sanitizers )
enable_sanitizers( miller_project_options )

# clang time profiling
if ( CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" )
  option( MILLER_ENABLE_BUILD_WITH_TIME_TRACE
    "Enable -ftime-trace to generate time tracing .json files on clang" OFF
  )
  if ( MILLER_ENABLE_BUILD_WITH_TIME_TRACE )
    add_compile_definitions( miller_project_options INTERFACE -ftime-trace )
  endif()
endif()

# allow for static analysis options
include( static_analyzers )

#
# dependencies
#

find_package( doctest CONFIG REQUIRED )
find_package( spdlog CONFIG REQUIRED )

find_path( REFL_CPP_INCLUDE_DIRS "refl.hpp" )
add_library( refl-cpp INTERFACE )
target_include_directories( refl-cpp INTERFACE ${REFL_CPP_INCLUDE_DIRS} )

#
# LLVM & MLIR & Clang
#
find_package( LLVM 16 REQUIRED CONFIG )
message( STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}" )
message( STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}" )

#
# MLIR
#
find_package( MLIR REQUIRED CONFIG )
message( STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}" )

#
# LLVM Libraries
#

list( APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}" )
list( APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}" )

include( TableGen )
include( AddLLVM )
include( AddMLIR )

#
# miller libraries
#
add_subdirectory( include/miller )
add_subdirectory( lib )

#
# miller tools
#
add_subdirectory( tools )

include( GNUInstallDirs )

option( MILLER_ENABLE_TESTING "Enable Test Builds" ON )

if ( MILLER_ENABLE_TESTING )
  enable_testing()
  add_subdirectory( test )
endif()
