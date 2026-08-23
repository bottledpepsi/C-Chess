#include "../../include/render/PieceRenderer.hpp"

#include <string>

namespace {
    constexpr int BOARD_SIZE = 8;
    constexpr float SQUARE_SIZE = 75.0f;
    constexpr float SIZE_MODIFIER = 0.9f;
    const sf::Vector2f BOARD_ORIGIN(28.f, 70.f);

    // Maps a chess::Piece to the AssetManager texture key used in App.cpp's pieceTextures table
    std::string textureKeyFor(chess::Piece piece) {
        const std::string colorPrefix =
            piece.color() == chess::Color::WHITE ? "w_" : "b_";

        switch (piece.type().internal()) {
            case chess::PieceType::underlying::PAWN:
                return colorPrefix + "pawn";
            case chess::PieceType::underlying::KNIGHT:
                return colorPrefix + "knight";
            case chess::PieceType::underlying::BISHOP:
                return colorPrefix + "bishop";
            case chess::PieceType::underlying::ROOK:
                return colorPrefix + "rook";
            case chess::PieceType::underlying::QUEEN:
                return colorPrefix + "queen";
            case chess::PieceType::underlying::KING:
                return colorPrefix + "king";
            default:
                return "";
        }
    }
}

PieceRenderer::PieceRenderer(
    const chess::Board &board,
    AssetManager &assetManager
)
    : board(board),
      assets(assetManager) {
}

void PieceRenderer::drawPieces(sf::RenderWindow &window) {
    for (int rank = 0; rank < BOARD_SIZE; ++rank) {
        for (int file = 0; file < BOARD_SIZE; ++file) {
            const chess::Square square(
                chess::File(file),
                chess::Rank(BOARD_SIZE - 1 - rank)
            );

            const chess::Piece piece = board.at(square);

            if (piece == chess::Piece::NONE) {
                continue;
            }

            const std::string textureKey = textureKeyFor(piece);
            if (textureKey.empty()) {
                continue;
            }

            sf::Texture &texture = assets.getTexture(textureKey);
            sf::Sprite sprite(texture);

            const sf::Vector2u textureSize = texture.getSize();

            if (textureSize.x > 0 && textureSize.y > 0) {
                sprite.setScale({
                    (SQUARE_SIZE * SIZE_MODIFIER) /
                        static_cast<float>(textureSize.x),
                    (SQUARE_SIZE * SIZE_MODIFIER) /
                        static_cast<float>(textureSize.y)
                });

                sprite.setOrigin({
                    static_cast<float>(textureSize.x) / 2.0f,
                    static_cast<float>(textureSize.y) / 2.0f
                });
            }

            sprite.setPosition({
                BOARD_ORIGIN.x + file * SQUARE_SIZE + SQUARE_SIZE / 2.0f,
                BOARD_ORIGIN.y + rank * SQUARE_SIZE + SQUARE_SIZE / 2.0f
            });

            window.draw(sprite);
        }
    }
}