cmake_minimum_required(VERSION 3.14)
project(EogenVirtualMachine)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "D:/ProgramFilesWithNoSpace/boost_1_70_0")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "D:/ProgramFilesWithNoSpace/boost_1_70_0/libs")

find_package(Boost)
IF (Boost_FOUND)
    message("-- Found boost")
    include_directories(${Boost_INCLUDE_DIR})
else()
    message("-- Boos not found")
endif()

include_directories(api)
include_directories("D:/My Documents/Projects/C++/EogenVirtualMachine/include")

add_executable(EogenVirtualMachine EogenDriver.cpp EogenRuntime.cpp api/IO.cpp api/IO.hpp api/SysCallHandler.cpp api/SysCallHandler.h)