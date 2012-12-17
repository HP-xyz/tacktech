Project(Tacktech)
cmake_minimum_required(VERSION 2.8)

OPTION (SHOW_DEBUG_OUTPUT "Displays all debug info" ON)

#I really hate the cmake verbose output
#set this to on if it is needed
set( CMAKE_VERBOSE_MAKEFILE OFF )

IF( NOT CMAKE_BUILD_TYPE )
	message (STATUS "###### WE ARE BUILD IN 'Debug' MODE")
	SET( CMAKE_BUILD_TYPE Debug )
	if (NOT WIN32)
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
	endif (NOT WIN32)
ENDIF( NOT CMAKE_BUILD_TYPE )

if(SHOW_DEBUG_OUTPUT)
	add_definitions(-D_SHOW_DEBUG_OUTPUT)
endif(SHOW_DEBUG_OUTPUT)

if(BUILD_32 AND UNIX)
    # only build 32-bit binaries
    add_definitions(-m32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
    set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -m32")
    set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -m32")
	message (STATUS "ATTENTION: Building 32bit - Should be done only when cross compiling")
endif(BUILD_32 AND UNIX)

IF( NOT CMAKE_BUILD_TYPE )
  message (STATUS "###### WE ARE BUILD IN 'Debug' MODE")
  SET( CMAKE_BUILD_TYPE Debug ) 
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

if (WIN32)
	set (QT_QMAKE_EXECUTABLE ${QT_DIR}/bin/qmake.exe)
	set (QT_MOC_EXECUTABLE ${QT_DIR}/bin/moc.exe)
	set (QT_RCC_EXECUTABLE ${QT_DIR}/bin/rcc.exe)
	set (QT_UIC_EXECUTABLE ${QT_DIR}/bin/uic.exe)
	set (QT_INSTALL_LIBS ${QT_DIR}/lib)
	MESSAGE (STATUS "QT_DIR: " ${QT_DIR})
	MESSAGE (STATUS "QT_QMAKE_EXECUTABLE: " ${QT_QMAKE_EXECUTABLE})
	MESSAGE (STATUS "QT_MOC_EXECUTABLE: " ${QT_MOC_EXECUTABLE})
	MESSAGE (STATUS "QT_RCC_EXECUTABLE: " ${QT_RCC_EXECUTABLE})
	MESSAGE (STATUS "QT_UIC_EXECUTABLE: " ${QT_UIC_EXECUTABLE})	
	find_package (Qt4 COMPONENTS QtMain QtNetwork QtCore QtGui REQUIRED)
else (WIN32)
	find_package (Qt4 COMPONENTS QtNetwork QtCore QtGui REQUIRED)
endif (WIN32)

include ( ${QT_USE_FILE} )
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Hack from random mailist
if (MINGW)
	SET (Boost_COMPILER  "-mgw46")
endif (MINGW)
if(BOOST_ROOT_DIR)
	set(BOOST_ROOT ${BOOST_ROOT_DIR})
endif(BOOST_ROOT_DIR)
if(BOOST_LIBRARY_DIR)
	set(BOOST_LIBRARYDIR ${BOOST_LIBRARY_DIR})
endif(BOOST_LIBRARY_DIR)
if(BOOST_INCLUDE_DIR)
	set(BOOST_INCLUDEDIR ${BOOST_INCLUDE_DIR})
endif(BOOST_INCLUDE_DIR)
message(STATUS "BOOST_ROOT: " ${BOOST_ROOT})
message(STATUS "BOOST_INCLUDEDIR: " ${BOOST_INCLUDEDIR})
message(STATUS "BOOST_LIBRARYDIR: " ${BOOST_LIBRARYDIR})

if (MSVC)
  message (STATUS "SETTING BOOST DEFINITIONS FOR MSVC") 
  add_definitions(-D_WIN32_WINNT=0x0501)
  add_definitions(-DBOOST_ALL_NO_LIB)
  #add_definitions(-DBOOST_ALL_DYN_LINK)
  add_definitions(-DBoost_LIB_DIAGNOSTIC_DEFINITIONS)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif (MSVC)
if (WIN32 AND __COMPILER_GNU)
        # mingw-gcc fails to link boost::thread
		message (STATUS "SETTING BOOST DEFINITIONS FOR GNU")
		add_definitions(-D_WIN32_WINNT=0x0501)
        set(Boost_USE_STATIC_LIBS ON)
		set(Boost_USE_MULTITHREADED ON)
        add_definitions(-DBOOST_ALL_NO_LIB)
        add_definitions(-DBOOST_THREAD_USE_LIB)
endif (WIN32 AND __COMPILER_GNU)

#Lets find boost
set (BOOST_COMPONENTS_NEEDED
	program_options
	thread
	system
	date_time
	regex
	filesystem 
	signals 
	chrono)
find_package(Boost 1.50.0 COMPONENTS ${BOOST_COMPONENTS_NEEDED} REQUIRED)

if(Boost_FOUND)
	MESSAGE( STATUS "BOOST Libraries " ${Boost_LIBRARIES} )
	FOREACH(BOOST_COMPONENT ${BOOST_COMPONENTS_NEEDED})
		STRING( TOUPPER ${BOOST_COMPONENT} BOOST_COMPONENT_UPCASE )
		MESSAGE( STATUS "Boost " ${BOOST_COMPONENT} ": " ${Boost_${BOOST_COMPONENT_UPCASE}_LIBRARY} )
		MESSAGE( STATUS "Boost " ${BOOST_COMPONENT} " Debug: " ${Boost_${BOOST_COMPONENT_UPCASE}_LIBRARY_DEBUG} )
		MESSAGE( STATUS "Boost " ${BOOST_COMPONENT} " Release: " ${Boost_${BOOST_COMPONENT_UPCASE}_LIBRARY_RELEASE} )
	ENDFOREACH(BOOST_COMPONENT)
endif(Boost_FOUND)

if (NOT WIN32)
	# We need this for unix machines since Qt internally uses
	# the pthread library (I think :D)
    find_package(Threads REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -ldl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt -ldl")
endif (NOT WIN32)

if (MINGW AND __COMPILER_GNU)
  find_package(Threads REQUIRED)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lwsock32 -lws2_32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lwsock32 -lws2_32")
endif (MINGW AND __COMPILER_GNU)

#Find the libvlcqt library
if (MINGW AND __COMPILER_GNU)
	message(STATUS "USING HARDCODED PATH FOR LIBVLC-QT")
	set (SYSTEM_LIBRARIES ws2_32 wsock32)
	set (LIBVLCQT_INCLUDE_DIR "G:\\Akademie\\Code\\DVCS\\tacktech_clean\\libvlc-qt\\include")
	set (LIBVLCQT_LIBRARY "G:\\Akademie\\Code\\DVCS\\tacktech_clean\\mingw32-build-libs\\win32\\libvlc-qt.dll")
elseif(MSVC)
	message(STATUS "Cannot build libvlc-qt with MSVC, disabling Tacktech_Display")
	set(BUILD_DISPLAY OFF)
else(MINGW AND __COMPILER_GNU)
	find_package(LIBVLCQT REQUIRED)
endif(MINGW AND __COMPILER_GNU)

OPTION (LOCAL_PUGIXML "Use local pugixml library (RECOMMENDED)" ON)

if (LOCAL_PUGIXML)
	message(STATUS "**** USING LOCAL PUGIXML ****")
	add_subdirectory (${Tacktech_SOURCE_DIR}/pugixml/scripts)
	set (PUGIXML_INCLUDE_DIR ${Tacktech_SOURCE_DIR}/pugixml/src)
endif (LOCAL_PUGIXML)
add_subdirectory(${Tacktech_SOURCE_DIR}/libb64)

add_subdirectory(${Tacktech_SOURCE_DIR}/Tacktech_Common)
set(Tacktech_Common_INCLUDE_DIR
	${Tacktech_SOURCE_DIR}/Tacktech_Common/src
	)

set(LIBB64_INCLUDE_DIR ${Tacktech_SOURCE_DIR}/libb64/include)

message("")
message (STATUS "Qt include dir	: ${QT_INCLUDE_DIR}")
message (STATUS "Qt libraries	: ${QT_LIBRARIES}")
if (LOCAL_PUGIXML)
	message (STATUS "Pugixml include dir: ${PUGIXML_INCLUDE_DIR}")
endif (LOCAL_PUGIXML)
message (STATUS "Libb64 include dir: ${LIBB64_INCLUDE_DIR}")
message (STATUS "Boost_INCLUDE_DIRS: " ${Boost_INCLUDE_DIRS})
message (STATUS "Boost_LIBRARIES: " ${Boost_LIBRARIES})
message (STATUS "Tacktech_Common_INCLUDE_DIR: " ${Tacktech_Common_INCLUDE_DIR})
message (STATUS "CMAKE_THREAD_LIBS_INIT: ${CMAKE_THREAD_LIBS_INIT}")
message (STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS} ")
message (STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS} ")
message (STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE} ")
message (STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE} ")
message (STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG} ")
message (STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG} ")
message("")

option (BUILD_MANAGER "Build Tacktech_Manager" ON)
option (BUILD_DISPLAY "Build Tacktech_Display" OFF)
option (BUILD_SERVER "Build Artemis_Server" OFF)

#The display app can only be build and used in
#nix for the time being. I suppose it could be
#cross compiled in linux, but for now I'm just
#adding this if statement to not build it in
#win32
if(BUILD_MANAGER)
	add_subdirectory(Tacktech_Manager)
endif(BUILD_MANAGER)
if(BUILD_DISPLAY)
	add_subdirectory(Tactek_Display)
endif(BUILD_DISPLAY)
if(BUILD_SERVER)
	add_subdirectory(Artemis_Server)
endif(BUILD_SERVER)