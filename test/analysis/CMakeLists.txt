add_executable( miller-test-analysis
    driver.cpp
    forward.cpp
)

target_link_libraries( miller-test-analysis
    PRIVATE
        doctest::doctest
        spdlog::spdlog
        mi::analysis
        mi::dialects
        mi::program
        mi::sema
    INTERFACE
        miller_project_options
        miller_project_warnings
)

target_compile_features( miller-test-analysis PRIVATE cxx_std_23 )

target_include_directories( miller-test-analysis
    PRIVATE ${DOCTEST_INCLUDE_DIR}
)

add_test(
  NAME test-analysis
  COMMAND "$<TARGET_FILE:miller-test-analysis>"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
