cmake_minimum_required(VERSION 3.10)

add_definitions(-DSS_API_EXPORT)
add_library(libssbase SHARED 
    "rbtree.c"
    )