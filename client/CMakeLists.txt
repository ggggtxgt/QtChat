cmake_minimum_required(VERSION 4.0)
project(QtClient)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/Qt/5.14.2/mingw73_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

# 包含头文件目录，以便能在 ui_xxx.h 文件中找到
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source/Headers)

set(QRC "res/qrc/rc.qrc")
set(ICO "res/qrc/Qtchat.rc"
        Source/Cpp/timerbtn.cpp
        Source/Headers/timerbtn.h)

add_executable(${PROJECT_NAME}
        ${QRC}
        ${ICO}
        Source/main.cpp
        Source/Headers/mainwindow.h
        Source/Forms/mainwindow.ui
        Source/Cpp/mainwindow.cpp
        Source/Cpp/logindialog.cpp
        Source/Headers/logindialog.h
        Source/Forms/logindialog.ui
        Source/Cpp/registerdialog.cpp
        Source/Headers/registerdialog.h
        Source/Forms/registerdialog.ui
        Source/Headers/global.h
        Source/Cpp/global.cpp
        Source/Headers/singleton.h
        Source/Cpp/httpmanager.cpp
        Source/Headers/httpmanager.h)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
)

# 复制config.ini到输出目录
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
