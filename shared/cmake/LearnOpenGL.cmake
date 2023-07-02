# CMake module by excd for LearnOpenGL tutorial projects.
# LearnOpenGL: https://learnopengl.com

# Project variables.
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(LIBRARY_SOURCE_DIR "${SHARED_DIR}/external")
set(LIBRARY_BINARY_DIR "${SHARED_DIR}/bin")
set(INCLUDE_DIR "${SHARED_DIR}/include")
set(SHARED_RESOURCE_DIR "${SHARED_DIR}/resources")
set(LOCAL_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")

# Project source files.
include_directories("${SHARED_DIR}/src")
file(
    GLOB_RECURSE PROJECT_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${SHARED_DIR}/src/*.cpp"
        "${SHARED_DIR}/src/*.h"
)

# Configure CMake.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_BINARY_DIR}/lib")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(
      CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
  )
endif()

# Configure GLFW.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Ignore GLFW docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Ignore GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Ignore GLFW examples" FORCE)

# Configure GLM.
set(GLM_TEST_ENABLE OFF CACHE BOOL "Ignore GLM tests" FORCE)

# Add libraries.
add_subdirectory(
    "${LIBRARY_SOURCE_DIR}/glfw-3.3-stable"
    "${LIBRARY_BINARY_DIR}/glfw"
	EXCLUDE_FROM_ALL
)
add_subdirectory(
    "${LIBRARY_SOURCE_DIR}/glm-stable"
    "${LIBRARY_BINARY_DIR}/glm"
	EXCLUDE_FROM_ALL
)
add_library(
	glad EXCLUDE_FROM_ALL
	"${LIBRARY_SOURCE_DIR}/glad/include/glad/glad.h"
	"${LIBRARY_SOURCE_DIR}/glad/src/glad.c"
)
add_library(stb INTERFACE EXCLUDE_FROM_ALL)
add_library(excd INTERFACE EXCLUDE_FROM_ALL)

# Include library directories.
target_include_directories(
    glad PUBLIC "${LIBRARY_SOURCE_DIR}/glad/include"
    stb INTERFACE "${INCLUDE_DIR}/stb"
    excd INTERFACE "${INCLUDE_DIR}/excd"
)

# Add source to project executable and link libraries.
add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCE})
target_link_libraries(
	${EXECUTABLE_NAME}
	PRIVATE
        glfw
		glm
        glad
		stb
		excd
)

# Set C and C++ standard.
set_target_properties(
	${EXECUTABLE_NAME}
	PROPERTIES
		C_STANDARD 11
		CMAKE_C_STANDARD_REQUIRED True
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

# Copy resources to build directory.
if(EXISTS ${SHARED_RESOURCE_DIR})
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND}
			-E copy_directory_if_different
				${SHARED_RESOURCE_DIR}
				"$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources"
	)
endif()
if(EXISTS ${LOCAL_RESOURCE_DIR})
	add_custom_command(
		TARGET ${EXECUTABLE_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND}
			-E copy_directory_if_different
				${LOCAL_RESOURCE_DIR}
				"$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources"
	)
endif()