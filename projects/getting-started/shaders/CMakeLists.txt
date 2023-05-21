# CMake for LearnOpenGL tutorial projects.
# Author:		https://github.com/excd
# LearnOpenGL:	https://learnopengl.com
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project variables.
set(PROJECT_NAME "LearnOpenGL")
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(REPO_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../..")   # Root directory of the repository.
set(INCLUDE_DIR "${REPO_ROOT_DIR}/include")                 # Include directory.
set(LIBRARY_SOURCE_DIR "${REPO_ROOT_DIR}/external")         # External library source directory.
set(LIBRARY_BINARY_DIR "${REPO_ROOT_DIR}/bin")              # Library binary directory.
# Project source files.
file(
    GLOB_RECURSE PROJECT_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# Project statement.
project (
	${PROJECT_NAME}
	VERSION 1.0.0
	LANGUAGES C CXX
)

# CMake variables.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib") # Output directory of static libraries.

# Include custom debugger output library.
add_library(debug_out INTERFACE)
target_include_directories(debug_out INTERFACE "${INCLUDE_DIR}/debugout")

# Configure and add GLFW.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Ignore GLFW docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Ignore GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Ignore GLFW examples" FORCE)
add_subdirectory("${LIBRARY_SOURCE_DIR}/glfw-3.3-stable" "${LIBRARY_BINARY_DIR}/glfw")

# Include glad.
add_library(
	glad
	"${LIBRARY_SOURCE_DIR}/glad/include/glad/glad.h"
	"${LIBRARY_SOURCE_DIR}/glad/src/glad.c"
)
target_include_directories(glad PUBLIC "${LIBRARY_SOURCE_DIR}/glad/include")

# Add source to project executable and link libraries.
add_executable (${EXECUTABLE_NAME} ${PROJECT_SOURCE})
target_link_libraries(
	${EXECUTABLE_NAME}
	PRIVATE
        glfw
        glad
        debug_out
)

# Set C++ standard.
set_target_properties(
	${EXECUTABLE_NAME}
	PROPERTIES
		CXX_STANDARD 20
		CMAKE_CXX_STANDARD_REQUIRED True
)

# Use Windows subsystem with main entry.
if (WIN32)
	set_property(
		TARGET ${EXECUTABLE_NAME}
		PROPERTY LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS"
	)
endif()