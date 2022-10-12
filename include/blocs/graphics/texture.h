#pragma once

namespace blocs
{
    /** @brief 2D image that's drawable by the renderer. */
    struct Texture
    {
        u32 id;
        i32 w;
        i32 h;

        Texture()
        {
            id = 0;
            w  = 0;
            h  = 0;
        }

        /**
         * @brief Creates a texture object from an existing
         * renderer texture identifier.
         *
         * @param id Existing texture ID.
         * @param w  Image width.
         * @param h  Image height.
         */
        Texture(const u32 id, const i32 w, const i32 h) : id(id), w(w), h(h) {}

        /**
         * @brief Creates a new texture from an image file.
         *
         * @param filepath Path to image file.
         * @param aliased  Whether or not to use anti-aliasing.
         */
        Texture(const str& filepath, bool aliased = true)
        {
#ifdef __EMSCRIPTEN__
            if (!calc::is_power_of_two(w) || !calc::is_power_of_two(h))
            {
                LOG_ERR("texture size must be power of two");
                return;
            }
#endif
            id = renderer::textures::generate(filepath, w, h, aliased);
        }

        /**
         * @brief Creates a new texture from an image file
         *
         * @param pixels  Pixel data buffer.
         * @param w       Image width.
         * @param h       Image height.
         * @param aliased Whether or not to use anti-aliasing.
         */
        Texture(
            const void* pixels, const i32 w, const i32 h, bool aliased = true
        )
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

        ~Texture() { renderer::textures::remove(id); }
    };
}
