Project(Tactek)
cmake_minimum_required(VERSION 2.8)

IF( NOT CMAKE_BUILD_TYPE )
  message (STATUS "###### WE ARE BUILD IN 'Debug' MODE")
  SET( CMAKE_BUILD_TYPE Debug ) 
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG" )
if (NOT WIN32)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif (NOT WIN32)
ENDIF( NOT CMAKE_BUILD_TYPE )

# THIS IS FOR LIBB64
# select a buffersize
# a larger size should be faster, but takes more runtime memory
#set(BUFFERSIZE 4096)
#set(BUFFERSIZE 65536)
set(BUFFERSIZE 16777216)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUFFERSIZE=${BUFFERSIZE}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUFFERSIZE=${BUFFERSIZE}")

message (STATUS "###### WE ARE BUILDING Tactek IN '${CMAKE_BUILD_TYPE}' MODE")

SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)

OPTION (LOCAL_PUGIXML "Use local pugixml library (RECOMMENDED)" ON)

if (LOCAL_PUGIXML)
	#lets include pugixml
	add_subdirectory (${Tactek_SOURCE_DIR}/pugixml/scripts)
	set (PUGIXML_INCLUDE_DIR ${Tactek_SOURCE_DIR}/pugixml/src)
endif (LOCAL_PUGIXML)
add_subdirectory(${Tactek_SOURCE_DIR}/libb64)

add_subdirectory(${Tactek_SOURCE_DIR}/Tacktech_Common)
set(Tacktech_Common_INCLUDE_DIR
	${Tactek_SOURCE_DIR}/Tacktech_Common/src
	)

set(LIBB64_INCLUDE_DIR ${Tactek_SOURCE_DIR}/libb64/include)

#The display app can only be build and used in
#nix for the time being. I suppose it could be
#cross compiled in linux, but for now I'm just
#adding this if statement to not build it in
#win32
add_subdirectory(Tacktech_Manager)
if (NOT WIN32)
	add_subdirectory(Tactek_Display)
endif (NOT WIN32)
add_subdirectory(Artemis_Server)
