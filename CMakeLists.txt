Project(Tactek)
cmake_minimum_required(VERSION 2.8)
 
message (STATUS "###### WE ARE BUILDING Tactek IN '${CMAKE_BUILD_TYPE}' MODE")

SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)
#The display app can only be build and used in
#nix for the time being. I suppose it could be
#cross compiled in linux, but for now I'm just
#adding this if statement to not build it in
#win32
add_subdirectory(Tacktech_Manager)
if (NOT WIN32)
	add_subdirectory(Tactek_Display)
endif (NOT WIN32)
#add_subdirectory(Artemis_Server)