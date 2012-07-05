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
