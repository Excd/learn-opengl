cmake_minimum_required(VERSION 3.23)

# Project variables.
set(PROJECT_NAME "LearnOpenGL")
set(SHARED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../shared")

# Project statement.
project(
	${PROJECT_NAME}
	VERSION 1.0.0
	LANGUAGES C CXX
)

# Load shared CMake module.
include(${SHARED_DIR}/cmake/LearnOpenGL.cmake)