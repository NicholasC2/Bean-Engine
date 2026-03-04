# About
Bean Engine is an abstraction layer that makes a layer between the operating system and the game, making it more simple to create cross-platform games.

# How to use
create a folder in `games/` and add `CMakeLists.txt`, CMakeLists should add the library `engine`, here is an example CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(PLATFORM_WINDOWS on)
elseif(UNIX)
    set(PLATFORM_LINUX on)
elseif(CMAKE_SYSTEM_NAME strequal Nintendo3DS)
    set(PLATFORM_3DS on)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../engine" "${CMAKE_CURRENT_BINARY_DIR}/engine")

add_executable(example src/main.cpp)

target_link_libraries(example PRIVATE engine)
target_include_directories(example PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../engine/include")

add_dependencies(example engine)

if(NOT PLATFORM_3DS)
    target_compile_definitions(example PRIVATE PLATFORM_WINDOWS=1)
    set_target_properties(example PROPERTIES WIN32_EXECUTABLE ON)

    set(RES_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/resource.rc")
    if(EXISTS "${RES_FILE_PATH}")
        target_sources(example PRIVATE "${RES_FILE_PATH}")
        target_include_directories(example PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/resources")
        target_compile_options(example PRIVATE "$<$<COMPILE_LANGUAGE:RC>:-I${CMAKE_CURRENT_SOURCE_DIR}/resources>")
    endif()
endif()

set(ASSETS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_custom_command(TARGET example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_SRC}"
        "$<TARGET_FILE_DIR:example>/assets"
)
```
