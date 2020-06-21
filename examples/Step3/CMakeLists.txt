cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(step3)

set(MITK_SUPERBUILD_DIR "/usr/mitk-build")
set(QT_DIR "/home/pop_os")

set(CMAKE_PREFIX_PATH
		"${MITK_SUPERBUILD_DIR}/MITK-build"
		"${MITK_SUPERBUILD_DIR}/ep/src/VTK/CMake/"
		"${QT_DIR}/Qt/5.12.8/gcc_64/lib/cmake/Qt5"
		)

find_package(MITK CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} Step3.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC_HEADER
		MitkCore
		MitkQtWidgetsExt
		)
