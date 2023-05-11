#
# Copyright (c) 2023-present, Trail of Bits, Inc.
# All rights reserved.
#
# This source code is licensed in accordance with the terms specified in
# the LICENSE file found in the root directory of this source tree.
#

cmake_minimum_required(VERSION 3.26)

set( CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a" )
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
target_compile_features( miller_project_options INTERFACE cxx_std_26 )
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

#
# miller libraries
#
add_subdirectory( lib )

add_library( miller INTERFACE )
target_link_libraries( miller
  INTERFACE
    mi
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include( GNUInstallDirs )

option( MILLER_ENABLE_TESTING "Enable Test Builds" ON )

if ( MILLER_ENABLE_TESTING )
  enable_testing()
  add_subdirectory( test )
endif()
