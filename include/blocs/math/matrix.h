#pragma once

#include <array>

#include <blocs/common.h>
#include <blocs/math/vec.h>

namespace blocs
{
    template<class T> struct mat4x4;
    using mat4x4f = mat4x4<f32>;
    using mat4x4i = mat4x4<i32>;
    
    /**
     * @brief Represents a four-by-four square matrix.
     * 
     * @tparam T Type of matrix values.
     */
    template<class T>
    struct mat4x4 : public std::array<std::array<T, 4>, 4>
    {
        mat4x4() {}
        mat4x4(std::array<std::array<T, 4>, 4> data)
            : std::array<std::array<T, 4>, 4>(data) {}
        mat4x4(std::array<std::array<T, 4>, 4>& data)
            : std::array<std::array<T, 4>, 4>(data) {}

        static const mat4x4 zero;
        static const mat4x4 one;
		static const mat4x4 identity;

        constexpr mat4x4 operator*(const mat4x4& mat) const
        {
            mat4x4 m = mat4x4::identity;

            m[0][0] = mat[0][0] * (*this)[0][0] + mat[0][1] * (*this)[1][0] + mat[0][2] * (*this)[2][1] + mat[0][3] * (*this)[3][0];
            m[0][1] = mat[0][0] * (*this)[0][1] + mat[0][1] * (*this)[1][1] + mat[0][2] * (*this)[2][2] + mat[0][3] * (*this)[3][1];
            m[0][2] = mat[0][0] * (*this)[0][2] + mat[0][1] * (*this)[1][2] + mat[0][2] * (*this)[2][2] + mat[0][3] * (*this)[3][2];
            m[0][3] = mat[0][0] * (*this)[0][3] + mat[0][1] * (*this)[1][3] + mat[0][2] * (*this)[2][3] + mat[0][3] * (*this)[3][3];

            m[1][0] = mat[1][0] * (*this)[0][0] + mat[1][1] * (*this)[1][0] + mat[1][2] * (*this)[2][1] + mat[1][3] * (*this)[3][0];
            m[1][1] = mat[1][0] * (*this)[0][1] + mat[1][1] * (*this)[1][1] + mat[1][2] * (*this)[2][2] + mat[1][3] * (*this)[3][1];
            m[1][2] = mat[1][0] * (*this)[0][2] + mat[1][1] * (*this)[1][2] + mat[1][2] * (*this)[2][2] + mat[1][3] * (*this)[3][2];
            m[1][3] = mat[1][0] * (*this)[0][3] + mat[1][1] * (*this)[1][3] + mat[1][2] * (*this)[2][3] + mat[1][3] * (*this)[3][3];
            
            m[2][0] = mat[2][0] * (*this)[0][0] + mat[2][1] * (*this)[1][0] + mat[2][2] * (*this)[2][1] + mat[2][3] * (*this)[3][0];
            m[2][1] = mat[2][0] * (*this)[0][1] + mat[2][1] * (*this)[1][1] + mat[2][2] * (*this)[2][2] + mat[2][3] * (*this)[3][1];
            m[2][2] = mat[2][0] * (*this)[0][2] + mat[2][1] * (*this)[1][2] + mat[2][2] * (*this)[2][2] + mat[2][3] * (*this)[3][2];
            m[2][3] = mat[2][0] * (*this)[0][3] + mat[2][1] * (*this)[1][3] + mat[2][2] * (*this)[2][3] + mat[2][3] * (*this)[3][3];

            m[3][0] = mat[3][0] * (*this)[0][0] + mat[3][1] * (*this)[1][0] + mat[3][2] * (*this)[2][1] + mat[3][3] * (*this)[3][0];
            m[3][1] = mat[3][0] * (*this)[0][1] + mat[3][1] * (*this)[1][1] + mat[3][2] * (*this)[2][2] + mat[3][3] * (*this)[3][1];
            m[3][2] = mat[3][0] * (*this)[0][2] + mat[3][1] * (*this)[1][2] + mat[3][2] * (*this)[2][2] + mat[3][3] * (*this)[3][2];
            m[3][3] = mat[3][0] * (*this)[0][3] + mat[3][1] * (*this)[1][3] + mat[3][2] * (*this)[2][3] + mat[3][3] * (*this)[3][3];

            return m;
        }

        constexpr mat4x4& operator*=(const mat4x4& mat)
        {
            (*this)[0][0] = mat[0][0] * (*this)[0][0] + mat[0][1] * (*this)[1][0] + mat[0][2] * (*this)[2][1] + mat[0][3] * (*this)[3][0];
            (*this)[0][1] = mat[0][0] * (*this)[0][1] + mat[0][1] * (*this)[1][1] + mat[0][2] * (*this)[2][2] + mat[0][3] * (*this)[3][1];
            (*this)[0][2] = mat[0][0] * (*this)[0][2] + mat[0][1] * (*this)[1][2] + mat[0][2] * (*this)[2][2] + mat[0][3] * (*this)[3][2];
            (*this)[0][3] = mat[0][0] * (*this)[0][3] + mat[0][1] * (*this)[1][3] + mat[0][2] * (*this)[2][3] + mat[0][3] * (*this)[3][3];

            (*this)[1][0] = mat[1][0] * (*this)[0][0] + mat[1][1] * (*this)[1][0] + mat[1][2] * (*this)[2][1] + mat[1][3] * (*this)[3][0];
            (*this)[1][1] = mat[1][0] * (*this)[0][1] + mat[1][1] * (*this)[1][1] + mat[1][2] * (*this)[2][2] + mat[1][3] * (*this)[3][1];
            (*this)[1][2] = mat[1][0] * (*this)[0][2] + mat[1][1] * (*this)[1][2] + mat[1][2] * (*this)[2][2] + mat[1][3] * (*this)[3][2];
            (*this)[1][3] = mat[1][0] * (*this)[0][3] + mat[1][1] * (*this)[1][3] + mat[1][2] * (*this)[2][3] + mat[1][3] * (*this)[3][3];
            
            (*this)[2][0] = mat[2][0] * (*this)[0][0] + mat[2][1] * (*this)[1][0] + mat[2][2] * (*this)[2][1] + mat[2][3] * (*this)[3][0];
            (*this)[2][1] = mat[2][0] * (*this)[0][1] + mat[2][1] * (*this)[1][1] + mat[2][2] * (*this)[2][2] + mat[2][3] * (*this)[3][1];
            (*this)[2][2] = mat[2][0] * (*this)[0][2] + mat[2][1] * (*this)[1][2] + mat[2][2] * (*this)[2][2] + mat[2][3] * (*this)[3][2];
            (*this)[2][3] = mat[2][0] * (*this)[0][3] + mat[2][1] * (*this)[1][3] + mat[2][2] * (*this)[2][3] + mat[2][3] * (*this)[3][3];

            (*this)[3][0] = mat[3][0] * (*this)[0][0] + mat[3][1] * (*this)[1][0] + mat[3][2] * (*this)[2][1] + mat[3][3] * (*this)[3][0];
            (*this)[3][1] = mat[3][0] * (*this)[0][1] + mat[3][1] * (*this)[1][1] + mat[3][2] * (*this)[2][2] + mat[3][3] * (*this)[3][1];
            (*this)[3][2] = mat[3][0] * (*this)[0][2] + mat[3][1] * (*this)[1][2] + mat[3][2] * (*this)[2][2] + mat[3][3] * (*this)[3][2];
            (*this)[3][3] = mat[3][0] * (*this)[0][3] + mat[3][1] * (*this)[1][3] + mat[3][2] * (*this)[2][3] + mat[3][3] * (*this)[3][3];

            return *this;
        }

        constexpr vec4<T> operator*(const vec4<T>& vec) const
        {
            mat4x4 m = mat4x4::identity;

            m[0][0] = (*this)[0][0] * vec.x;
            m[0][1] = (*this)[0][1] * vec.x;
            m[0][2] = (*this)[0][2] * vec.x;
            m[0][3] = (*this)[0][3] * vec.x;

            m[1][0] = (*this)[1][0] * vec.y;
            m[1][1] = (*this)[1][1] * vec.y;
            m[1][2] = (*this)[1][2] * vec.y;
            m[1][3] = (*this)[1][3] * vec.y;
                        
            m[2][0] = (*this)[2][0] * vec.z;
            m[2][1] = (*this)[2][1] * vec.z;
            m[2][2] = (*this)[2][2] * vec.z;
            m[2][3] = (*this)[2][3] * vec.z;

            m[3][0] = (*this)[3][0] * vec.w;
            m[3][1] = (*this)[3][1] * vec.w;
            m[3][2] = (*this)[3][2] * vec.w;
            m[3][3] = (*this)[3][3] * vec.w;

            return {
                m[0][0] + m[1][0] + m[2][0] + m[3][0],
                m[0][1] + m[1][1] + m[2][1] + m[3][1],
                m[0][2] + m[1][2] + m[2][2] + m[3][2],
                m[0][3] + m[1][3] + m[2][3] + m[3][3],
            };
        }

        constexpr vec2<T> operator*(const vec2<T>& vec) const
        {
            vec4<T> v = (*this) * vec4<T>(vec.x, vec.y, 0.0f, 1.0f);
            return { v.x, v.y };
        }

        constexpr vec3<T> operator*(const vec3<T>& vec) const
        {
            vec4<T> v = (*this) * vec4<T>(vec.x, vec.y, 0.0f, 1.0f);
            return { v.x, v.y, v.z };
        }

        friend constexpr vec4<T> operator*(const vec4<T>& vec, const mat4x4& mat)
		{
            mat4x4 m = mat4x4::identity;

            m[0][0] = mat[0][0] * vec.x;
            m[0][1] = mat[0][1] * vec.x;
            m[0][2] = mat[0][2] * vec.x;
            m[0][3] = mat[0][3] * vec.x;

            m[1][0] = mat[1][0] * vec.y;
            m[1][1] = mat[1][1] * vec.y;
            m[1][2] = mat[1][2] * vec.y;
            m[1][3] = mat[1][3] * vec.y;
                      
            m[2][0] = mat[2][0] * vec.z;
            m[2][1] = mat[2][1] * vec.z;
            m[2][2] = mat[2][2] * vec.z;
            m[2][3] = mat[2][3] * vec.z;

            m[3][0] = mat[3][0] * vec.w;
            m[3][1] = mat[3][1] * vec.w;
            m[3][2] = mat[3][2] * vec.w;
            m[3][3] = mat[3][3] * vec.w;

            return {
                m[0][0] + m[1][0] + m[2][0] + m[3][0],
                m[0][1] + m[1][1] + m[2][1] + m[3][1],
                m[0][2] + m[1][2] + m[2][2] + m[3][2],
                m[0][3] + m[1][3] + m[2][3] + m[3][3],
            };
		}

        friend constexpr vec2<T> operator*(const vec2<T>& vec, const mat4x4& mat)
		{
            vec4<T> v = vec4(vec.x, vec.y, 0.0f, 1.0f) * mat;
            return { v.x, v.y };
		}

        friend constexpr vec3<T> operator*(const vec3<T>& vec, const mat4x4& mat)
		{
            vec4<T> v = vec4(vec.x, vec.y, 0.0f, 1.0f) * mat;
            return { v.x, v.y, v.z };
		}

        /**
         * @brief Creates an orthographic projection.
         * 
         * @param left   Left bound of the plane.
         * @param right  Right bound of the plane.
         * @param bottom Bottom bound of the plane.
         * @param top    Top bound of the plane.
         * 
         * @return Orthographic projection matrix.
         */
        constexpr static mat4x4 ortho(const T left, const T right, const T bottom, const T top)
        {
            mat4x4 result = mat4x4::identity;

            result[0][0] = (T) 2 / (right - left);
            result[1][1] = (T) 2 / (top - bottom);
            result[2][2] = (T)-1;
            result[3][0] =    -(right + left) / (right - left);
            result[3][1] =    -(top + bottom) / (top - bottom);

            return result;
        }

        /**
         * @brief Creates an orthographic projection.
         * 
         * @param left   Left bound of the plane.
         * @param right  Right bound of the plane.
         * @param bottom Bottom bound of the plane.
         * @param top    Top bound of the plane.
         * @param z_near Depth of near plane.
         * @param z_far  Depth of far plane.
         * 
         * @return Orthographic projection matrix.
         */
        constexpr static mat4x4 ortho(const T left, const T right, const T bottom, const T top, const T z_near, const T z_far)
        {
            assert(z_near < 0.f && z_far > 0.f && "ERROR: near plane must be negative and far plane must be positive");
            mat4x4 result = mat4x4::identity;

            result[0][0] = (T) 2 / (right - left);
            result[1][1] = (T) 2 / (top - bottom);
            result[2][2] = (T)-1 / (z_near - z_far);
            result[3][0] =    -(right + left) / (right - left);
            result[3][1] =    -(top + bottom) / (top - bottom);
            result[3][2] =    -z_near / (z_near - z_far);

            return result;
        }

        /**
         * @brief Creates an orthographic projection.
         * 
         * @param width  Left bound of the plane.
         * @param height Right bound of the plane.
         * 
         * @return Orthographic projection matrix.
         */
        constexpr static mat4x4 ortho(const T width, const T height)
        {
            return ortho(0, width, height, 0, -1.f, 1.f);
        }

        constexpr static mat4x4 perspective(const T fov, const T aspect, const T z_near, const T z_far)
        {
            assert(z_near < 0.f && z_far > 0.f && "ERROR: near plane must be negative and far plane must be positive");

            T tan_half_fov = tan(fov / (T)2);
            mat4x4 result = mat4x4::identity;

            result[0][0] =  (T)1 / (aspect * tan_half_fov);
            result[1][1] =  (T)1 / (tan_half_fov);
            result[2][2] =    -(z_far + z_near) / (z_far - z_near);
            result[2][3] = (T)-1;
            result[3][2] =    -((T)2 * z_far * z_near) / (z_far - z_near);
            
            return result;            
        }

        /**
         * @brief Creates a new translation.
         * 
         * @param x X position.
         * @param y y position.
         * @param z Z position.
         * 
         * @return Translation matrix.
         */
        constexpr static mat4x4 translation(const T x, const T y, const T z)
        {
            mat4x4 result = mat4x4::identity;

            result[3][0] = x;
            result[3][1] = y;
            result[3][2] = z;

            return result;
        }

        /**
         * @brief Translates a matrix to another location on the coordinate plane.
         * 
         * @param mat Input matrix to translate.
         * @param x New X position.
         * @param y New Y position.
         * @param z New Z position.
         * 
         * @return Translated matrix.
         */
        constexpr static mat4x4 translate(const mat4x4& mat, const T x, const T y, const T z)
        {
            mat4x4 result = mat4x4(mat);

            result[3][0] = mat[0][0] * x + mat[1][0] * y + mat[2][0] * z + mat[3][0];
            result[3][1] = mat[0][1] * x + mat[1][1] * y + mat[2][1] * z + mat[3][1];
            result[3][2] = mat[0][2] * x + mat[1][2] * y + mat[2][2] * z + mat[3][2];
            result[3][3] = mat[0][3] * x + mat[1][3] * y + mat[2][3] * z + mat[3][3];

            return result;
        }

        /**
         * @brief Creates a new scaling matrix.
         * 
         * @param x X axis scale.
         * @param y Y axis scale.
         * @param z Z axis scale.
         * 
         * @return Scaling matrix.
         */
        constexpr static mat4x4 scale(const T x, const T y, const T z)
        {
            mat4x4 result = mat4x4::identity;

            result[0][0] = x;
            result[1][1] = y;
            result[2][2] = z;

            return result;
        }

        /**
         * @brief Creates a new rotation matrix around Z axis.
         * 
         * @param rads Angle radians.
         * 
         * @return Rotation matrix.
         */
        constexpr static mat4x4 rotateZ(T rads)
        {
            mat4x4 result = mat4x4::identity;

            result[0][0] = (T)cos(rads);
            result[0][1] = (T)sin(rads);
            result[1][0] = -result[0][1];
            result[1][1] = result[0][0];

            return result;
        }

        /**
         * @brief Takes an input matrix and outputs the inverse.
         * 
         * @param m   Original input matrix.
         * @param im  Inverted output matrix.
         * @param det Determinant.
         */
        constexpr static inline void invert(const mat4x4& m, mat4x4& im, f64& det)
        {
            f64 A2323 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
            f64 A1323 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
            f64 A1223 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
            f64 A0323 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
            f64 A0223 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
            f64 A0123 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
            f64 A2313 = m[1][2] * m[3][3] - m[1][3] * m[3][2];
            f64 A1313 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
            f64 A1213 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
            f64 A2312 = m[1][2] * m[2][3] - m[1][3] * m[2][2];
            f64 A1312 = m[1][1] * m[2][3] - m[1][3] * m[2][1];
            f64 A1212 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
            f64 A0313 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
            f64 A0213 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
            f64 A0312 = m[1][0] * m[2][3] - m[1][3] * m[2][0];
            f64 A0212 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
            f64 A0113 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
            f64 A0112 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

            det = m[0][0] * ( m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223 )
                - m[0][1] * ( m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223 )
                + m[0][2] * ( m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123 )
                - m[0][3] * ( m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123 );
            det = 1 / det;

            im[0][0] = static_cast<T>(det *   ( m[1][1] * A2323 - m[1][2] * A1323 + m[1][3] * A1223 ));
            im[0][1] = static_cast<T>(det * - ( m[0][1] * A2323 - m[0][2] * A1323 + m[0][3] * A1223 ));
            im[0][2] = static_cast<T>(det *   ( m[0][1] * A2313 - m[0][2] * A1313 + m[0][3] * A1213 ));
            im[0][3] = static_cast<T>(det * - ( m[0][1] * A2312 - m[0][2] * A1312 + m[0][3] * A1212 ));
            im[1][0] = static_cast<T>(det * - ( m[1][0] * A2323 - m[1][2] * A0323 + m[1][3] * A0223 ));
            im[1][1] = static_cast<T>(det *   ( m[0][0] * A2323 - m[0][2] * A0323 + m[0][3] * A0223 ));
            im[1][2] = static_cast<T>(det * - ( m[0][0] * A2313 - m[0][2] * A0313 + m[0][3] * A0213 ));
            im[1][3] = static_cast<T>(det *   ( m[0][0] * A2312 - m[0][2] * A0312 + m[0][3] * A0212 ));
            im[2][0] = static_cast<T>(det *   ( m[1][0] * A1323 - m[1][1] * A0323 + m[1][3] * A0123 ));
            im[2][1] = static_cast<T>(det * - ( m[0][0] * A1323 - m[0][1] * A0323 + m[0][3] * A0123 ));
            im[2][2] = static_cast<T>(det *   ( m[0][0] * A1313 - m[0][1] * A0313 + m[0][3] * A0113 ));
            im[2][3] = static_cast<T>(det * - ( m[0][0] * A1312 - m[0][1] * A0312 + m[0][3] * A0112 ));
            im[3][0] = static_cast<T>(det * - ( m[1][0] * A1223 - m[1][1] * A0223 + m[1][2] * A0123 ));
            im[3][1] = static_cast<T>(det *   ( m[0][0] * A1223 - m[0][1] * A0223 + m[0][2] * A0123 ));
            im[3][2] = static_cast<T>(det * - ( m[0][0] * A1213 - m[0][1] * A0213 + m[0][2] * A0113 ));
            im[3][3] = static_cast<T>(det *   ( m[0][0] * A1212 - m[0][1] * A0212 + m[0][2] * A0112 ));
        }

        friend std::ostream& operator<<(std::ostream& os, const mat4x4& mat)
        {
            for (u32 y = 0; y < 4; y++)
            {
                for (u32 x = 0; x < 4; x++)
                {
                    os << mat[y][x] << ",\t";
                }
                os << "\n";
            }
            return os << std::endl;
        }
    };

    template<class T>
    inline const mat4x4<T> mat4x4<T>::zero = mat4x4<T>({{
        { (T)0, (T)0, (T)0, (T)0 },
        { (T)0, (T)0, (T)0, (T)0 },
        { (T)0, (T)0, (T)0, (T)0 },
        { (T)0, (T)0, (T)0, (T)0 }
    }});

    template<class T>
    inline const mat4x4<T> mat4x4<T>::one = mat4x4<T>({{
        { (T)1, (T)1, (T)1, (T)1 },
        { (T)1, (T)1, (T)1, (T)1 },
        { (T)1, (T)1, (T)1, (T)1 },
        { (T)1, (T)1, (T)1, (T)1 }
    }});

    template<class T>
    inline const mat4x4<T> mat4x4<T>::identity = mat4x4<T>({{
        { (T)1, (T)0, (T)0, (T)0 },
        { (T)0, (T)1, (T)0, (T)0 },
        { (T)0, (T)0, (T)1, (T)0 },
        { (T)0, (T)0, (T)0, (T)1 }
    }});
}
