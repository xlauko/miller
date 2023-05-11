add_library( mi )

target_link_libraries( mi
  INTERFACE
    vai_project_options
    vai_project_warnings
)

target_sources( mi
  PUBLIC
    FILE_SET cxx_modules
    TYPE CXX_MODULES
    FILES domain.cxx
)
