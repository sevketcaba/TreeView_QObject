cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME TreeView_QObject)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(WIN32)
option(TVO_ENABLE_BUILD_SHAREDLIB "Shared Libraries" ON)
if(TVO_ENABLE_BUILD_SHAREDLIB)
    add_definitions(-DBUILD_SHAREDLIB)
endif(TVO_ENABLE_BUILD_SHAREDLIB)
endif(WIN32)

add_subdirectory(common)
add_subdirectory(components)
add_subdirectory(src)
add_subdirectory(resources)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
	MANUAL_FINALIZATION
	${SOURCE_FILES}
	${HEADER_FILES}
	${COMMON_HEADERS}
	${RESOURCE_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
	add_library(${PROJECT_NAME} SHARED
	    ${SOURCE_FILES}
	    ${HEADER_FILES}
	    ${COMMON_HEADERS}
	    ${RESOURCE_FILES}
	)
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(${PROJECT_NAME}
	${SOURCE_FILES}
	${HEADER_FILES}
	${COMMON_HEADERS}
	${RESOURCE_FILES}
    )
endif()
endif()

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    src
    common
)
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${LIBS}
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
