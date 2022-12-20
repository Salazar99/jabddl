cmake_minimum_required(VERSION 3.22.1)
project(jabddl
        VERSION 1.0
        DESCRIPTION "A library to manage BDDs"
        LANGUAGES CXX
)
add_executable(jabddl

    include/jabddl.hpp
    src/jabddl.cpp
    include/vertex.hpp
    src/vertex.cpp
    include/expr.hpp
    src/expr.cpp
    src/main.cpp)

add_executable(jabddl_test

    include/jabddl.hpp
    src/jabddl.cpp
    include/vertex.hpp
    src/vertex.cpp
    include/expr.hpp
    src/expr.cpp
    src/test.cpp)

 target_compile_features(jabddl PRIVATE cxx_std_17)   
 target_compile_options(jabddl PRIVATE -Wall -Wextra -Wpedantic)
 
 target_compile_features(jabddl_test PRIVATE cxx_std_17)   
 target_compile_options(jabddl_test PRIVATE -Wall -Wextra -Wpedantic)