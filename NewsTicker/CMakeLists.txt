project (NewsTicker)

cmake_minimum_required( VERSION 2.8 )

message (STATUS "###### WE ARE BUILD IN '${CMAKE_BUILD_TYPE}' MODE")

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
	find_package (Qt4 COMPONENTS QtNetwork QtCore QtGui)	
else (MSVC)
	find_package (Qt4 COMPONENTS QtNetwork QtCore QtGui REQUIRED)
endif (MSVC)

include ( ${QT_USE_FILE} )
ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories (
					${CMAKE_SOURCE_DIR} 
					${CMAKE_CURRENT_BINARY_DIR}
					${QT_QTCORE_INCLUDE_DIR} 
					${QT_QTGUI_INCLUDE_DIR} 
					${QT_QTCORE_INCLUDE_DIR} 
					${QT_QTGUI_INCLUDE_DIR} 
					${QT_INCLUDE_DIR}
					)
					
set (NewsTicker_HDRS
	${NewsTicker_SOURCE_DIR}/src/NewsTicker.h
	)
	
set (NewsTicker_SRCS
	main.cpp
	${NewsTicker_SOURCE_DIR}/src/NewsTicker.cpp
	)
	
set (NewsTicker_RSCS
	)
QT4_ADD_RESOURCES(RSCS ${NewsTicker_RSCS})
QT4_WRAP_CPP(MOCS ${NewsTicker_HDRS})

message("")
message (STATUS "Qt include dir	: ${QT_INCLUDE_DIR}")
message (STATUS "Qt libraries	: ${QT_LIBRARIES}")
message (STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS} ")
message (STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS} ")
message (STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE} ")
message (STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE} ")
message (STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG} ")
message (STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG} ")
message("")

add_executable ( NewsTicker ${NewsTicker_SRCS} ${NewsTicker_HDRS} ${MOCS} ${RSCS})
target_link_libraries ( NewsTicker 
						${QT_LIBRARIES}
						${QT_QTCORE_LIBRARY} 
						${QT_QTGUI_LIBRARY}
						${CMAKE_THREAD_LIBS_INIT}
						)