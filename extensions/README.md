## Extensions

...

### Coroutines

...

### Dear ImGui

Simple API's are provided for [Dear ImGui](https://github.com/ocornut/imgui) to render debug UI during `Stage::UI`. To enable, set the `BLOCS_DEBUG_IMGUI` flag in CMake to `ON`.

To set up call `imgui::init()` and pass in the window and renderer. Additionally, you must create an `ImGUIConfig` object for later use.

```cpp
imgui_config = new imgui::ImGUIConfig{
  imgui::create(),
  imgui::get_io()
};
imgui::init(window, renderer);
```

To enable interaction (keyboard and mouse events) you must pass call `imgui::events()` from the `Game::on_event` lambda and pass a reference to the `Event` object.

```cpp
game->on_event = [](Event& event) { imgui::events(event); };
```

Use `imgui::new_frame()` to begin a new draw call and `imgui::draw()` to end it.

```cpp
// Only call this ONCE before rendering all UI
imgui::new_frame();

// The capital "I" in "ImGui" is not a typo-- this method belongs to the Dear ImGui library
ImGui::ShowDemoWindow()

// Only call this ONCE after rendering all UI
imgui::draw(world.resources.get<ImGUIConfig>().io, window, renderer);
```

### FMOD

...
