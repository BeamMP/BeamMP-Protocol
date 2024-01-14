cmake_minimum_required(VERSION 3.18)

project(BeamMP-Protocol)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    include/State.h
    include/Transport.h
    include/Util.h
    include/Identification.h
    src/Identification.cpp
    src/Transport.cpp)

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
