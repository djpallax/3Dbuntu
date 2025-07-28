cmake_minimum_required(VERSION 3.10)
project(3Dbuntu VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets SerialPort)

add_executable(3Dbuntu
    src/main.cpp
    src/MainWindow.cpp
)

target_link_libraries(3Dbuntu Qt5::Widgets)
target_link_libraries(3Dbuntu Qt5::Widgets Qt5::SerialPort)
target_include_directories(3Dbuntu PRIVATE src)
