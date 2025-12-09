cmake_minimum_required(VERSION 4.0)
project(GateServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(../../../../application/boost)
link_directories(../../../../application/boost/stage/lib)

add_executable(${PROJECT_NAME}
        GateServer.cpp
)