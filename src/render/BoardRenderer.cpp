#include "../../include/render/BoardRenderer.hpp"


const sf::Color LightColor(238, 238, 210);
const sf::Color DarkColor(118, 150, 86);


BoardRenderer::BoardRenderer(
    const chess::Board &board,
    AssetManager &assetManager
)
    : board(board),
      assets(assetManager) {
}

void BoardRenderer::drawBoard(sf::RenderWindow &window) {
    sf::RectangleShape square;
    square.setSize({SQUARE_SIZE, SQUARE_SIZE});

    sf::Text notation(assets.getFont("arial"));
    notation.setCharacterSize(16);

    for (int rank = 0; rank < BOARD_SIZE; ++rank) {
        for (int file = 0; file < BOARD_SIZE; ++file) {
            float x = file * SQUARE_SIZE;
            float y = rank * SQUARE_SIZE;

            square.setPosition({x, y});

            bool lightSquare = (file + rank) % 2 == 0;

            if (lightSquare)
                square.setFillColor(LightColor);
            else
                square.setFillColor(sf::Color(DarkColor));

            window.draw(square);

            if (rank == BOARD_SIZE - 1) {
                notation.setString(
                    static_cast<char>('a' + file)
                );

                notation.setPosition({
                    x + SQUARE_SIZE / 1.25f,
                    y + SQUARE_SIZE - 22.f
                });

                notation.setFillColor(
                    lightSquare
                        ? sf::Color(DarkColor)
                        : sf::Color(LightColor)
                );

                window.draw(notation);
            }

            if (file == 0) {
                notation.setString(
                    std::to_string(BOARD_SIZE - rank)
                );

                notation.setPosition({
                    x + 5.f,
                    y + 3.f
                });

                notation.setFillColor(
                    lightSquare
                        ? sf::Color(DarkColor)
                        : sf::Color(LightColor)
                );

                window.draw(notation);
            }
        }
    }
}
