cmake_minimum_required(VERSION 3.20)

project(Chroma)

add_executable(Chroma
	src/main.cpp
	src/Chroma.cpp
)

target_include_directories(Chroma PRIVATE ${CMAKE_SOURCE_DIR}/include)
