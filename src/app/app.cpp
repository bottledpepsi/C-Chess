#include <SFML/Graphics.hpp>

#include "../../include/app/AssetManager.hpp"
#include "../../include/render/BoardRenderer.hpp"

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
        "C-Chess"
    );

    chess::Board board;
    BoardRenderer boardRenderer(board, assets);

    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        boardRenderer.drawBoard(window);

        window.display();
    }
    return 0;
}
