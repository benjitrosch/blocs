#pragma once

#include <unordered_map>

namespace blocs
{
    /**
     * @brief Basic data container for affecting
     * shader uniform values.
     */
    struct Shader
    {
    private:
        std::unordered_map<str, i32> m_locations{};

    public:
        u32 id;

        /**
         * @brief Creates an uninitialized shader object
         * (will not work without setting valid id).
         */
        Shader() : id(0) {}

        /**
         * @brief Creates a new shader object from an
         * existing renderer shader id.
         *
         * @param id Shader ID.
         */
        Shader(u32 id) : id(id) {}

        /** @brief Binds shader to renderer context. */
        void bind() { renderer::shaders::bind(id); }

        /**
         * @brief Apply a value to a shader uniform.
         *
         * @tparam T Value type.
         * @param name  Uniform name.
         * @param value Uniform value.
         */
        template<typename T>
        void apply(const str& name, T value)
        {
            i32 location = -1;
            if (m_locations.find(name) != m_locations.end())
            {
                location = m_locations[name];
            }
            else
            {
                location = renderer::shaders::get_uniform_location(id, name);
                m_locations[name] = location;
            }

            renderer::shaders::set_uniform(location, value);
        }

        /**
         * @brief Apply a value to a shader array uniform.
         *
         * @tparam T     Value type.
         * @param name   Uniform name.
         * @param count  Number of elements.
         * @param values Uniform values.
         */
        template<typename T>
        void apply(const str& name, size count, T values)
        {
            i32 location = -1;
            if (m_locations.find(name) != m_locations.end())
            {
                location = m_locations[name];
            }
            else
            {
                location = renderer::shaders::get_uniform_location(id, name);
                m_locations[name] = location;
            }

            renderer::shaders::set_uniform(location, count, values);
        }
    };
}
