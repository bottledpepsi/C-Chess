#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include "../app/AssetManager.hpp"
#include "../chess/chess.hpp"


class BoardRenderer {
public:
    float pixelScale = 1.0f;

    BoardRenderer(
        const chess::Board &board,
        AssetManager &assetManager
    );

    void drawBoard(
        sf::RenderWindow &window,
        std::optional<chess::Square> selectedSquare = std::nullopt,
        const std::vector<chess::Square> &legalDestinations = {},
        const std::vector<chess::Square> &legalCaptures = {}
    );

private:
    const chess::Board board;
    AssetManager &assets;
    sf::RectangleShape square;
    sf::Text notation;

    sf::CircleShape DestinationDot;
    sf::CircleShape captureRing;
};
