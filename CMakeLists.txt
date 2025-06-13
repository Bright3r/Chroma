cmake_minimum_required(VERSION 3.20)

project(chroma)

add_executable(chroma
	src/main.cpp
	src/Chroma.cpp
)

target_include_directories(chroma PRIVATE ${CMAKE_SOURCE_DIR}/include)
