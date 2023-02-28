cmake_minimum_required(VERSION 3.16)
project(YADERCA C)

add_compile_options(-m32)
add_link_options(-m32)

add_executable(YADERCA main.c)
