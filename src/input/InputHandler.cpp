#include "../../include/input/InputHandler.hpp"
#include "../../include/input/BoardCoords.hpp"
#include "../../include/app/WindowAspectRatio.hpp"

InputHandler::InputHandler(chess::Board &board, sf::RenderWindow &window, sf::View &gameView)
    : board_(board),
      window_(window),
      gameView_(gameView) {
}

void InputHandler::handleEvent(const sf::Event &event, float pixelScale) {
    if (event.is<sf::Event::Closed>()) {
        window_.close();
        return;
    }

    if (const auto *mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            handleMouseClick(mouseButtonPressed->position, pixelScale);
        }
        return;
    }

    if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        handleKeyPress(keyPressed->code);
        return;
    }
}

std::optional<chess::Square> InputHandler::selectedSquare() const {
    return selected_;
}

const std::vector<chess::Square> &InputHandler::legalDestinations() const {
    return legalDestinations_;
}

std::vector<chess::Square> InputHandler::legalCaptures() const {
    std::vector<chess::Square> captures;

    for (const chess::Square &sq: legalDestinations_) {
        if (board_.at(sq) != chess::Piece::NONE) {
            captures.push_back(sq);
        }
    }

    return captures;
}

bool InputHandler::awaitingPromotionChoice() const {
    return awaitingPromotion_;
}

void InputHandler::choosePromotion(chess::PieceType type) {
    if (!awaitingPromotion_) {
        return;
    }

    for (const chess::Move &move: candidateMoves_) {
        if (move.from() == promotionFrom_ &&
            move.to() == promotionTo_ &&
            move.typeOf() == chess::Move::PROMOTION &&
            move.promotionType() == type) {
            board_.makeMove(move);
            clearSelection();
            return;
        }
    }
}

void InputHandler::handleMouseClick(sf::Vector2i pixelPos, float pixelScale) {
    if (awaitingPromotion_) {
        return;
    }

    const sf::Vector2f boardLocalPos = window_.mapPixelToCoords(pixelPos, gameView_);
    const chess::Square clicked = BoardCoords::screenToSquare(boardLocalPos, pixelScale);

    if (!clicked.is_valid()) {
        clearSelection();
        return;
    }

    if (!selected_.has_value()) {
        const chess::Piece piece = board_.at(clicked);

        if (piece == chess::Piece::NONE || piece.color() != board_.sideToMove()) {
            return;
        }

        chess::Movelist allLegalMoves;
        chess::movegen::legalmoves(allLegalMoves, board_);

        candidateMoves_.clear();
        legalDestinations_.clear();

        for (const chess::Move &move: allLegalMoves) {
            if (move.from() == clicked) {
                candidateMoves_.add(move);
                legalDestinations_.push_back(move.to());
            }
        }

        selected_ = clicked;
        return;
    }

    if (clicked == selected_.value()) {
        clearSelection();
        return;
    }

    const chess::Piece clickedPiece = board_.at(clicked);
    if (clickedPiece != chess::Piece::NONE && clickedPiece.color() == board_.sideToMove()) {
        chess::Movelist allLegalMoves;
        chess::movegen::legalmoves(allLegalMoves, board_);

        candidateMoves_.clear();
        legalDestinations_.clear();

        for (const chess::Move &move: allLegalMoves) {
            if (move.from() == clicked) {
                candidateMoves_.add(move);
                legalDestinations_.push_back(move.to());
            }
        }

        selected_ = clicked;
        return;
    }

    attemptMove(selected_.value(), clicked);
}

void InputHandler::attemptMove(chess::Square from, chess::Square to) {
    std::vector<const chess::Move *> matches;

    for (const chess::Move &move: candidateMoves_) {
        if (move.from() == from && move.to() == to) {
            matches.push_back(&move);
        }
    }

    if (matches.empty()) {
        clearSelection();
        return;
    }

    if (matches.size() == 1) {
        board_.makeMove(*matches.front());
        clearSelection();
        return;
    }

    awaitingPromotion_ = true;
    promotionFrom_ = from;
    promotionTo_ = to;
}

void InputHandler::handleKeyPress(sf::Keyboard::Key key) {
    if (key != sf::Keyboard::Key::F11) {
        return;
    }

    fullscreen_ = !fullscreen_;

    if (fullscreen_) {
        window_.create(
            sf::VideoMode::getDesktopMode(),
            "C-Chess",
            sf::Style::Default,
            sf::State::Fullscreen
        );
    } else {
        window_.create(
            sf::VideoMode({852, 766}),
            "C-Chess",
            sf::Style::Default,
            sf::State::Windowed
        );

        WindowAspectRatio::lock(
            window_,
            852,
            766
        );
    }

    window_.setVerticalSyncEnabled(true);
}

void InputHandler::clearSelection() {
    selected_.reset();
    candidateMoves_.clear();
    legalDestinations_.clear();
    awaitingPromotion_ = false;
    promotionFrom_ = chess::Square::NO_SQ;
    promotionTo_ = chess::Square::NO_SQ;
}
