#
# miller program representation
#
add_library( mi-analysis )

target_link_libraries( mi-analysis
  PUBLIC
    mi::coro
    mi::util
    mi::program
    mi::sema
  INTERFACE
    miller_project_options
    miller_project_warnings
  PRIVATE
    spdlog::spdlog
    spdlog::spdlog_header_only
)

target_sources( mi-analysis
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      analysis.mpp
      dataflow.mpp
      forward.mpp
      result.mpp
)

add_library( mi::analysis ALIAS mi-analysis )
