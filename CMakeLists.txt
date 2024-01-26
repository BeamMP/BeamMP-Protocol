cmake_minimum_required(VERSION 3.18)

project(BeamMP-Protocol)

message(STATUS "MSVC -> forcing use of statically-linked runtime.")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    include/ClientInfo.h
    include/Compression.h
    include/Hash.h
    include/ImplementationInfo.h
    include/ModRequest.h
    include/ModResponse.h
    include/Packet.h
    include/ProtocolVersion.h
    include/ServerInfo.h
    include/State.h
    include/Transport.h
    include/Util.h
    include/Version.h
    src/ClientInfo.cpp
    src/Compression.cpp
    src/Hash.cpp
    src/ImplementationInfo.cpp
    src/ModRequest.cpp
    src/ModResponse.cpp
    src/Packet.cpp
    src/ProtocolVersion.cpp
    src/ServerInfo.cpp
    src/Transport.cpp
    src/Version.cpp
)

add_library(protocol STATIC
    ${SOURCES})

find_package(zstd REQUIRED)

target_link_libraries(protocol zstd::libzstd_static)
target_include_directories(protocol PUBLIC include)
target_compile_options(protocol PRIVATE -DDOCTEST_CONFIG_DISABLE=1)

if (NOT BEAMMP_NO_PROTOCOL_TESTS)
    find_package(doctest REQUIRED)
    add_executable(protocol-tests
        src/tests.cpp
        ${SOURCES})
    target_include_directories(protocol-tests PUBLIC include)
    target_link_libraries(protocol-tests doctest::doctest zstd::libzstd_static)
endif()
