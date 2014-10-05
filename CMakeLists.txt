cmake_minimum_required(VERSION 2.8)
project(ARToolKitPlus)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

option(ARTK_BUILD_EXAMPLES "Build sample programs and utilies" FALSE)
option(ARTK_USE_BOOST "Compile with boost (OpenCV calibration file support)" FALSE)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DAR_LIBRARY)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "release" CACHE STRING "" FORCE)
endif ()

if(ARTK_USE_BOOST)
    add_definitions(-DAR_WITH_BOOST)
endif(ARTK_USE_BOOST)

set(AR_SOURCE_DIR ${PROJECT_SOURCE_DIR})
include_directories(${AR_SOURCE_DIR}/include)

# ARToolkitPlus core files (headers and sources)
file(GLOB_RECURSE HEADERS_AR ${AR_SOURCE_DIR}/include/ARToolKitPlus/*.h)
file(GLOB SOURCES_AR
	${AR_SOURCE_DIR}/src/*.cpp
	${AR_SOURCE_DIR}/src/core/*.cpp
	${AR_SOURCE_DIR}/src/extra/*.cpp
	${AR_SOURCE_DIR}/src/librpp/*.cpp
	)

add_library(ARToolKitPlus SHARED ${HEADERS_AR} ${SOURCES_AR})

if(ARTK_BUILD_EXAMPLES)
    add_subdirectory(bin/)
endif(ARTK_BUILD_EXAMPLES)

# make install
install(TARGETS ARToolKitPlus 
LIBRARY DESTINATION lib  
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

