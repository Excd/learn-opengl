# CMakeList.txt : CMake project for getting-started, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("getting-started")

# Variables
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Ignore GLFW docs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Ignore GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Ignore GLFW examples" FORCE)
add_subdirectory(${LIB_DIR}/glfw-3.3-stable glfw)

# glad
add_library(glad ${LIB_DIR}/glad/include/glad/glad.h ${LIB_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${LIB_DIR}/glad/include)

# Add source to this project's executable.
set(PROJECT_SOURCE "src/main.cpp" "src/main.h")
add_executable (getting-started ${PROJECT_SOURCE})
target_link_libraries(getting-started PRIVATE ${OPENGL_LIBRARIES} glfw glad)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET getting-started PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
