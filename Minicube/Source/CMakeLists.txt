include_directories(${CMAKE_SOURCE_DIR}/Minicube/Header)

file(GLOB_RECURSE SRCS *.cpp *.vertex *.fragment)
file(GLOB_RECURSE HDRS *.h)

add_executable(Minicube ${SRCS} ${HDRS})
target_link_libraries(Minicube glfw)

set(THREADS_PREFER_PTHREADS_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Minicube Threads::Threads)