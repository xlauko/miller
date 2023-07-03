add_executable( miller-test-util
    driver.cpp
    function.cpp
)

target_link_libraries( miller-test-util
    PRIVATE
        doctest::doctest
        mi::util
    INTERFACE
        miller_project_options
        miller_project_warnings
)

target_compile_features( miller-test-util PRIVATE cxx_std_23 )

target_include_directories( miller-test-util
    PRIVATE ${DOCTEST_INCLUDE_DIR}
)

add_test(
  NAME test-util
  COMMAND "$<TARGET_FILE:miller-test-util>"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
