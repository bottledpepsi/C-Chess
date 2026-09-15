#pragma once

#include <SFML/Graphics.hpp>
#include "../app/AssetManager.hpp"
#include "BoardLayout.hpp"

class TrayRenderer {
public:
    static constexpr float TRAY_HEIGHT = 64.f;
    static constexpr sf::Color TRAY_COLOUR{52, 52, 52};
    float pixelScale = 1.0f;

    TrayRenderer(
        AssetManager &assetManager
    );

    void drawTrays(
        sf::RenderWindow &window,
        const BoardLayout &layout
    );

private:
    AssetManager &assets;
    sf::RectangleShape tray;
    sf::Text label;
};
