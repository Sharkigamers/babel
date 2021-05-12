cmake_minimum_required(VERSION 3.8)
project(CPP_babel_2019)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")
set(RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    add_definitions("-fPIC -g -Wextra -Wall -isystem")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


include(build/conanbuildinfo.cmake)
include(build/conan_paths.cmake)
conan_basic_setup(KEEP_RPATHS)
include_directories(${CMAKE_INCLUDE_PATH})

find_package(portaudio REQUIRED)
find_package(opus REQUIRED)
find_package(sqlite3 REQUIRED)
find_package(Boost REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

find_package(PkgConfig)

find_path(CRITERION_INCLUDE_DIR criterion/criterion.h
          PATH_SUFFIXES criterion)

find_library(CRITERION_LIBRARY NAMES criterion libcriterion)

set(CRITERION_LIBRARIES ${CRITERION_LIBRARY})
set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Criterion DEFAULT_MSG
                                  CRITERION_LIBRARY CRITERION_INCLUDE_DIR)

mark_as_advanced(CRITERION_INCLUDE_DIR CRITERION_LIBRARY)

include_directories( "common/" )
include_directories( "common/client/" )
include_directories( "common/protocol/" )
include_directories(CRITERION_INCLUDE_DIRS)
include_directories( "server/file_management/" )
include_directories( "server/clients/" )
include_directories( "server/server/" )
include_directories( "server/server/include" )
include_directories( "client/clientUI/chatScreen" )
include_directories( "client/clientUI/loginScreen" )
include_directories( "client/clientUI/signInScreen" )
include_directories( "client/clientSocket" )
include_directories( "client/clientUI/" )
include_directories( "client/include/portAudio" )
include_directories( "client/include/opus" )

set(SRCS_SERVER 
    server/file_management/CSQLite.cpp
    server/file_management/CFileManager.cpp
    server/clients/CServerClient.cpp
    server/clients/CUdpClient.cpp
    server/server/src/CServer.cpp
    server/server/src/CTcpConnection.cpp
    server/server/src/CTcpServer.cpp
    server/server/src/CUdpServer.cpp
    server/server/src/CCommandHandler.cpp
)

set(MAIN_SERVER
    server/main.cpp
)

set(MAIN_CLIENT
    client/src/main.cpp    
)

set(SRCS_CLIENT 
    client/clientUI/loginScreen/mainwindow.cpp   
    client/clientUI/chatScreen/chatwindow.cpp 
    client/clientUI/signInScreen/signinscreen.cpp
    client/clientSocket/QtSocket.cpp  
    client/src/portAudio/CAudio.cpp
    client/src/opus/COpus.cpp
    client/clientSocket/QtDataSocket.cpp
    client/clientUI/chatScreen/CCallManager.cpp 
    client/clientUI/chatScreen/ContactInfo.cpp 
)

set(SRCS_COMMON
    common/client/CAUser.cpp
    common/protocol/AProtocol.cpp
    common/JsonUtility.cpp
)

set(SRCS_TEST
    tests/unit_tests.cpp
)


add_executable(babel_client ${SRCS_CLIENT} ${MAIN_CLIENT} ${SRCS_COMMON} )
add_executable(babel_server ${SRCS_SERVER} ${MAIN_SERVER} ${SRCS_COMMON} )

if(UNIX)
    add_executable(unit_test ${SRCS_TEST} ${SRCS_SERVER} ${SRCS_CLIENT} ${SRCS_COMMON} )
endif()

target_link_libraries(babel_client Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)

if(WIN32)
	target_link_libraries(babel_server libboost_system libboost_chrono libboost_thread libboost_serialization
        Threads::Threads sqlite3)
	target_link_libraries(babel_client portaudio_x64)
    target_link_libraries(babel_client libboost_system libboost_chrono libboost_thread libboost_serialization Threads::Threads opus)
elseif (UNIX)
	target_link_libraries(babel_server boost_system boost_chrono boost_thread boost_serialization
        Threads::Threads sqlite3 dl)
	target_link_libraries(unit_test boost_system boost_chrono boost_thread boost_serialization
        Threads::Threads sqlite3 Qt5::Widgets Qt5::Network dl criterion gcov jack asound portaudio opus)
	target_link_libraries(babel_client jack asound)
	target_link_libraries(babel_client portaudio opus)
	target_compile_options(unit_test PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_libraries(babel_client boost_system boost_chrono boost_thread Threads::Threads opus boost_serialization)
endif()

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/bin/qt.conf "[Paths]\nPrefix = ")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/bin/qt.conf ${CONAN_QT_ROOT})
