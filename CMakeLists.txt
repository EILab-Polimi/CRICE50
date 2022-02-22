# cmake version
# cmake_minimum_required (VERSION 3.0)
# project(RICE)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
# set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# # set (DLIB_PATH /Users/angelocarlino/models/dlib)

# set (SOURCES
# 	src/RICEMain.cpp src/RICE.cpp src/economy/Econ.cpp 
# 	src/carbon/Carbon.cpp src/climate/Climate.cpp 
# 	src/economy/EconAgent.cpp src/emodps/moeaframework.c
# 	)

# add_executable(RICE50++ ${SOURCES})


# add_subdirectory(../dlib dlib_build)


# target_link_libraries(RICE50++ dlib:dlib)



cmake_minimum_required(VERSION 2.8.12)
# Every project needs a name.  We call this the "examples" project.
project(RICE50)

set (SOURCES
	src/RICEMain.cpp src/RICE.cpp src/economy/Econ.cpp 
	src/carbon/Carbon.cpp src/climate/Climate.cpp 
	src/economy/EconAgent.cpp src/moeaframework/moeaframework.c
	src/moeaframework/param_function.cpp src/moeaframework/ncRBF.cpp 
	src/moeaframework/rbf.cpp src/moeaframework/ann.cpp 
	src/moeaframework/ann_mo.cpp src/moeaframework/pwLinear.cpp 
	src/moeaframework/utils.cpp src/moeaframework/param_policy.cpp
	)


# Tell cmake we will need dlib.  This command will pull in dlib and compile it
# into your project.  Note that you don't need to compile or install dlib.  All
# cmake needs is the dlib source code folder and it will take care of everything.
add_subdirectory(../dlib dlib_build)

# If you have cmake 3.14 or newer you can even use FetchContent instead of
# add_subdirectory() to pull in dlib as a dependency.  So instead of using the
# above add_subdirectory() command, you could use the following three commands
# to make dlib available:
#  include(FetchContent)
#  FetchContent_Declare(dlib
#      GIT_REPOSITORY https://github.com/davisking/dlib.git
#      GIT_TAG        v19.18
#  )
#  FetchContent_MakeAvailable(dlib)


# The next thing we need to do is tell CMake about the code you want to
# compile.  We do this with the add_executable() statement which takes the name
# of the output executable and then a list of .cpp files to compile.  Here we
# are going to compile one of the dlib example programs which has only one .cpp
# file, assignment_learning_ex.cpp.  If your program consisted of multiple .cpp
# files you would simply list them here in the add_executable() statement.  
add_executable(RICE50 ${SOURCES})
# Finally, you need to tell CMake that this program, assignment_learning_ex,
# depends on dlib.  You do that with this statement: 
target_link_libraries(RICE50 dlib::dlib)

add_custom_command(TARGET RICE50 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/settings $<TARGET_FILE_DIR:RICE50>/settings)

add_custom_command(TARGET RICE50 PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data_ed57 $<TARGET_FILE_DIR:RICE50>/data_ed57)

