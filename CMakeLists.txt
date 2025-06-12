cmake_minimum_required(VERSION 3.20)

project(anneal)

add_executable(anneal
	src/main.cpp
)

target_include_directories(anneal PRIVATE ${CMAKE_SOURCE_DIR}/include)
