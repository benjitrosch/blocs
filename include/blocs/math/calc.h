#pragma once

#include <algorithm>

#include <blocs/math/vec.h>
#include <blocs/math/color.h>
#include <blocs/math/matrix.h>

namespace blocs
{
    namespace calc
    {
        constexpr f64 deg_2_rad = M_PI / 180.0;
        constexpr f64 rad_2_deg = 180.0 / M_PI;

        template<typename T>
        inline bool is_power_of_two(T n)
        {
            if (n == 0) return false;
            return (ceil(log2(n)) == floor(log2(n)));
        }

        template<typename T>
        inline T sgn(T val)
        {
            return (T)(T(0) < val) - (val < T(0));
        }

        template<typename T>
        inline T lerp(T a, T b, T t)
        {
            return a + (b - a) * t;
        }

        template<typename T>
        inline T approach(T n, T target, T delta)
        {
            return n < target ? std::min(n + delta, target)
                              : std::max(n - delta, target);
        }

        template<typename T>
        inline T map(T n, T old_min, T old_max, T new_min, T new_max)
        {
            return new_min +
                   ((n - old_min) / (old_max - old_min)) * (new_max - new_min);
        }

        template<typename T>
        inline T clamp(T n, T min, T max)
        {
            return std::max(min, std::min(n, max));
        }

        template<typename T>
        inline T clamp_map(T n, T old_min, T old_max, T new_min, T new_max)
        {
            return map(
                calc::clamp(n, old_min, old_max), old_min, old_max, new_min,
                new_max
            );
        }

        template<typename T>
        inline vec2<T> clamp_magnitude(vec2<T> a, f32 max)
        {
            T length = sqrt(pow(a.x, 2) + pow(a.y, 2));
            T mult   = 1;

            if (length > max)
            {
                mult = max / length;
            }

            return vec2(a.x * mult, a.y * mult);
        }

        template<typename T>
        inline vec2<T> clamp(vec2<T> current, vec2<T> min, vec2<T> max)
        {
            T x = clamp(current.x, min.x, max.x);
            T y = clamp(current.y, min.y, max.y);

            return vec2(x, y);
        }

        template<typename T>
        inline vec2<T> lerp(vec2<T> a, vec2<T> b, f32 t)
        {
            return vec2(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
        }

        template<typename T>
        inline vec2<T> lerp_bezier(
            const vec2<T>& a, const vec2<T>& b, const vec2<T>& c, f32 t
        )
        {
            return lerp(lerp(a, b, t), lerp(b, c, t), t);
        }

        template<typename T>
        inline vec2<T> lerp_bezier(
            const vec2<T>& a, const vec2<T>& b, const vec2<T>& c,
            const vec2<T>& d, f32 t
        )
        {
            return lerp_bezier(lerp(a, b, t), lerp(b, c, t), lerp(c, d, t), t);
        }

        inline color lerp(color a, color b, f32 t)
        {
            return color(
                (u8)(lerp(a.r / 255.f, b.r / 255.f, t) * 255),
                (u8)(lerp(a.g / 255.f, b.g / 255.f, t) * 255),
                (u8)(lerp(a.b / 255.f, b.b / 255.f, t) * 255),
                (u8)(lerp(a.a / 255.f, b.a / 255.f, t) * 255)
            );
        }

        inline i32 hex(i32 r, i32 g, i32 b)
        {
            return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
        }

        inline i32 hex(const color& c) { return hex(c.r, c.g, c.b); }

        inline color transparentize(const color& col, f32 a)
        {
            return color(col.r, col.g, col.b, (u8)(a * 255));
        }

        inline f32 hue(f32 p, f32 q, f32 t)
        {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
            if (t < 1.f / 2.f) return q;
            if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;

            return p;
        }

        /**
         * @brief Converts an HSLA color to RGBA.
         *
         * @param h Hue.
         * @param s Saturation.
         * @param l Lightness.
         * @param a Alpha.
         *
         * @return color RGBA value.
         */
        inline color hsla(f32 h, f32 s, f32 l, f32 a)
        {
            vec3f result = {};

            if (s == 0.f)
            {
                result.x = result.y = result.z = l;
            }
            else
            {
                f32 q = l < 0.5f ? l * (1.f + s) : l + s - l * s;
                f32 p = 2.f * l - q;

                result.x = hue(p, q, h + 1.f / 3.f);
                result.y = hue(p, q, h);
                result.z = hue(p, q, h - 1.f / 3.f);
            }

            return {result.x, result.y, result.z, a};
        }

        template<typename T>
        inline T smooth_damp(T a, T b, T& curr_val, f32 t, f32 dt)
        {
            T smoothtime = t > (T)0.0001 ? t : (T)0.0001;
            T omega      = (T)2.0 / smoothtime;

            T x = omega * dt;
            T exp =
                (T)1.0 / ((T)1.0 + x + (T)0.48 * x * x + (T)0.235 * x * x * x);
            T change      = a - b;
            T original_to = b;

            // Clamp maximum speed
            T max_change = INFINITY * smoothtime;
            change       = calc::clamp(change, -max_change, max_change);
            b            = a - change;

            T temp   = (curr_val + omega * change) * dt;
            curr_val = (curr_val - omega * temp) * exp;
            T output = b + (change + temp) * exp;

            // Prevent overshooting
            if (original_to - a > (T)0.0 == output > original_to)
            {
                output   = original_to;
                curr_val = (output - original_to) / dt;
            }

            return output;
        }

        template<typename T>
        inline vec2<T> smooth_damp(vec2<T> a, vec2<T> b, f32 t, f32 dt)
        {
            f32 smoothtime = t > (T)0.0001 ? t : (T)0.0001;
            f32 omega      = 2.0 / t;

            f32 dot = omega * dt;
            f32 exp = (T)1.0 / ((T)1.0 + dot + (T)0.48 * dot * dot +
                                (T)0.235 * dot * dot * dot);

            T change_x = a.x - b.x;
            T change_y = a.y - b.y;

            vec2 current = a;
            vec2 target  = b;

            f32 max_change    = INFINITY * smoothtime;
            f32 max_change_sq = max_change * max_change;

            f32 sq_dist = change_x * change_x + change_y * change_y;
            if (sq_dist > max_change_sq)
            {
                f32 mag  = sqrt(sq_dist);
                change_x = change_x / mag * max_change;
                change_y = change_y / mag * max_change;
            }

            target.x = a.x - change_x;
            target.y = a.y - change_y;

            T temp_x = (a.x + omega * change_x) * dt;
            T temp_y = (a.y + omega * change_y) * dt;

            current.x = (a.x - omega * temp_x) * exp;
            current.y = (a.y - omega * temp_y) * exp;

            T output_x = target.x + (change_x + temp_x) * exp;
            T output_y = target.y + (change_y + temp_y) * exp;

            f32 orig_minus_current_x = a.x - current.x;
            f32 orig_minus_current_y = a.y - current.y;
            f32 out_minus_orig_x     = output_x - a.x;
            f32 out_minus_orig_y     = output_y - a.y;
            if (orig_minus_current_x * out_minus_orig_x +
                    orig_minus_current_y * out_minus_orig_y >
                (T)0.0)
            {
                output_x = a.x;
                output_y = a.y;
            }

            T x = output_x;
            T y = output_y;

            return vec2(x, y);
        }

        template<typename T>
        inline T dist(vec2<T> vec0, vec2<T> vec1)
        {
            T v0 = vec0.x - vec1.x;
            T v1 = vec0.y - vec1.y;
            return (T)sqrt((v0 * v0) + (v1 * v1));
        }

        template<typename T>
        inline T dot(const vec2<T> vec0, const vec2<T> vec1)
        {
            return vec0.x * vec1.x + vec0.y * vec1.y;
        }

        template<typename T>
        inline T triple_dot(
            const vec2<T> vec0, const vec2<T> vec1, const vec2<T> vec2
        )
        {
            return {
                vec0.y * (vec1.x * vec2.y - vec1.y * vec2.x),
                vec0.x * (vec1.y * vec2.x - vec1.x * vec2.y)};
        }

        template<typename T1, typename T2>
        inline vec2<T1> vec_from_angle(T2 angle)
        {
            T1 vx = (T1)cos(angle);
            T1 vy = (T1)sin(angle);
            return vec2<T1>(vx, vy);
        }

        template<typename T1, typename T2>
        inline T1 angle_from_vec(vec2<T2> vec)
        {
            return (T1)atan2(vec.y, vec.x);
        }

        template<typename T1, typename T2>
        inline T1 angle_between_vecs(vec2<T2> vec0, vec2<T2> vec1)
        {
            return acos(
                calc::dot(vec0, vec1) / vec0.magnitude() * vec1.magnitude()
            );
        }

        template<typename T>
        inline T vec_length_squared(vec2<T> vec)
        {
            return (vec.x * vec.x) + (vec.y * vec.y);
        }

        template<typename T>
        inline T vec_length(vec2<T> vec)
        {
            return (T)sqrt(vec_length_squared(vec));
        }

        template<typename T>
        inline vec2<T> vec_perpendicular(vec2<T> vec)
        {
            return {-vec.y, vec.x};
        }

        template<typename T>
        inline vec2<T> vec_perpendicular_neg(vec2<T> vec)
        {
            return {vec.y, -vec.x};
        }

        template<typename T>
        inline vec2<T> vec_major(vec2<T> vec)
        {
            return vec.x >= vec.y ? vec.x : vec.y;
        }

        template<typename T>
        inline vec2<T> rotate_around(vec2<T> vec, vec2<T> center, const T angle)
        {
            const vec2<T> relative = vec - center;
            const f32     ca       = cos(angle);
            const f32     sa       = sin(angle);
            const vec2<T> rotated  = {
                ca * relative.x - sa * relative.y,
                sa * relative.x + ca * relative.y};
            return rotated + center;
        }

        template<typename T>
        constexpr inline vec2<T> transform(
            const vec2<T>& vec, const mat4x4<T>& mat
        )
        {
            return vec2<T>(
                (vec.x * mat[0][0]) + (vec.y * mat[1][0]) + mat[3][0],
                (vec.x * mat[0][1]) + (vec.y * mat[1][1]) + mat[3][1]
            );
        }

        template<typename T>
        vec2<T> inline world_to_screen(
            const vec2<T>& pos, const mat4x4<T>& cam, f32 zoom
        )
        {
            return transform(pos, cam) * zoom;
        }

        template<typename T>
        vec2<T> inline screen_to_world(const vec2<T>& pos, const mat4x4<T>& cam)
        {
            mat4x4<T> im = mat4x4<T>();
            f64       det;
            mat4x4<T>::invert(cam, im, det);
            return transform(pos, im);
        }
    }
}
