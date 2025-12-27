# CMakeLists.txt for Ahead Qt6 GUI
# El Build configuration - bey7added el Qt we el source files
# © 2024 Ahead Team - Ain Shams University / UEL

cmake_minimum_required(VERSION 3.16)
project(Ahead VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Backend source files (RadixTree)
set(BACKEND_SOURCES
    ../Node.cpp
    ../RadixTree.cpp
)

set(BACKEND_HEADERS
    ../Node.h
    ../RadixTree.h
)

# GUI source files
set(GUI_SOURCES
    main.cpp
    main_window.cpp
    radix_tree_model.cpp
    status_manager.cpp
    ghost_line_edit.cpp
    snowflake_widget.cpp
    side_menu.cpp
)

set(GUI_HEADERS
    theme.h
    main_window.h
    radix_tree_model.h
    status_manager.h
    ghost_line_edit.h
    snowflake_widget.h
    side_menu.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${BACKEND_SOURCES}
    ${BACKEND_HEADERS}
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    resources.qrc
)

# Link Qt6 Widgets
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# macOS specific settings
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_MACOSX})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.radixtree.ahead"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE_ICON_FILE icon.icns
    )
endif()

# Windows specific settings
if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
    target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
endif()
