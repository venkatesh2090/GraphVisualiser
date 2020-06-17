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


file(COPY "${PROJECT_SOURCE_DIR}/GraphVisualiser/Shaders" DESTINATION "${PROJECT_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
target_include_directories(GraphVisualiser PRIVATE ${OPENGL_INCLUDE_DIR})
list(APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})

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

target_include_directories(GraphVisualiser PRIVATE "Dependencies/exprtk")
