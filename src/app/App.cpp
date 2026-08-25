#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <string>
#include "../../include/app/AssetManager.hpp"
#include "../../include/app/WindowAspectRatio.hpp"
#include "../../include/render/BoardRenderer.hpp"
#include "../../include/render/PieceRenderer.hpp"
#include "../../include/input/InputHandler.hpp"

constexpr float WIN_W = 852.f;
constexpr float WIN_H = 766.f;

float updateView(sf::RenderWindow &window, sf::View &view) {
    const float gameAspect = WIN_W / WIN_H;

    const sf::Vector2u windowSize = window.getSize();

    const float windowAspect =
            static_cast<float>(windowSize.x) /
            static_cast<float>(windowSize.y);

    float viewportHeightFraction = 1.f;

    if (windowAspect > gameAspect) {
        const float viewportWidth =
                gameAspect / windowAspect;

        view.setViewport(sf::FloatRect(
            {(1.f - viewportWidth) / 2.f, 0.f},
            {viewportWidth, 1.f}
        ));
    } else {
        const float viewportHeight =
                windowAspect / gameAspect;

        viewportHeightFraction = viewportHeight;

        view.setViewport(sf::FloatRect(
            {0.f, (1.f - viewportHeight) / 2.f},
            {1.f, viewportHeight}
        ));
    }

    window.setView(view);
    const float viewportHeightPixels =
            viewportHeightFraction * static_cast<float>(windowSize.y);

    return viewportHeightPixels / WIN_H;
}

int main() {
    AssetManager assets;

    if (!assets.loadFont(
        "arial",
        "assets/fonts/arial.ttf"
    )) {
        return 1;
    }

    const std::vector<std::pair<std::string, std::string> > pieceTextures = {
        {"b_bishop", "assets/images/b_bishop.png"},
        {"b_king", "assets/images/b_king.png"},
        {"b_knight", "assets/images/b_knight.png"},
        {"b_pawn", "assets/images/b_pawn.png"},
        {"b_queen", "assets/images/b_queen.png"},
        {"b_rook", "assets/images/b_rook.png"},
        {"w_bishop", "assets/images/w_bishop.png"},
        {"w_king", "assets/images/w_king.png"},
        {"w_knight", "assets/images/w_knight.png"},
        {"w_pawn", "assets/images/w_pawn.png"},
        {"w_queen", "assets/images/w_queen.png"},
        {"w_rook", "assets/images/w_rook.png"},
    };

    for (const auto &[name, path]: pieceTextures) {
        if (!assets.loadTexture(name, path)) {
            return 1;
        }
    }

    sf::RenderWindow window(
        sf::VideoMode({852, 766}),
        "C-Chess",
        sf::Style::Default,
        sf::State::Windowed
    );

    sf::View gameView(
        sf::FloatRect(
            {0.f, 0.f},
            {WIN_W, WIN_H}
        )
    );

    float pixelScale = updateView(window, gameView);

    WindowAspectRatio::lock(
        window,
        852,
        766
    );

    chess::Board board;
    BoardRenderer boardRenderer(board, assets);
    PieceRenderer pieceRenderer(board, assets);
    InputHandler inputHandler(board, window, gameView);

    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            inputHandler.handleEvent(*event, pixelScale);
        }

        pixelScale = updateView(window, gameView);

        window.clear();

        boardRenderer.drawBoard(
            window,
            pixelScale,
            inputHandler.selectedSquare(),
            inputHandler.legalDestinations()
        );
        pieceRenderer.drawPieces(window);

        window.display();
    }

    return 0;
}
