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
        std::unordered_map<str, i32> m_locations {};

    public:
        u32 id;

        Shader() : id(0) {}
        Shader(u32 id) : id(id) {}

        void bind()
        {
            renderer::shaders::bind(id);
        }

        template<class T>
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

        template<class T>
        void apply(const str& name, size count, T value)
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
            
            renderer::shaders::set_uniform(location, count, value);
        }
    };
}
