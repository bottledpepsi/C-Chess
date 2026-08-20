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

    void drawBoard(sf::RenderWindow &window);

private:
    chess::Board board;
    AssetManager &assets;

    static constexpr int BOARD_SIZE = 8;
    static constexpr float SQUARE_SIZE = 80.0f;
    static constexpr bool FLIPPED = false;
};
