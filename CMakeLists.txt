cmake_minimum_required(VERSION 3.20)
project(DevHubCpp VERSION 1.1.0 LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

include_directories(src)

set(SOURCES
    src/main.cpp
    src/domain/Project.hpp
    src/infrastructure/ProjectInspector.hpp
    src/infrastructure/ProjectInspector.cpp
    src/infrastructure/ProcessLauncher.hpp
    src/infrastructure/ProcessLauncher.cpp
    src/infrastructure/SqliteProjectRepository.hpp
    src/infrastructure/SqliteProjectRepository.cpp
    src/ui/MainWindow.hpp
    src/ui/MainWindow.cpp
    resources.qrc
    app_icon.rc
)

add_executable(DevHubCpp WIN32 ${SOURCES})

target_link_libraries(DevHubCpp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)
