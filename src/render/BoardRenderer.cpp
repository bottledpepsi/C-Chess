#include "../../include/render/BoardRenderer.hpp"
#include "../../include/render/BoardConstants.hpp"
#include "../../include/input/BoardCoords.hpp"


const sf::Color LightColor(238, 238, 210);
const sf::Color DarkColor(118, 150, 86);
const sf::Color SelectedTint(246, 246, 105);
const sf::Color CheckTint(255, 55, 55);

namespace {
    sf::Color blend(sf::Color base, sf::Color tint, float tintWeight) {
        const auto mix = [&](std::uint8_t b, std::uint8_t t) {
            return static_cast<std::uint8_t>(
                static_cast<float>(b) * (1.f - tintWeight) + static_cast<float>(t) * tintWeight
            );
        };
        return sf::Color(mix(base.r, tint.r), mix(base.g, tint.g), mix(base.b, tint.b));
    }
}

BoardRenderer::BoardRenderer(
    const chess::Board &board,
    AssetManager &assetManager
)
    : board(board),
      assets(assetManager),
      notation(assets.getFont("arial")),
      DestinationDot(1.f),
      captureRing(1.f) {
    DestinationDot.setFillColor(sf::Color(0, 0, 0, 90));
    captureRing.setFillColor(sf::Color::Transparent);
    captureRing.setOutlineColor(sf::Color(0, 0, 0, 90));
}

void BoardRenderer::drawBoard(
    sf::RenderWindow &window,
    const BoardLayout &layout,
    std::optional<chess::Square> selectedSquare,
    const std::vector<chess::Square> &legalDestinations,
    const std::vector<chess::Square> &legalCaptures
) {
    const float squareSize = layout.squareSize;

    square.setSize({squareSize, squareSize});

    const float dotRadius = (squareSize * 0.16f) * pixelScale;
    DestinationDot.setRadius(dotRadius);
    DestinationDot.setOrigin({dotRadius, dotRadius});

    const float ringRadius = (squareSize / 2.f - 4.f) * pixelScale;
    captureRing.setRadius(ringRadius);
    captureRing.setOutlineThickness(-4.f);
    captureRing.setOrigin({ringRadius, ringRadius});

    const unsigned int scaledCharacterSize =
            AssetManager::sharpCharacterSize(16, pixelScale);

    notation.setCharacterSize(scaledCharacterSize);

    notation.setScale({1.f, 1.f});

    for (int screenRank = 0; screenRank < BoardConstants::BOARD_SIZE; ++screenRank) {
        for (int screenFile = 0; screenFile < BoardConstants::BOARD_SIZE; ++screenFile) {
            const int boardFile = BoardConstants::FLIPPED
                                      ? BoardConstants::BOARD_SIZE - 1 - screenFile
                                      : screenFile;
            const int boardRank = BoardConstants::FLIPPED
                                      ? screenRank
                                      : BoardConstants::BOARD_SIZE - 1 - screenRank;

            const chess::Square sq{chess::File(boardFile), chess::Rank(boardRank)};
            const sf::Vector2f pos = BoardCoords::squareToScreen(sq, layout);
            const float x = pos.x;
            const float y = pos.y;

            square.setPosition({x, y});

            bool lightSquare = (boardFile + boardRank) % 2 != 0;

            sf::Color fillColor = lightSquare ? LightColor : sf::Color(DarkColor);

            if (board.inCheck()) {
                const chess::Square kingSquare = board.kingSq(board.sideToMove());

                if (kingSquare == sq) {
                    fillColor = blend(fillColor, CheckTint, 0.85f);
                }
            }

            if (selectedSquare.has_value() && selectedSquare.value() == sq) {
                fillColor = blend(fillColor, SelectedTint, 0.45f);
            }

            square.setFillColor(fillColor);

            window.draw(square);

            const sf::Vector2f center{
                x + squareSize / 2.f,
                y + squareSize / 2.f
            };

            if (std::find(legalCaptures.begin(), legalCaptures.end(), sq) != legalCaptures.end()) {
                captureRing.setPosition(center);
                window.draw(captureRing);
            } else if (std::find(legalDestinations.begin(), legalDestinations.end(), sq) !=
                       legalDestinations.end()) {
                DestinationDot.setPosition(center);
                window.draw(DestinationDot);
            }

            if (screenRank == BoardConstants::BOARD_SIZE - 1) {
                notation.setString(
                    static_cast<char>('a' + boardFile)
                );

                notation.setPosition({
                    x + squareSize / 1.25f,
                    y + squareSize - 22.f * pixelScale
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
                    x + 5.f * pixelScale,
                    y + 3.f * pixelScale
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
