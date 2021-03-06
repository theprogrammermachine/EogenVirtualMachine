cmake_minimum_required(VERSION 3.10.2)
project(EogenVirtualMachine)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/keyhan/globe_libs/boost_1_70_0")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/keyhan/globe_libs/boost_1_70_0/libs")

find_package(Boost)
IF (Boost_FOUND)
    message("-- Found boost")
    include_directories(${Boost_INCLUDE_DIR})
else()
    message("-- Boost not found")
endif()

include_directories(/home/keyhan/globe_libs/boost_1_70_0)
include_directories(/home/keyhan/globe_libs/mpir-3.0.0)
include_directories(/home/keyhan/projects/c++/EogenVirtualMachine/api)
include_directories(/home/keyhan/projects/c++/EogenVirtualMachine/include)

add_executable(
        EogenVirtualMachine
        v3/main.c
        v3/Rider.h
        v3/Dictionary.h
        v3/List.h
        v3/Stack.h
        v3/Models.h
        v3/Dictionary.c)

target_compile_options(EogenVirtualMachine PRIVATE -lmpir -lgmp -lm)

target_link_libraries(EogenVirtualMachine gmp m)
