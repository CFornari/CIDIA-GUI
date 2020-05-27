cmake_minimum_required(VERSION 3.17)

project(GalleryCore LANGUAGES CXX)

if(NOT APPLE)
		set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
		${PROJECT_NAME}ConfigVersion.cmake
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
		)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../installation)

find_package(Qt5 COMPONENTS Core Sql REQUIRED)

set(SOURCE_FILES
		Album.cpp
		AlbumDao.cpp
		AlbumModel.cpp
		DatabaseManager.cpp
		Picture.cpp
		PictureDao.cpp
		PictureModel.cpp
		)

set(HEADER_FILES
		include/gallery-core_global.h
		include/Album.h
		include/AlbumDao.h
		include/AlbumModel.h
		include/DatabaseManager.h
		include/Picture.h
		include/PictureDao.h
		include/PictureModel.h
		)

add_library(${PROJECT_NAME} SHARED
		${SOURCE_FILES}
		${HEADER_FILES}
		)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

set_target_properties(
		${PROJECT_NAME} PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
		)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Sql)

target_compile_definitions(${PROJECT_NAME}
		PUBLIC cxx_std_14
		PRIVATE GALLERYCORE_LIBRARY)

target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)

#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION .)

#install(TARGETS ${PROJECT_NAME}
#		EXPORT mylib-targets
#		ARCHIVE DESTINATION lib
#		LIBRARY DESTINATION lib
#		RUNTIME DESTINATION bin)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}_export_target
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
						COMPONENT   ${PROJECT_NAME}_RunTime
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
						COMPONENT          ${PROJECT_NAME}_RunTime
						NAMELINK_COMPONENT ${PROJECT_NAME}_Development
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
						COMPONENT   ${PROJECT_NAME}_Development
		)

install(EXPORT ${PROJECT_NAME}_export_target
#		NAMESPACE ${PROJECT_NAME}::
		FILE ${PROJECT_NAME}Config.cmake
		DESTINATION cmake
		EXPORT_LINK_INTERFACE_LIBRARIES)

export(TARGETS ${PROJECT_NAME} FILE ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}Config.cmake)
