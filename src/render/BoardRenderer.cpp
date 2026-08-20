#include "../../include/render/BoardRenderer.hpp"
#include <SFML/Graphics.hpp>

void BoardRenderer::drawBoard(sf::RenderWindow& window) {
    sf::RectangleShape square;

    square.setSize({SQUARE_SIZE, SQUARE_SIZE});

    for (int rank = 0; rank < BOARD_SIZE; ++rank) {

        for (int file = 0; file < BOARD_SIZE; ++file) {

            float x = file * SQUARE_SIZE;
            float y = rank * SQUARE_SIZE;

            square.setPosition({x, y});

            if ((file + rank) % 2 == 0) {
                square.setFillColor(sf::Color(238, 238, 210));

                }
            else {
                square.setFillColor(sf::Color(118, 150, 86));
            }

            window.draw(square);
        }
    }
};
