#
# miller program representation
#
add_library( mi-program )

target_link_libraries( mi-program
  PUBLIC
    mi::coro
    mi::util
  INTERFACE
    miller_project_options
    miller_project_warnings
  PRIVATE
    spdlog::spdlog
    spdlog::spdlog_header_only
    refl-cpp
)

target_sources( mi-program
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      cfg.mpp
      operation.mpp
      program.mpp
)

add_library( mi::program ALIAS mi-program )
