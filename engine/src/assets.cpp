#include "engine/assets.h"
#include "engine/gfx/renderer.h"
#include <SDL3/SDL.h>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

#define ASSET_PATH ".\\assets"

namespace Assets {
    static std::unordered_map<std::string, Asset> assets;

    bool loadAllAssets() {
        if (!std::filesystem::exists(ASSET_PATH)) {
            return false;
        }

        for (const auto& entry :
         std::filesystem::recursive_directory_iterator(ASSET_PATH))
        {
            if (!entry.is_regular_file())
                continue;

            std::ifstream file(entry.path(), std::ios::binary);
            if (!file)
                continue;

            std::vector<uint8_t> data(
                (std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>()
            );

            std::string key = std::filesystem::relative(entry.path(), ASSET_PATH).string();
            std::replace(key.begin(), key.end(), '\\', '/');

            assets.emplace(key, Asset(entry.path().string(), std::move(data)));
        }

        return true;
    }

    Asset* getAsset(const std::string& path) {
        auto it = assets.find(path);
        if (it != assets.end())
            return &it->second;

        std::string errorMsg = "Invalid asset location: " + path;
        Renderer::showError(errorMsg.c_str());

        return {};
    }

    void unloadAll() {
        assets.clear();
    }
}