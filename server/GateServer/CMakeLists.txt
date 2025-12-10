cmake_minimum_required(VERSION 4.0)
project(GateServer)

set(CMAKE_CXX_STANDARD 14)

# boost
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/boost)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/boost/stage/lib)

# jsoncpp 头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/include)

add_executable(${PROJECT_NAME}
        GateServer.cpp
        Cpp/CServer.cpp
        Headers/CServer.h
        Headers/const.h
        Cpp/HttpConnection.cpp
        Headers/HttpConnection.h
        Headers/Singleton.h
        Cpp/LogicSystem.cpp
        Headers/LogicSystem.h
)

# jsoncpp lib库
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../application/jsoncpp/libjson/lib/json_vc71_libmtd.lib
)