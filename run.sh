#! /bin/sh
g++-11 -g --std=c++17 main_test.cpp jabddl.cpp -o jabddl
./jabddl -f input.txt 