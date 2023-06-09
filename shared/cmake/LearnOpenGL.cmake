# Custom CMake module for LearnOpenGL tutorial projects.
# LearnOpenGL: https://learnopengl.com

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project variables.
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(LIBRARY_SOURCE_DIR "${SHARED_DIR}/external")
set(LIBRARY_BINARY_DIR "${SHARED_DIR}/bin")
set(INCLUDE_DIR "${SHARED_DIR}/include")
set(RESOURCE_DIR "${SHARED_DIR}/resources")
set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders")

# CMake variables.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_BINARY_DIR}/lib")

# Project source files.
file(
    GLOB_RECURSE PROJECT_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

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

# Include stb libraries.
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${INCLUDE_DIR}/stb")

# Include my custom libraries.
add_library(excd INTERFACE)
target_include_directories(excd INTERFACE "${INCLUDE_DIR}/excd")

# Add source to project executable and link libraries.
add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCE})
target_link_libraries(
	${EXECUTABLE_NAME}
	PRIVATE
        glfw
        glad
		stb
		excd
)

# Copy shared resources to build directory.
if(EXISTS ${RESOURCE_DIR})
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND}
			-E copy_directory_if_different
				${RESOURCE_DIR}
				"$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources"
	)
endif()

# Copy project shaders to build directory.
if(EXISTS ${SHADER_DIR})
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND}
			-E copy_directory_if_different
				${SHADER_DIR}
				"$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources/shaders"
	)
endif()

# Set C++ standard.
set_target_properties(
	${EXECUTABLE_NAME}
	PROPERTIES
		CXX_STANDARD 20
		CMAKE_CXX_STANDARD_REQUIRED True
)

# Use Windows subsystem with main entry.
if(WIN32)
	set_property(
		TARGET ${EXECUTABLE_NAME}
		PROPERTY LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS"
	)
endif()