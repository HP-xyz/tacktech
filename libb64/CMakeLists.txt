project(libb64)

cmake_minimum_required(VERSION 2.8)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared instead of static library")

include_directories(
	${libb64_SOURCE_DIR}/include)

set(SOURCES 
	${libb64_SOURCE_DIR}/src/cdecode.c
	${libb64_SOURCE_DIR}/src/cencode.c)
	
if(BUILD_SHARED_LIBS)
    add_library(libb64 SHARED ${SOURCES})
else()
    add_library(libb64 STATIC ${SOURCES})
endif()

install(TARGETS libb64
		ARCHIVE
		DESTINATION lib)