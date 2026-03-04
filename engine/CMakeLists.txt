cmake_minimum_required(VERSION 3.20)
project(engine)

include(FetchContent)

set(ENGINE_SOURCES)

if(PLATFORM_3DS)
    add_compile_definitions(PLATFORM_3DS=1)
    list(APPEND ENGINE_SOURCES
        src/3ds/render.cpp
        src/3ds/input.cpp
        src/3ds/assets.cpp
    )
else()
    add_compile_definitions(PLATFORM_SDL=1)
    list(APPEND ENGINE_SOURCES
        src/renderer.cpp
        src/engine.cpp
        src/input.cpp
        src/assets.cpp
        src/texture.cpp
    )
endif()

add_library(engine ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC include)

if(NOT PLATFORM_3DS)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.2
    )
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SDL3)

    target_link_libraries(engine PUBLIC SDL3::SDL3-static)
endif()

FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.1.1
)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(box2d)
target_link_libraries(engine PUBLIC box2d::box2d)

if(PLATFORM_3DS)
    target_link_libraries(engine PRIVATE ctru citro2d citro3d)

    set(ASSET_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets_headers")
    file(MAKE_DIRECTORY "${ASSET_HEADER_DIR}")

    set(ASSET_MASTER_HEADER "${ASSET_HEADER_DIR}/assets_generated.h")
    file(WRITE "${ASSET_MASTER_HEADER}"
        "// Auto-generated asset header\n"
        "#pragma once\n"
        "#include <stddef.h>\n"
        "#include <stdint.h>\n"
        "#include <unordered_map>\n"
        "#include <string>\n\n"
        "namespace AssetsRaw {\n\n"
        "struct RawAsset {\n"
        "    const uint8_t* data;\n"
        "    size_t size;\n"
        "};\n\n"
        "static std::unordered_map<std::string, RawAsset> assetMap;\n\n"
        "void initAssets() {\n"
    )

    set(GENERATED_SOURCES "")
    foreach(asset ${ASSET_FILES})
        file(RELATIVE_PATH asset_rel_path "${ASSET_DIR}" ${asset})
        string(REPLACE "\\" "/" asset_rel_path ${asset_rel_path})
        get_filename_component(asset_name ${asset} NAME)
        string(REPLACE "." "_" sym_name ${asset_name})

        set(header "${ASSET_HEADER_DIR}/${asset_rel_path}.h")
        set(source "${ASSET_HEADER_DIR}/${asset_rel_path}.c")
        get_filename_component(header_dir ${header} DIRECTORY)
        file(MAKE_DIRECTORY "${header_dir}")

        add_custom_command(
            OUTPUT ${header} ${source}
            COMMAND bin2c -d "${header}" -o "${source}" -n ${sym_name} "${asset}"
            DEPENDS ${asset}
            COMMENT "Generating C header & source for asset: ${asset_rel_path}"
        )

        list(APPEND GENERATED_HEADERS ${header})
        list(APPEND GENERATED_SOURCES ${source})
    endforeach()

    target_sources(engine PRIVATE ${GENERATED_SOURCES})

    file(APPEND "${ASSET_MASTER_HEADER}" "}\n} // namespace AssetsRaw\n")

    add_custom_target(assets_headers ALL DEPENDS ${GENERATED_HEADERS} "${ASSET_MASTER_HEADER}")
    add_dependencies(engine assets_headers)
    target_include_directories(engine PRIVATE "${ASSET_HEADER_DIR}")
endif()