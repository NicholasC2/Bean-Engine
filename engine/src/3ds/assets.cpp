#include "assets_generated.h"
#include "assets.h"
#include "render.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace Assets {

static std::unordered_map<std::string, Asset> assets;

bool loadAllAssets() {
    AssetsRaw::initAssets();

    for (const auto& [name, raw] : AssetsRaw::assetMap) {
        std::vector<uint8_t> data(raw.data, raw.data + raw.size);
        assets.emplace(name, Asset(name, std::move(data)));
    }

    return true;
}

Asset* getAsset(const std::string& path) {
    auto it = assets.find(path);
    if (it != assets.end())
        return &it->second;

    Render::showError(("Invalid asset location: " + path).c_str());
    return nullptr;
}

void unloadAll() {
    assets.clear();
}

} // namespace Assets