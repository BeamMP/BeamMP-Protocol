cmake_minimum_required(VERSION 3.18)

project(BeamMP-Protocol)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    include/ImplementationInfo.h
    include/State.h
    include/Transport.h
    include/Util.h
    include/Version.h
    include/ClientInfo.h
    include/ServerInfo.h
    include/ModRequest.h
    src/ModRequest.cpp
    src/ClientInfo.cpp
    src/Version.cpp
    src/ServerInfo.cpp
    src/ImplementationInfo.cpp
    src/Transport.cpp
    include/Hash.h
    src/Hash.cpp
    src/ModResponse.cpp
    include/ModResponse.h
    include/ProtocolVersion.h
    src/ProtocolVersion.cpp
    src/Compression.cpp
    include/Compression.h
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
