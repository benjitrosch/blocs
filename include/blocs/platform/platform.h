#pragma once

#include <functional>

#include <SDL.h>

#include <blocs/common.h>
#include <blocs/math/vec.h>
#include <blocs/math/shapes.h>

#include <blocs/platform/input.h>

namespace blocs
{
    using Window   = SDL_Window*;
    using Renderer = SDL_GLContext;

    using Event = SDL_Event;

    namespace platform
    {
        /**
         * @brief Creates a new window and configures renderer backend settings.
         *
         * @param title      Title of window (appears in titlebar).
         * @param w          Width of window.
         * @param h          Height of window.
         * @param fullscreen Whether to start in fullscreen.
         * @param vsync      Whether vsync enabled or not.
         * @param flags      Additional window flags to change default state.
         *
         * @return Window object.
         */
        inline Window init(
            cstr title, i32 w, i32 h, bool fullscreen, bool vsync, u32 flags
        )
        {
#ifndef __EMSCRIPTEN__
            flags |= SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
            if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

            if (SDL_Init(
                    SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER |
                    SDL_INIT_GAMECONTROLLER
                ) != 0)
#else
            if (SDL_Init(SDL_INIT_VIDEO) != 0)
#endif
            {
                str error_message = "SDL_init HAS FAILED. REASON: ";
                str error_reason  = SDL_GetError();
                LOG_ERR((error_message + error_reason).c_str());
                return nullptr;
            }

            flags |= SDL_WINDOW_OPENGL;

#ifdef __EMSCRIPTEN__
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(
                SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE
            );
            SDL_GL_SetAttribute(
                SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
            );
#endif
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

            Window window = SDL_CreateWindow(
                title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                flags
            );

            if (window == nullptr)
            {
                LOG_ERR(
                    "SDL_CreateWindow HAS FAILED. REASON: " << SDL_GetError()
                );
                return nullptr;
            }

            return window;
        }

        /**
         * @brief Creates a new render context to control graphics state.
         *
         * @param window Window to render onto.
         * @param vsync  Whether vsync enabled or not.
         *
         * @return Renderer object.
         */
        inline Renderer create_context(Window window, bool vsync = true)
        {
            Renderer context = SDL_GL_CreateContext(window);

            if (context == nullptr)
            {
                LOG_ERR(
                    "SDL_GL_CreateContext HAS FAILED. REASON: "
                    << SDL_GetError()
                );
                return nullptr;
            }

            SDL_GL_MakeCurrent(window, context);

#ifndef __EMSCRIPTEN__
            SDL_GL_SetSwapInterval(vsync ? 1 : 0);
#endif

            return context;
        }

        /**
         * @param window Window.
         * @param x      Output x position.
         * @param y      Output y position.
         */
        inline void get_pos(Window window, i32* x, i32* y)
        {
            SDL_GetWindowPosition(window, x, y);
        }

        /**
         * @param window Window.
         *
         * @return Vector2 of Window's position.
         */
        inline vec2i get_pos(Window window)
        {
            i32 x;
            i32 y;
            SDL_GetWindowPosition(window, &x, &y);
            return {x, y};
        }

        /**
         * @brief Moves window to position.
         *
         * @param window Window.
         * @param x      New x position.
         * @param y      New y position.
         */
        inline void set_pos(Window window, i32 x, i32 y)
        {
#ifndef __EMSCRIPTEN__
            SDL_SetWindowPosition(window, x, y);
#endif
        }

        /**
         * @param window Window.
         * @param w      Drawable width.
         * @param h      Drawable height.
         */
        inline void get_draw_size(Window window, i32* w, i32* h)
        {
#ifndef __EMSCRIPTEN__
            SDL_GL_GetDrawableSize(window, w, h);
#endif
        }

        /**
         * @param window Window.
         *
         * @return Vector2 of drawable size.
         */
        inline vec2i get_draw_size(Window window)
        {
            i32 w;
            i32 h;
#ifndef __EMSCRIPTEN__
            get_draw_size(window, &w, &h);
#endif
            return {w, h};
        }

        /**
         * @param window Window.
         * @param w      Window width.
         * @param h      Window height.
         */
        inline void get_size(Window window, i32* w, i32* h)
        {
            SDL_GetWindowSize(window, w, h);
        }

        /**
         * @param window Window.
         *
         * @return Vector2 of window size.
         */
        inline vec2i get_size(Window window)
        {
            i32 w;
            i32 h;
            get_size(window, &w, &h);
            return {w, h};
        }

        /**
         * @param window Window.
         * @param w      New width of window.
         * @param h      New height of window.
         */
        inline void set_size(Window window, i32 w, i32 h)
        {
            SDL_SetWindowSize(window, w, h);
        }

        inline i32 get_current_display(Window window)
        {
#ifndef __EMSCRIPTEN__
            return SDL_GetWindowDisplayIndex(window);
#endif
            return -1;
        }

        inline vec2i get_display_size(i32 display_id = 0)
        {
            SDL_DisplayMode mode;

            if (display_id < SDL_GetNumVideoDisplays())
            {
                if (SDL_GetDesktopDisplayMode(display_id, &mode) == 0)
                {
                    return {mode.w, mode.h};
                }
            }

            return {0, 0};
        }

        inline recti get_display_rect(i32 display_id = 0)
        {
            SDL_Rect r;

            if (display_id < SDL_GetNumVideoDisplays())
            {
                if (SDL_GetDisplayBounds(display_id, &r) == 0)
                {
                    return {r.x, r.y, r.x + r.w, r.y + r.h};
                }
            }

            return {0, 0, 0, 0};
        }

        inline void set_fullscreen(
            Window window, bool enabled, bool hardware = false,
            recti bounds = {0, 0, 0, 0}
        )
        {
#ifdef __EMSCRIPTEN__
            SDL_SetWindowFullscreen(window, enabled ? SDL_TRUE : SDL_FALSE);
#else

#ifdef __APPLE__
            hardware = true;
#endif

            if (hardware)
            {
                SDL_SetWindowFullscreen(
                    window, enabled ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
                );
            }
            // Non-hardware fullscreen does NOT work with toggle,
            // fullscreen state must be kept manually
            else
            {
                SDL_SetWindowBordered(window, enabled ? SDL_FALSE : SDL_TRUE);
                SDL_SetWindowResizable(window, enabled ? SDL_FALSE : SDL_TRUE);

                if (enabled)
                {
                    bounds = get_display_rect(get_current_display(window));
                }

                auto [x, y, w, h] = bounds;
                set_pos(window, x, y);
                set_size(window, w, h);
            }
#endif
        }

        inline void toggle_fullscreen(Window window)
        {
            set_fullscreen(
                window, !(SDL_GetWindowFlags(window) &
#ifndef __EMSCRIPTEN__
                          SDL_WINDOW_FULLSCREEN_DESKTOP
#else
                          SDL_WINDOW_FULLSCREEN
#endif
                        )
            );
        }

        /**
         * @param window Window.
         *
         * @return Whether the window is active (focused, not minimized) or not.
         */
        inline bool is_window_active(Window window)
        {
            u32 flags = SDL_GetWindowFlags(window);
            return (flags & SDL_WINDOW_INPUT_FOCUS) != 0 &&
                   (flags & SDL_WINDOW_MINIMIZED) == 0;
        }

        /**
         * @brief Sets a new title for the window.
         *
         * @param window Window.
         * @param title  New title.
         */
        inline void set_title(Window window, cstr title)
        {
            SDL_SetWindowTitle(window, title);
        }

        inline void hide_cursor() { SDL_ShowCursor(SDL_DISABLE); }

        inline void show_cursor() { SDL_ShowCursor(SDL_ENABLE); }

        inline void cursor(bool show)
        {
            if (show)
                show_cursor();
            else
                hide_cursor();
        }

        /**
         * @brief Process platform events (input, window events, etc.).
         *
         * @param window   Window object.
         * @param input    Input state handler.
         * @param on_event Callback to inject external logic using Event object.
         * @param on_quit  Callback to quit the application.
         */
        inline void events(
            Window window, std::function<void(Event& event)> on_event,
            std::function<void()> on_quit
        )
        {
            input::events();

            Event event;
            while (SDL_PollEvent(&event))
            {
                on_event(event);

                switch (event.type)
                {
                    case SDL_QUIT:
                        on_quit();
                        break;

                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_CLOSE &&
                            event.window.windowID == SDL_GetWindowID(window))
                        {
                            on_quit();
                        }
                        break;

                    case SDL_MOUSEWHEEL:
                        input::mouse.curr_mouse_wheel_x = event.wheel.x;
                        input::mouse.curr_mouse_wheel_y = event.wheel.y;
                        break;

#ifndef __EMSCRIPTEN__
                    case SDL_CONTROLLERDEVICEADDED:
                    {
                        auto index = event.cdevice.which;
                        if (index >= 0 && index < MAX_NUM_CONTROLLERS)
                        {
                            SDL_GameController* ctrl =
                                SDL_GameControllerOpen(index);

                            cstr name    = SDL_GameControllerName(ctrl);
                            u16  vendor  = SDL_GameControllerGetVendor(ctrl);
                            u16  product = SDL_GameControllerGetProduct(ctrl);
                            u16  version =
                                SDL_GameControllerGetProductVersion(ctrl);

                            SDL_Joystick* joystick =
                                SDL_GameControllerGetJoystick(ctrl);
                            i32 num_buttons = SDL_JoystickNumButtons(joystick);
                            i32 num_axis    = SDL_JoystickNumAxes(joystick);

                            input::connect_controller(
                                index, name, vendor, product, version,
                                num_buttons, num_axis
                            );
                            input::gamepads[index] = ctrl;
                        }
                    }
                    break;

                    case SDL_CONTROLLERDEVICEREMOVED:
                    {
                        auto index = input::get_controller_index_by_instance_id(
                            event.cdevice.which
                        );
                        if (index >= 0)
                        {
                            input::disconnect_controller(index);
                            SDL_GameControllerClose(input::gamepads[index]);
                        }
                    }
                    break;
#endif
                }
            }
        }

        inline cstr get_app_path()
        {
#ifndef __EMSCRIPTEN__
            return SDL_GetBasePath();
#endif
            return "";
        }

        inline str get_app_path_str() { return str(get_app_path()); }

        inline cstr get_user_path(cstr name)
        {
#ifndef __EMSCRIPTEN__
            return SDL_GetPrefPath(nullptr, name);
#endif
            return "";
        }

        inline void* get_Gl_function() { return (void*)&SDL_GL_GetProcAddress; }

        inline void swap(Window window) { SDL_GL_SwapWindow(window); }

        inline void shutdown(Window window, Renderer renderer)
        {
            SDL_GL_DeleteContext(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    };
}
