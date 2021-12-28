cmake_minimum_required(VERSION 3.18)

project(Minicube)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Bin)
link_libraries(${OPENGL_LIBRARY} GLEW freetype)
link_directories(${CMAKE_SOURCE_DIR}/Lib)
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty)

include_directories(${CMAKE_SOURCE_DIR}/Minicube/Header)

file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/Minicube/Source/*.cpp)
file(GLOB_RECURSE HDRS *.h)

add_executable(Minicube ${SRCS} ${HDRS})
target_link_libraries(Minicube glfw)

set(THREADS_PREFER_PTHREADS_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Minicube Threads::Threads)
