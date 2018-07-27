cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link 'program' with what we want to test and the GTest and pthread library
INCLUDE_DIRECTORIES(${YOUR_DIRECTORY})
INCLUDE_DIRECTORIES(${YOUR_DIRECTORY}/src)
INCLUDE_DIRECTORIES(${YOUR_DIRECTORY}/headers)
INCLUDE_DIRECTORIES(${YOUR_DIRECTORY}/UnitTests)
add_executable(program 
	UnitTests/Task2_Test.cpp 
	src/FunctionTask.cpp 
	headers/FunctionTask.h)
target_link_libraries(program ${GTEST_LIBRARIES} pthread)
