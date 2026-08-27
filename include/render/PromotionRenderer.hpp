#pragma once
#include <array>
#include "SFML/Graphics.hpp"
#include "../app/AssetManager.hpp"
#include "../chess/chess.hpp"
#include "../input/InputHandler.hpp"


class PromotionRenderer {
public:
    PromotionRenderer(
        AssetManager &assetManager,
        InputHandler &inputHandler
    );

    void drawPromotion(sf::RenderWindow &window);

    void handleClick(sf::Vector2f boardLocalPos);

private:
    AssetManager &assets;
    InputHandler &input;

    sf::Text title;

    // The four promotion choices offered to the player
    static constexpr std::array<chess::PieceType::underlying, 4> CHOICES = {
        chess::PieceType::QUEEN,
        chess::PieceType::ROOK,
        chess::PieceType::BISHOP,
        chess::PieceType::KNIGHT
    };

    std::array<sf::FloatRect, CHOICES.size()> optionBounds{};
};
