cmake_minimum_required(VERSION 3.10)

project(GraphVisualiser VERSION 1.0)
add_executable(GraphVisualiser Hello.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# GLFW dependencies
set(GLFW_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw")

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)

add_subdirectory("${GLFW_SOURCE}")
include_directories(GraphVisualiser PRIVATE "${GLFW_SOURCE}/include")
list(APPEND EXTRA_LIBS glfw)


# GLM dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glm")

# GLAD dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glad")
list(APPEND EXTRA_LIBS glad)

target_link_libraries(GraphVisualiser PRIVATE ${EXTRA_LIBS})

target_include_directories(GraphVisualiser PRIVATE "Dependencies/exprtk")
