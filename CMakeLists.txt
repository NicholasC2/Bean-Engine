cmake_minimum_required(VERSION 3.20)
# project(example)

if(WIN32)
    set(PLATFORM_WINDOWS ON)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Nintendo3DS")
    # set(PLATFORM_3DS ON)
    # not supported yet
endif()

add_subdirectory(engine)
# add_subdirectory(games/example)