#pragma once

#include <SFML/Graphics.hpp>

#include "../app/AssetManager.hpp"
#include "../chess/chess.hpp"

class PieceRenderer {
public:
    PieceRenderer(
        const chess::Board &board,
        AssetManager &assetManager
    );

    void drawPieces(sf::RenderWindow &window);

private:
    const chess::Board &board;
    AssetManager &assets;
};
