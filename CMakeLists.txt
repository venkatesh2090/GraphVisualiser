cmake_minimum_required(VERSION 3.10)

project(GraphVisualiser VERSION 1.0)

add_executable(GraphVisualiser 
			   "GraphVisualiser/Application.cpp"
			   "GraphVisualiser/plot.cpp"
			   "GraphVisualiser/shader.cpp"
			   "GraphVisualiser/plot.h"
			   "GraphVisualiser/shader.h"
			  )


if (WIN32)
	target_compile_options(GraphVisualiser PRIVATE "/bigobj")
endif()

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/Dependencies/glm/CMakeLists.txt  AND NOT EXISTS ${PROJECT_SOURCE_DIR}/Dependencies/exprtk/CMakeLists.txt AND NOT EXISTS ${PROJECT_SOURCE_DIR}/Dependencies/glfw/CMakeLists.txt)
	message(FATAL_ERROR "Please run \n\"git submodule update --init\"")
endif()

file(COPY "${PROJECT_SOURCE_DIR}/GraphVisualiser/Shaders" DESTINATION "${PROJECT_BINARY_DIR}")

set(SHADER_PATH "${PROJECT_BINARY_DIR}/Shaders")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

configure_file(GraphVisualiser/GraphVisualiser.h.in GraphVisualiser.h)

# GLFW dependencies
set(GLFW_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw")

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)

add_subdirectory("${GLFW_SOURCE}")
include_directories("${GLFW_SOURCE}/include")
list(APPEND EXTRA_LIBS glfw)

# GLM dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glm")
list(APPEND EXTRA_LIBS glm)

# GLAD dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glad")
list(APPEND EXTRA_LIBS glad)

target_link_libraries(GraphVisualiser PRIVATE ${EXTRA_LIBS})

target_include_directories(GraphVisualiser PRIVATE "Dependencies/exprtk"
													${PROJECT_BINARY_DIR})
