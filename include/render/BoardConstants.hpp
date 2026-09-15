#pragma once

class BoardConstants {
public:
    static constexpr uint8_t BOARD_SIZE = 8;

    // Base/reference square size used only as the design-time scale for
    // things like default window size. Actual on-screen square size is
    // computed dynamically each frame by BoardLayout, based on the
    // current window size, to support free window stretching.
    static constexpr float REFERENCE_SQUARE_SIZE = 75.0f;

    static const bool FLIPPED = false;
};
