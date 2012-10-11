project (Tacktech_Display)

cmake_minimum_required( VERSION 2.8 )

IF( NOT CMAKE_BUILD_TYPE )
  message (STATUS "###### WE ARE BUILD IN 'Debug' MODE")
  SET( CMAKE_BUILD_TYPE Debug ) 
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG" )
ENDIF( NOT CMAKE_BUILD_TYPE )

if (WIN32)
	message(STATUS "THIS IS _NOT_ A UNIX BUILD")
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
	find_package (Qt4 COMPONENTS QtNetwork QtCore QtGui)	
else (WIN32)
	message(STATUS "THIS IS A UNIX BUILD")
	find_package (Qt4 COMPONENTS QtNetwork QtCore QtGui REQUIRED)
endif (WIN32)

#Find the libvlcqt library
if (NOT WIN32)
find_package(LIBVLCQT REQUIRED)
else (NOT WIN32)
	set (LIBVLCQT_INCLUDE_DIR "E:\\Akademie\\Code\\DVCS\\tacktech_clean\\libvlc-qt\\include")
	set (LIBVLCQT_LIBRARY "E:\\Akademie\\Code\\DVCS\\tacktech_clean\\mingw32-build-libs\\win32\\libvlc-qt.dll")
endif (NOT WIN32)
#Lets find boost
find_package(Boost 1.47.0 COMPONENTS program_options REQUIRED)
if (WIN32)
	add_definitions(-D_WIN32_WINDOWS)
endif (WIN32)

if (NOT WIN32)
	# We need this for unix machines since Qt internally uses
	# the pthread library (I think :D)
    find_package(Threads REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -ldl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt -ldl")
else (NOT WIN32)
	add_definitions(-D_WIN32_WINNT=0x0501 -DWINVER=0x0501)
endif (NOT WIN32)

include ( ${QT_USE_FILE} )
#SET(QT_USE_QTNETWORK TRUE)

ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories (
					${CMAKE_SOURCE_DIR} 
					${CMAKE_CURRENT_BINARY_DIR}
					${QT_QTCORE_INCLUDE_DIR} 
					${QT_QTGUI_INCLUDE_DIR} 
					${QT_QTNETWORK_INCLUDE_DIR} 
					${QT_INCLUDE_DIR}
					${QT_INCLUDES}
					${LIBVLCQT_INCLUDE_DIR}
					${LIBB64_INCLUDE_DIR}
					${PUGIXML_INCLUDE_DIR}
					${Boost_INCLUDE_DIRS}
					${Tacktech_Common_INCLUDE_DIR}
					)
set ( Tacktech_Display_SRCS
	${Tacktech_Display_SOURCE_DIR}/src/Tactek_Display.cpp
        ${Tacktech_Display_SOURCE_DIR}/src/Playlist.cpp
	)
set ( Tacktech_Display_HDRS
		${Tacktech_Display_SOURCE_DIR}/src/Tactek_Display.h
        ${Tacktech_Display_SOURCE_DIR}/src/Playlist.h
	)
set ( Tacktech_Display_RSCS
		${Tacktech_Display_SOURCE_DIR}/resources/Tactek_Display.qrc
	)
set ( Tacktech_Display_UI_FORMS
		${Tacktech_Display_SOURCE_DIR}/ui/Tactek_Display.ui
	)
QT4_ADD_RESOURCES(RSCS ${Tacktech_Display_RSCS})
QT4_WRAP_UI (UIS ${Tacktech_Display_UI_FORMS})
QT4_WRAP_CPP(MOCS ${Tacktech_Display_HDRS})

message("")
message (STATUS "Qt include dir	: ${QT_INCLUDE_DIR}")
message (STATUS "Qt libraries	: ${QT_LIBRARIES}")
message (STATUS "Libvlc-qt library: ${LIBVLCQT_LIBRARY}")
message (STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS} ")
message (STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS} ")
message (STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE} ")
message (STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE} ")
message (STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG} ")
message (STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG} ")
message("")

add_executable ( Tacktech_Display
				${Tacktech_Display_SOURCE_DIR}/main.cpp
				${Tacktech_Display_SRCS} 
				${Tacktech_Display_HDRS} 
				${MOCS} 
				${RSCS} 
				${UIS})

target_link_libraries ( Tacktech_Display 
						${QT_LIBRARIES}
						${QT_QTCORE_LIBRARY} 
						${QT_QTGUI_LIBRARY} 
						${QT_QTNETWORK_LIBRARY} 
						${LIBVLCQT_LIBRARY}
						${CMAKE_THREAD_LIBS_INIT}
						${Boost_LIBRARIES}
						libb64
						pugixml
						Tacktech_Common
						ws2_32
						wsock32
                        )
