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
  PRIVATE
    refl-cpp
)

target_sources( mi-dialects
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      dialects.ccm
      imp.ccm
)

add_library( mi::dialects ALIAS mi-dialects )
