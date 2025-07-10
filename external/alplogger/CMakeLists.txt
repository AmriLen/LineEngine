# CMakeList.txt: проект CMake для alp_logger; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.

cmake_minimum_required(VERSION 3.16)

project(alplogger)

add_library(alplogger STATIC 
	include/alplogger/alplogger.h
	src/alplogger.cpp
)

target_include_directories(alplogger PUBLIC include)