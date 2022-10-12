#pragma once

#include <blocs/common.h>
#include <blocs/math/vec.h>

namespace blocs
{
    struct color
    {
        u8 r, g, b, a;

        constexpr color() : r(0), g(0), b(0), a(0) {}
        constexpr color(u8 v) : r(v), g(v), b(v), a(v) {}
        constexpr color(u8 r, u8 g, u8 b, u8 a) : r(r), g(g), b(b), a(a) {}
        constexpr color(f32 v) : r(v * 255), g(v * 255), b(v * 255), a(v * 255)
        {
        }
        constexpr color(f32 r, f32 g, f32 b, f32 a)
            : r(r * 255), g(g * 255), b(b * 255), a(a * 255)
        {
        }
        constexpr color(const color& color)
            : r(color.r), g(color.g), b(color.b), a(color.a)
        {
        }
        constexpr color(const vec3<f32>& vec)
            : r(vec.x * 255), g(vec.y * 255), b(vec.z * 255), a(255)
        {
        }
        constexpr color(const vec4<f32>& vec)
            : r(vec.x * 255), g(vec.y * 255), b(vec.z * 255), a(vec.w * 255)
        {
        }

        constexpr color(i32 rgb)
            : r((u8)((rgb & 0xFF0000) >> 16)), g((u8)((rgb & 0x00FF00) >> 8)),
              b((u8)((rgb & 0x0000FF))), a(255)
        {
        }

        constexpr color(i32 rgb, f32 a)
            : r((u8)((rgb & 0xFF0000) >> 16) * a),
              g((u8)((rgb & 0x00FF00) >> 8) * a), b((u8)((rgb & 0x0000FF)) * a),
              a(255 * a)
        {
        }

        constexpr void premultiply()
        {
            r = r * a / 255;
            g = g * a / 255;
            b = b * a / 255;
        }

        constexpr vec3f to_vec3() const
        {
            return vec3f(r / 255.f, g / 255.f, b / 255.f);
        }

        constexpr vec4f to_vec4() const
        {
            return vec4f(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
        }

        constexpr color operator+(const color& color) const
        {
            u8 dr = r + color.r;
            u8 dg = g + color.g;
            u8 db = b + color.b;
            u8 da = a + color.a;

            return {dr, dg, db, da};
        }

        constexpr color operator-(const color& color) const
        {
            u8 dr = r - color.r;
            u8 dg = g - color.g;
            u8 db = b - color.b;
            u8 da = a - color.a;

            return {dr, dg, db, da};
        }

        constexpr color operator*(const color& color) const
        {
            u8 dr = r * color.r;
            u8 dg = g * color.g;
            u8 db = b * color.b;
            u8 da = a * color.a;

            return {dr, dg, db, da};
        }

        template<typename T>
        constexpr color operator*(T num) const
        {
            u8 dr = r * num;
            u8 dg = g * num;
            u8 db = b * num;
            u8 da = a * num;

            return color(dr, dg, db, da);
        }

        constexpr color operator/(const color& color) const
        {
            u8 dr = r / color.r;
            u8 dg = g / color.g;
            u8 db = b / color.b;
            u8 da = a / color.a;

            return {dr, dg, db, da};
        }

        template<typename T>
        constexpr color operator/(T num) const
        {
            u8 dr = r / num;
            u8 dg = g / num;
            u8 db = b / num;
            u8 da = a / num;

            return color(dr, dg, db, da);
        }

        constexpr color& operator+=(const color& color)
        {
            this->r += color.r;
            this->g += color.g;
            this->b += color.b;
            this->a += color.a;

            return *this;
        }

        constexpr color& operator-=(const color& color)
        {
            this->r -= color.r;
            this->g -= color.g;
            this->b -= color.b;
            this->a -= color.a;

            return *this;
        }

        constexpr color& operator*=(const color& color)
        {
            this->r *= color.r;
            this->g *= color.g;
            this->b *= color.b;
            this->a *= color.a;

            return *this;
        }

        template<typename T>
        constexpr color& operator*=(T num)
        {
            this->r *= num;
            this->g *= num;
            this->b *= num;
            this->a *= num;

            return *this;
        }

        constexpr color& operator/=(const color& color)
        {
            this->r /= color.r;
            this->g /= color.g;
            this->b /= color.b;
            this->a /= color.a;

            return *this;
        }

        template<typename T>
        constexpr color& operator/=(T num)
        {
            this->r /= num;
            this->g /= num;
            this->b /= num;
            this->a /= num;

            return *this;
        }

        constexpr bool operator==(const color& color) const
        {
            return r == color.r && g == color.g && b == color.b && a == color.a;
        }

        constexpr bool operator!=(const color& color) const
        {
            return r != color.r || g != color.g || b != color.b || a != color.a;
        }

        friend std::ostream& operator<<(std::ostream& os, const color& vec)
        {
            return os << "(" << vec.r << ", " << vec.g << ", " << vec.b << ", "
                      << vec.a << ")";
        }

        static const color clear;
        static const color black;
        static const color white;
        static const color red;
        static const color green;
        static const color blue;
        static const color yellow;
        static const color purple;
        static const color cyan;
    };

    inline const color color::clear  = {0.f, 0.f, 0.f, 0.f};
    inline const color color::white  = {1.f, 1.f, 1.f, 1.f};
    inline const color color::black  = {0.f, 0.f, 0.f, 1.f};
    inline const color color::red    = {1.f, 0.f, 0.f, 1.f};
    inline const color color::green  = {0.f, 1.f, 0.f, 1.f};
    inline const color color::blue   = {0.f, 0.f, 1.f, 1.f};
    inline const color color::yellow = {1.f, 1.f, 0.f, 1.f};
    inline const color color::purple = {1.f, 0.f, 1.f, 1.f};
    inline const color color::cyan   = {0.f, 1.f, 1.f, 1.f};
}
