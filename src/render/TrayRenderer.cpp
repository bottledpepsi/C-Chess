#include "../../include/render/TrayRenderer.hpp"


TrayRenderer::TrayRenderer(
    AssetManager &assetManager
)
    : assets(assetManager),
      label(assets.getFont("arial")) {
    tray.setSize({TrayRenderer::TRAY_WIDTH, TrayRenderer::TRAY_HEIGHT});
    tray.setFillColor(TrayRenderer::TRAY_COLOUR);}

void TrayRenderer::drawTrays(
    sf::RenderWindow &window
) {
    const unsigned int scaledCharacterSize =
            AssetManager::sharpCharacterSize(16, pixelScale);

    label.setCharacterSize(scaledCharacterSize);

    if (pixelScale > 0.f) {
        const float textScale = 1.f / pixelScale;
        label.setScale({textScale, textScale});

        tray.setPosition({0.f, 0.f});
        window.draw(tray);

        tray.setPosition({0.f, 704.f});
        window.draw(tray);
    }
}
