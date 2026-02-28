# About
Bean Engine is an abstraction layer that makes a layer between the operating system and the game, making it more simple to create cross-platform games.

# How to use
create a folder in `games/` and add `CMakeLists.txt`, cmakelists should add the library `engine`, here is an example CMakeLists.txt
```cmake
add_executable(deathsmode src/main.cpp)
target_link_libraries(deathsmode PRIVATE engine)

if(PLATFORM_WINDOWS)
    target_compile_definitions(deathsmode PRIVATE PLATFORM_WINDOWS=1)
    set_target_properties(deathsmode PROPERTIES WIN32_EXECUTABLE ON)

    set(RES_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/resource.rc")
    if(EXISTS "${RES_FILE_PATH}")
        target_sources(deathsmode PRIVATE "${RES_FILE_PATH}")
        target_include_directories(deathsmode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/resources")
        target_compile_options(deathsmode PRIVATE "$<$<COMPILE_LANGUAGE:RC>:-I${CMAKE_CURRENT_SOURCE_DIR}/resources>")
    endif()
endif()

if(PLATFORM_3DS)
    add_dependencies(deathsmode engine assets_headers)
    target_link_libraries(deathsmode PRIVATE engine)
endif()

if(NOT PLATFORM_3DS)
    set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/games/deathsmode/assets")
    add_custom_command(TARGET deathsmode POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_SRC}"
        "$<TARGET_FILE_DIR:deathsmode>/assets"
        COMMENT "Copying deathsmode assets"
    )
endif()
```
