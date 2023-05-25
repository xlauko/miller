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
)

target_sources( mi-util
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      bigint.mpp
      box.mpp
      util.mpp
)

add_library( mi::util ALIAS mi-util )
