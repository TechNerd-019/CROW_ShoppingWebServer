cmake_minimum_required(VERSION 3.7)

project(Salah-CSCN72050-Lab10)

set(CMAKE_CXX_STANDARD 20)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(Salah-CSCN72050-Lab10 Source.cpp)
target_link_libraries(Salah-CSCN72050-Lab10 ${Boost_LIBRARIES} Threads::Threads)
