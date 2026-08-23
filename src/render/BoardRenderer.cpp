#include "../../include/render/BoardRenderer.hpp"


const sf::Color LightColor(238, 238, 210);
const sf::Color DarkColor(118, 150, 86);
const sf::Vector2f BOARD_ORIGIN(28.f, 70.f);


BoardRenderer::BoardRenderer(
    const chess::Board &board,
    AssetManager &assetManager
)
    : board(board),
      assets(assetManager) {
}

void BoardRenderer::drawBoard(sf::RenderWindow &window, float pixelScale) {
    sf::RectangleShape square;
    square.setSize({SQUARE_SIZE, SQUARE_SIZE});

    const unsigned int scaledCharacterSize =
        AssetManager::sharpCharacterSize(16, pixelScale);

    sf::Text notation(assets.getFont("arial"));
    notation.setCharacterSize(scaledCharacterSize);

    if (pixelScale > 0.f) {
        const float textScale = 1.f / pixelScale;
        notation.setScale({textScale, textScale});
    }

    for (int rank = 0; rank < BOARD_SIZE; ++rank) {
        for (int file = 0; file < BOARD_SIZE; ++file) {
            float x = BOARD_ORIGIN.x + file * SQUARE_SIZE;
            float y = BOARD_ORIGIN.y + rank * SQUARE_SIZE;

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
