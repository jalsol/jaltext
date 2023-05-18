cmake_minimum_required(VERSION 3.22)
project(jaltext_server VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

include_directories(src)

add_executable(${PROJECT_NAME}
    src/main.cpp

    src/server/server.hpp src/server/server.cpp

    src/types/aliases.hpp
    src/types/addr_info.hpp src/types/addr_info.cpp
    src/types/sockaddr.hpp src/types/sockaddr.cpp
)

