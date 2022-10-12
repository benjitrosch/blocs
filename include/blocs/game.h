#pragma once

#include <blocs/common.h>

#include <blocs/platform/input.h>
#include <blocs/platform/platform.h>
#include <blocs/graphics/renderer.h>

namespace blocs
{
    namespace app
    {
        extern Window   window;
        extern Renderer renderer;
    }

    /**
     * @brief Controls the game loop, window, and render context.
     */
    class Game
    {
    private:
        struct FrameTime
        {
            /** Start frame time of game. */
            i64 start;
            /** Previous frame time. */
            i64 prev;

            u32 frame_count;
            f64 frame_elapsed_time;
            f32 update_rate = 1.0f;

            i64 accumulator;
            i64 averager_residual;

            i64 vsync_maxerror;
            i64 snap_frequencies[7];
            i64 time_averager[4];

            bool resync;
        };

        FrameTime m_frame_time{};

        /**
         * Whether or not the game is running (true means systems have
         * initialized and the update loop is active, false means
         * pre-initialization or shutdown).
         */
        bool m_running;

        void clear_backbuffer() { renderer::clear(bg_color); }

        /**
         * @brief
         * Disposes window and OpenGL context, frees texture assets from memory,
         * and then quits the application.
         */
        void shutdown()
        {
            if (on_shutdown != nullptr) on_shutdown();
            platform::shutdown(app::window, app::renderer);
        }

    public:
        /** Target framerate (expressed as frames per second). */
        u8   framerate = 60;
        /** Whether to update on a fixed timestep or not. */
        bool fixed     = false;

        cstr  title    = "";
        color bg_color = color::black;

        /**
         * @brief Initializes the game window, creates a renderer, and sets the
         * game's running state to true.
         *
         * @param title 	 Window title.
         * @param v 		 Version of application.
         * @param w 		 Width of the window.
         * @param h 		 Height of the window.
         * @param fullscreen Whether to start the window in fullscreen or not.
         * Defaults to false.
         * @param vsync 	 Whether to use vsync to synchronize frame rate
         * with monitor refresh rate. Defaults to true.
         * @param flags 	 Flags to alter the state of the window (e.g.
         * fullscreen).
         */
        Game(
            cstr title, i32 w, i32 h, bool fullscreen = false,
            bool vsync = true, u32 flags = 0
        )
            : title(title)
        {
            app::window = platform::init(title, w, h, fullscreen, vsync, flags);

            assert(app::window && "Could not create a window");

            app::renderer = platform::create_context(app::window, vsync);
            renderer::bind((GLLoadFunc)platform::get_Gl_function());

            assert(app::renderer && "Could not create a renderer context");

            // If window and renderer were created succesfully then the game has
            // successfully started to run.
            m_running = true;

            // Improves performance of std::cout.
            std::ios_base::sync_with_stdio(false);
        }

        ~Game() {}

        Game& background(const color& color)
        {
            bg_color = color;
            return *this;
        }

        /**
         * @brief Run event systems to access inputs, window events, and more.
         *
         * @param event Reference to event union structure.
         */
        std::function<void(Event& event)> on_event = nullptr;

        std::function<void()> on_shutdown = nullptr;

        /**
         * @brief Run startup systems in the order they were added.
         */
        void start();

        /**
         * @brief Updates game logic once per frame. Useful for mutating values
         * or handling input.
         */
        void update();

        /**
         * @brief Rendering logic to draw onto the screen. Called after gameplay
         * update steps have completed.
         */
        void render();

        /** @brief Ends the application and begins cleanup. */
        void exit() { m_running = false; }

        /**
         * @brief Begins the game loop.
         *
         * @param framerate  Target framerate of game.
         * @param fixed 	 Whether to use a fixed timestep or not.
         * Defaults to false.
         *
         * @return Exit code.
         */
        i32 run(u8 framerate = 60, bool fixed = false);

#ifndef __EMSCRIPTEN__
    private:
#endif
        /**
         * @brief Calls event, update, and render logic on schedule using
         * timestep configurations defined in `Game::run()`.
         */
        void step();
    };
}
