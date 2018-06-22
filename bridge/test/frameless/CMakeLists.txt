cmake_minimum_required(VERSION 3.5.0)

project(frameless-test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

IF (WIN32)
endif (WIN32)

if (APPLE)
    add_definitions("-x objective-c++")
    add_executable(${PROJECT_NAME} main.cpp filter.h MyWindow.cpp item_title.cpp)
    target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Qml Qt5::Quick Qt5::WebEngine frameless "-framework Cocoa")
endif (APPLE)
