cmake_minimum_required(VERSION 3.3)
project(TrackingShot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(OpenGL/include)
include_directories(glad)

link_directories(OpenGL/lib)

set(SOURCE_FILES main.cpp OpenGL/glad/glad.c Shaders/Shader.cpp Shaders/Shader.h Lib/Camera.cpp Lib/Camera.h Lib/Datatypes/PathPoint.cpp Lib/Datatypes/PathPoint.h Lib/Datatypes/Spline.cpp Lib/Datatypes/Spline.h Lib/Objects/Cube.h Lib/Objects/Plane.h Lib/Objects/Object.h Lib/Consts.h)
add_executable(TrackingShot ${SOURCE_FILES})

find_library(glfw3 REQUIRED)
target_link_libraries(TrackingShot glfw3 opengl32)