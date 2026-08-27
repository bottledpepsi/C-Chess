#include "../../include/render/PromotionRenderer.hpp"
#include "../../include/render/BoardConstants.hpp"
#include "../../include/input/BoardCoords.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <string>

namespace {
    std::string textureKeyFor(chess::PieceType type, chess::Color color) {
        const std::string colorPrefix = color == chess::Color::WHITE ? "w_" : "b_";

        switch (type.internal()) {
            case chess::PieceType::underlying::KNIGHT:
                return colorPrefix + "knight";
            case chess::PieceType::underlying::BISHOP:
                return colorPrefix + "bishop";
            case chess::PieceType::underlying::ROOK:
                return colorPrefix + "rook";
            case chess::PieceType::underlying::QUEEN:
                return colorPrefix + "queen";
            default:
                return "";
        }
    }
}

PromotionRenderer::PromotionRenderer(AssetManager &assetManager, InputHandler &inputHandler)
    : assets(assetManager),
      input(inputHandler),
      title(assets.getFont("arial"), "Promotion") {
    title.setCharacterSize(24);
    title.setFillColor(sf::Color(230, 230, 230));
}

void PromotionRenderer::drawPromotion(sf::RenderWindow &window) {
    if (!input.awaitingPromotionChoice()) {
        return;
    }

    constexpr float PANEL_WIDTH = 340.f;
    constexpr float PANEL_HEIGHT = 170.f;
    constexpr float PANEL_PADDING = 24.f;
    constexpr float TITLE_HEIGHT = 36.f;

    constexpr float OPTION_COUNT = static_cast<float>(CHOICES.size());
    constexpr float OPTION_SPACING = 12.f;
    constexpr float OPTION_SIZE =
            (PANEL_WIDTH - 2.f * PANEL_PADDING - (OPTION_COUNT - 1.f) * OPTION_SPACING) / OPTION_COUNT;

    const float boardCenterX =
            BoardCoords::BOARD_ORIGIN.x + BoardConstants::SQUARE_SIZE * BoardConstants::BOARD_SIZE / 2.f;
    const float boardCenterY =
            BoardCoords::BOARD_ORIGIN.y + BoardConstants::SQUARE_SIZE * BoardConstants::BOARD_SIZE / 2.f;

    const sf::Vector2f panelPosition(
        boardCenterX - PANEL_WIDTH / 2.f,
        boardCenterY - PANEL_HEIGHT / 2.f
    );

    // Dim the board behind the panel so the picker reads as a modal choice.
    sf::RectangleShape overlay({
        BoardConstants::SQUARE_SIZE * BoardConstants::BOARD_SIZE,
        BoardConstants::SQUARE_SIZE * BoardConstants::BOARD_SIZE
    });
    overlay.setPosition(BoardCoords::BOARD_ORIGIN);
    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    window.draw(overlay);

    sf::RectangleShape panel;
    panel.setSize({PANEL_WIDTH, PANEL_HEIGHT});
    panel.setPosition(panelPosition);
    panel.setFillColor(sf::Color(40, 40, 40));
    panel.setOutlineColor(sf::Color(180, 180, 180));
    panel.setOutlineThickness(2.f);

    window.draw(panel);

    const sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({
        panelPosition.x + (PANEL_WIDTH - titleBounds.size.x) / 2.f - titleBounds.position.x,
        panelPosition.y + 14.f
    });

    window.draw(title);

    const chess::Color promotingColor = input.promotionColor();

    const float optionsTop = panelPosition.y + TITLE_HEIGHT + (PANEL_HEIGHT - TITLE_HEIGHT - OPTION_SIZE) / 2.f;

    sf::RectangleShape optionBox({OPTION_SIZE, OPTION_SIZE});
    optionBox.setOutlineThickness(2.f);

    for (std::size_t i = 0; i < CHOICES.size(); ++i) {
        const float x = panelPosition.x + PANEL_PADDING + static_cast<float>(i) * (OPTION_SIZE + OPTION_SPACING);

        optionBounds[i] = sf::FloatRect({x, optionsTop}, {OPTION_SIZE, OPTION_SIZE});

        optionBox.setPosition({x, optionsTop});
        optionBox.setFillColor(sf::Color(60, 60, 60));
        optionBox.setOutlineColor(sf::Color(120, 120, 120));
        window.draw(optionBox);

        const std::string textureKey = textureKeyFor(chess::PieceType(CHOICES[i]), promotingColor);
        if (textureKey.empty()) {
            continue;
        }

        sf::Texture &texture = assets.getTexture(textureKey);
        sf::Sprite sprite(texture);

        const sf::Vector2u textureSize = texture.getSize();

        if (textureSize.x > 0 && textureSize.y > 0) {
            constexpr float SIZE_MODIFIER = 0.8f;

            sprite.setScale({
                (OPTION_SIZE * SIZE_MODIFIER) / static_cast<float>(textureSize.x),
                (OPTION_SIZE * SIZE_MODIFIER) / static_cast<float>(textureSize.y)
            });

            sprite.setOrigin({
                static_cast<float>(textureSize.x) / 2.0f,
                static_cast<float>(textureSize.y) / 2.0f
            });
        }

        sprite.setPosition({x + OPTION_SIZE / 2.f, optionsTop + OPTION_SIZE / 2.f});

        window.draw(sprite);
    }
}

void PromotionRenderer::handleClick(sf::Vector2f boardLocalPos) {
    if (!input.awaitingPromotionChoice()) {
        return;
    }

    for (std::size_t i = 0; i < CHOICES.size(); ++i) {
        if (optionBounds[i].contains(boardLocalPos)) {
            input.choosePromotion(chess::PieceType(CHOICES[i]));
            return;
        }
    }
}
