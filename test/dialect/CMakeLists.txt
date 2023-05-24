add_executable( miller-test-dialects
    driver.cpp
    core.cpp
)

target_link_libraries( miller-test-dialects
    PRIVATE
        doctest::doctest
        mi::dialects
    INTERFACE
        miller_project_options
        miller_project_warnings
)

target_compile_features( miller-test-dialects PRIVATE cxx_std_23 )

target_include_directories( miller-test-dialects
    PRIVATE ${DOCTEST_INCLUDE_DIR}
)

add_test(
  NAME test-dialects
  COMMAND "$<TARGET_FILE:miller-test-dialects>"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
