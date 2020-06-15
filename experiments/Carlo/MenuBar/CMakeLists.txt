cmake_minimum_required(VERSION 3.5)

project(MenuBar LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH
		"/home/pop_os/Qt/5.12.8/gcc_64/lib/cmake/Qt5/"
		)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory("${CMAKE_SOURCE_DIR}/SpaceXStyle")

add_executable(${PROJECT_NAME}
		main.cpp
		MainWindow.cpp
		MainWindow.h

		MainWindow.ui
		)

target_link_libraries(${PROJECT_NAME} PRIVATE
		Qt5::Widgets
		SpaceXStyle
		)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

target_include_directories(${PROJECT_NAME} PUBLIC
		"${CMAKE_SOURCE_DIR}/SpaceXStyle"
		)

