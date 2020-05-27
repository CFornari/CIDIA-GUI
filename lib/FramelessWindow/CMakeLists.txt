cmake_minimum_required(VERSION 3.17)

project(FramelessWindow LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_PREFIX_PATH "/home/pop_os/Qt/5.12.8/gcc_64/lib/cmake/Qt5/")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(cpp_files
		framelesswindow.cpp
		windowdragger.cpp
		DarkStyle.cpp
		)

set(header_files
		include/FramelessWindow_global.h
		include/framelesswindow.h
		include/windowdragger.h
		include/DarkStyle.h
		)

set(ui_files
		framelesswindow.ui
		)

set(qrc_files
		framelesswindow.qrc
		darkstyle.qrc
		)

add_library(${PROJECT_NAME} SHARED ${cpp_files} ${header_files} ${ui_files} ${qrc_files})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

target_compile_definitions(${PROJECT_NAME}
		PUBLIC cxx_std_14
		PRIVATE FRAMELESSWINDOW_LIBRARY)

target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)

#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION .)

install(TARGETS ${PROJECT_NAME}
		EXPORT mylib-targets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin)

install(EXPORT mylib-targets
#		NAMESPACE ${PROJECT_NAME}::
		FILE ${PROJECT_NAME}Config.cmake
		DESTINATION cmake
		EXPORT_LINK_INTERFACE_LIBRARIES)

export(TARGETS ${PROJECT_NAME} FILE ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}Config.cmake)
