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

private:
    std::unordered_map<std::string, sf::Font> fonts;
};
