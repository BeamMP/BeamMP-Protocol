cmake_minimum_required(VERSION 3.18)

project(BeamMP-Protocol)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    include/Identification.h
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
    src/Identification.cpp
    src/ImplementationInfo.cpp
    src/Identification.cpp
    src/Transport.cpp
    include/Hash.h
    src/Hash.cpp)

add_library(protocol STATIC
    ${SOURCES})

target_include_directories(protocol PUBLIC include)
target_compile_options(protocol PRIVATE -DDOCTEST_CONFIG_DISABLE=1)

if (BEAMMP_PROTOCOL_TESTS)
    find_package(doctest REQUIRED)
    add_executable(protocol-tests
        src/tests.cpp
        ${SOURCES})
    target_include_directories(protocol-tests PUBLIC include)
    target_link_libraries(protocol-tests doctest::doctest)
endif()
