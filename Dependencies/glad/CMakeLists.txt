add_library(glad "src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
target_include_directories(glad PRIVATE ${OPENGL_INCLUDE_DIR})
list(APPEND EXTRA_LIBS ${OPENGL_LIBRARIES})
