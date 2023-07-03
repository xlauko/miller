#
# miller domain semantics
#
add_library( mi-sema )

target_link_libraries( mi-sema
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

target_sources( mi-sema
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      domain.mpp
      environment.mpp
      sema.mpp
      unit.mpp
)

add_library( mi::sema ALIAS mi-sema )
