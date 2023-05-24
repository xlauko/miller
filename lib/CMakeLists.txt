add_subdirectory( util )

add_subdirectory( coro )
add_subdirectory( dialect )
add_subdirectory( program )
#
# miller config module
#
add_library( mi-config )

target_link_libraries( mi-config
  INTERFACE
    miller_project_options
    miller_project_warnings
)

target_sources( mi-config
  PUBLIC
    FILE_SET cxx_modules
    TYPE CXX_MODULES
    FILES
      config.mpp
  PUBLIC
    FILE_SET cxx_header_units
    BASE_DIRS
      $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>
    TYPE CXX_MODULE_HEADER_UNITS
    FILES
      $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/miller/version.h>
)

target_include_directories( mi-config PUBLIC
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>
  $<INSTALL_INTERFACE:include>
)

add_library( mi::config ALIAS mi-config )

#
# miller domains module
#
add_library( mi-domains )

target_link_libraries( mi-domains
  INTERFACE
    miller_project_options
    miller_project_warnings
)

target_sources( mi-domains
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES domain.mpp
)

add_library( mi::domains ALIAS mi-domains )
