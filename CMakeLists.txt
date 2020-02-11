cmake_minimum_required(VERSION 3.13)
project(MyTinyCo)

set(CMAKE_CXX_STANDARD 11)

include(GNUInstallDirs)

find_package(Threads REQUIRED)

list(APPEND MyTinyCo_SRCS
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Acceptor.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Buffer.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Channel.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Evloop.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/IOthread.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/IOthreadpool.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/InetAddr.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Kqueue.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/SocketOpt.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Tcpconn.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/TcpServer.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/TimeEventSet.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/NowTime.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Connector.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/TcpClient.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Taskthreadpool.cpp
        ${PROJECT_SOURCE_DIR}/MyTinyCo/Log.cpp
        )

if(CMAKE_HOST_APPLE)
    add_definitions(-DOS_MACOSX)
elseif(CMAKE_HOST_UNIX)
    add_definitions(-DOS_LINUX)
else(CMAKE_HOST_APPLE)
    message(FATAL_ERROR "Platform not supported")
endif(CMAKE_HOST_APPLE)

option(BUILD_MyTinyCo_SHARED_LIBRARY "Build Handy Shared Library" OFF)
option(BUILD_MyTinyCo_STATIC_LIBRARY "Build Handy Shared Library" ON)
option(BUILD_MyTinyCo_EXAMPLES "Build Handy Examples" OFF)

##Shared Library
if(BUILD_MyTinyCo_SHARED_LIBRARY)
    add_library(MyTinyCo SHARED ${MyTinyCo_SRCS})
    target_include_directories(MyTinyCo PUBLIC ${PROJECT_SOURCE_DIR}/MyTinyCo)
    target_link_libraries(MyTinyCo Threads::Threads)
    install(TARGETS MyTinyCo DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(BUILD_MyTinyCo_SHARED_LIBRARY)

##Static library
if(BUILD_MyTinyCo_STATIC_LIBRARY)
    add_library(MyTinyCo_S STATIC ${MyTinyCo_SRCS})
    target_include_directories(MyTinyCo_S PUBLIC ${PROJECT_SOURCE_DIR}/MyTinyCo/)
    target_link_libraries(MyTinyCo_S Threads::Threads)
    install(TARGETS MyTinyCo_S DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(BUILD_MyTinyCo_STATIC_LIBRARY)

if(BUILD_MyTinyCo_SHARED_LIBRARY OR BUILD_MyTinyCo_STATIC_LIBRARY)
    install(FILES
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Acceptor.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Buffer.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/CallBack.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Channel.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Connector.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Evloop.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/InetAddr.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/IOthread.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/IOthreadpool.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Kqueue.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Log.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Noncopyable.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/NowTime.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Poller.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Socket.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/SocketOpt.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Taskthreadpool.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/TcpClient.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/Tcpconn.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/TcpServer.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/TimeEvent.h
            ${PROJECT_SOURCE_DIR}/MyTinyCo/TimeEventSet.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MyTinyCo)
endif(BUILD_MyTinyCo_SHARED_LIBRARY OR BUILD_MyTinyCo_STATIC_LIBRARY)


function(add_MyTinyCo_executable EXECUTABLE_NAME EXECUTABLE_SOURCES)
    add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})
    target_link_libraries(${EXECUTABLE_NAME} MyTinyCo_S)
    target_include_directories(${EXECUTABLE_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
    install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction(add_MyTinyCo_executable)

add_MyTinyCo_executable(echo-cli example/EchoClient.cpp)
add_MyTinyCo_executable(echo-svr example/EchoServer.cpp)
add_MyTinyCo_executable(discard-cli example/DiscardClient.cpp)
add_MyTinyCo_executable(discard-svr example/DiscardServer.cpp)
add_MyTinyCo_executable(daytime-svr example/DaytimeServer.cpp)