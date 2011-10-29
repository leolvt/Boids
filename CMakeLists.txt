# Initial setup
cmake_minimum_required(VERSION 2.6)
project(Boids)

# Help cmake find custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/cmake"
  "${CMAKE_SOURCE_DIR}/cmake/Modules"
)

# Find the GL libraries we are using
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)

# Find GLM Lib needed for math
find_package(GLM REQUIRED)

# Gather all the libs into one variable
set(BOIDS_LIBS 
    ${BOIDS_LIBS} 
    ${OPENGL_LIBRARIES} 
    ${GLFW_LIBRARIES} 
    ${GLEW_LIBRARY}
)

# Define paths to include and link
include_directories(
    ${PROJECT_SOURCE_DIR}/include 
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Add our source dir to cmake
add_subdirectory(src)
