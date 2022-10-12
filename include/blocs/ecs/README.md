## Entity Component System

This is my personal C++ [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system) for prototyping games. The goals of this library are: ease of use, modularity, and to follow principles of [Data Oriented Design](https://en.wikipedia.org/wiki/Data-oriented_design) to benefit application performance.

---

### World

The world stores all data for entities and components, and provides them to systems. The `EntityManager`, `ComponentManager`, and `SystemManager` can be accessed from the world.

### Entity

Entities are represented by `uint64`. That's it.

```cpp
Entity entity = world.spawn_entity();
```

### Components

Components are [structs](https://en.wikipedia.org/wiki/Passive_data_structure). No logic, no inheritance; just [packed](https://en.wikipedia.org/wiki/Data_structure_alignment) data containers.

```cpp
struct Transform
{
  float x;
  float y;
};
```

You can associate a component with entity using `ComponentManager::add<T>()`.

```cpp
world.components.add<Transform>(entity);
```

The ECS uses the entity as an identifier to lookup components. An entity becomes an id for an array of contiguously stored component data.

### Systems

Systems implement logic to act on groups of shared components.

That sounds more complex than components and entities but they're just functions. Define a global function with the parameters `(World& world)` and call `SystemManager::add()`. 

```cpp
void system(World& world)
{
  LOG_DEBUG("hello world!");
};

world.systems.add(system);
```

#### Queries

Use `World::query<Components...>()` (with 0 ... n # of types) to pass a lambda that will be called on every entity that shares those component types.

For example, let's say we want to move an entity by applying physics transformations from a Rigidbody components to the Transform component.

```cpp
world.query<Transform, Rigidbody>([&](Transform& transform, Rigidbody& rigidbody)
{
  // Calculate velocity based on acceleration over time (deltaTime)
  rigidbody.velocity += rigidbody.acceleration * time.dt;

  // Add new velocity to transform
  transform.position += rigidbody.velocity;
});
```

You can use `World::iter<Components...>()` to return a `QueryResult` with an iterator that allows you to loop over components manually. The `iter()` method can be faster than `query()` and offers more control. However, it is recommended to only iterate backwards if you are creating or removing entities within your systems.

```cpp
auto query = world.iter<Transform, Rigidbody>();
for (auto [e, transform, rigidbody] : query)
{
  rigidbody.velocity += rigidbody.acceleration * time.dt;
  transform.position += rigidbody.velocity;
}
```

#### Setup

Setup Systems are systems that are called once when the game runs.

```cpp
void spawn_entities(World& world)
{
  // Spawns a player
  auto player = world.spawn_entity()
    .add_component<Transform>(32, 32)
    .add_component<PlayerController>();
    
  // Spawns an enemy
  auto enemy = world.spawn_entity()
    .add_component<Transform>(64, 64)
    .add_component<AIBehavior>();
}

world.systems.add_setup(spawn_entities);
```

#### Stages

Stages sequence your system code. By default, all systems are registered to the UPDATE stage.

These are the default stages (in order of execution):

``` js
// ──── START

┌────── EVENT
│
│   ┌── EARLY UPDATE
├───┼── UPDATE
│   └── LATE UPDATE
│
└───┬── DRAW
    └── UI
    
// ──── SHUTDOWN
```

<b>Update</b> runs once per frame. Early Update and Late Update occur immediately before and after respectively.

<b>Draw</b> renders the visual state of the game after all gameplay update steps have completed.

### Resources

Resources are global data that can be accessed and mutated by systems. Only one resource per type can be stored.

```cpp
struct Scoreboard
{
  uint8_t current_score;
  uint8_t high_score;
};
```

We can add the resource using `World::add_resource<T>()`.

```cpp
world.resources.add<Scoreboard>();
```

They can be retrieved using `World::get_resource<T>()`.

```cpp
void log_high_score(World& world)
{
  auto* score = world.resources.get<Scoreboard>();
  LOG_DEBUG(score->high_score);
}
```

---

### Acknowledgements

Zig creator Andrew Kelley's talk <a href="https://vimeo.com/649009599?embedded=true&source=video_title&owner=45836359">*"A Practical Guide to Applying Data-Oriented Design"*</a> at Handmade Seattle.

Mike Acton's <a href="https://www.youtube.com/watch?v=rX0ItVEVjHc">*"Data-Oriented Design in C++"*</a> from CppCon 2014.

Lead Software Engineer on Overwatch Timothy Ford's GDC talk <a href="https://www.youtube.com/watch?v=zrIY0eIyqmI">*"Overwatch's Gameplay Architecture and Netcode"*</a> where he details the teams discovery and adoption of ECS.
