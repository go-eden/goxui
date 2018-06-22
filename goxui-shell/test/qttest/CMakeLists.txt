cmake_minimum_required(VERSION 3.5.0)

project(qttest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Qml Qt5::Quick)
