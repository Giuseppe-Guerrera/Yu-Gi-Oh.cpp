cmake_minimum_required(VERSION 3.28.3)
project(Yu-Gi-Oh.cpp VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)
add_definitions(-DASIO_STANDALONE)

option(ENABLE_SANITIZERS "Enable sanitizers" ON)
option(ENABLE_SANITIZE_ADDRESS ON)
option(ENABLE_SANITIZE_UB OFF)
option(ENABLE_LTO "Enables Link Time/Interprocedural Optimazation" ON)
option(ENABLE_TESTS "Enable unit test builds" ON)
option(CREATE_DOCS "Automatically creates docs on build" OFF)

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(Warnings)
add_compiler_warnings()

if(ENABLE_SANITIZERS)
    include(Sanitizers)
    add_sanitizer_flags()
endif()
if(ENABLE_LTO)
    include(LTO)
    enable_lto()
endif()
if(ENABLE_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif()
if(CREATE_DOCS)
    include(Doxygen)
endif()

find_package(asio CONFIG REQUIRED)
add_subdirectory(src)

set(SOURCES
    src/main.cpp
    src/card.cpp
    src/deck.cpp
    src/king.cpp
    src/print.cpp
    src/load_files.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE peer)