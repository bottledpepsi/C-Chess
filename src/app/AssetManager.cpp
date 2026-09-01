#include "../../include/app/AssetManager.hpp"

#include <algorithm>

#include "SFML/Audio/SoundBuffer.hpp"

bool AssetManager::loadFont(
    const std::string &name,
    const std::filesystem::path &path
) {
    sf::Font font;

    if (!font.openFromFile(path))
        return false;

    font.setSmooth(true);

    fonts.emplace(name, std::move(font));
    return true;
}

sf::Font &AssetManager::getFont(const std::string &name) {
    return fonts.at(name);
}

bool AssetManager::loadTexture(
    const std::string &name,
    const std::filesystem::path &path
) {
    sf::Texture texture;

    if (!texture.loadFromFile(path))
        return false;

    texture.setSmooth(true);

    textures.emplace(name, std::move(texture));
    return true;
}

sf::Texture &AssetManager::getTexture(const std::string &name) {
    return textures.at(name);
}

unsigned int AssetManager::sharpCharacterSize(
    unsigned int baseCharacterSize,
    float pixelScale
) {
    return std::max(
        1u,
        static_cast<unsigned int>(
            std::round(static_cast<float>(baseCharacterSize) * pixelScale)
        )
    );
}

bool AssetManager::loadSound(
    const std::string &name,
    const std::filesystem::path &path
    ) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(path))
        return false;

    sounds.emplace(name, std::move(buffer));
    return true;
}

sf::SoundBuffer &AssetManager::getSound(const std::string &name) {
    return sounds.at(name);
}
