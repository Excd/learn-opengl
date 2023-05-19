# CMake for LearnOpenGL tutorial projects.
# Author:		https://github.com/Excd/
# LearnOpenGL:	https://learnopengl.com/
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project variables.
set(PROJECT_NAME "LearnOpenGL")
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib")	# Source directory of libraries.
file(GLOB_RECURSE PROJECT_SOURCE "src/*.cpp" "src/*.h") 	# Project source files.

# Project statement.
project (
	${PROJECT_NAME}
	VERSION 1.0.0
	LANGUAGES C CXX
)

# CMake variables.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib") # Output directory of static libraries.

# Configure and add GLFW.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Ignore GLFW docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Ignore GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Ignore GLFW examples" FORCE)
add_subdirectory("${LIB_DIR}/glfw-3.3-stable" "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw")

# Add glad.
add_library(
	glad
	"${LIB_DIR}/glad/include/glad/glad.h"
	"${LIB_DIR}/glad/src/glad.c"
)
target_include_directories(glad PUBLIC "${LIB_DIR}/glad/include")

# Add source to project executable and link libraries.
add_executable (${EXECUTABLE_NAME} ${PROJECT_SOURCE})
target_link_libraries(
	${EXECUTABLE_NAME}
	PRIVATE glfw glad
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
		PROPERTY
			LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS"
	)
endif()