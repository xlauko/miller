add_subdirectory( util )

add_subdirectory( analysis )
add_subdirectory( coro )
add_subdirectory( dialect )
add_subdirectory( domains )
add_subdirectory( lsp )
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
)

target_include_directories( mi-config PUBLIC
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>
  $<INSTALL_INTERFACE:include>
)

add_library( mi::config ALIAS mi-config )
