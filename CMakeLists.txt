cmake_minimum_required(VERSION 3.14)
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

include_directories(/home/keyhan/projects/c++/EogenVirtualMachine/api)
include_directories("/home/keyhan/projects/c++/EogenVirtualMachine/include")
include_directories("/usr/local/lib")
include_directories("/usr/local/include")

set(GMP_PREFIX "" CACHE PATH "path ")


find_path(GMP_INCLUDE_DIR gmp.h gmpxx.h
        PATHS ${GMP_PREFIX}/include /usr/include /usr/local/include )

find_library(GMP_LIBRARY NAMES gmp libgmp
        PATHS ${GMP_PREFIX}/lib /usr/lib /usr/local/lib)


if(GMP_INCLUDE_DIR AND GMP_LIBRARY)
    get_filename_component(GMP_LIBRARY_DIR ${GMP_LIBRARY} PATH)
    set(GMP_FOUND TRUE)
endif()

if(GMP_FOUND)
    if(NOT GMP_FIND_QUIETLY)
        MESSAGE(STATUS "Found GMP: ${GMP_LIBRARY}")
    endif()
elseif(GMP_FOUND)
    if(GMP_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find GMP")
    endif()
endif()

add_executable(
        EogenVirtualMachine
        EogenDriver.cpp
        EogenRuntime.cpp
        api/IO.cpp
        api/IO.hpp
        api/SysCallHandler.cpp
        api/SysCallHandler.hpp
        api/Time.cpp
        api/Time.h)

target_link_libraries(EogenVirtualMachine gmp)