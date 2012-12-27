project(Tacktech_Common)

cmake_minimum_required(VERSION 2.8)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared instead of static library")

link_libraries(${QT_LIBRARIES})
include_directories (${QT_INCLUDES})

include_directories(
		${Boost_INCLUDE_DIRS}
		${PUGIXML_INCLUDE_DIR}
		)

set (TACKTECH_COMMON_MOC_HDRS
    ${Tacktech_Common_SOURCE_DIR}/src/Tacktech_Network_Manager.h
    )
    
		
set(TACKTECH_COMMON_HDRS 
	${Tacktech_Common_SOURCE_DIR}/src/Group_Container.h
	${Tacktech_Common_SOURCE_DIR}/src/Group_Playlist_Container.h
	${Tacktech_Common_SOURCE_DIR}/src/Playlist_Container.h
	${Tacktech_Common_SOURCE_DIR}/src/Display_Client.h
	${Tacktech_Common_SOURCE_DIR}/src/Display_Client_Container.h
	${Tacktech_Common_SOURCE_DIR}/src/Playlist.h
	${Tacktech_Common_SOURCE_DIR}/src/Filelist.h
	)
set(TACKTECH_COMMON_SRCS
	${Tacktech_Common_SOURCE_DIR}/src/Group_Container.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Group_Playlist_Container.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Playlist_Container.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Display_Client.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Display_Client_Container.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Playlist.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Filelist.cpp
	${Tacktech_Common_SOURCE_DIR}/src/Tacktech_Network_Manager.cpp
	)

QT4_WRAP_CPP(MOCS ${TACKTECH_COMMON_MOC_HDRS})

message(STATUS "Using BOOST libraries for COMMON: " ${Boost_LIBRARIES})

if(BUILD_SHARED_LIBS)
    add_library(Tacktech_Common
				SHARED
				${TACKTECH_COMMON_HDRS}
				${TACKTECH_COMMON_SRCS}
				${TACKTECH_COMMON_MOC_HDRS}
				${MOCS}
				)
	target_link_libraries(Tacktech_Common
				pugixml
				${Boost_LIBRARIES}
				${QT_LIBRARIES}
				)
else(BUILD_SHARED_LIBS)
    add_library(Tacktech_Common
				STATIC
				${TACKTECH_COMMON_HDRS}
				${TACKTECH_COMMON_SRCS}
				${TACKTECH_COMMON_MOC_HDRS}
				${MOCS}
				)
	target_link_libraries(Tacktech_Common
				pugixml
				${Boost_LIBRARIES}
				${QT_LIBRARIES}
				)
endif(BUILD_SHARED_LIBS)
