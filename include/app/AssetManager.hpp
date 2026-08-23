#pragma once

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    bool loadFont(
        const std::string &name,
        const std::filesystem::path &path
    );

    sf::Font &getFont(const std::string &name);

    bool loadTexture(
        const std::string &name,
        const std::filesystem::path &path
    );

    sf::Texture &getTexture(const std::string &name);

    static unsigned int sharpCharacterSize(
        unsigned int baseCharacterSize,
        float pixelScale
    );

private:
    std::unordered_map<std::string, sf::Font> fonts;
    std::unordered_map<std::string, sf::Texture> textures;
};
