#pragma once

#include <SFML/Graphics.hpp>
#include "../app/AssetManager.hpp"
#include "../chess/chess.hpp"


class BoardRenderer {
public:
    BoardRenderer(
        const chess::Board &board,
        AssetManager &assetManager
    );

    void drawBoard(sf::RenderWindow &window, float pixelScale = 1.0f);

private:
    const chess::Board board;
    AssetManager &assets;
    sf::RectangleShape square;
    sf::Text notation;
};
