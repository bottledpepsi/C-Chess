#include "../../include/render/BoardLayout.hpp"
#include "../../include/render/BoardConstants.hpp"

#include <algorithm>

BoardLayout BoardLayout::compute(
    const sf::Vector2u windowSize,
    const float trayHeight,
    const float sideMargin,
    const float verticalMargin,
    const float panelWidth
) {
    BoardLayout layout;

    const auto windowWidth = static_cast<float>(windowSize.x);
    const auto windowHeightF = static_cast<float>(windowSize.y);

    layout.trayHeight = trayHeight;

    const float boardAreaWidth = std::max(windowWidth - panelWidth, 1.f);
    layout.trayWidth = boardAreaWidth;

    layout.topTrayPosition = {0.f, 0.f};
    layout.bottomTrayPosition = {0.f, std::max(windowHeightF - trayHeight, trayHeight)};

    // Space left for the board between the two trays.
    const float availableWidth =
            std::max(boardAreaWidth - 2.f * sideMargin, 1.f);

    const float availableHeight =
            std::max(
                windowHeightF - 2.f * trayHeight - 2.f * verticalMargin,
                1.f
            );

    const float boardSize = std::max(std::min(availableWidth, availableHeight), 1.f);

    layout.boardSize = boardSize;
    layout.squareSize = boardSize / static_cast<float>(BoardConstants::BOARD_SIZE);

    layout.boardOrigin = {
        (boardAreaWidth - boardSize) / 2.f,
        trayHeight + (windowHeightF - 2.f * trayHeight - boardSize) / 2.f
    };

    constexpr float PANEL_GAP = 20.f;
    const float pixelScale = layout.squareSize / BoardConstants::REFERENCE_SQUARE_SIZE;
    const float boardRightEdge = layout.boardOrigin.x + boardSize;
    const float panelX = boardRightEdge + PANEL_GAP * pixelScale;

    layout.panelWidth = std::max(windowWidth - panelX, 0.f);
    layout.panelHeight = windowHeightF;
    layout.panelPosition = {panelX, 0.f};

    return layout;
}
