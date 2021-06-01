cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(noncoop)
list(APPEND CMAKE_PREFIX_PATH "/Users/angelocarlino/models/libtorch")
find_package(Torch REQUIRED)
message(STATUS "passed")
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

add_executable(RICE50++ src/RICEMain.cpp)
target_link_libraries(RICE50++ rice moeas carbons climates economies opt)
file(COPY settings DESTINATION ${CMAKE_BINARY_DIR})
file(COPY data_ed57 DESTINATION ${CMAKE_BINARY_DIR})
file(COPY opt DESTINATION ${CMAKE_BINARY_DIR})

