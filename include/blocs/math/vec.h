#pragma once

#include <blocs/common.h>

namespace blocs
{
    template<class T> struct vec2;
    template<class T> struct vec3;
    template<class T> struct vec4;

	using vec2f = vec2<f32>;
	using vec2i = vec2<i32>;
	using point = vec2<i32>;

    using vec3f = vec3<f32>;
    using vec3i = vec3<i32>;
	template<class T>
	using vertex = vec3<T>;
    using vertf = vec3<f32>;
	using verti = vec3<i32>;

    using vec4f = vec4<f32>;
    using vec4i = vec4<i32>;
	template<class T>
	using rect = vec4<T>;
	using rectf = vec4<f32>;
	using recti = vec4<i32>;
    using color = vec4<f32>;

	template<class T>
	struct vec2
	{
		T x;
		T y;

		constexpr vec2()
			: x((T)0), y((T)0)
		{}

		constexpr vec2(T v)
			: x(v), y(v)
		{}

		constexpr vec2(T x, T y)
			: x(x), y(y)
		{}

		constexpr vec2(const vec2<T>& vec)
			: x(vec.x), y(vec.y)
		{}

		static const vec2 zero;
		static const vec2 one;
		static const vec2 up;
		static const vec2 down;
		static const vec2 left;
		static const vec2 right;

		constexpr vec2 operator+(const vec2 vec) const
		{
			T dx = x + vec.x;
			T dy = y + vec.y;

			return vec2(dx, dy);
		}
		constexpr vec2 operator+(T num) const
		{
			T dx = x + num;
			T dy = y + num;

			return vec2(dx, dy);
		}
		constexpr vec2 operator-(const vec2 vec) const
		{
			T dx = x - vec.x;
			T dy = y - vec.y;

			return vec2(dx, dy);
		}
		constexpr vec2 operator-(T num) const
		{
			T dx = x - num;
			T dy = y - num;

			return vec2(dx, dy);
		}
		constexpr vec2 operator*(const vec2 vec) const
		{
			T dx = x * vec.x;
			T dy = y * vec.y;

			return vec2(dx, dy);
		}
		constexpr vec2 operator*(T num) const
		{
			T dx = x * num;
			T dy = y * num;

			return vec2(dx, dy);
		}
		constexpr vec2 operator/(const vec2 vec) const
		{
			T dx = x / vec.x;
			T dy = y / vec.y;

			return vec2(dx, dy);
		}
		constexpr vec2 operator/(T num) const
		{
			T dx = x / num;
			T dy = y / num;

			return vec2(dx, dy);
		}

		constexpr vec2& operator+=(const vec2 vec)
		{
			this->x += vec.x;
			this->y += vec.y;

			return *this;
		}
		constexpr vec2& operator+=(T num)
		{
			this->x += num;
			this->y += num;

			return *this;
		}
		constexpr vec2& operator-=(const vec2 vec)
		{
			this->x -= vec.x;
			this->y -= vec.y;

			return *this;
		}
		constexpr vec2& operator-=(T num)
		{
			this->x -= num;
			this->y -= num;

			return *this;
		}
		constexpr vec2& operator*=(const vec2 vec)
		{
			this->x *= vec.x;
			this->y *= vec.y;

			return *this;
		}
		constexpr vec2& operator*=(T num)
		{
			this->x *= num;
			this->y *= num;

			return *this;
		}
		constexpr vec2& operator/=(const vec2 vec)
		{
			this->x /= vec.x;
			this->y /= vec.y;

			return *this;
		}
		constexpr vec2& operator/=(T num)
		{
			this->x /= num;
			this->y /= num;

			return *this;
		}

		constexpr bool operator==(const vec2 vec) const
		{
			return x == vec.x && y == vec.y;
		}
		constexpr bool operator!=(const vec2 vec) const
		{
			return x != vec.x || y != vec.y;
		}

		friend std::ostream& operator<<(std::ostream& os, const vec2& vec)
        {
            return os << "(" << vec.x << ", " << vec.y << ")";
        }

		constexpr T sqrt_magnitude() const
		{
			return x * x + y * y;
		}

		constexpr T magnitude() const
		{
			return sqrt(sqrt_magnitude());
		}

		constexpr vec2 normalized() const
		{
			if (!x && !y) return { x, y };
			T val = (T)1 / magnitude();
            return { x * val, y * val };
		}
	};

    template<class T>
	struct vec3
	{
		T x;
		T y;
        T z;

		constexpr vec3()
			: x((T)0), y((T)0), z((T)0)
		{}

		constexpr vec3(T v)
			: x(v), y(v), z(v)
		{}

		constexpr vec3(T x, T y, T z)
			: x(x), y(y), z(z)
		{}

		constexpr vec3(const vec3<T>& vec3)
			: x(vec3.x), y(vec3.y), z(vec3.z)
		{}

		static const vec3 zero;
		static const vec3 one;

		constexpr vec2<T> xy() const
		{
			return { this->x, this->y };
		}

		constexpr vec3 operator+(const vec3 vec3) const
		{
			T dx = x + vec3.x;
			T dy = y + vec3.y;
            T dz = z + vec3.z;

			return vec3(dx, dy, dz);
		}
		constexpr vec3 operator-(const vec3 vec3) const
		{
			T dx = x - vec3.x;
			T dy = y - vec3.y;
			T dz = z - vec3.z;

			return vec3(dx, dy, dz);
		}
		constexpr vec3 operator*(const vec3 vec3) const
		{
			T dx = x * vec3.x;
			T dy = y * vec3.y;
			T dz = z * vec3.z;

			return vec3(dx, dy, dz);
		}
		constexpr vec3 operator*(T num) const
		{
			T dx = x * num;
			T dy = y * num;
			T dz = z * num;

			return vec3(dx, dy, dz);
		}
		constexpr vec3 operator/(const vec3 vec3) const
		{
			T dx = x / vec3.x;
			T dy = y / vec3.y;
			T dz = z / vec3.z;

			return vec3(dx, dy, dz);
		}
		constexpr vec3 operator/(T num) const
		{
			T dx = x / num;
			T dy = y / num;
			T dz = z / num;

			return vec3(dx, dy, dz);
		}

		constexpr vec3& operator+=(const vec3 vec3)
		{
			this->x += vec3.x;
			this->y += vec3.y;
			this->z += vec3.z;

			return *this;
		}
		constexpr vec3& operator-=(const vec3 vec3)
		{
			this->x -= vec3.x;
			this->y -= vec3.y;
			this->z -= vec3.z;

			return *this;
		}
		constexpr vec3& operator*=(const vec3 vec3)
		{
			this->x *= vec3.x;
			this->y *= vec3.y;
			this->z *= vec3.z;

			return *this;
		}
		constexpr vec3& operator*=(T num)
		{
			this->x *= num;
			this->y *= num;
			this->z *= num;

			return *this;
		}
		constexpr vec3& operator/=(const vec3 vec3)
		{
			this->x /= vec3.x;
			this->y /= vec3.y;
			this->z /= vec3.z;

			return *this;
		}
		constexpr vec3& operator/=(T num)
		{
			this->x /= num;
			this->y /= num;
			this->z /= num;

			return *this;
		}

		constexpr bool operator==(const vec3 vec3) const
		{
			return x == vec3.x && y == vec3.y && z == vec3.z;
		}
		constexpr bool operator!=(const vec3 vec3) const
		{
			return x != vec3.x || y != vec3.y || z != vec3.z;
		}

		friend std::ostream& operator<<(std::ostream& os, const vec3& vec)
        {
            return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        }
	};

    template<class T>
	struct vec4
	{
		T x;
		T y;
        T z;
        T w;

		constexpr vec4()
			: x((T)0), y((T)0), z((T)0), w((T)0)
		{}

		constexpr vec4(T v)
			: x(v), y(v), z(v), w(v)
		{}

		constexpr vec4(T x, T y, T z, T w)
			: x(x), y(y), z(z), w(w)
		{}

		constexpr vec4(const vec4<T>& vec4)
			: x(vec4.x), y(vec4.y), z(vec4.z), w(vec4.w)
		{}

		constexpr vec4(i32 rgb, T a = (T)1)
			:
			x((u8)((rgb & 0xFF0000) >> 16) / (T)255),
			y((u8)((rgb & 0x00FF00) >> 8)  / (T)255),
			z((u8)(rgb & 0x0000FF) 		   / (T)255),
			w(a)
		{}

		static const vec4 zero;
		static const vec4 one;

		static const color clear;
		static const color black;
		static const color white;
		static const color red;
		static const color green;
		static const color blue;
		static const color yellow;
		static const color purple;
		static const color cyan;

		constexpr vec2<T> xy() const
		{
			return { this->x, this->y };
		}

		constexpr vec3<T> xyz() const
		{
			return { this->x, this->y, this->z };
		}

		constexpr vec4 operator+(const vec4 vec4) const
		{
			T dx = x + vec4.x;
			T dy = y + vec4.y;
            T dz = z + vec4.z;
			T dw = w + vec4.w;

			return vec4(dx, dy, dz, dw);
		}
		constexpr vec4 operator-(const vec4 vec4) const
		{
			T dx = x - vec4.x;
			T dy = y - vec4.y;
			T dz = z - vec4.z;
			T dw = w - vec4.w;

			return vec4(dx, dy, dz, dw);
		}
		constexpr vec4 operator*(const vec4 vec4) const
		{
			T dx = x * vec4.x;
			T dy = y * vec4.y;
			T dz = z * vec4.z;
			T dw = w * vec4.w;

			return vec4(dx, dy, dz, dw);
		}
		constexpr vec4 operator*(T num) const
		{
			T dx = x * num;
			T dy = y * num;
			T dz = z * num;
			T dw = w * num;

			return vec4(dx, dy, dz, dw);
		}
		constexpr vec4 operator/(const vec4 vec4) const
		{
			T dx = x / vec4.x;
			T dy = y / vec4.y;
			T dz = z / vec4.z;
			T dw = w / vec4.w;

			return vec4(dx, dy, dz, dw);
		}
		constexpr vec4 operator/(T num) const
		{
			T dx = x / num;
			T dy = y / num;
			T dz = z / num;
			T dw = w / num;

			return vec4(dx, dy, dz, dw);
		}

		constexpr vec4& operator+=(const vec4 vec4)
		{
			this->x += vec4.x;
			this->y += vec4.y;
			this->z += vec4.z;
			this->w += vec4.w;

			return *this;
		}
		constexpr vec4& operator-=(const vec4 vec4)
		{
			this->x -= vec4.x;
			this->y -= vec4.y;
			this->z -= vec4.z;
			this->w -= vec4.w;

			return *this;
		}
		constexpr vec4& operator*=(const vec4 vec4)
		{
			this->x *= vec4.x;
			this->y *= vec4.y;
			this->z *= vec4.z;
			this->w *= vec4.w;

			return *this;
		}
		constexpr vec4& operator*=(T num)
		{
			this->x *= num;
			this->y *= num;
			this->z *= num;
			this->w *= num;

			return *this;
		}
		constexpr vec4& operator/=(const vec4 vec4)
		{
			this->x /= vec4.x;
			this->y /= vec4.y;
			this->z /= vec4.z;
			this->w /= vec4.w;

			return *this;
		}
		constexpr vec4& operator/=(T num)
		{
			this->x /= num;
			this->y /= num;
			this->z /= num;
			this->w /= num;

			return *this;
		}

		constexpr bool operator==(const vec4 vec4) const
		{
			return x == vec4.x && y == vec4.y && z == vec4.z && w == vec4.w;
		}
		constexpr bool operator!=(const vec4 vec4) const
		{
			return x != vec4.x || y != vec4.y || z != vec4.z || w != vec4.w;
		}

		friend std::ostream& operator<<(std::ostream& os, const vec4& vec)
        {
            return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        }
	};

	template<class T> inline const vec2<T> vec2<T>::zero 	= { (T) 0, (T) 0 };
	template<class T> inline const vec2<T> vec2<T>::one 	= { (T) 1, (T) 1 };
	template<class T> inline const vec2<T> vec2<T>::up 		= { (T) 0, (T)-1 };
	template<class T> inline const vec2<T> vec2<T>::down 	= { (T) 0, (T) 1 };
	template<class T> inline const vec2<T> vec2<T>::left 	= { (T)-1, (T) 0 };
	template<class T> inline const vec2<T> vec2<T>::right 	= { (T) 1, (T) 0 };

	template<class T> inline const vec3<T> vec3<T>::zero 	= { (T)0, (T)0, (T)0 };
	template<class T> inline const vec3<T> vec3<T>::one 	= { (T)1, (T)1, (T)1 };

	template<class T> inline const vec4<T> vec4<T>::zero 	= { (T)0, (T)0, (T)0, (T)0 };
	template<class T> inline const vec4<T> vec4<T>::one 	= { (T)1, (T)1, (T)1, (T)1 };

	template<> inline const color color::clear 				= { 0.f, 0.f, 0.f, 0.f };
	template<> inline const color color::white 				= { 1.f, 1.f, 1.f, 1.f };
	template<> inline const color color::black 				= { 0.f, 0.f, 0.f, 1.f };
	template<> inline const color color::red    			= { 1.f, 0.f, 0.f, 1.f };
	template<> inline const color color::green  			= { 0.f, 1.f, 0.f, 1.f };
	template<> inline const color color::blue   			= { 0.f, 0.f, 1.f, 1.f };
	template<> inline const color color::yellow 			= { 1.f, 1.f, 0.f, 1.f };
	template<> inline const color color::purple 			= { 1.f, 0.f, 1.f, 1.f };
	template<> inline const color color::cyan   			= { 0.f, 1.f, 1.f, 1.f };
}
