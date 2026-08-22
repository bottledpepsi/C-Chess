#pragma once
#include "SFML/Window/Keyboard.hpp"

class InputHandler {
public:
    sf::Keyboard::Key getKey(sf::Keyboard::Key key);
};
