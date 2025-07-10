# CMakeList.txt: проект CMake для GLAD; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.

cmake_minimum_required(VERSION 3.16)

project(glad)

add_library(glad STATIC 
	include/glad/glad.h
	src/glad.c
)

target_include_directories(glad PUBLIC include)