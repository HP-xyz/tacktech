project (Tacktech_Manager)
cmake_minimum_required( VERSION 2.8 )

#If we're on MSVC and building in Release mode
IF(CMAKE_BUILD_TYPE MATCHES Release)
  if (MSVC)
    message (STATUS "Setting GUI_TYPE to WIN32")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_ITERATOR_DEBUG_LEVEL=0" )
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
	set(GUI_TYPE WIN32)
  endif(MSVC)
endif(CMAKE_BUILD_TYPE MATCHES Release)

link_libraries(${QT_LIBRARIES}
				${QT_QTMAIN_LIBRARY}
				${QT_QTCORE_LIBRARY} 
				${QT_QTGUI_LIBRARY} 
				${QT_QTNETWORK_LIBRARY} 
				)
#link_directories(${BOOST_ROOT}/lib)
include_directories (
					${CMAKE_SOURCE_DIR} 
					${CMAKE_CURRENT_BINARY_DIR}
					${QT_QTCORE_INCLUDE_DIR} 
					${QT_QTGUI_INCLUDE_DIR} 
					${QT_INCLUDE_DIR}
					${QT_INCLUDES}
					${PUGIXML_INCLUDE_DIR}
					${LIBB64_INCLUDE_DIR}
					${Boost_INCLUDE_DIRS}
					${Tacktech_Common_INCLUDE_DIR}
					)
	
set (Tacktech_Manager_Non_MOC_HDRS
	${Tacktech_Manager_SOURCE_DIR}/src/Typed_QTreeWidgetItem.h
	)
					
set ( Tacktech_Manager_SRCS
		${Tacktech_Manager_SOURCE_DIR}/main.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Tacktech_Manager.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Edit_Group.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Computer_Dialog.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Group_Dialog.cpp	
		#${Tacktech_Manager_SOURCE_DIR}/src/Edit_Playlist.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Playlist_Dialog.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_File_Dialog.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Pause_Dialog.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Typed_QTreeWidgetItem.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Select_Playlist_Dialog.cpp
		#${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Files_To_Server_Dialog.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data_Container.cpp
		${Tacktech_Manager_SOURCE_DIR}/src/Tacktech_Manager_MainWindow.cpp
	)
set ( Tacktech_Manager_HDRS
		#${Tacktech_Manager_SOURCE_DIR}/src/Tacktech_Manager.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Edit_Group.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Computer_Dialog.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Group_Dialog.h		
		#${Tacktech_Manager_SOURCE_DIR}/src/Edit_Playlist.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Playlist_Dialog.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_File_Dialog.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Add_Pause_Dialog.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Select_Playlist_Dialog.h
		#${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data.h
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Data_Container.h
		${Tacktech_Manager_SOURCE_DIR}/src/Upload_Files_To_Server_Dialog.h
		${Tacktech_Manager_SOURCE_DIR}/src/Tacktech_Manager_MainWindow.h
	)
#FOR NOW, THERE ARE NO RESOURCES
set ( Tacktech_Manager_RSCS
		#${Tacktech_Manager_SOURCE_DIR}/resources/Tacktech_Manager.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Edit_Group.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Add_Computer_Dialog.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Add_Group_Dialog.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Edit_Playlist.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Add_Playlist_Dialog.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Add_File_Dialog.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Add_Pause_Dialog.qrc
		#${Tacktech_Manager_SOURCE_DIR}/resources/Select_Playlist_Dialog.qrc
	)
set ( Tacktech_Manager_UI_FORMS
		#${Tacktech_Manager_SOURCE_DIR}/ui/Tacktech_Manager.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Edit_Group.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Add_Computer_Dialog.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Add_Group_Dialog.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Edit_Playlist.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Add_Playlist_Dialog.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Add_File_Dialog.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Add_Pause_Dialog.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Select_Playlist_Dialog.ui
		#${Tacktech_Manager_SOURCE_DIR}/ui/Upload_Data.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Upload_Files_To_Server_Dialog.ui
		${Tacktech_Manager_SOURCE_DIR}/ui/Tacktech_Manager_MainWindow.ui
	)
QT4_ADD_RESOURCES(RSCS ${Tacktech_Manager_RSCS})
QT4_WRAP_UI (UIS ${Tacktech_Manager_UI_FORMS})
QT4_WRAP_CPP(MOCS ${Tacktech_Manager_HDRS})

add_executable ( Tacktech_Manager 
				${GUI_TYPE}
				${Tacktech_Manager_Non_MOC_HDRS}
				${Tacktech_Manager_SRCS} 
				${Tacktech_Manager_HDRS} 
				${MOCS} 
				${RSCS} 
				${UIS}
				)

target_link_libraries ( Tacktech_Manager 
					pugixml
					libb64
					Tacktech_Common
					${QT_LIBRARIES}
					${QT_QTCORE_LIBRARIES} 
					${QT_QTGUI_LIBRARIES} 
					${QT_QTNETWORK_LIBRARIES} 
					${CMAKE_THREAD_LIBS_INIT}
					${Boost_LIBRARIES}
					)
if (WIN32)
	INCLUDE(InstallRequiredSystemLibraries)
						
	install (TARGETS Tacktech_Manager
			RUNTIME
			DESTINATION bin)
	install (FILES ${Tacktech_Manager_SOURCE_DIR}/config.ini
			DESTINATION bin)
			
	set(CPACK_PACKAGE_NAME "Tacktech_Manager")
	set(CPACK_PACKAGE_VENDOR "Tacktech")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tacktech Advertising")
	set(CPACK_PACKAGE_VERSION "1.0.0")
	set(CPACK_PACKAGE_VERSION_MAJOR "1")
	set(CPACK_PACKAGE_VERSION_MINOR "0")
	set(CPACK_PACKAGE_VERSION_PATCH "0")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "Tacktech")
	set(CPACK_CREATE_DESKTOP_LINKS "${EXE_TARGET_NAME}")
	set(CPACK_RESOURCE_FILE_LICENSE "${Tacktech_Manager_SOURCE_DIR}/License.txt")
	include (CPack)
endif (WIN32)