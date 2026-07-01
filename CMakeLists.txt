cmake_minimum_required(VERSION 4.2)
project(RBtree C)

set(CMAKE_C_STANDARD 11)

add_executable(RBtree main.c
        RBTree.h
        RBTree.c)
