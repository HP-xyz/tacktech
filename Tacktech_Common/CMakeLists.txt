project(Tacktech_Common)

cmake_minimum_required(VERSION 2.8)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared instead of static library")

include_directories(
		${Boost_INCLUDE_DIRS}
		${PUGIXML_INCLUDE_DIR}
		)

set(TACKTECH_COMMON_HDRS 
	${Tacktech_Common_SOURCE_DIR}/src/Group_Container.h
	${Tacktech_Common_SOURCE_DIR}/src/Group_Playlist_Container.h
	${Tacktech_Common_SOURCE_DIR}/src/Playlist_Container.h
	)
set(TACKTECH_COMMON_SRCS
	${Tacktech_Common_SOURCE_DIR}/src/Group_Container.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Group_Playlist_Container.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Playlist_Container.cpp
	)

if(BUILD_SHARED_LIBS)
    add_library(Tacktech_Common
				SHARED
				${TACKTECH_COMMON_HDRS}
				${TACKTECH_COMMON_SRCS}
				)
	target_link_libraries(Tacktech_Common
							pugixml)
else(BUILD_SHARED_LIBS)
    add_library(Tacktech_Common
				STATIC
				${TACKTECH_COMMON_HDRS}
				${TACKTECH_COMMON_SRCS}
				)
	target_link_libraries(Tacktech_Common
				pugixml)
endif(BUILD_SHARED_LIBS)
