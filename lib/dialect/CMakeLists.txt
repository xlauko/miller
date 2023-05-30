#
# miller program dialects
#
add_library( mi-dialects )

target_link_libraries( mi-dialects
  PUBLIC
    mi::coro
    mi::util
    mi::program
  INTERFACE
    miller_project_options
    miller_project_warnings

)

target_sources( mi-dialects
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      imp.mpp
)

add_library( mi::dialects ALIAS mi-dialects )
