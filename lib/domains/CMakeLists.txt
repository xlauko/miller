#
# miller domain semantics
#
add_library( mi-domains )

target_link_libraries( mi-domains
  PUBLIC
    mi::coro
    mi::program
    mi::util
  INTERFACE
    miller_project_options
    miller_project_warnings
  PRIVATE
    spdlog::spdlog
    spdlog::spdlog_header_only
    refl-cpp
)

target_sources( mi-domains
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      domain.ccm
      environment.ccm
      domains.ccm
      unit.ccm
      vars.ccm
)

add_library( mi::domains ALIAS mi-domains )
