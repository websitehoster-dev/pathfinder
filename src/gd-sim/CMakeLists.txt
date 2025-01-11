cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)
project(gd-sim VERSION 0.1.0 LANGUAGES CXX)

#set(DEMO 1)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/**/*.cpp)
add_library(gd-sim STATIC ${SOURCES})

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
target_compile_options(gd-sim PRIVATE $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -g> $<$<CONFIG:Release>:-O3 -fno-math-errno -fno-trapping-math -fno-rtti -fvisibility=hidden -fomit-frame-pointer>)

if (DEMO)
	target_compile_definitions(gd-sim PUBLIC -DDEMO=1)
endif()

target_compile_features(gd-sim PRIVATE cxx_std_20)
target_include_directories(gd-sim PUBLIC include)