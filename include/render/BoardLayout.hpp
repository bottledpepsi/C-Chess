#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

struct BoardLayout {
    sf::Vector2f boardOrigin{0.f, 0.f};
    float squareSize = 1.f;
    float boardSize = 1.f; // squareSize * BoardConstants::BOARD_SIZE

    float trayWidth = 0.f;
    float trayHeight = 0.f;
    sf::Vector2f topTrayPosition{0.f, 0.f};
    sf::Vector2f bottomTrayPosition{0.f, 0.f};

    static BoardLayout compute(
        sf::Vector2u windowSize,
        float trayHeight,
        float sideMargin,
        float verticalMargin
    );
};
