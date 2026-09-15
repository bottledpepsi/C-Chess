#include "../../include/input/BoardCoords.hpp"
#include "../../include/render/BoardConstants.hpp"

chess::Square BoardCoords::screenToSquare(sf::Vector2f boardLocalPos, const BoardLayout &layout) {
    const float relativeX = boardLocalPos.x - layout.boardOrigin.x;
    const float relativeY = boardLocalPos.y - layout.boardOrigin.y;

    if (relativeX < 0.f || relativeY < 0.f || layout.squareSize <= 0.f) {
        return chess::Square::NO_SQ;
    }

    const int screenFile = static_cast<int>(relativeX / layout.squareSize);
    const int screenRank = static_cast<int>(relativeY / layout.squareSize);

    if (screenFile < 0 || screenFile >= BoardConstants::BOARD_SIZE ||
        screenRank < 0 || screenRank >= BoardConstants::BOARD_SIZE) {
        return chess::Square::NO_SQ;
    }

    const int boardFile = BoardConstants::FLIPPED
                              ? BoardConstants::BOARD_SIZE - 1 - screenFile
                              : screenFile;
    const int boardRank = BoardConstants::FLIPPED
                              ? screenRank
                              : BoardConstants::BOARD_SIZE - 1 - screenRank;

    return chess::Square(chess::File(boardFile), chess::Rank(boardRank));
}

sf::Vector2f BoardCoords::squareToScreen(chess::Square sq, const BoardLayout &layout) {
    if (!sq.is_valid()) {
        return layout.boardOrigin;
    }

    const int boardFile = sq.file();
    const int boardRank = sq.rank();

    const int screenFile = BoardConstants::FLIPPED
                               ? BoardConstants::BOARD_SIZE - 1 - boardFile
                               : boardFile;
    const int screenRank = BoardConstants::FLIPPED
                               ? boardRank
                               : BoardConstants::BOARD_SIZE - 1 - boardRank;

    const float x = layout.boardOrigin.x + static_cast<float>(screenFile) * layout.squareSize;
    const float y = layout.boardOrigin.y + static_cast<float>(screenRank) * layout.squareSize;

    return sf::Vector2f(x, y);
}
