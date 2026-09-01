#include "../../include/input/BoardCoords.hpp"
#include "../../include/render/BoardConstants.hpp"

const sf::Vector2f BoardCoords::BOARD_ORIGIN(60.f, 84.f);

chess::Square BoardCoords::screenToSquare(sf::Vector2f boardLocalPos, float) {
    const float relativeX = boardLocalPos.x - BOARD_ORIGIN.x;
    const float relativeY = boardLocalPos.y - BOARD_ORIGIN.y;

    if (relativeX < 0.f || relativeY < 0.f) {
        return chess::Square::NO_SQ;
    }

    const int screenFile = static_cast<int>(relativeX / BoardConstants::SQUARE_SIZE);
    const int screenRank = static_cast<int>(relativeY / BoardConstants::SQUARE_SIZE);

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

sf::Vector2f BoardCoords::squareToScreen(chess::Square sq, float) {
    if (!sq.is_valid()) {
        return BOARD_ORIGIN;
    }

    const int boardFile = sq.file();
    const int boardRank = sq.rank();

    const int screenFile = BoardConstants::FLIPPED
                               ? BoardConstants::BOARD_SIZE - 1 - boardFile
                               : boardFile;
    const int screenRank = BoardConstants::FLIPPED
                               ? boardRank
                               : BoardConstants::BOARD_SIZE - 1 - boardRank;

    const float x = BOARD_ORIGIN.x + static_cast<float>(screenFile) * BoardConstants::SQUARE_SIZE;
    const float y = BOARD_ORIGIN.y + static_cast<float>(screenRank) * BoardConstants::SQUARE_SIZE;

    return sf::Vector2f(x, y);
}
