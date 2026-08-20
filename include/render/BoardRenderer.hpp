#include <SFML/Graphics.hpp>

class BoardRenderer {
    public:
        void drawBoard(sf::RenderWindow& window);

    private:
        static constexpr int BOARD_SIZE = 8;
        static constexpr float SQUARE_SIZE = 80.0f;
        static constexpr bool FLIPPED = false;

};

