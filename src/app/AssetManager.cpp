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
