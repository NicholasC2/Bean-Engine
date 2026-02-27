include(FetchContent)

function(import_box2d target_name)

    FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.1.1
    )

    set(BUILD_SHARED_LIBS OFF)

    FetchContent_MakeAvailable(box2d)

    target_link_libraries(${target_name}
        PUBLIC box2d::box2d
    )

endfunction()

function(import_sdl3 target_name)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.0
    )

    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(SDL3)

    target_link_libraries(${target_name}
        PRIVATE SDL3::SDL3-static
    )
endfunction()

set(ENGINE_SOURCES)

if(PLATFORM_3DS)
    add_compile_definitions(PLATFORM_3DS=1)
    list(APPEND ENGINE_SOURCES 
        src/3ds/render.cpp
        src/3ds/input.cpp
    )
else()
    add_compile_definitions(PLATFORM_SDL=1)
    list(APPEND ENGINE_SOURCES 
        src/render.cpp
        src/input.cpp
    )
endif()

add_library(engine ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC include)

if(PLATFORM_WINDOWS)
    import_box2d(engine)
    import_sdl3(engine)
    target_link_libraries(engine PRIVATE)

elseif(PLATFORM_LINUX)
    import_box2d(engine)
    import_sdl3(engine)
    target_link_libraries(engine PRIVATE)

elseif(PLATFORM_3DS)
    # link 3ds libs here
endif()