//
// Created by danil on 22.09.2022.
//

#include "Assets.h"
#include <unordered_map>
#include <functional>
#include <memory>

namespace assets {
    template<class Key, class Value>
    class assets_repo {
    public:
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

    std::shared_ptr<gl::Texture> texture(const std::string& name) {
        return textures.computeIfAbsent(name, [](const std::string& key) {
            return new gl::Texture(key);
        });
    }
}
