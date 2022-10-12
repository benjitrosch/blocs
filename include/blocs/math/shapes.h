#pragma once

#include <blocs/common.h>
#include <blocs/math/vec.h>

namespace blocs
{
    template<class T> struct circle;

    using circlef = circle<f32>;
    using circlei = circle<i32>;

    template<class T>
    struct circle
    {
        vec2<T> center;
        T radius;

        constexpr circle()
            : center(vec2<T>::zero()), radius((T)0)
        {}
        
        constexpr circle(T x, T y, T radius)
            : center(vec2<T>(x, y)), radius(radius)
        {}

        constexpr circle(const circle<T>& circle)
			: center(circle.center), radius(circle.radius)
		{}
    };

    template<class T> struct line;

    using linef = line<f32>;
    using linei = line<i32>;

    template<class T>
    struct line
    {
        vec2<T> a;
        vec2<T> b;

        constexpr line()
            : a(vec2<T>::zero()), b(vec2<T>::zero())
        {}

        constexpr line(const vec2<T>& a, const vec2<T>& b)
            : a(a), b(b)
        {}
        
        constexpr line(T x0, T y0, T x1, T y1)
            : a(vec2<T>(x0, y0)), b(vec2<T>(x1, y1))
        {}
    };

    template<class T> struct quad;

    using quadf = quad<f32>;
    using quadi = quad<i32>;

    template<class T>
    struct quad
    {
        vec2<T> a;
        vec2<T> b;
        vec2<T> c;
        vec2<T> d;

        constexpr quad()
            : a(vec2<T>::zero()), b(vec2<T>::zero()), c(vec2<T>::zero()), d(vec2<T>::zero())
        {}
        
        constexpr quad(const vec2<T> a, const vec2<T> b, const vec2<T> c, const vec2<T> d)
            : a(a), b(b), c(c), d(d)
        {}
    };
}
