project (Tacktech_Display)
cmake_minimum_required( VERSION 2.8 )

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
	#${Tacktech_Display_SOURCE_DIR}/src/News_Ticker_Thread.cpp
	#${Tacktech_Display_SOURCE_DIR}/src/News_Ticker.cpp
	#${Tacktech_Display_SOURCE_DIR}/src/NewsTicker.cpp
	)
set ( Tacktech_Display_HDRS
		${Tacktech_Display_SOURCE_DIR}/src/Tactek_Display.h
		#${Tacktech_Display_SOURCE_DIR}/src/News_Ticker_Thread.h
		#${Tacktech_Display_SOURCE_DIR}/src/News_Ticker.h
		#${Tacktech_Display_SOURCE_DIR}/src/NewsTicker.h
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
						${SYSTEM_LIBRARIES}
						)  
if (WIN32)
	INCLUDE(InstallRequiredSystemLibraries)
						
	install (TARGETS Tacktech_Display
			RUNTIME
			DESTINATION Tacktech_Display/bin)
	install (FILES ${Tacktech_Display_SOURCE_DIR}/config.ini "${QT_LIBRARIES}" "${LIBVLCQT_LIBRARY}"
			DESTINATION Tacktech_Display/bin)
			
	set(CPACK_PACKAGE_NAME "Tacktech_Display")
	set(CPACK_PACKAGE_VENDOR "Tacktech")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tacktech Advertising")
	set(CPACK_PACKAGE_VERSION "1.0.0")
	set(CPACK_PACKAGE_VERSION_MAJOR "1")
	set(CPACK_PACKAGE_VERSION_MINOR "0")
	set(CPACK_PACKAGE_VERSION_PATCH "0")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "Tacktech")
	set(CPACK_CREATE_DESKTOP_LINKS "${EXE_TARGET_NAME}")
	set(CPACK_RESOURCE_FILE_LICENSE "${Tacktech_Display_SOURCE_DIR}/License.txt")
	include (CPack)
else (WIN32)
	 install (TARGETS Tacktech_Display
			 RUNTIME
			 DESTINATION Tacktech_Display/bin)
	 install (FILES ${Tacktech_Display_SOURCE_DIR}/config.ini
			 DESTINATION Tacktech_Display/bin)
			
	set(CPACK_PACKAGE_NAME "Tacktech_Display")
	set(CPACK_PACKAGE_VENDOR "Tacktech")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tacktech Advertising")
	set(CPACK_PACKAGE_VERSION "1.0.0")
	set(CPACK_PACKAGE_VERSION_MAJOR "1")
	set(CPACK_PACKAGE_VERSION_MINOR "0")
	set(CPACK_PACKAGE_VERSION_PATCH "0")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "Tacktech")
	set(CPACK_RESOURCE_FILE_LICENSE "${Tacktech_Display_SOURCE_DIR}/License.txt")
	set(CPACK_RPM_PACKAGE_SUMMARY "Tacktech Display application")
	set(CPACK_RPM_PACKAGE_NAME "Tacktech_Display")
	set(CPACK_RPM_PACKAGE_VERSION "1")
	set(CPACK_RPM_PACKAGE_RELEASE "fedora1")
	set(CPACK_RPM_PACKAGE_LICENSE "${Tacktech_Display_SOURCE_DIR}/License.txt")
	set(CPACK_RPM_PACKAGE_GROUP "Applications/Multimedia")
	set(CPACK_RPM_PACKAGE_VENDOR "Tacktech")
	set(CPACK_RPM_PACKAGE_DESCRIPTION "Displays media that is defined through other Tacktech products")
	set(CPACK_GENERATOR                 "RPM")
	include (CPack)
endif (WIN32)