#pragma once
#include "SFML/Graphics.hpp"
#include "../app/AssetManager.hpp"
#include "../input/InputHandler.hpp"
#include "BoardLayout.hpp"


class GameOverRenderer {
public:
    GameOverRenderer(
        AssetManager &assetManager,
        const InputHandler &inputHandler
    );

    void drawGameOver(sf::RenderWindow &window, const BoardLayout &layout);

private:
    AssetManager &assets;
    const InputHandler &input;

    sf::RectangleShape overlay;
    sf::Text title;
    sf::Text subtitle;

    std::string buildTitle() const;

    std::string buildSubtitle() const;
};
