#
# miller coroutine definitions
#
add_library( mi-coro )

target_link_libraries( mi-coro
  INTERFACE
    miller_project_options
    miller_project_warnings
)

target_sources( mi-coro
  PUBLIC
    FILE_SET miller_modules
    TYPE CXX_MODULES
    FILES
      concepts.ccm
      coro.ccm
      fmap.ccm
      generator.ccm
      recursive_generator.ccm
      scope.ccm
      task.ccm
)

add_library( mi::coro ALIAS mi-coro )
