#include "../../include/render/GameOverRenderer.hpp"
#include "../../include/render/BoardConstants.hpp"
#include "../../include/input/BoardCoords.hpp"

GameOverRenderer::GameOverRenderer(AssetManager &assetManager, const InputHandler &inputHandler)
    : assets(assetManager),
      input(inputHandler),
      title(assets.getFont("arial"), ""),
      subtitle(assets.getFont("arial"), "") {
    title.setCharacterSize(32);
    title.setFillColor(sf::Color(240, 240, 240));
    title.setStyle(sf::Text::Bold);

    subtitle.setCharacterSize(18);
    subtitle.setFillColor(sf::Color(200, 200, 200));
}

std::string GameOverRenderer::buildTitle() const {
    switch (input.gameResultReason()) {
        case chess::GameResultReason::CHECKMATE:
            return "Checkmate";
        case chess::GameResultReason::STALEMATE:
            return "Stalemate";
        case chess::GameResultReason::INSUFFICIENT_MATERIAL:
            return "Draw";
        case chess::GameResultReason::THREEFOLD_REPETITION:
            return "Draw";
        case chess::GameResultReason::FIFTY_MOVE_RULE:
            return "Draw";
        default:
            return "Game Over";
    }
}

std::string GameOverRenderer::buildSubtitle() const {
    // sideToMove() is the side that is currently unable to move (checkmate/stalemate
    // are only ever detected on the side to move), so the winner is the other side.
    const chess::Color sideToMove = input.board().sideToMove();

    switch (input.gameResultReason()) {
        case chess::GameResultReason::CHECKMATE: {
            const chess::Color winner = ~sideToMove;
            return (winner == chess::Color::WHITE ? std::string("White") : std::string("Black")) + " wins";
        }
        case chess::GameResultReason::STALEMATE:
            return "Draw by stalemate";
        case chess::GameResultReason::INSUFFICIENT_MATERIAL:
            return "Draw by insufficient material";
        case chess::GameResultReason::THREEFOLD_REPETITION:
            return "Draw by threefold repetition";
        case chess::GameResultReason::FIFTY_MOVE_RULE:
            return "Draw by fifty-move rule";
        default:
            return "";
    }
}

void GameOverRenderer::drawGameOver(sf::RenderWindow &window) {
    if (!input.isGameOver()) {
        return;
    }

    const float boardSize = BoardConstants::SQUARE_SIZE * BoardConstants::BOARD_SIZE;

    overlay.setSize({boardSize, boardSize});
    overlay.setPosition(BoardCoords::BOARD_ORIGIN);
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    const float boardCenterX = BoardCoords::BOARD_ORIGIN.x + boardSize / 2.f;
    const float boardCenterY = BoardCoords::BOARD_ORIGIN.y + boardSize / 2.f;

    title.setString(buildTitle());
    subtitle.setString(buildSubtitle());

    const sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({
        boardCenterX - titleBounds.size.x / 2.f - titleBounds.position.x,
        boardCenterY - 30.f
    });
    window.draw(title);

    const sf::FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition({
        boardCenterX - subtitleBounds.size.x / 2.f - subtitleBounds.position.x,
        boardCenterY + 14.f
    });
    window.draw(subtitle);
}
