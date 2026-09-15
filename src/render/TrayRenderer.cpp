#include "../../include/render/TrayRenderer.hpp"


TrayRenderer::TrayRenderer(
    AssetManager &assetManager
)
    : assets(assetManager),
      label(assets.getFont("arial")) {
    tray.setSize({0.f, TrayRenderer::TRAY_HEIGHT});
    tray.setFillColor(TrayRenderer::TRAY_COLOUR);
}

void TrayRenderer::drawTrays(
    sf::RenderWindow &window,
    const BoardLayout &layout
) {
    const unsigned int scaledCharacterSize =
            AssetManager::sharpCharacterSize(16, pixelScale);

    label.setCharacterSize(scaledCharacterSize);

    if (pixelScale > 0.f) {
        const float textScale = 1.f / pixelScale;
        label.setScale({textScale, textScale});

        // Trays stay fixed-height and pinned to the top/bottom edges,
        // stretching only horizontally to match the current window width.
        tray.setSize({layout.trayWidth, layout.trayHeight});

        tray.setPosition(layout.topTrayPosition);
        window.draw(tray);

        tray.setPosition(layout.bottomTrayPosition);
        window.draw(tray);
    }
}
