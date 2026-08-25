#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include "../app/AssetManager.hpp"
#include "../chess/chess.hpp"


class BoardRenderer {
public:
    BoardRenderer(
        const chess::Board &board,
        AssetManager &assetManager
    );

    void drawBoard(
        sf::RenderWindow &window,
        float pixelScale = 1.0f,
        std::optional<chess::Square> selectedSquare = std::nullopt,
        const std::vector<chess::Square> &legalDestinations = {}
    );

private:
    const chess::Board board;
    AssetManager &assets;
    sf::RectangleShape square;
    sf::Text notation;
};
