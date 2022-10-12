#pragma once

#include <blocs/math/calc.h>

namespace blocs
{
    struct Image
    {
        i32 w      = 0;
        i32 h      = 0;
        u8* pixels = nullptr;
        bool from_stb = false;

        Image() = delete;
        Image(const str& filepath)
        {
            i32 bpp;
            from_stb = true;
            pixels = stbi_load(filepath.c_str(), &w, &h, &bpp, 4);
            assert(pixels && "ERROR: could not load texture");    
        }

        Image(u8* pixels, const i32 w, const i32 h)
            : pixels(pixels), w(w), h(h)
        {}

        Image(const i32 w, const i32 h)
            : w(w), h(h)
        {
            pixels = new u8[w * h * 4];
            memset(pixels, 0, w * h * 4 * sizeof(u8));
        }

        ~Image()
        {
            assert(pixels != nullptr && "ERROR: cannot dispose an unloaded image");
            if (from_stb)
                stbi_image_free(pixels);
            else
                delete[] pixels;
        }

        void set(u8* data, const recti& dst)
        {
            for (i32 y = 0; y < dst.w; y++)
	        {
                i32 from = y * dst.z;
                i32 to = dst.x + (dst.y + y) * w;
                memcpy(pixels + to * 4, data + from * 4, dst.z * 4 * sizeof(u8));
            }
        }
        
        i32* colors()
        {
            i32* colors = new i32[w * h];
            i32 size = 0;
            for (i32 i = 0; i < w * h * 4; i += 4)
            {
                color c;
                c.x = pixels[i + 0] / 255.f;
                c.y = pixels[i + 1] / 255.f;
                c.z = pixels[i + 2] / 255.f;
                c.w = pixels[i + 3] / 255.f;
                colors[size] = calc::hex(c);
                size++;
            }
            return colors;
        }
    };
}