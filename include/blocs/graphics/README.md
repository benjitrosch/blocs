## Rendering

Basic 2D rendering with shaders can be achieved using OpenGL 3.3 either via helper functions in the `renderer` namespace or from a limited set of exposed OpenGL function bindings.

For a comprehensive list of supported OpenGL functions refer to [renderer.h](https://github.com/benjitrosch/blocs/blob/main/include/blocs/graphics/renderer.h).

### SpriteBatch

The `SpriteBatch` class provides a simple API to render 2D images and batch draw calls.

```cpp
auto* sprite_batch = new SpriteBatch();
auto* player_texture = new Texture("assets/sprites/player.png");

// Clip the region within the source texture
vec4f src = {  0,   0, 16, 16 };
// Set the target position and scale
vec4f dst = { 50, 100, 16, 16 };

mat4x4f matrix = mat4x4f::ortho(640, 480);

sprite_batch->begin(matrix);
sprite_batch->draw(player_texture->id, src, dst);
sprite_batch->end();
```

The SpriteBatch class can render geometric primitives, both filled and outlined.

```cpp
line(start, end, thickness, color);

bezier(start, curve, end, thickness, steps, color);
bezier(start, curve0, curve1, end, thickness, steps, color);

empty_triangle(p0, p1, p2, thickness, color);
triangle(p0, p1, p2, color);

empty_rect(bounds, thickness, color);
rect(bounds, color);

empty_circle(center, radius, thickness, steps, color);
circle(center, radius, steps, color);
```

### SpriteAtlas

The `SpriteAtlas` class combines multiple textures using an efficient [bin packing algorithm](https://en.wikipedia.org/wiki/Bin_packing_problem).

```cpp
auto* sprite_atlas = new SpriteAtlas();

// Add a texture to the atlas by filepath
sprite_atlas->add("assets/funny_cat_picture_00.png");
sprite_atlas->add("assets/funny_cat_picture_01.png");

// Organize textures using bin packing algo
sprite_atlas->pack();
// Blit individual texture data to final texture
sprite_atlas->generate_texture();
```

### SpriteFont

The `SpriteFont` class generates a texture atlas of glyphs from a [TrueType Font](https://en.wikipedia.org/wiki/TrueType) file (which gets stored in a `Font` struct).

```cpp
auto* sprite_font = new SpriteFont("assets/fonts/roboto.ttf", 96);

sprite_batch->text(*sprite_font, "abc123!@#", 0, 0);
```

An external SpriteAtlas can replace SpriteFont's internal atlas to further reduce draw calls.

```cpp
auto* sprite_atlas = new SpriteAtlas();

auto* font = new SpriteFont("assets/fonts/dogica.ttf", 96, sprite_atlas);
auto* sprite = sprite_atlas->add("assets/sprites/cool_dog.png");

// All one draw call due to sharing a single atlas
sprite_batch->begin();
sprite_batch->draw(sprite->texture, { sprite->x, sprite->y, sprite->w, sprite->h }, { 0, 0, 16, 16 });
sprite_batch->text(*font, "woof woof!", 0, 0);
sprite_batch->end();
```
