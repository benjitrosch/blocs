#pragma once

#include <blocs/common.h>
#include <blocs/math/vec.h>

namespace blocs
{
    template<typename T>
    struct rect;

    using rectf = rect<f32>;
    using recti = rect<i32>;

    template<typename T>
    struct rect
    {
        T x, y;
        T w, h;

        constexpr rect() : x((T)0), y((T)0), w((T)0), h((T)0) {}
        constexpr rect(T v) : x(v), y(v), w(v), h(v) {}
        constexpr rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
        constexpr rect(const rect<T>& rect)
            : x(rect.x), y(rect.y), w(rect.w), h(rect.h)
        {
        }

        static const rect zero;
        static const rect one;

        constexpr rect operator+(const rect& rect) const
        {
            T dx = x + rect.x;
            T dy = y + rect.y;
            T dw = w + rect.w;
            T dh = h + rect.h;

            return rect(dx, dy, dw, dh);
        }

        constexpr rect operator+(const vec2<T>& vec) const
        {
            T dx = x + vec.x;
            T dy = y + vec.y;

            return rect(dx, dy, w, h);
        }

        constexpr rect operator-(const rect& rect) const
        {
            T dx = x - rect.x;
            T dy = y - rect.y;
            T dw = w - rect.w;
            T dh = h - rect.h;

            return rect(dx, dy, dw, dh);
        }

        constexpr rect operator-(const vec2<T>& vec) const
        {
            T dx = x - vec.x;
            T dy = y - vec.y;

            return rect(dx, dy, w, h);
        }

        constexpr rect operator*(const rect& rect) const
        {
            T dx = x * rect.x;
            T dy = y * rect.y;
            T dw = w * rect.w;
            T dh = h * rect.h;

            return rect(dx, dy, dw, dh);
        }

        constexpr rect operator*(T num) const
        {
            T dx = x * num;
            T dy = y * num;
            T dw = w * num;
            T dh = h * num;

            return rect(dx, dy, dw, dh);
        }

        constexpr rect operator/(const rect& rect) const
        {
            T dx = x / rect.x;
            T dy = y / rect.y;
            T dw = w / rect.w;
            T dh = h / rect.h;

            return rect(dx, dy, dw, dh);
        }

        constexpr rect operator/(T num) const
        {
            T dx = x / num;
            T dy = y / num;
            T dw = w / num;
            T dh = h / num;

            return rect(dx, dy, dw, dh);
        }

        constexpr rect& operator+=(const rect& rect)
        {
            this->x += rect.x;
            this->y += rect.y;
            this->w += rect.w;
            this->h += rect.h;

            return *this;
        }

        constexpr rect& operator+=(const vec2<T>& vec)
        {
            this->x += vec.x;
            this->y += vec.y;

            return *this;
        }

        constexpr rect& operator-=(const rect& rect)
        {
            this->x -= rect.x;
            this->y -= rect.y;
            this->w -= rect.w;
            this->h -= rect.h;

            return *this;
        }

        constexpr rect& operator-=(const vec2<T>& vec)
        {
            this->x -= vec.x;
            this->y -= vec.y;

            return *this;
        }

        constexpr rect& operator*=(const rect rect)
        {
            this->x *= rect.x;
            this->y *= rect.y;
            this->w *= rect.w;
            this->h *= rect.h;

            return *this;
        }

        constexpr rect& operator*=(T num)
        {
            this->x *= num;
            this->y *= num;
            this->w *= num;
            this->h *= num;

            return *this;
        }

        constexpr rect& operator/=(const rect& rect)
        {
            this->x /= rect.x;
            this->y /= rect.y;
            this->w /= rect.w;
            this->h /= rect.h;

            return *this;
        }

        constexpr rect& operator/=(T num)
        {
            this->x /= num;
            this->y /= num;
            this->w /= num;
            this->h /= num;

            return *this;
        }

        constexpr bool operator==(const rect& rect) const
        {
            return x == rect.x && y == rect.y && w == rect.w && h == rect.h;
        }

        constexpr bool operator!=(const rect& rect) const
        {
            return x != rect.x || y != rect.y || w != rect.w || h != rect.h;
        }

        friend std::ostream& operator<<(std::ostream& os, const rect& rect)
        {
            return os << "(" << rect.x << ", " << rect.y << ", " << rect.w
                      << ", " << rect.h << ")";
        }

        constexpr vec2<T> pos() const { return vec2<T>(x, y); }

        constexpr vec2<T> extent() const { return vec2<T>(x + w, y + h); }

        constexpr vec2<T> center() const
        {
            return vec2<T>(x + w / (T)2, y + h / (T)2);
        }

        constexpr bool intersects(const rect& b) const
        {
            return (
                (x + w >= b.x) & (x <= b.x + b.w) & (y + h >= b.y) &
                (y <= b.y + b.h)
            );
        }

        constexpr bool contains(const vec2<T>& point) const
        {
            if (point.x < x || point.x > x + w) return false;
            if (point.y < y || point.y > y + h) return false;
            return true;
        }

        constexpr bool contains(const rect<T>& rect) const
        {
            return x < rect.x && y < rect.y && x + w > rect.x + rect.w &&
                   y + h > rect.y + rect.h;
        }

        constexpr rect scale(T sx, T sy) const
        {
            return rect(x * sx, y * sy, w * sx, h * sy);
        }

        constexpr rect expand(T sx, T sy) const
        {
            return rect(x - sx, y - sy, w + sx * 2, h + sy * 2);
        }

        constexpr rect translate(T dx, T dy)
        {
            return rect(x + dx, y + dx, w, h);
        }

        constexpr rect translate(vec2<T> pos)
        {
            return rect(x + pos.x, y + pos.y, w, h);
        }
    };

    template<typename T>
    inline const rect<T> rect<T>::zero = {(T)0, (T)0, (T)0, (T)0};
    template<typename T>
    inline const rect<T> rect<T>::one = {(T)1, (T)1, (T)1, (T)1};

    template<typename T>
    struct circle;

    using circlef = circle<f32>;
    using circlei = circle<i32>;

    template<typename T>
    struct circle
    {
        vec2<T> center;
        T       radius;

        constexpr circle() : center(vec2<T>::zero()), radius((T)0) {}

        constexpr circle(T x, T y, T radius)
            : center(vec2<T>(x, y)), radius(radius)
        {
        }

        constexpr circle(const circle<T>& circle)
            : center(circle.center), radius(circle.radius)
        {
        }
    };

    template<typename T>
    struct line;

    using linef = line<f32>;
    using linei = line<i32>;

    template<typename T>
    struct line
    {
        vec2<T> a;
        vec2<T> b;

        constexpr line() : a(vec2<T>::zero()), b(vec2<T>::zero()) {}

        constexpr line(const vec2<T>& a, const vec2<T>& b) : a(a), b(b) {}

        constexpr line(T x0, T y0, T x1, T y1)
            : a(vec2<T>(x0, y0)), b(vec2<T>(x1, y1))
        {
        }
    };

    template<typename T>
    struct quad;

    using quadf = quad<f32>;
    using quadi = quad<i32>;

    template<typename T>
    struct quad
    {
        vec2<T> a;
        vec2<T> b;
        vec2<T> c;
        vec2<T> d;

        constexpr quad()
            : a(vec2<T>::zero()), b(vec2<T>::zero()), c(vec2<T>::zero()),
              d(vec2<T>::zero())
        {
        }

        constexpr quad(
            const vec2<T> a, const vec2<T> b, const vec2<T> c, const vec2<T> d
        )
            : a(a), b(b), c(c), d(d)
        {
        }
    };
}
