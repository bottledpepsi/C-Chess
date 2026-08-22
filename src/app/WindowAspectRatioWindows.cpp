#include "../../include/app/WindowAspectRatio.hpp"

#ifdef _WIN32

#include <windows.h>

namespace {
    WNDPROC originalWindowProc = nullptr;
    float aspectRatio = 1.f;

    LRESULT CALLBACK AspectRatioWindowProc(
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    ) {
        if (message == WM_SIZING) {
            auto *rect = reinterpret_cast<RECT *>(lParam);

            RECT windowRect{};
            RECT clientRect{};

            GetWindowRect(hwnd, &windowRect);
            GetClientRect(hwnd, &clientRect);

            const int frameWidth =
                    (windowRect.right - windowRect.left) -
                    (clientRect.right - clientRect.left);

            const int frameHeight =
                    (windowRect.bottom - windowRect.top) -
                    (clientRect.bottom - clientRect.top);

            int width =
                    rect->right - rect->left - frameWidth;

            int height =
                    rect->bottom - rect->top - frameHeight;

            if (width < 1)
                width = 1;

            if (height < 1)
                height = 1;

            switch (wParam) {
                case WMSZ_LEFT:
                case WMSZ_RIGHT: {
                    height =
                            static_cast<int>(
                                width / aspectRatio
                            );

                    if (wParam == WMSZ_LEFT) {
                        rect->top =
                                rect->bottom -
                                (height + frameHeight);
                    } else {
                        rect->bottom =
                                rect->top +
                                (height + frameHeight);
                    }

                    break;
                }

                case WMSZ_TOP:
                case WMSZ_BOTTOM: {
                    width =
                            static_cast<int>(
                                height * aspectRatio
                            );

                    if (wParam == WMSZ_TOP) {
                        rect->left =
                                rect->right -
                                (width + frameWidth);
                    } else {
                        rect->right =
                                rect->left +
                                (width + frameWidth);
                    }

                    break;
                }

                case WMSZ_TOPLEFT:
                case WMSZ_TOPRIGHT:
                case WMSZ_BOTTOMLEFT:
                case WMSZ_BOTTOMRIGHT: {
                    width =
                            static_cast<int>(
                                height * aspectRatio
                            );

                    const int totalWidth =
                            width + frameWidth;

                    const int totalHeight =
                            height + frameHeight;

                    switch (wParam) {
                        case WMSZ_TOPLEFT:
                            rect->left =
                                    rect->right - totalWidth;
                            rect->top =
                                    rect->bottom - totalHeight;
                            break;

                        case WMSZ_TOPRIGHT:
                            rect->right =
                                    rect->left + totalWidth;
                            rect->top =
                                    rect->bottom - totalHeight;
                            break;

                        case WMSZ_BOTTOMLEFT:
                            rect->left =
                                    rect->right - totalWidth;
                            rect->bottom =
                                    rect->top + totalHeight;
                            break;

                        case WMSZ_BOTTOMRIGHT:
                            rect->right =
                                    rect->left + totalWidth;
                            rect->bottom =
                                    rect->top + totalHeight;
                            break;

                        default:
                            break;
                    }

                    break;
                }

                default:
                    break;
            }

            return TRUE;
        }

        return CallWindowProc(
            originalWindowProc,
            hwnd,
            message,
            wParam,
            lParam
        );
    }
}

namespace WindowAspectRatio {
    void lock(
        sf::RenderWindow &window,
        unsigned int width,
        unsigned int height
    ) {
        aspectRatio =
                static_cast<float>(width) /
                static_cast<float>(height);

        HWND hwnd = window.getNativeHandle();

        originalWindowProc =
                reinterpret_cast<WNDPROC>(
                    SetWindowLongPtr(
                        hwnd,
                        GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>(
                            AspectRatioWindowProc
                        )
                    )
                );
    }
}

#endif
