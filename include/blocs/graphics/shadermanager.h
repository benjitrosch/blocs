#pragma once

#include <unordered_map>

#include <blocs/graphics/shader.h>

namespace blocs
{
    /** @brief Manages shader storage and lookup. */
    class ShaderManager
    {
    private:
        std::unordered_map<str, Shader> m_shaders;

    public:
        /**
         * @brief Adds an existing shader.
         *
         * @param name   Shader name.
         * @param shader Shader ID.
         * @return Shader.
         */
        Shader& add(const str& name, u32 shader)
        {
            if (m_shaders.find(name) != m_shaders.end())
            {
                LOG_ERR("shader already in library");
                return m_shaders[name];
            }

            return m_shaders.insert({name, {shader}}).first->second;
        }

        /**
         * @brief Creates and adds a fragment shader from a file.
         *
         * @param name     Shader name.
         * @param filepath Path to fragment shader file.
         * @return Shader.
         */
        Shader& add(const str& name, const str& filepath)
        {
            if (m_shaders.find(name) != m_shaders.end())
            {
                LOG_ERR("shader already in library");
                return m_shaders[name];
            }

            str frag   = renderer::shaders::parse(filepath);
            u32 shader = renderer::shaders::generate(
                renderer::shaders::DEFAULT_2D_VERTEX_SHADER, frag
            );

            return m_shaders.insert({name, {shader}}).first->second;
        }

        /**
         * @brief Creates and adds a shader from
         * vertex and fragment shader files.
         *
         * @param name          Shader name.
         * @param vert_filepath Path to vertex shader file.
         * @param frag_filepath Path to fragment shader file.
         * @return Shader.
         */
        Shader& add(
            const str& name, const str& vert_filepath, const str& frag_filepath
        )
        {
            if (m_shaders.find(name) != m_shaders.end())
            {
                LOG_ERR("shader already in library");
                return m_shaders[name];
            }

            str vert   = renderer::shaders::parse(vert_filepath);
            str frag   = renderer::shaders::parse(frag_filepath);
            u32 shader = renderer::shaders::generate(vert, frag);

            return m_shaders.insert({name, {shader}}).first->second;
        }

        /**
         * @brief Retrieves a shader.
         *
         * @param name Shader name.
         * @return Shader.
         */
        Shader& get(const str& name)
        {
            assert(
                m_shaders.find(name) != m_shaders.end() &&
                "ERROR: no shader found for that name"
            );
            return m_shaders[name];
        }

        /**
         * @brief Check if a shader exists.
         *
         * @param name Shader name.
         * @return Whether shader name is valid map key.
         */
        bool has(const str& name) const
        {
            return m_shaders.find(name) != m_shaders.end();
        }
    };
}
