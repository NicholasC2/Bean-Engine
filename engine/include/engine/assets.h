#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

namespace Assets {
    class Asset {
    public:
        Asset() = default;

        Asset(const std::string& path, std::vector<uint8_t>&& data)
            : m_path(path), m_data(std::move(data)) {}

        const std::string& getPath() const {
            return m_path;
        }

        const uint8_t* getData() const {
            return m_data.data();
        }

        size_t getSize() const {
            return m_data.size();
        }

        bool isValid() const {
            return !m_data.empty();
        }

    private:
        std::string m_path;
        std::vector<uint8_t> m_data;
    };

    bool loadAllAssets();
    Asset* getAsset(const std::string& path);
    void unloadAll();
}