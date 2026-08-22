#include <SFML/Graphics.hpp>

#include "../../include/app/AssetManager.hpp"
#include "../../include/render/BoardRenderer.hpp"

void updateView(sf::RenderWindow &window, sf::View &view) {
    const float gameAspect = 852.f / 766.f;

    const sf::Vector2u windowSize = window.getSize();

    const float windowAspect =
            static_cast<float>(windowSize.x) /
            static_cast<float>(windowSize.y);

    if (windowAspect > gameAspect) {
        const float viewportWidth = gameAspect / windowAspect;

        view.setViewport(sf::FloatRect(
            {(1.f - viewportWidth) / 2.f, 0.f},
            {viewportWidth, 1.f}
        ));
    } else {
        const float viewportHeight = windowAspect / gameAspect;

        view.setViewport(sf::FloatRect(
            {0.f, (1.f - viewportHeight) / 2.f},
            {1.f, viewportHeight}
        ));
    }

    window.setView(view);
}

int main() {
    AssetManager assets;

    if (!assets.loadFont(
        "arial",
        "assets/fonts/arial.ttf"
    )) {
        return 1;
    }

    sf::RenderWindow window(
        sf::VideoMode({852, 766}),
        "C-Chess",
        sf::Style::Default,
        sf::State::Windowed
    );

    bool fullscreen = false;

    sf::View gameView(
        sf::FloatRect(
            {0.f, 0.f},
            {852.f, 766.f}
        )
    );

    updateView(window, gameView);

    chess::Board board;
    BoardRenderer boardRenderer(board, assets);

    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto *keyPressed =
                    event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::F11) {
                    fullscreen = !fullscreen;

                    if (fullscreen) {
                        window.create(
                            sf::VideoMode::getDesktopMode(),
                            "C-Chess",
                            sf::Style::Default,
                            sf::State::Fullscreen
                        );
                    } else {
                        window.create(
                            sf::VideoMode({852, 766}),
                            "C-Chess",
                            sf::Style::Default,
                            sf::State::Windowed
                        );
                    }

                    updateView(window, gameView);

                    window.setVerticalSyncEnabled(true);
                }
            }
        }

        window.clear();

        boardRenderer.drawBoard(window);

        window.display();
    }

    return 0;
}
