cmake_minimum_required(VERSION 3.10)

add_executable(test_libssbase
    "test_main.cpp"
    "test_rbtree.cpp"
    )

find_package(GTest CONFIG REQUIRED)
target_link_libraries(test_libssbase
    libssbase
    GTest::gtest
    )
