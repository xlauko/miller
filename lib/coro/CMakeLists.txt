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
      concepts.mpp
      coro.mpp
      fmap.mpp
      generator.mpp
      scope.mpp
      task.mpp
)

add_library( mi::coro ALIAS mi-coro )
