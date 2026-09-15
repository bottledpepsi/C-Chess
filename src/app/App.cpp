#include <SFML/Graphics.hpp>

#include <vector>
#include <utility>
#include <string>
#include <filesystem>
#include "../../include/app/AssetManager.hpp"
#include "../../include/app/Config.hpp"
#include "../../include/render/BoardConstants.hpp"
#include "../../include/render/BoardLayout.hpp"
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

// Resizes the view to always match the window 1:1 in pixels (no
// letterboxing), so the window can be stretched freely. Layout of the
// board and UI within that space is handled separately by BoardLayout.
void updateView(sf::RenderWindow &window, sf::View &view) {
    const sf::Vector2u windowSize = window.getSize();

    view.setSize({
        static_cast<float>(windowSize.x),
        static_cast<float>(windowSize.y)
    });
    view.setCenter({
        static_cast<float>(windowSize.x) / 2.f,
        static_cast<float>(windowSize.y) / 2.f
    });

    window.setView(view);
}

int main() {
    setWorkingDirectoryToExecutablePath();

    const Config config = Config::load();

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
            {static_cast<float>(config.windowWidth), static_cast<float>(config.windowHeight)}
        )
    );

    updateView(window, gameView);

    // Margins (in pixels) kept around the board within the space between
    // the two trays; the board itself scales to fill whatever is left.
    constexpr float BOARD_SIDE_MARGIN = 40.f;
    constexpr float BOARD_VERTICAL_MARGIN = 20.f;

    BoardLayout layout = BoardLayout::compute(
        window.getSize(),
        TrayRenderer::TRAY_HEIGHT,
        BOARD_SIDE_MARGIN,
        BOARD_VERTICAL_MARGIN
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
            if (event->is<sf::Event::Resized>()) {
                updateView(window, gameView);
            }

            inputHandler.handleEvent(*event, layout);

            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    const sf::Vector2f boardLocalPos =
                            window.mapPixelToCoords(mouseButtonPressed->position, gameView);
                    promotionRenderer.handleClick(boardLocalPos);
                }
            }
        }

        layout = BoardLayout::compute(
            window.getSize(),
            TrayRenderer::TRAY_HEIGHT,
            BOARD_SIDE_MARGIN,
            BOARD_VERTICAL_MARGIN
        );

        const float pixelScale = layout.squareSize / BoardConstants::REFERENCE_SQUARE_SIZE;
        boardRenderer.pixelScale = pixelScale;
        trayRenderer.pixelScale = pixelScale;

        window.clear();

        boardRenderer.drawBoard(
            window,
            layout,
            inputHandler.selectedSquare(),
            inputHandler.legalDestinations(),
            inputHandler.legalCaptures()
        );

        trayRenderer.drawTrays(window, layout);

        pieceRenderer.drawPieces(window, layout);

        promotionRenderer.drawPromotion(window, layout);

        gameOverRenderer.drawGameOver(window, layout);

        window.display();
    }

    return 0;
}
