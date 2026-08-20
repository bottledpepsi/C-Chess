#include <SFML/Graphics.hpp>
#include "include/render/BoardRenderer.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({640, 640}),
        "C-Chess"
    );

    BoardRenderer boardRenderer;

    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        boardRenderer.drawBoard(window);

        window.display();
    }
        return 0;
}