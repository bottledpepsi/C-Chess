#include "../../include/app/AssetManager.hpp"

bool AssetManager::loadFont(
    const std::string &name,
    const std::filesystem::path &path
) {
    sf::Font font;

    if (!font.openFromFile(path))
        return false;

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
