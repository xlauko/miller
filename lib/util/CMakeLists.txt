#
# miller util representation
#
add_library( mi-util )

target_link_libraries( mi-util
  INTERFACE
    miller_project_options
    miller_project_warnings
  PRIVATE
    spdlog::spdlog
    spdlog::spdlog_header_only
    refl-cpp
    nlohmann_json::nlohmann_json
)

target_sources( mi-util
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      bigint.ccm
      box.ccm
      concepts.ccm
      format.ccm
      function.ccm
      observer.ccm
      overloaded.ccm
      refl.ccm
      report.ccm
      stdint.ccm
      string.ccm
      strong_type.ccm
      tristate.ccm
      util.ccm
)

add_library( mi::util ALIAS mi-util )
