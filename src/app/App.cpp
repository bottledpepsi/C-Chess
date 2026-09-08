#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <string>
#include <filesystem>
#include "../../include/app/AssetManager.hpp"
#include "../../include/app/Config.hpp"
#include "../../include/app/WindowAspectRatio.hpp"
#include "../../include/render/BoardRenderer.hpp"
#include "../../include/render/PieceRenderer.hpp"
#include "../../include/input/InputHandler.hpp"
#include "../../include/render/PromotionRenderer.hpp"
#include "../../include/render/GameOverRenderer.hpp"
#include "../../include/render/TrayRenderer.hpp"


#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

void setWorkingDirectoryToExecutablePath() {
#if defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);

    std::string buffer(size, '\0');
    if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
        return;
    }

    std::filesystem::path exePath = std::filesystem::canonical(buffer);
    std::filesystem::path exeDir = exePath.parent_path();

    std::filesystem::current_path(exeDir);
#endif
}

float updateView(sf::RenderWindow &window, sf::View &view, float gameWidth, float gameHeight) {
    const float gameAspect = gameWidth / gameHeight;

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

    return viewportHeightPixels / gameHeight;
}

int main() {
    setWorkingDirectoryToExecutablePath();

    const Config config = Config::load();

    const auto windowWidth = static_cast<float>(config.windowWidth);
    const auto windowHeight = static_cast<float>(config.windowHeight);

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

    const std::vector<std::pair<std::string, std::string> > soundFiles = {
        {"move", "assets/audio/move.ogg"},
        {"capture", "assets/audio/capture.ogg"}
    };

    for (const auto &[name, path]: soundFiles) {
        if (!assets.loadSound(name, path)) {
            return 1;
        }
    }

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(
        sf::VideoMode({config.windowWidth, config.windowHeight}),
        "C-Chess",
        sf::Style::Default,
        sf::State::Windowed,
        settings
    );

    sf::View gameView(
        sf::FloatRect(
            {0.f, 0.f},
            {windowWidth, windowHeight}
        )
    );

    float pixelScale = updateView(window, gameView, windowWidth, windowHeight);

    WindowAspectRatio::lock(
        window,
        config.windowWidth,
        config.windowHeight
    );

    if (config.startFullscreen) {
        window.create(
            sf::VideoMode::getDesktopMode(),
            "C-Chess",
            sf::Style::Default,
            sf::State::Fullscreen,
            settings
        );
    }

    chess::Board board;
    BoardRenderer boardRenderer(board, assets);
    TrayRenderer trayRenderer(assets);
    PieceRenderer pieceRenderer(board, assets);
    InputHandler inputHandler(
        board,
        window,
        gameView,
        assets.getSound("move"),
        assets.getSound("capture"),
        config
    );
    PromotionRenderer promotionRenderer(assets, inputHandler);
    GameOverRenderer gameOverRenderer(assets, inputHandler);

    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            inputHandler.handleEvent(*event, pixelScale);

            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    const sf::Vector2f boardLocalPos =
                            window.mapPixelToCoords(mouseButtonPressed->position, gameView);
                    promotionRenderer.handleClick(boardLocalPos);
                }
            }
        }

        pixelScale = updateView(window, gameView, windowWidth, windowHeight);

        window.clear();

        boardRenderer.drawBoard(
            window,
            inputHandler.selectedSquare(),
            inputHandler.legalDestinations(),
            inputHandler.legalCaptures()
        );

        trayRenderer.drawTrays(window);

        pieceRenderer.drawPieces(window);

        promotionRenderer.drawPromotion(window);

        gameOverRenderer.drawGameOver(window);

        window.display();
    }

    return 0;
}
