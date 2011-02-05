cmake_minimum_required(VERSION 2.8)
project(ARToolKitPlus)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)


if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DAR_LIBRARY)
else(WIN32)
    add_definitions(-Wall -g -O3)
endif(WIN32)


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

if(BUILD_EXAMPLES)
    add_subdirectory(bin/)
endif(BUILD_EXAMPLES)

# make install
set(CMAKE_INSTALL_PREFIX /usr)
install(TARGETS ARToolKitPlus LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

