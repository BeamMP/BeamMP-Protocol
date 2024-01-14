cmake_minimum_required(VERSION 3.18)

project(BeamMP-Protocol)

set(CMAKE_CXX_STANDARD 20)

add_library(protocol STATIC
    include/State.h
    include/Transport.h
    include/Util.h
    src/Transport.cpp
)

target_include_directories(protocol PUBLIC include)

if (BEAMMP_PROTOCOL_TESTS)
    find_package(doctest REQUIRED)
    add_executable(protocol-tests
        src/tests.cpp
        include/State.h
        include/Transport.h
        include/Util.h
        src/Transport.cpp)
    target_include_directories(protocol-tests PUBLIC include)
    target_link_libraries(protocol-tests doctest::doctest)
endif()
