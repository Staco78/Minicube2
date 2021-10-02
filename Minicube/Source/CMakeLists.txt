include_directories(${CMAKE_SOURCE_DIR}/Minicube/Header)

file(GLOB_RECURSE SRCS *.cpp *.vertex *.fragment)
file(GLOB_RECURSE HDRS *.h)

add_executable(Minicube ${SRCS} ${HDRS})
target_link_libraries(Minicube glfw3 glew32)