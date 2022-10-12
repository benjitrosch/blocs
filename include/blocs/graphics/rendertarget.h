#pragma once

namespace blocs
{
    /** @brief Texture that will be written to after flush. */
    struct RenderTarget
    {
    private:
        bool m_set;

    public:
        i32 w;
        i32 h;

        u32 fbo;
        u32 rto;

        /**
         * @brief Generates a new render texture.
         *
         * @param w Width.
         * @param h Height.
         */
        RenderTarget(i32 w, i32 h) : w(w), h(h), m_set(false)
        {
            fbo = renderer::frame_buffer::generate(w, h);
            rto = renderer::frame_buffer::generate_render_texture(fbo, w, h);
        }

        RenderTarget()                               = delete;
        RenderTarget(const RenderTarget&)            = delete;
        RenderTarget& operator=(const RenderTarget&) = delete;

        ~RenderTarget()
        {
            renderer::frame_buffer::remove(fbo);
            renderer::textures::remove(rto);
        }

        /**
         * @brief Resizes render texture.
         *
         * @param w New width.
         * @param h New height.
         */
        void resize(const i32 w, const i32 h)
        {
            renderer::frame_buffer::resize_render_texture(rto, w, h);
        }

        /**
         * @brief Sets the RenderTarget as the target texture
         * to be written to after drawing.
         */
        void set()
        {
            renderer::frame_buffer::bind(fbo);

            renderer::clear(color::clear);
            renderer::viewport(0, 0, w, h);

            m_set = true;
        }

        /**
         * @brief Sets the RenderTarget as the target texture to be written to
         * after drawing.
         *
         * @param w New width.
         * @param h New height.
         */
        void set(const i32 w, const i32 h)
        {
            resize(w, h);
            set();
        }

        /**
         * @brief Unsets the RenderTarget allowing the texture data
         * to be flushed to the window's viewport.
         */
        void flush()
        {
            if (!m_set)
            {
                LOG_ERR("cannot flush rendertarget without setting");
                return;
            }

            renderer::frame_buffer::unbind();

            m_set = false;
        }
    };
}
