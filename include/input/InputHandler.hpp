#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <optional>
#include <vector>

#include "../chess/chess.hpp"

class InputHandler {
public:
    InputHandler(chess::Board &board,
        sf::RenderWindow &window,
        sf::View &gameView,
        const sf::SoundBuffer &moveSoundBuffer,
        const sf::SoundBuffer &captureSoundBuffer
        );

    void handleEvent(const sf::Event &event, float pixelScale);

    std::optional<chess::Square> selectedSquare() const;

    const std::vector<chess::Square> &legalDestinations() const;

    std::vector<chess::Square> legalCaptures() const;

    bool awaitingPromotionChoice() const;

    chess::Square promotionSquare() const;

    chess::Color promotionColor() const;

    void choosePromotion(chess::PieceType type);

    const chess::Board &board() const;

    bool isGameOver() const;

    chess::GameResult gameResult() const;

    chess::GameResultReason gameResultReason() const;

private:
    chess::Board &board_;
    sf::RenderWindow &window_;
    sf::View &gameView_;

    sf::Sound moveSound_;
    sf::Sound captureSound_;

    std::optional<chess::Square> selected_;
    chess::Movelist candidateMoves_;
    bool fullscreen_ = false;

    std::vector<chess::Square> legalDestinations_;

    bool awaitingPromotion_ = false;
    chess::Square promotionFrom_;
    chess::Square promotionTo_;

    std::pair<chess::GameResultReason, chess::GameResult> gameOverState_ =
            {chess::GameResultReason::NONE, chess::GameResult::NONE};

    void refreshGameOverState();

    void handleMouseClick(sf::Vector2i pixelPos, float pixelScale);

    void handleKeyPress(sf::Keyboard::Key key);

    void attemptMove(chess::Square from, chess::Square to);

    void clearSelection();

    void playMoveSound(const chess::Move &move);
};
