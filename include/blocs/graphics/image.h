#pragma once

#include <blocs/math/calc.h>

namespace blocs
{
    /** @brief 2D image data container. */
    struct Image
    {
        i32 w = 0;
        i32 h = 0;

        u8* pixels = nullptr;

        // Whether image data was generated from the stb_image library.
        bool from_stb = false;

        Image() = delete;

        /**
         * @brief Loads an image from a file.
         *
         * @param filepath Path to file.
         */
        Image(const str& filepath)
        {
            i32 bpp;
            from_stb = true;
            pixels   = stbi_load(filepath.c_str(), &w, &h, &bpp, 4);
            assert(pixels && "ERROR: could not load texture");
        }

        /**
         * @brief Creates a new image object from
         * existing pixel data.
         *
         * @param pixels Pixel data buffer.
         * @param w      Image width.
         * @param h      Image height.
         */
        Image(u8* pixels, const i32 w, const i32 h) : pixels(pixels), w(w), h(h)
        {
        }

        /**
         * @brief Creates an empty image of a size.
         *
         * @param w Image width.
         * @param h Image height.
         */
        Image(const i32 w, const i32 h) : w(w), h(h)
        {
            pixels = new u8[w * h * 4];
            memset(pixels, 0, w * h * 4 * sizeof(u8));
        }

        ~Image()
        {
            assert(
                pixels != nullptr && "ERROR: cannot dispose an unloaded image"
            );

            if (from_stb)
                stbi_image_free(pixels);
            else
                delete[] pixels;
        }

        /**
         * @brief Sets pixel data for a region
         * of the image.
         *
         * @param data Pixel data buffer.
         * @param dst  Destination rectangle.
         */
        void set(u8* data, const recti& dst)
        {
            if (dst.w > w || dst.h > h)
            {
                return;
            }

            for (i32 y = 0; y < dst.h; y++)
            {
                i32 from = y * dst.w;
                i32 to   = dst.x + (dst.y + y) * w;
                memcpy(
                    pixels + to * 4, data + from * 4, dst.w * 4 * sizeof(u8)
                );
            }
        }

        /**
         * @brief Creates a new array of hex colors
         * from every four values in the pixel data.
         *
         * @return Pointer to new colors array.
         */
        i32* colors()
        {
            i32* colors = new i32[w * h];
            i32  size   = 0;
            for (i32 i = 0; i < w * h * 4; i += 4)
            {
                color c;
                c.r          = pixels[i + 0];
                c.g          = pixels[i + 1];
                c.b          = pixels[i + 2];
                c.a          = pixels[i + 3];
                colors[size] = calc::hex(c);
                size++;
            }
            return colors;
        }
    };
}
