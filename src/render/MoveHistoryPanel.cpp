#include "../../include/render/MoveHistoryPanel.hpp"
#include "../../include/app/AssetManager.hpp"

#include <string>

MoveHistoryPanel::MoveHistoryPanel(
    AssetManager &assetManager
)
    : assets(assetManager),
      label(assets.getFont("arial")) {
    background.setFillColor(MoveHistoryPanel::PANEL_COLOUR);
    rowHighlight.setFillColor(MoveHistoryPanel::ROW_ALT_COLOUR);
}

void MoveHistoryPanel::addMove(const std::string &moveText) {
    moves.push_back(moveText);
}

void MoveHistoryPanel::clear() {
    moves.clear();
}

void MoveHistoryPanel::drawPanel(
    sf::RenderWindow &window,
    const BoardLayout &layout
) {
    background.setSize({layout.panelWidth, layout.panelHeight});
    background.setPosition(layout.panelPosition);
    window.draw(background);

    const unsigned int scaledCharacterSize =
            AssetManager::sharpCharacterSize(16, pixelScale);

    label.setCharacterSize(scaledCharacterSize);
    label.setScale({1.f, 1.f});
    label.setFillColor(MoveHistoryPanel::TEXT_COLOUR);

    const float padding = MoveHistoryPanel::PADDING * pixelScale;
    const float rowHeight = MoveHistoryPanel::ROW_HEIGHT * pixelScale;
    const float numberColumnWidth = 40.f * pixelScale;
    const float columnWidth = (layout.panelWidth - numberColumnWidth - 2.f * padding) / 2.f;

    const std::size_t rowCount = (moves.size() + 1) / 2;

    for (std::size_t row = 0; row < rowCount; ++row) {
        const float rowY = layout.panelPosition.y + padding + static_cast<float>(row) * rowHeight;

        if (row % 2 == 1) {
            rowHighlight.setSize({layout.panelWidth, rowHeight});
            rowHighlight.setPosition({layout.panelPosition.x, rowY});
            window.draw(rowHighlight);
        }

        label.setString(std::to_string(row + 1) + ".");
        label.setPosition({layout.panelPosition.x + padding, rowY});
        window.draw(label);

        const std::size_t whiteIndex = row * 2;
        label.setString(moves[whiteIndex]);
        label.setPosition({layout.panelPosition.x + padding + numberColumnWidth, rowY});
        window.draw(label);

        const std::size_t blackIndex = whiteIndex + 1;
        if (blackIndex < moves.size()) {
            label.setString(moves[blackIndex]);
            label.setPosition({
                layout.panelPosition.x + padding + numberColumnWidth + columnWidth,
                rowY
            });
            window.draw(label);
        }
    }
}
