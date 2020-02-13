# Install script for directory: /Users/jialuhu/CLionProjects/MySNetCo

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jialuhu/CLionProjects/MySNetCo/cmake-build-debug/libMyTinyCo_S.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libMyTinyCo_S.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libMyTinyCo_S.a")
    execute_process(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libMyTinyCo_S.a")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MyTinyCo" TYPE FILE FILES
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Acceptor.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Buffer.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/CallBack.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Channel.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Connector.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Evloop.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/InetAddr.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/IOthread.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/IOthreadpool.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/IoPoller.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Log.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Noncopyable.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/NowTime.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Poller.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Socket.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/SocketOpt.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Taskthreadpool.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/TcpClient.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/Tcpconn.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/TcpServer.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/TimeEvent.h"
    "/Users/jialuhu/CLionProjects/MySNetCo/MyTinyCo/TimeEventSet.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jialuhu/CLionProjects/MySNetCo/cmake-build-debug/echo-cli")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/echo-cli" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/echo-cli")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/echo-cli")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jialuhu/CLionProjects/MySNetCo/cmake-build-debug/echo-svr")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/echo-svr" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/echo-svr")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/echo-svr")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jialuhu/CLionProjects/MySNetCo/cmake-build-debug/discard-cli")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/discard-cli" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/discard-cli")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/discard-cli")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jialuhu/CLionProjects/MySNetCo/cmake-build-debug/discard-svr")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/discard-svr" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/discard-svr")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/discard-svr")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jialuhu/CLionProjects/MySNetCo/cmake-build-debug/daytime-svr")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/daytime-svr" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/daytime-svr")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/daytime-svr")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/jialuhu/CLionProjects/MySNetCo/cmake-build-debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
