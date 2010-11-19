cmake_minimum_required(VERSION 2.4)
project( AR )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

link_directories( ${LIBRARY_OUTPUT_PATH} )
include_directories( "${AR_SOURCE_DIR}/include" )

# ARToolkitPlus core files (headers and sources)
file(GLOB_RECURSE HEADERS_AR "${AR_SOURCE_DIR}/include/ARToolkitPlus/*.h")
file(GLOB SOURCES_AR
	"${AR_SOURCE_DIR}/src/*.cpp"
	"${AR_SOURCE_DIR}/src/core/*.cpp"
	"${AR_SOURCE_DIR}/src/extra/*.cpp"
	"${AR_SOURCE_DIR}/src/librpp/*.cpp"
	)

add_library(ARToolkitPlus
	SHARED
	${HEADERS_AR}
	${SOURCES_AR}
)

set_target_properties(ARToolkitPlus PROPERTIES DEBUG_POSTFIX d)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DAR_LIBRARY)
else(WIN32)
	set(CMAKE_C_FLAGS_DEBUG "-Wall -g ")
	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g ")
endif(WIN32)

if(SLB_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif(SLB_BUILD_TESTS)

if(SLB_BUILD_EXAMPLES)
	file(GLOB ALL_EXAMPLES "${SLB_SOURCE_DIR}/examples/*")
	foreach(example ${ALL_EXAMPLES})
		add_subdirectory(${example})
	endforeach(example)
endif(SLB_BUILD_EXAMPLES)
