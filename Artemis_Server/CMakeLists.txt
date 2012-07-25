project(artemis_server)
cmake_minimum_required(VERSION 2.8)

OPTION (LOCAL_PUGIXML "Use local pugixml library" ON)
OPTION (DEBUG_ALL "Displays all debug info" ON)

if (DEBUG_ALL)
	add_definitions(-D_DEBUG)
endif (DEBUG_ALL)

#We do this for boost on windows
if (WIN32 AND NOT CYGWIN)
	add_definitions(-D_WIN32_WINNT=0x0501)
endif (WIN32 AND NOT CYGWIN)

#Needed for windows
#Change this to your boost path
if (WIN32)
  #SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/boost")
  #SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/boost/lib")
  #message (STATUS "Using user defined boost path")
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON) 
  set(Boost_USE_STATIC_RUNTIME OFF)
endif (WIN32)

#Lets find boost
find_package(Boost 1.47.0 COMPONENTS program_options thread system date_time regex filesystem signals chrono REQUIRED)
if (NOT WIN32)
  find_package (Threads)
endif (NOT WIN32)

set (ARTEMIS_SERVER_SRCS
	${artemis_server_SOURCE_DIR}/src/Artemis_Server.cpp
	${artemis_server_SOURCE_DIR}/src/Artemis_Server_Connection.cpp
	${artemis_server_SOURCE_DIR}/src/Artemis_Request_Handler.cpp
	${artemis_server_SOURCE_DIR}/src/Artemis_Network_Sender_Connection.cpp
	${artemis_server_SOURCE_DIR}/src/Group_Container.cpp
	${artemis_server_SOURCE_DIR}/src/Playlist_Container.cpp
	${artemis_server_SOURCE_DIR}/src/Group_Playlist_Container.cpp
	)
set (ARTEMIS_SERVER_HDRS
	${artemis_server_SOURCE_DIR}/src/Artemis_Server.h
	${artemis_server_SOURCE_DIR}/src/Artemis_Server_Connection.h
	${artemis_server_SOURCE_DIR}/src/Artemis_Request_Handler.h
	${artemis_server_SOURCE_DIR}/src/Artemis_Network_Sender_Connection.h
	${artemis_server_SOURCE_DIR}/src/Group_Container.h
	${artemis_server_SOURCE_DIR}/src/Playlist_Container.h
	${artemis_server_SOURCE_DIR}/src/Group_Playlist_Container.h
	)

message ("")
message (STATUS "	==============================================")
message (STATUS "	Using boost_libraries: ${Boost_LIBRARIES}")
message (STATUS "	Using boost inclucdes: ${Boost_INCLUDE_DIRS}")
if (LOCAL_PUGIXML)
	message (STATUS "	Pugixml include dir: ${PUGIXML_INCLUDE_DIR}")
	message (STATUS "	Pugixml library: ${pugixml_LIBRARY}")
endif (LOCAL_PUGIXML)
message ("")

include_directories(
		${Boost_INCLUDE_DIRS}
		${PUGIXML_INCLUDE_DIR}
		)

add_executable(Artemis_Server 
		main.cpp
		${ARTEMIS_SERVER_SRCS}
		${ARTEMIS_SERVER_HDRS}
		)
target_link_libraries(Artemis_Server ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} pugixml libb64)


