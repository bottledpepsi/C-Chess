#pragma once

#include <SFML/Graphics.hpp>

#include <optional>
#include <vector>

#include "../chess/chess.hpp"

class InputHandler {
public:
    InputHandler(chess::Board &board, sf::RenderWindow &window, sf::View &gameView);

    void handleEvent(const sf::Event &event, float pixelScale);

    std::optional<chess::Square> selectedSquare() const;

    const std::vector<chess::Square> &legalDestinations() const;

    std::vector<chess::Square> legalCaptures() const;

    bool awaitingPromotionChoice() const;

    void choosePromotion(chess::PieceType type);

private:
    chess::Board &board_;
    sf::RenderWindow &window_;
    sf::View &gameView_;

    std::optional<chess::Square> selected_;
    chess::Movelist candidateMoves_;
    bool fullscreen_ = false;

    std::vector<chess::Square> legalDestinations_;

    bool awaitingPromotion_ = false;
    chess::Square promotionFrom_;
    chess::Square promotionTo_;

    void handleMouseClick(sf::Vector2i pixelPos, float pixelScale);

    void handleKeyPress(sf::Keyboard::Key key);

    void attemptMove(chess::Square from, chess::Square to);

    void clearSelection();
};
