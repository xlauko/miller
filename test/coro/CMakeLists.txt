add_executable( miller-test-coro
    driver.cpp
    generator.cpp
    traits.cpp
)

target_link_libraries( miller-test-coro
    PRIVATE
        doctest::doctest
        mi::coro
    INTERFACE
        miller_project_options
        miller_project_warnings
)

target_compile_features( miller-test-coro PRIVATE cxx_std_23 )

target_include_directories( miller-test-coro
    PRIVATE ${DOCTEST_INCLUDE_DIR}
)

add_test(
  NAME test-coro
  COMMAND "$<TARGET_FILE:miller-test-coro>"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
