project (Tacktech_Manager)
cmake_minimum_required( VERSION 2.8 )

IF( NOT CMAKE_BUILD_TYPE )
  message (STATUS "###### WE ARE BUILD IN 'Debug' MODE")
  SET( CMAKE_BUILD_TYPE Debug ) 
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG" )
ENDIF( NOT CMAKE_BUILD_TYPE )

#If we're on MSVC and building in Release mode
IF(CMAKE_BUILD_TYPE MATCHES Release)
  if (MSVC)
    message (STATUS "Setting GUI_TYPE to WIN32")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ITERATOR_DEBUG_LEVEL=0" )
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
	set(GUI_TYPE WIN32)
  endif(MSVC)
endif(CMAKE_BUILD_TYPE MATCHES Release)

if (MSVC)
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
else (MSVC)
	find_package (Qt4 COMPONENTS QtNetwork QtCore QtGui REQUIRED)
endif (MSVC)

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
find_package(Boost 1.47.0 REQUIRED)

if (NOT WIN32)
	# We need this for unix machines since Qt internally uses
	# the pthread library (I think :D)
    find_package(Threads REQUIRED)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt -ldl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt -ldl")
endif (NOT WIN32)

include ( ${QT_USE_FILE} )
#SET(QT_USE_QTNETWORK TRUE)

ADD_DEFINITIONS(${QT_DEFINITIONS})
link_libraries(${QT_LIBRARIES}
				${QT_QTMAIN_LIBRARY}
				${QT_QTCORE_LIBRARY} 
				${QT_QTGUI_LIBRARY} 
				${QT_QTNETWORK_LIBRARY} 
				)
include_directories (
					${CMAKE_SOURCE_DIR} 
					${CMAKE_CURRENT_BINARY_DIR}
					${QT_QTCORE_INCLUDE_DIR} 
					${QT_QTGUI_INCLUDE_DIR} 
					${QT_INCLUDE_DIR}
					${QT_INCLUDES}
					${PUGIXML_INCLUDE_DIR}
					${Boost_INCLUDE_DIRS}
					)
set (Tacktech_Manager_Non_MOC_HDRS
	${Tacktech_Manager_SOURCE_DIR}/src/Typed_QTreeWidgetItem.h
	${Tacktech_Manager_SOURCE_DIR}/src/Group_Container.h
	${Tacktech_Manager_SOURCE_DIR}/src/Group_Playlist_Container.h
	${Tacktech_Manager_SOURCE_DIR}/src/Playlist_Container.h
	)
					
set ( Tacktech_Manager_SRCS
		${Tacktech_Manager_SOURCE_DIR}/main.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Tacktech_Manager.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Edit_Group.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Computer_Dialog.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Group_Dialog.cpp	
		${Tacktech_Manager_SOURCE_DIR}/src/Edit_Playlist.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Playlist_Container.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Playlist_Dialog.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Add_File_Dialog.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Pause_Dialog.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Group_Playlist_Container.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Typed_QTreeWidgetItem.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Group_Container.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Select_Playlist_Dialog.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data_Container.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Send_Data.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Start_Send_Data_Thread.cpp
	)
set ( Tacktech_Manager_HDRS
		${Tacktech_Manager_SOURCE_DIR}/src/Tacktech_Manager.h
		${Tacktech_Manager_SOURCE_DIR}/src/Edit_Group.h
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Computer_Dialog.h
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Group_Dialog.h		
		${Tacktech_Manager_SOURCE_DIR}/src/Edit_Playlist.h
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Playlist_Dialog.h
		${Tacktech_Manager_SOURCE_DIR}/src/Add_File_Dialog.h
		${Tacktech_Manager_SOURCE_DIR}/src/Add_Pause_Dialog.h
		${Tacktech_Manager_SOURCE_DIR}/src/Select_Playlist_Dialog.h
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data.h
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data_Container.h
		${Tacktech_Manager_SOURCE_DIR}/src/Send_Data.h
		${Tacktech_Manager_SOURCE_DIR}/src/Start_Send_Data_Thread.h
	)
#FOR NOW, THERE ARE NO RESOURCES
set ( Tacktech_Manager_RSCS
		${Tacktech_Manager_SOURCE_DIR}/resources/Tacktech_Manager.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Edit_Group.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Add_Computer_Dialog.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Add_Group_Dialog.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Edit_Playlist.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Add_Playlist_Dialog.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Add_File_Dialog.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Add_Pause_Dialog.qrc
		${Tacktech_Manager_SOURCE_DIR}/resources/Select_Playlist_Dialog.qrc
	)
set ( Tacktech_Manager_UI_FORMS
		${Tacktech_Manager_SOURCE_DIR}/ui/Tacktech_Manager.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Edit_Group.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Add_Computer_Dialog.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Add_Group_Dialog.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Edit_Playlist.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Add_Playlist_Dialog.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Add_File_Dialog.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Add_Pause_Dialog.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Select_Playlist_Dialog.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Upload_Data.ui
	)
QT4_ADD_RESOURCES(RSCS ${Tacktech_Manager_RSCS})
QT4_WRAP_UI (UIS ${Tacktech_Manager_UI_FORMS})
QT4_WRAP_CPP(MOCS ${Tacktech_Manager_HDRS})

message("")
message (STATUS "Qt include dir	: ${QT_INCLUDE_DIR}")
message (STATUS "Qt libraries	: ${QT_LIBRARIES}")
if (LOCAL_PUGIXML)
	message (STATUS "Pugixml include dir: ${PUGIXML_INCLUDE_DIR}")
	message (STATUS "Pugixml library: ${pugixml}")
endif (LOCAL_PUGIXML)
message (STATUS "Boost_INCLUDE_DIRS: " ${Boost_INCLUDE_DIRS})
message (STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS} ")
message (STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS} ")
message (STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE} ")
message (STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE} ")
message (STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG} ")
message (STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG} ")
message("")

add_executable ( Tacktech_Manager 
				${GUI_TYPE}
				${Tacktech_Manager_Non_MOC_HDRS}
				${Tacktech_Manager_SRCS} 
				${Tacktech_Manager_HDRS} 
				${MOCS} 
				${RSCS} 
				${UIS})
target_link_libraries ( Tacktech_Manager 
						pugixml
						${QT_LIBRARIES}
						${QT_QTCORE_LIBRARIES} 
						${QT_QTGUI_LIBRARIES} 
						${QT_QTNETWORK_LIBRARIES} 
                        ${CMAKE_THREAD_LIBS_INIT}
                        )