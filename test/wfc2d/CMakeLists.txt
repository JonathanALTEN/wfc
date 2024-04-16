if (BUILD_TESTING)
    # Define the test executables
    add_executable(wfc2d_test wfc2d_tests.cpp)

    # Link each test executable with Google Test and your library
    target_link_libraries(wfc2d_test PRIVATE wfc gtest_main)

    # Define the tests
    include(GoogleTest)
    gtest_discover_tests(wfc2d_test)
endif()