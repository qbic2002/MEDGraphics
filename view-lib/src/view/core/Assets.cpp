//
// Created by danil on 22.09.2022.
//

#include <view/core/Assets.h>
#include <unordered_map>
#include <functional>
#include "img/imageLoader.h"

namespace assets {
    /// Shared object repository based on shared smart pointers and unordered map
    /// @tparam Key - key type
    /// @tparam Value - value type
    template<class Key, class Value>
    class assets_repo {
    public:
        /// Return value by key if exists; create, put in storage and return created otherwise
        std::shared_ptr<Value> computeIfAbsent(const Key& key, std::function<Value*(const Key& key)> provider) {
            auto iter = storage.find(key);
            if (iter != storage.end()) {
                return iter->second.lock();
            }

            auto deleter = [this, key](Value* value) {
                storage.erase(key);
                delete value;
            };

            std::shared_ptr<Value> shared(provider(key), deleter);
            storage[key] = std::weak_ptr(shared);

            return shared;
        }

    private:
        std::unordered_map<Key, std::weak_ptr<Value>> storage;
    };

    assets_repo<std::string, gl::Texture> textures;
    assets_repo<unsigned, gl::FontRenderer> fontRenderers;

    /// Hash-like convert std::string to unsigned int
    std::hash<std::string> stringHasher;

    std::shared_ptr<gl::Texture> texture(const std::filesystem::path& file) {
        return textures.computeIfAbsent(file.string(), [](const std::string& key) {
            auto* raster = img::loadImageData(key);
            auto* result = new gl::Texture(*raster);
            delete raster;
            return result;
        });
    }

    std::shared_ptr<gl::FontRenderer> fontRenderer(const std::filesystem::path& file, unsigned fontSize) {
        return fontRenderers.computeIfAbsent(unsigned(stringHasher(file.string()) + fontSize),
                                             [file, fontSize](const unsigned& key) {
                                                 return new gl::FontRenderer(file, fontSize);
                                             });
    }
}
