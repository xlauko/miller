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
)

target_sources( mi-util
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      bigint.mpp
      box.mpp
      concepts.mpp
      format.mpp
      function.mpp
      observer.mpp
      overloaded.mpp
      refl.mpp
      report.mpp
      string.mpp
      tristate.mpp
      util.mpp
)

add_library( mi::util ALIAS mi-util )
