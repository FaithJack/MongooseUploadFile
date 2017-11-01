cmake_minimum_required(VERSION 2.8)

PROJECT(hello_2) 
FILE(GLOB SOURCE_1 "${CMAKE_SOURCE_DIR}/mycpp/*.cpp") #下面2句告诉CMake，源文件在哪里？main.cpp
FILE(GLOB SOURCE_2 "${CMAKE_SOURCE_DIR}/myc/*.c")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/") #告诉CMake头文件在哪里？
ADD_EXECUTABLE(hello_2 ${SOURCE_1} ${SOURCE_2})
ADD_DEFINITIONS(-DMG_ENABLE_HTTP_STREAMING_MULTIPART)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(hello_2 Threads::Threads)
