cmake_minimum_required(VERSION 3.17)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # not necessary, but encouraged

project(demo)

#
# set vars
#
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
SET(LIB_NAME "simple-zmq-server")
SET(DEMO_BINARY "__demo__")
SET(INCLUDE_DIR "/usr/local/include")

#
#  wrangle files
#
find_library(${LIB_NAME} STATIC lib)

include_directories(${INCLUDE_DIR})

file(GLOB EXE_SOURCES "src/*.cc")


#
# link
#
add_executable(${DEMO_BINARY} ${EXE_SOURCES})

target_link_libraries(${DEMO_BINARY} PUBLIC ${LIB_NAME})


# still need to link these...
#
find_package(cppzmq)
target_link_libraries(${DEMO_BINARY} PUBLIC cppzmq)



