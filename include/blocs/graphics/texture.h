#pragma once

namespace blocs
{
    struct Texture
    {
        u32 id;
        i32 w;
        i32 h;

        Texture()
        {
            id = 0;
            w = 0;
            h = 0;
        }

        Texture(const u32 id, const i32 w, const i32 h) : id(id), w(w), h(h) {}

        Texture(const str& filepath, bool aliased = true)
        {
            id = renderer::textures::generate(filepath, w, h, aliased);
#ifdef __EMSCRIPTEN__
            if (!calc::is_power_of_two(w) || !calc::is_power_of_two(h))
            {
                LOG_ERR("texture size must be power of two");
            }
#endif
        }

        Texture(const void* pixels, const i32 w, const i32 h, bool aliased = true)
            : w(w), h(h)
        {
            id = renderer::textures::generate(pixels, w, h, aliased);
#ifdef __EMSCRIPTEN__
            if (!calc::is_power_of_two(w) || !calc::is_power_of_two(h))
            {
                LOG_ERR("texture size must be power of two");
            }
#endif
        }

        ~Texture()
        {
            renderer::textures::remove(id);
        }
    };
}
