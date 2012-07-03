project(artemis_server)
cmake_minimum_required(VERSION 2.8)

OPTION (LOCAL_CONNECTOR_C "Use local connector/c library" ON)
OPTION (LOCAL_CONNECTOR_CPP "Use local connector/c++ library" ON)
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
find_package(Boost 1.47.0 COMPONENTS program_options thread system date_time regex filesystem signals REQUIRED)
if (NOT WIN32)
  find_package (Threads)
endif (NOT WIN32)

if(Boost_FOUND)
#Continue only if boost is found
    include_directories(${Boost_INCLUDE_DIRS})

	message ("")
	message (STATUS "	==============================================")
	message (STATUS "	Using boost_libraries: ${Boost_LIBRARIES}")
	message (STATUS "	Using boost inclucdes: ${Boost_INCLUDE_DIRS}")
	if (LOCAL_PUGIXML)
		message (STATUS "	Pugixml include dir: ${PUGIXML_INCLUDE_DIR}")
		message (STATUS "	Pugixml library: ${pugixml}")
	endif (LOCAL_PUGIXML)
	message ("")

	include_directories(${CONNECTOR_C_INCLUDE_DIR} 
			${CONNECTOR_CPP_INCLUDE_DIR}
			${Boost_INCLUDE_DIRS}
			${PUGIXML_INCLUDE_DIR})
	#link_directories(${CONNECTOR_C_LIBRARY_DIR}
	#				 ${CONNECTOR_CPP_LIBRARY_DIR})
	
    add_executable(Artemis_Server 
			main.cpp
			Artemis_Server.cpp Artemis_Server.h
			Artemis_Server_Connection.cpp Artemis_Server_Connection.h
			Artemis_Request_Handler.cpp Artemis_Request_Handler.h
			)
    target_link_libraries(Artemis_Server ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CONNECTOR_CPP_LIBRARY} pugixml)

endif(Boost_FOUND)

