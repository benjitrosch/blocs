## Debugging

Debug-only tools are available by enabling debug mode with the `#define DEBUG` preprocessor directive.

### Logging

The `LOG` macros output text to the terminal. Outside of debug mode they do nothing.

```cpp
// Only logs what gets put inside
LOG_DEBUG("general purpose debugger output");

// Appends filename and line number before message
// and adds an identifiable color in terminal
LOG_INFO("to show informational messages");               // Cyan
LOG_GOOD("to show success messages");                     // Green
LOG_WARN("to show warnings");                             // Yellow
LOG_ERR("to show error messages, also calls `abort()`!"); // Red
```

### Tests (WIP)

Basic methods for writing a unit testing suite are provided through the `DESCRIBE()` and `EXPECT()` macros.

Equality can be tested using `EXPECT()` to return a `test_result` object.
```cpp
int sum = 2 + 2;
EXPECT("2 + 2 should equal 4", sum, 4);
```

A vector of `test_result` objects can be passed to `DESCRIBE` to form a testing suite which logs the results of the suite to the terminal.

```cpp
DESCRIBE("computers know basic math", {
  EXPECT("1 + 1", 2, 2),
  EXPECT("2 + 2", 5, 4),
});

// FAIL main.cpp
// "computers know basic math"
//       ✓ "1 + 1"
//       x "2 + 2"
//           expect:	4
//           result:	5
// 
//       ran 2 test(s)
//       pass:	1
//       fail:	1
```

### JSON Serializer

Component data can be serialized to a human readable JSON format. Single components, vectors of components, and maps of components can all be serialized.

To make a component "serializable," use the `SERIALIZE(Type, Args...)` macro. The first parameter accepts a type of component, and the following parameters accept any number of fields that exist on the structure.

```cpp
SERIALIZE(Transform, x, y);
LOG_DEBUG(serializer::json::to_json(Transform { 5, 5 }));

// Outputs `{ "x": 5, "y": 5 }`
```

### Explorer

The Explorer can be used to monitor the `ecs::World`'s state. On a separate thread an HTTP server will be created which listens for requests on the passed in port (default 4001). From the Explorer's monitor tab you can configure the client to poll the game server and display the state.

To monitor a component, include the type in the `monitor<Components...>()` template. Only components that have been serialized are able to be monitored (see [JSON Serializer](#json-serializer)).

```cpp
SERIALIZE(Transform, x, y);
SERIALIZE(Rigidbody, x, y);

explorer::monitor(
  explorer::MonitorComponentsQuery<Transform, Rigidbody>{},
  explorer::MonitorSystemsQuery<Physics>{},
  explorer::MonitorResourcesQuery<>{},
  4001
);
```

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
imgui::draw(world.resources.get<ImGUIConfig>().io, ctx.window, ctx.renderer);
```

### Tracy Profiler

You can profile your app in realtime using [tracy](https://github.com/wolfpld/tracy). To enable, set the `BLOCS_DEBUG_TRACY` flag in CMake to `ON`.

To get started, include the header in your game's `main.cpp` entrypoint.

```cpp
#include <tracy/Tracy.hpp>
```

You must also add the `ZoneScoped` macro at the beginning of every scope you want to measure.

```cpp
int main(int argc, char* argv[])
{
  ZoneScoped;
  return Game("profiler demo")
    .run();
}
```

Run the tracy profiler UI application to view the results. The executable is available for download on Windows, but OSX users will need to build the profiler themselves by following these steps:

```bash
# Install dependencies with brew
brew install freetype capstone gtk glfw

# Clone the repo locally and navigate to the profiler build folder
git clone https://github.com/wolfpld/tracy.git
cd tracy/profiler/build/unix

# Run the `release` make command to build for osx
make release
```