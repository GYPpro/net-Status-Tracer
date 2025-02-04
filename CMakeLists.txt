set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "Installation directory" FORCE)

cmake_minimum_required(VERSION 3.0)
project(nst)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
)

include_directories(include)

add_executable(nst ${SOURCE_FILES})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(nst pthread stdc++fs)
    target_link_options(nst PRIVATE -static-libstdc++ -static-libgcc)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(nst ws2_32)
    target_link_options(nst PRIVATE -static)
endif()

install(TARGETS nst DESTINATION bin)