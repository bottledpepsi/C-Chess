#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../app/AssetManager.hpp"
#include "BoardLayout.hpp"

class MoveHistoryPanel {
public:
    static constexpr float PANEL_WIDTH = 300.f;
    static constexpr sf::Color PANEL_COLOUR{38, 38, 38};
    static constexpr sf::Color ROW_ALT_COLOUR{46, 46, 46};
    static constexpr sf::Color TEXT_COLOUR{230, 230, 230};
    static constexpr float ROW_HEIGHT = 28.f;
    static constexpr float PADDING = 12.f;

    float pixelScale = 1.0f;

    explicit MoveHistoryPanel(AssetManager &assetManager);

    void addMove(const std::string &moveText);

    void clear();

    void drawPanel(
        sf::RenderWindow &window,
        const BoardLayout &layout
    );

private:
    AssetManager &assets;
    sf::RectangleShape background;
    sf::RectangleShape rowHighlight;
    sf::Text label;
    std::vector<std::string> moves;
};
