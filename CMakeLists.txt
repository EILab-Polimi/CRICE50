cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(noncoop)
set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory(src)

add_executable(RICE50++ src/RICEMain.cpp)
target_link_libraries(RICE50++ rice moeas carbons climates economies)

file(COPY settings DESTINATION ${CMAKE_BINARY_DIR})
file(COPY data_ed57 DESTINATION ${CMAKE_BINARY_DIR})
file(COPY opt DESTINATION ${CMAKE_BINARY_DIR})

