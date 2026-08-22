#include "../../include/app/WindowAspectRatio.hpp"

#ifdef __APPLE__

#import <Cocoa/Cocoa.h>

namespace WindowAspectRatio
{
    void lock(
        sf::RenderWindow& window,
        unsigned int width,
        unsigned int height
    )
    {
        NSWindow* nsWindow =
            (__bridge NSWindow*)window.getNativeHandle();

        if (!nsWindow)
            return;

        [nsWindow setAspectRatio:
            NSMakeSize(
                static_cast<CGFloat>(width),
                static_cast<CGFloat>(height)
            )
        ];
    }
}

#endif