#include <blocs/game.h>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <emscripten/html5.h>

	void step_game_instance(void* instance)
	{
		static_cast<blocs::Game*>(instance)->step();
	}
#endif

using namespace blocs;

i32 Game::run(u8 framerate, bool fixed)
{
	assert(framerate > 0 && "Framerate must be greater than 0");

	this->framerate = framerate;
	this->fixed = fixed;

	m_frame_time = {};

	// compute how many ticks one update should be
	i64 desired_frametime = SDL_GetPerformanceFrequency() / framerate;

	// these are to snap deltaTime to vsync values if it's close enough
	m_frame_time.vsync_maxerror = (i64)(SDL_GetPerformanceFrequency() * .0002);

	i64 time_60hz = SDL_GetPerformanceFrequency() / 60; //since this is about snapping to common vsync values
	i64 snap_frequencies[] = {
		 time_60hz,				// 60fps
		 time_60hz * 2,			// 30fps
		 time_60hz * 3,			// 20fps
		 time_60hz * 4,			// 15fps
		(time_60hz + 1) / 2,	// 120fps // 120hz, 240hz, or higher need to round up, so that adding 120hz twice guaranteed is at least the same as adding time_60hz once
		(time_60hz + 2)	/ 3,	// 180fps
		(time_60hz + 3)	/ 4,	// 240fps
	};
	memcpy(m_frame_time.snap_frequencies, snap_frequencies, sizeof(i64) * 7);

	// this is for delta time averaging
	i64 time_averager[] = { desired_frametime, desired_frametime, desired_frametime, desired_frametime };
	memcpy(m_frame_time.time_averager, time_averager, sizeof(i64) * 4);
	m_frame_time.averager_residual = 0;

	m_frame_time.resync		 = true;
	m_frame_time.start	  	 = SDL_GetPerformanceCounter();
	m_frame_time.prev		 = SDL_GetPerformanceCounter();
	m_frame_time.accumulator = 0;

	start();
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(&step_game_instance, this, 0, 1);
#else
	while (m_running) step();
#endif
	shutdown();
	platform::shutdown(window, renderer);
	return 0;
}

void Game::step()
{	
	// frame timer
	i64 current_frame_time = SDL_GetPerformanceCounter();
	i64 delta_time = current_frame_time - m_frame_time.prev;
	m_frame_time.prev = current_frame_time;

	Time time = {};
	time.total = (current_frame_time - m_frame_time.start) / (f64)SDL_GetPerformanceFrequency();
	time.ticks = SDL_GetTicks();

	f64 fixed_deltatime = 1.0 / framerate;
	i64 desired_frametime = SDL_GetPerformanceFrequency() / framerate;

	// handle unexpected timer anomalies (overflow, extra slow frames, etc)
	if (delta_time > desired_frametime * 8)
	{ // ignore extra-slow frames
		delta_time = desired_frametime;
	}
	if (delta_time < 0)
	{
		delta_time = 0;
	}

	// vsync time snapping
	for (i64 snap : m_frame_time.snap_frequencies)
	{
		if (std::abs(delta_time - snap) < m_frame_time.vsync_maxerror)
		{
			delta_time = snap;
			break;
		}
	}

	// delta time averaging
	for (i32 i = 0; i < 4 - 1; i++)
	{
		m_frame_time.time_averager[i] = m_frame_time.time_averager[i + 1];
	}
	m_frame_time.time_averager[4 - 1] = delta_time;
	i64 averager_sum = 0;
	for (i32 i = 0; i < 4; i++)
	{
		averager_sum += m_frame_time.time_averager[i];
	}
	delta_time = averager_sum / 4;

	m_frame_time.averager_residual += averager_sum % 4;
	delta_time 					   += m_frame_time.averager_residual / 4;
	m_frame_time.averager_residual %= 4;

	// add to the accumulator
	m_frame_time.accumulator += delta_time;

	// spiral of death protection
	if (m_frame_time.accumulator > desired_frametime * 8)
	{
		m_frame_time.resync = true;
	}

	// timer resync if requested
	if (m_frame_time.resync)
	{
		m_frame_time.accumulator = 0;
		delta_time = desired_frametime;
		m_frame_time.resync = false;
	}

	platform::events(
		window,
		&input,
		[&](Event& event) { if (on_event != nullptr) on_event(event); },
		[&](){ exit(); }
	);

	if (fixed)
	{
		time.dt = fixed_deltatime;
		while (m_frame_time.accumulator >= desired_frametime)
		{
			input.update();
			update(time);
			m_frame_time.accumulator -= desired_frametime;
		}
	}
	else
	{
		i64 consumedDeltaTime = delta_time;

		while (m_frame_time.accumulator >= desired_frametime)
		{
			time.dt = fixed_deltatime;
			// cap variable update's dt to not be larger than fixed update, and interleave it (so game state can always get animation frames it needs)
			if (consumedDeltaTime > desired_frametime)
			{
				input.update();
				update(time);
				consumedDeltaTime -= desired_frametime;
			}
			m_frame_time.accumulator -= desired_frametime;
		}

		time.dt = (f64)consumedDeltaTime / SDL_GetPerformanceFrequency();
		input.update();
		update(time);
	}
	
	{
		clear_backbuffer();
		render(time);
		platform::swap(window);
	}
}
