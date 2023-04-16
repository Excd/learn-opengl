# Excd's CMake for LearnOpenGL tutorial projects.
# Author:		https://github.com/Excd/
# LearnOpenGL:	https://learnopengl.com/
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project
project ("LearnOpenGL")

# Set static library output directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# Variables
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib) # Directory of library sources.

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Ignore GLFW docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Ignore GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Ignore GLFW examples" FORCE)
add_subdirectory(${LIB_DIR}/glfw-3.3-stable ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)

# glad
add_library(
	glad
	${LIB_DIR}/glad/include/glad/glad.h
	${LIB_DIR}/glad/src/glad.c
)
target_include_directories(glad PUBLIC ${LIB_DIR}/glad/include)

# Add source to this project's executable.
set(PROJECT_SOURCE "src/main.cpp" )
add_executable (${CMAKE_PROJECT_NAME} ${PROJECT_SOURCE})

# Link libraries.
target_link_libraries(
	${CMAKE_PROJECT_NAME}
	PRIVATE ${OPENGL_LIBRARIES} glfw glad
)

# Set C++ standard.
set_property(
	TARGET ${CMAKE_PROJECT_NAME}
	PROPERTY
		CXX_STANDARD 20
)

# Hide command prompt on Windows.
if (WIN32)
	set_property(
		TARGET ${CMAKE_PROJECT_NAME}
		PROPERTY
			LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS"
	)
endif()