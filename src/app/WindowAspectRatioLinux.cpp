#include "../../include/app/WindowAspectRatio.hpp"

#ifdef __linux__

#include <SFML/Window/WindowHandle.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace WindowAspectRatio {
    void lock(
        sf::RenderWindow &window,
        unsigned int width,
        unsigned int height
    ) {
        Display *display =
                XOpenDisplay(nullptr);

        if (!display)
            return;

        Window xWindow =
                reinterpret_cast<Window>(
                    window.getNativeHandle()
                );

        XSizeHints hints{};

        hints.flags = PAspect;

        hints.min_aspect.x = width;
        hints.min_aspect.y = height;

        hints.max_aspect.x = width;
        hints.max_aspect.y = height;

        XSetWMNormalHints(
            display,
            xWindow,
            &hints
        );

        XFlush(display);
        XCloseDisplay(display);
    }
}

#endif
