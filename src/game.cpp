#include <blocs/game.h>
#include <blocs/time.h>
#include <blocs/platform/input.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

void step_game_instance(void* instance)
{
    static_cast<blocs::Game*>(instance)->step();
}
#endif

using namespace blocs;

Window   app::window   = nullptr;
Renderer app::renderer = nullptr;

f64 time::dt    = 0.0f;
f64 time::total = 0.0f;
u32 time::ticks = 0U;

namespace
{
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

    FrameTime frame_time{};
}

i32 Game::run(u8 framerate, bool fixed)
{
    assert(framerate > 0 && "Framerate must be greater than 0");

    this->framerate = framerate;
    this->fixed     = fixed;

    frame_time = {};

    // compute how many ticks one update should be
    i64 desired_frametime = SDL_GetPerformanceFrequency() / framerate;

    // these are to snap deltaTime to vsync values if it's close enough
    frame_time.vsync_maxerror = (i64)(SDL_GetPerformanceFrequency() * .0002);

    i64 time_60hz = SDL_GetPerformanceFrequency() /
                    60;  // since this is about snapping to common vsync values
    i64 snap_frequencies[] = {
        time_60hz,            // 60fps
        time_60hz * 2,        // 30fps
        time_60hz * 3,        // 20fps
        time_60hz * 4,        // 15fps
        (time_60hz + 1) / 2,  // 120fps // 120hz, 240hz, or higher need to round
                              // up, so that adding 120hz twice guaranteed is at
                              // least the same as adding time_60hz once
        (time_60hz + 2) / 3,  // 180fps
        (time_60hz + 3) / 4,  // 240fps
    };
    memcpy(frame_time.snap_frequencies, snap_frequencies, sizeof(i64) * 7);

    // this is for delta time averaging
    i64 time_averager[] = {
        desired_frametime, desired_frametime, desired_frametime,
        desired_frametime};
    memcpy(frame_time.time_averager, time_averager, sizeof(i64) * 4);
    frame_time.averager_residual = 0;

    frame_time.resync      = true;
    frame_time.start       = SDL_GetPerformanceCounter();
    frame_time.prev        = SDL_GetPerformanceCounter();
    frame_time.accumulator = 0;

    start();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&step_game_instance, this, 0, 1);
#else
    while (m_running) step();
#endif
    shutdown();
    platform::shutdown(app::window, app::renderer);
    return 0;
}

void Game::step()
{
    // frame timer
    i64 current_frame_time = SDL_GetPerformanceCounter();
    i64 delta_time         = current_frame_time - frame_time.prev;
    frame_time.prev        = current_frame_time;

    time::total = (current_frame_time - frame_time.start) /
                  (f64)SDL_GetPerformanceFrequency();
    time::ticks = SDL_GetTicks();

    f64 fixed_deltatime   = 1.0 / framerate;
    i64 desired_frametime = SDL_GetPerformanceFrequency() / framerate;

    // handle unexpected timer anomalies (overflow, extra slow frames, etc)
    if (delta_time > desired_frametime * 8)
    {  // ignore extra-slow frames
        delta_time = desired_frametime;
    }
    if (delta_time < 0)
    {
        delta_time = 0;
    }

    // vsync time snapping
    for (i64 snap : frame_time.snap_frequencies)
    {
        if (std::abs(delta_time - snap) < frame_time.vsync_maxerror)
        {
            delta_time = snap;
            break;
        }
    }

    // delta time averaging
    for (i32 i = 0; i < 4 - 1; i++)
    {
        frame_time.time_averager[i] = frame_time.time_averager[i + 1];
    }
    frame_time.time_averager[4 - 1] = delta_time;
    i64 averager_sum                = 0;
    for (i32 i = 0; i < 4; i++)
    {
        averager_sum += frame_time.time_averager[i];
    }
    delta_time = averager_sum / 4;

    frame_time.averager_residual += averager_sum % 4;
    delta_time += frame_time.averager_residual / 4;
    frame_time.averager_residual %= 4;

    // add to the accumulator
    frame_time.accumulator += delta_time;

    // spiral of death protection
    if (frame_time.accumulator > desired_frametime * 8)
    {
        frame_time.resync = true;
    }

    // timer resync if requested
    if (frame_time.resync)
    {
        frame_time.accumulator = 0;
        delta_time             = desired_frametime;
        frame_time.resync      = false;
    }

    platform::events(
        app::window,
        [&](Event& event)
        {
            if (on_event != nullptr) on_event(event);
        },
        [&]() { exit(); }
    );

    if (fixed)
    {
        time::dt = fixed_deltatime;
        while (frame_time.accumulator >= desired_frametime)
        {
            input::update();
            update();
            frame_time.accumulator -= desired_frametime;
        }
    }
    else
    {
        i64 consumedDeltaTime = delta_time;

        while (frame_time.accumulator >= desired_frametime)
        {
            time::dt = fixed_deltatime;
            // cap variable update's dt to not be larger than fixed update, and
            // interleave it (so game state can always get animation frames it
            // needs)
            if (consumedDeltaTime > desired_frametime)
            {
                input::update();
                update();
                consumedDeltaTime -= desired_frametime;
            }
            frame_time.accumulator -= desired_frametime;
        }

        time::dt = (f64)consumedDeltaTime / SDL_GetPerformanceFrequency();
        input::update();
        update();
    }

    {
        clear_backbuffer();
        render();
        platform::swap(app::window);
    }
}
