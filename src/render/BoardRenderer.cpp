#include "../../include/render/BoardRenderer.hpp"
#include "../../include/render/BoardConstants.hpp"


const sf::Color LightColor(238, 238, 210);
const sf::Color DarkColor(118, 150, 86);
const sf::Vector2f BOARD_ORIGIN(28.f, 70.f);


BoardRenderer::BoardRenderer(
    const chess::Board &board,
    AssetManager &assetManager
)
    : board(board),
      assets(assetManager),
      notation(assets.getFont("arial")) {
    square.setSize({BoardConstants::SQUARE_SIZE, BoardConstants::SQUARE_SIZE});
}

void BoardRenderer::drawBoard(sf::RenderWindow &window, float pixelScale) {

    const unsigned int scaledCharacterSize =
            AssetManager::sharpCharacterSize(16, pixelScale);

    notation.setCharacterSize(scaledCharacterSize);

    if (pixelScale > 0.f) {
        const float textScale = 1.f / pixelScale;
        notation.setScale({textScale, textScale});
    }

    for (int screenRank = 0; screenRank < BoardConstants::BOARD_SIZE; ++screenRank) {
        for (int screenFile = 0; screenFile < BoardConstants::BOARD_SIZE; ++screenFile) {
            const int boardFile = BoardConstants::FLIPPED
                ? BoardConstants::BOARD_SIZE - 1 - screenFile
                : screenFile;
            const int boardRank = BoardConstants::FLIPPED
                ? screenRank
                : BoardConstants::BOARD_SIZE - 1 - screenRank;

            float x = BOARD_ORIGIN.x + screenFile * BoardConstants::SQUARE_SIZE;
            float y = BOARD_ORIGIN.y + screenRank * BoardConstants::SQUARE_SIZE;

            square.setPosition({x, y});

            bool lightSquare = (boardFile + boardRank) % 2 != 0;

            if (lightSquare)
                square.setFillColor(LightColor);
            else
                square.setFillColor(sf::Color(DarkColor));

            window.draw(square);

            if (screenRank == BoardConstants::BOARD_SIZE - 1) {
                notation.setString(
                    static_cast<char>('a' + boardFile)
                );

                notation.setPosition({
                    x + BoardConstants::SQUARE_SIZE / 1.25f,
                    y + BoardConstants::SQUARE_SIZE - 22.f
                });

                notation.setFillColor(
                    lightSquare
                        ? sf::Color(DarkColor)
                        : sf::Color(LightColor)
                );

                window.draw(notation);
            }

            if (screenFile == 0) {
                notation.setString(
                    std::to_string(boardRank + 1)
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
