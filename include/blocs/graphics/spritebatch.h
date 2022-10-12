#pragma once

#ifdef DEBUG
#include <blocs/time.h>
#endif

#include <blocs/graphics/texture.h>
#include <blocs/graphics/spritefont.h>

namespace blocs
{
    enum class SpriteFlip : uchar
    {
        NONE       = 0,
        HORIZONTAL = 1,
        VERTICAL   = 2,
        BOTH       = HORIZONTAL | VERTICAL,
    };

    namespace impl
    {
        /**
         * @brief Abstract base class for sprite renderer classes
         * with types for sprites, vertex layouts, quads and other
         * shapes.
         */
        class SpriteRenderer
        {
        protected:
            /**
             * @brief Data stored in a single vertex
             * of a sprite.
             */
            struct Vertex
            {
                vec3f pos;
                vec2f tex;
                vec4f col, fil;
            };

            /**
             * @brief Sprite metadata including
             * position, texture, source, and
             * color etc.
             */
            struct Sprite
            {
                rectf      src, dst;
                color      col, fil;
                vec2f      org;
                f32        rot, z;
                u32        tex;
                f32        tex_w, tex_h;
                SpriteFlip flip;
            };

            /**
             * @brief Quad metadata with positions
             * and colors for each of the four corners.
             */
            struct ShapeQuad
            {
                f32   x0, y0, x1, y1, x2, y2, x3, y3;
                color c0, c1, c2, c3;
            };

            /**
             * @brief Triangle metadata with positions
             * and colors for each of the three points.
             */
            struct ShapeTri
            {
                f32   x0, y0, x1, y1, x2, y2;
                color c0, c1, c2;
            };

            // Number of vertices per triangle
            static constexpr size VERTEX_PER_TRI  = 3;
            // Number of vertices per quad
            static constexpr size VERTEX_PER_QUAD = 4;
            // Number of non-repeatex vertex indices per triangle
            static constexpr size INDEX_PER_TRI   = 3;
            // Number of non-repeated vertex indices per quad
            static constexpr size INDEX_PER_QUAD  = 6;
            // Maximum number of quads that can be batched at a time
            // (equal to maximum value of an unsigned 16 bit integer)
            static constexpr size MAX_QUADS       = 65535;
            // Maximum number of vertices that can be stored
            static constexpr size MAX_VERTEX      = MAX_QUADS * VERTEX_PER_QUAD;
            // Maximum number of indices that can be stored
            static constexpr size MAX_INDEX       = MAX_QUADS * INDEX_PER_QUAD;

            SpriteRenderer() {}
            ~SpriteRenderer() {}
        };
    }

    /** @brief Renders 2D sprites and batches draw calls. */
    class SpriteBatch : impl::SpriteRenderer
    {
    private:
        mat4x4f m_transform{};
        Shader  m_defaultShader{};
        Shader  m_activeShader{};
        u32     m_activeTexture;

        bool m_began = false;

        u32 m_vao;
        u32 m_vbo;
        u32 m_ibo;

        size m_numVertex = 0;
        size m_numIndex  = 0;

        Vertex* m_vertices = nullptr;
        Vertex* m_buffer   = nullptr;
        u32*    m_indices  = nullptr;

        rectf m_scissor{};
        bool  m_hasScissor = false;

        bool m_shouldSetTexture = false;

#ifdef DEBUG
        Stopwatch m_timer;
#endif

        /**
         * @brief Transforms source rect coordinates into a one-by-one
         * space through dividing position by texture size.
         *
         * @param src Source rectangle.
         * @param w   Texture width.
         * @param h   Texture height.
         * @return    Normalized rectangle.
         */
        constexpr inline rectf normalize_quad(
            const rectf& src, const f32 w, const f32 h
        )
        {
            return rectf(src.x / w, src.y / h, src.w / w, src.h / h);
        }

        /**
         * @brief Writes three vertices of a triangle into the batcher buffer.
         *
         * @param target Vertex buffer pointer.
         * @param x0     X position of point one.
         * @param y0     Y position of point one.
         * @param x1     X position of point two.
         * @param y1     Y position of point two.
         * @param x2     X position of point three.
         * @param y2     Y position of point three.
         * @param c0     Color near point one.
         * @param c1     Color near point two.
         * @param c2     Color near point three.
         */
        constexpr inline Vertex* push_tri(
            Vertex* target, f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2,
            const color& c0, const color& c1, const color& c2,
            const color& f0 = color::clear, const color& f1 = color::clear,
            const color& f2 = color::clear
        )
        {
            target->pos = {x0, y0, 0};
            target->tex = {};
            target->col = c0.to_vec4();
            target->fil = f0.to_vec4();
            target++;

            target->pos = {x1, y1, 0};
            target->tex = {};
            target->col = c1.to_vec4();
            target->fil = f1.to_vec4();
            target++;

            target->pos = {x2, y2, 0};
            target->tex = {};
            target->col = c2.to_vec4();
            target->fil = f2.to_vec4();
            target++;

            return target;
        }

        /**
         * @brief Writes four vertices of a quad into the batcher buffer.
         *
         * @param target Vertex buffer pointer.
         * @param x0     X position of point one.
         * @param y0     Y position of point one.
         * @param x1     X position of point two.
         * @param y1     Y position of point two.
         * @param x2     X position of point three.
         * @param y2     Y position of point three.
         * @param x3     X position of point four.
         * @param y3     Y position of point four.
         * @param c0     Color near point one.
         * @param c1     Color near point two.
         * @param c2     Color near point three.
         * @param c3     Color near point four.
         */
        constexpr inline Vertex* push_quad(
            Vertex* target, f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2,
            f32 x3, f32 y3, const color& c0, const color& c1, const color& c2,
            const color& c3, const color& f0 = color::clear,
            const color& f1 = color::clear, const color& f2 = color::clear,
            const color& f3 = color::clear
        )
        {
            target->pos = {x0, y0, 0};
            target->tex = {};
            target->col = c0.to_vec4();
            target->fil = f0.to_vec4();
            target++;

            target->pos = {x1, y1, 0};
            target->tex = {};
            target->col = c1.to_vec4();
            target->fil = f1.to_vec4();
            target++;

            target->pos = {x2, y2, 0};
            target->tex = {};
            target->col = c2.to_vec4();
            target->fil = f2.to_vec4();
            target++;

            target->pos = {x3, y3, 0};
            target->tex = {};
            target->col = c3.to_vec4();
            target->fil = f3.to_vec4();
            target++;

            return target;
        }

        /**
         * @brief Writes four vertices of a quad into the batcher buffer.
         *
         * @param target Vertex buffer pointer.
         * @param src    Source rectangle from texture.
         * @param dst    Destination rectangle.
         * @param z      Z depth value.
         * @param col    Tint color.
         * @param fill   Fill color.
         * @param flip   Flip direction (default none).
         */
        constexpr inline Vertex* push_quad(
            Vertex* target, const rectf& src, const rectf& dst, const f32 z,
            const color& col, const color& fill, const SpriteFlip flip
        )
        {
            vec2f bl   = {src.x, src.y};
            vec2f br   = {src.x + src.w, src.y};
            vec2f tr   = {src.x + src.w, src.y + src.h};
            vec2f tl   = {src.x, src.y + src.h};
            f32   temp = 0;

            if (flip == SpriteFlip::VERTICAL || flip == SpriteFlip::BOTH)
            {
                temp = br.y;
                br.y = tl.y;
                tl.y = temp;

                temp = bl.y;
                bl.y = tr.y;
                tr.y = temp;
            }

            if (flip == SpriteFlip::HORIZONTAL || flip == SpriteFlip::BOTH)
            {
                temp = br.x;
                br.x = tl.x;
                tl.x = temp;

                temp = bl.x;
                bl.x = tr.x;
                tr.x = temp;
            }

            // Bottom Left
            target->pos = {dst.x, dst.y, z};
            target->tex = bl;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            // Bottom Right
            target->pos = {dst.x + dst.w, dst.y, z};
            target->tex = br;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            // Top Right
            target->pos = {dst.x + dst.w, dst.y + dst.h, z};
            target->tex = tr;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            // Top Left
            target->pos = {dst.x, dst.y + dst.h, z};
            target->tex = tl;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            return target;
        }

        /**
         * @brief Writes four vertices of a quad into the batcher buffer.
         *
         * @param target Vertex buffer pointer.
         * @param src    Source rectangle from texture.
         * @param dst    Destination rectangle.
         * @param z      Z depth value.
         * @param org    Origin position, point of rotation.
         * @param sin    Sin of rotation value.
         * @param cos    Cos of rotation value.
         * @param col    Tint color.
         * @param fill   Fill color.
         * @param flip   Flip direction (default none).
         */
        constexpr inline Vertex* push_quad(
            Vertex* target, const rectf& src, const rectf& dst, const f32 z,
            const vec2f org, const f32 sin, const f32 cos, const color& col,
            const color& fill, const SpriteFlip flip
        )
        {
            vec2f bl   = {src.x, src.y};
            vec2f br   = {src.x + src.w, src.y};
            vec2f tr   = {src.x + src.w, src.y + src.h};
            vec2f tl   = {src.x, src.y + src.h};
            f32   temp = 0;

            if (flip == SpriteFlip::VERTICAL || flip == SpriteFlip::BOTH)
            {
                temp = br.y;
                br.y = tl.y;
                tl.y = temp;

                temp = bl.y;
                bl.y = tr.y;
                tr.y = temp;
            }

            if (flip == SpriteFlip::HORIZONTAL || flip == SpriteFlip::BOTH)
            {
                temp = br.x;
                br.x = tl.x;
                tl.x = temp;

                temp = bl.x;
                bl.x = tr.x;
                tr.x = temp;
            }

            f32 x  = dst.x + org.x;
            f32 y  = dst.y + org.y;
            f32 w  = dst.w;
            f32 h  = dst.h;
            f32 dx = -org.x;
            f32 dy = -org.y;

            // Bottom Left
            target->pos = {x + dx * cos - dy * sin, y + dx * sin + dy * cos, z};
            target->tex = bl;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            // Bottom Right
            target->pos = {
                x + (dx + w) * cos - dy * sin, y + (dx + w) * sin + dy * cos,
                z};
            target->tex = br;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            // Top Right
            target->pos = {
                x + (dx + w) * cos - (dy + h) * sin,
                y + (dx + w) * sin + (dy + h) * cos, z};
            target->tex = tr;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            // Top Left
            target->pos = {
                x + dx * cos - (dy + h) * sin, y + dx * sin + (dy + h) * cos,
                z};
            target->tex = tl;
            target->col = col.to_vec4();
            target->fil = fill.to_vec4();
            target++;

            return target;
        }

        /**
         * @brief Binds a texture by id to slot zero.
         *
         * @param tex Texture ID.
         */
        void set_texture(u32 tex)
        {
            m_shouldSetTexture = false;
            m_activeTexture    = tex;
            renderer::textures::bind(m_activeTexture, 0);
        }

        /** @brief Resets the batcher state. */
        void reset()
        {
            m_buffer    = &m_vertices[0];
            m_numVertex = 0;
            m_numIndex  = 0;
        }

        /**
         * @brief Pushes all buffers to renderer context
         * and draws to the viewport (then resets everything).
         */
        void flush()
        {
            if (m_hasScissor) renderer::scissor(m_scissor);

            renderer::vertex_array::bind(m_vao);
            renderer::index_buffer::bind(m_ibo);
            renderer::vertex_buffer::subdata(
                m_numVertex * sizeof(Vertex), m_vertices
            );
            renderer::index_buffer::subdata(m_numIndex, m_indices);

            renderer::draw(m_numIndex);

            reset();

#ifdef DEBUG
            num_draw_calls++;
#endif
        }

        /**
         * @brief Constructs quad vertices from sprite metadata
         * and inserts into vertex buffer and index buffer.
         *
         * @param sprite Sprite metadata.
         */
        void draw_sprite(Sprite sprite)
        {
            if (m_numVertex >= MAX_QUADS * VERTEX_PER_QUAD) flush();

            if (m_activeTexture != sprite.tex)
            {
                flush();
                m_shouldSetTexture = true;
            }

            if (m_shouldSetTexture) set_texture(sprite.tex);

            m_indices[m_numIndex + 0] = 0 + m_numVertex;  //      2
            m_indices[m_numIndex + 1] = 1 + m_numVertex;  //    / |
            m_indices[m_numIndex + 2] = 2 + m_numVertex;  //  0---1
            m_indices[m_numIndex + 3] = 0 + m_numVertex;  //  3---2
            m_indices[m_numIndex + 4] = 2 + m_numVertex;  //  | /
            m_indices[m_numIndex + 5] = 3 + m_numVertex;  //  0
            m_numIndex += INDEX_PER_QUAD;

            m_buffer =
                sprite.rot == 0
                    ? push_quad(
                          m_buffer,
                          normalize_quad(
                              sprite.src, sprite.tex_w, sprite.tex_h
                          ),
                          sprite.dst, sprite.z, sprite.col, sprite.fil,
                          sprite.flip
                      )
                    : push_quad(
                          m_buffer,
                          normalize_quad(
                              sprite.src, sprite.tex_w, sprite.tex_h
                          ),
                          sprite.dst, sprite.z, sprite.org, sin(sprite.rot),
                          cos(sprite.rot), sprite.col, sprite.fil, sprite.flip
                      );
            m_numVertex += VERTEX_PER_QUAD;
        }

        /**
         * @brief Constructs quad vertices and inserts
         * into vertex buffer and index buffer.
         *
         * @param shape Quad metadata.
         */
        void draw_shape(ShapeQuad shape)
        {
            if (m_numVertex >= MAX_QUADS * VERTEX_PER_QUAD) flush();

            m_indices[m_numIndex + 0] = 0 + m_numVertex;  //      2
            m_indices[m_numIndex + 1] = 1 + m_numVertex;  //    / |
            m_indices[m_numIndex + 2] = 2 + m_numVertex;  //  0---1
            m_indices[m_numIndex + 3] = 0 + m_numVertex;  //  3---2
            m_indices[m_numIndex + 4] = 2 + m_numVertex;  //  | /
            m_indices[m_numIndex + 5] = 3 + m_numVertex;  //  0
            m_numIndex += INDEX_PER_QUAD;

            m_buffer = push_quad(
                m_buffer, shape.x0, shape.y0, shape.x1, shape.y1, shape.x2,
                shape.y2, shape.x3, shape.y3, color::clear, color::clear,
                color::clear, color::clear, shape.c0, shape.c1, shape.c2,
                shape.c3
            );
            m_numVertex += VERTEX_PER_QUAD;
        }

        /**
         * @brief Constructs triangle vertices and inserts
         * into vertex buffer and index buffer.
         *
         * @param shape Triangle metadta.
         */
        void draw_shape(ShapeTri shape)
        {
            if (m_numVertex >= MAX_QUADS * VERTEX_PER_QUAD) flush();

            m_indices[m_numIndex + 0] = 0 + m_numVertex;  //      2
            m_indices[m_numIndex + 1] = 1 + m_numVertex;  //    / |
            m_indices[m_numIndex + 2] = 2 + m_numVertex;  //  0---1
            m_numIndex += INDEX_PER_TRI;

            m_buffer = push_tri(
                m_buffer, shape.x0, shape.y0, shape.x1, shape.y1, shape.x2,
                shape.y2, color::clear, color::clear, color::clear, shape.c0,
                shape.c1, shape.c2
            );
            m_numVertex += VERTEX_PER_TRI;
        }

        /**
         * @brief Creates a quad object based on bounds position.
         *
         * @param start Top left position.
         * @param end   Bottom right position.
         * @param t     Thickness.
         * @param col0  Color near top left.
         * @param col1  Color near bottom right.
         * @return Quad metadata.
         */
        ShapeQuad get_quad(
            const vec2f& start, const vec2f& end, f32 t, const color& col0,
            const color& col1
        )
        {
            vec2f perp;

            if (start.x == end.x)
                perp = vec2f::right;
            else if (start.y == end.y)
                perp = vec2f::up;
            else
            {
                vec2f normal = (end - start).normalized();
                perp         = vec2f(normal.y, -normal.x);
            }

            vec2f pos0 = start + perp * t * 0.5f;
            vec2f pos1 = end + perp * t * 0.5f;
            vec2f pos2 = end - perp * t * 0.5f;
            vec2f pos3 = start - perp * t * 0.5f;

            return {
                pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y,
                pos3.x, pos3.y, col0,   col1,   col1,   col0,
            };
        }

        /**
         * @brief Connects the ends of two quads.
         *
         * @param a Quad one.
         * @param b Quad two.
         * @return Connected quad metadata.
         */
        ShapeQuad connect_quads(ShapeQuad& a, ShapeQuad& b)
        {
            b.x0 = a.x1;
            b.y0 = a.y1;
            b.x3 = a.x2;
            b.y3 = a.y2;
            return b;
        }

    public:
#ifdef DEBUG
        size num_draw_calls;
        f64  time_to_draw;
#endif

        SpriteBatch()
        {
            m_vertices = new Vertex[MAX_VERTEX];
            m_indices  = new u32[MAX_INDEX];

            m_vao = renderer::vertex_array::generate();
            m_vbo = renderer::vertex_buffer::generate(
                MAX_QUADS * VERTEX_PER_QUAD * sizeof(Vertex)
            );
            m_ibo = renderer::index_buffer::generate(MAX_INDEX);

            renderer::buffer_layout layout = {
                {renderer::AttributeType::VEC3, "a_pos"},
                {renderer::AttributeType::VEC2, "a_tex"},
                {renderer::AttributeType::VEC4, "a_color"},
                {renderer::AttributeType::VEC4, "a_fill"}};
            renderer::vertex_array::add_buffer(m_vao, m_vbo, layout);

            m_defaultShader = {renderer::shaders::generate(
                renderer::shaders::DEFAULT_2D_VERTEX_SHADER,
                renderer::shaders::DEFAULT_2D_FRAGMENT_SHADER
            )};
        }

        SpriteBatch(const SpriteBatch&)            = delete;
        SpriteBatch& operator=(const SpriteBatch&) = delete;

        ~SpriteBatch()
        {
            renderer::vertex_buffer::remove(m_vbo);
            renderer::index_buffer::remove(m_ibo);
            renderer::vertex_array::remove(m_vao);

            delete[] m_vertices;
            delete[] m_buffer;
        }

        /**
         * @brief Forces the SpriteBatcher to set the active texture to the
         * next texture encountered rather than using the cached id (for use
         * when binding a texture id outside the current SpriteBatch).
         */
        void force_set_texture() { m_shouldSetTexture = true; }

        /**
         * @brief Begins a new sprite batch with the specified render state.
         *
         * @param transform Transformation matrix applied to quads.
         * @param shader    Shader effect applied to batch.
         * @param blend     Blend mode applied to quad.
         * @param cull      Culling mode (aka facing direction) of quad.
         * @param scissor   Masks sprite outside scissor bounds.
         */
        void begin(
            const mat4x4f& transform, Shader shader,
            renderer::BlendMode blend   = renderer::BlendMode::NonPremultiplied,
            renderer::Cull      cull    = renderer::Cull::NONE,
            rectf               scissor = rectf::one * -1
        )
        {
            reset();

            m_transform    = transform;
            m_activeShader = shader;

#ifndef __EMSCRIPTEN__
            renderer::cull(cull);

            renderer::enable_blend();
            renderer::blend(blend.color_eq, blend.alpha_eq);
            renderer::blend(
                blend.color_src, blend.color_dst, blend.alpha_src,
                blend.alpha_dst
            );
            renderer::mask(blend.mask);
#endif

            renderer::vertex_array::bind(m_vao);
            renderer::vertex_buffer::bind(m_vbo);

            shader.bind();
            shader.apply("u_world", transform);
            shader.apply(
                "u_flip", renderer::frame_buffer::is_bound() ? -1.f : 1.f
            );
            shader.apply("u_texture", 0);

            if (scissor.w > -1 && scissor.h > -1)
            {
                m_scissor    = scissor;
                m_hasScissor = true;
            }

#ifdef DEBUG
            num_draw_calls = 0;
            time_to_draw   = 0;
            m_timer.reset();
#endif
            m_began = true;
        }

        /**
         * @brief Begins a new sprite batch with the specified render state.
         *
         * @param transform Transformation matrix applied to quads.
         */
        void begin(mat4x4f transform = mat4x4f::ortho(1, 1))
        {
            begin(transform, m_defaultShader);
        }

        /**
         * @brief Begins a new sprite batch with the specified render state.
         *
         * @param transform Transformation matrix applied to quads.
         * @param blend     Blend mode applied to quad.
         * @param cull      Culling mode (aka facing direction) of quad.
         * @param scissor   Masks sprite outside scissor bounds.
         */
        void begin(
            const mat4x4f& transform, renderer::BlendMode blend,
            renderer::Cull cull    = renderer::Cull::NONE,
            const rectf&   scissor = rectf::one * -1
        )
        {
            begin(transform, m_defaultShader, blend);
        }

        /**
         * @brief Begins a new sprite batch with the specified render state.
         *
         * @param transform Transformation matrix applied to quads.
         * @param cull      Culling mode (aka facing direction) of quad.
         * @param scissor   Masks sprite outside scissor bounds.
         */
        void begin(
            const mat4x4f& transform, renderer::Cull cull,
            const rectf& scissor = rectf::one * -1
        )
        {
            begin(
                transform, m_defaultShader,
                renderer::BlendMode::NonPremultiplied, cull
            );
        }

        /**
         * @brief Begins a new sprite batch with the specified render state.
         *
         * @param transform Transformation matrix applied to quads.
         * @param scissor   Masks sprite outside scissor bounds.
         */
        void begin(const mat4x4f& transform, const rectf& scissor)
        {
            begin(
                transform, m_defaultShader,
                renderer::BlendMode::NonPremultiplied, renderer::Cull::NONE,
                scissor
            );
        }

        /** @brief Flushes batched quads to the screen. */
        void end()
        {
            if (!m_began)
            {
                LOG_ERR("cannot call end() before begin()");
                return;
            }

            if (m_numVertex > 0) flush();

            if (m_hasScissor)
            {
                m_hasScissor = false;
                renderer::disable_scissor();
            }

#ifdef DEBUG
            m_timer.stop();
            time_to_draw = m_timer.get_time_elapsed_ms();
#endif
            m_began = false;
        }

        /**
         * @brief Draw a sprite as part of the current batch.
         *
         * @param tex  Texture to draw.
         * @param src  Source region to render from the texture.
         * @param dst  Region on the screen to draw onto.
         * @param org  Center of rotation.
         * @param rot  Rotation of sprite.
         * @param z    Z-Depth of sprite.
         * @param col  Color of sprite.
         * @param flip Flip-mode of sprite.
         */
        void draw(
            Texture* tex, const rectf& src, const rectf& dst,
            const vec2f& org = vec2f::zero, f32 rot = 0, f32 z = 0,
            const color& col = color::white, const color& fill = color::clear,
            const SpriteFlip flip = SpriteFlip::NONE
        )
        {
            if (!m_began)
            {
                LOG_ERR("cannot draw without spritebatch begin");
                return;
            }

            if (!tex)
            {
                LOG_ERR("cannot draw null texture pointer");
                return;
            }

            draw_sprite(
                {src, dst, col, fill, org, rot, z, tex->id, (f32)tex->w,
                 (f32)tex->h, flip}
            );
        }

        void draw(Texture* tex, vec2f src, vec2f dst, color col = color::white)
        {
            draw(
                tex, {src.x, src.y, (f32)tex->w, (f32)tex->h},
                {dst.x, dst.y, (f32)tex->w, (f32)tex->h}, vec2f::zero, 0, 0, col
            );
        }

        void draw(Texture* tex, vec2f dst, color col = color::white)
        {
            draw(
                tex, {0, 0, (f32)tex->w, (f32)tex->h},
                {dst.x, dst.y, (f32)tex->w, (f32)tex->h}, vec2f::zero, 0, 0, col
            );
        }

        /**
         * @brief Draw a sprite as part of the current batch.
         *
         * @param tex  Texture id to draw.
         * @param src  Source region to render from the texture.
         * @param dst  Region on the screen to draw onto.
         * @param org  Center of rotation.
         * @param rot  Rotation of the sprite.
         * @param z    Z-Depth of sprite.
         * @param col  Color of sprite.
         * @param flip Flip-mode of sprite.
         */
        void draw(
            u32 tex, const rectf& src, const rectf& dst,
            const vec2f& org = vec2f::zero, f32 rot = 0, f32 z = 0,
            const color& col = color::white, const color& fill = color::clear,
            const SpriteFlip flip = SpriteFlip::NONE
        )
        {
            if (!m_began)
            {
                LOG_ERR("cannot draw without spritebatch begin");
                return;
            }

            draw_sprite(
                {src, dst, col, fill, org, rot, z, tex, src.w, src.h, flip}
            );
        }

        /**
         * @brief Draw multiple glyph quads for a string as part of the current
         * batch.
         *
         * @param font  SpriteFont to use for rendering.
         * @param str   Text to render.
         * @param x     X position of first glyph.
         * @param y     Y position of first glyph.
         * @param scale Scale.
         * @param col   Color.
         * @param rot   Rotation.
         */
        void text(
            SpriteFont* font, cstr str, f32 x, f32 y,
            const vec2f scale = vec2f::one, const color& col = color::white,
            f32 rot = 0.0f
        )
        {
            if (!m_began)
            {
                LOG_ERR("cannot draw text without spritebatch begin");
                return;
            }

            u32   prev   = 0;
            vec2f offset = {0, font->ascent + font->descent};
            while (*str)
            {
                const auto ch = font->get_character(*str);
                if (ch.node != nullptr)
                {
                    vec2f at = (offset + ch.offset) * scale;
                    if (prev > 0) at.x += font->get_kerning(*str, prev);
                    auto     node = ch.node;
                    Texture* tex  = node->texture;
                    rectf    src  = {
                        (f32)node->x, (f32)node->y, (f32)node->w, (f32)node->h};
                    rectf dst = {
                        x + at.x, y + at.y, (f32)node->w * scale.x,
                        (f32)node->h * scale.y};
                    draw(tex, src, dst, vec2f::zero, rot, 0, col);
                }
                offset.x += ch.advance;
                prev = *str;
                ++str;
            }
        }

        /**
         * @brief Draw multiple glyph quads for a string as part of the current
         * batch.
         *
         * @param font  SpriteFont to use for rendering.
         * @param str   Text to render.
         * @param x     X position of first glyph.
         * @param y     Y position of first glyph.
         * @param scale Scale.
         * @param col   Color.
         * @param rot   Rotation.
         */
        void text(
            SpriteFont* font, const str& str, f32 x, f32 y,
            const vec2f scale = vec2f::one, const color& col = color::white,
            f32 rot = 0.0f
        )
        {
            text(font, str.c_str(), x, y, scale, col, rot);
        }

        /**
         * @brief Draws a line between two points.
         *
         * @param start Position of first point.
         * @param end   Position of second point.
         * @param t     Line thickness.
         * @param col0  Start color.
         * @param col1  End color.
         */
        void line(
            const vec2f& start, const vec2f& end, f32 t, const color& col0,
            const color& col1
        )
        {
            draw_shape(get_quad(start, end, t, col0, col1));
        }

        /**
         * @brief Draws a line between two points.
         *
         * @param start Position of first point.
         * @param end   Position of second point.
         * @param t     Line thickness.
         * @param col   Line color.
         */
        void line(const vec2f& start, const vec2f& end, f32 t, const color& col)
        {
            line(start, end, t, col, col);
        }

        /**
         * @brief Draws a cubic bezier line.
         *
         * @param start Position of start point.
         * @param b     Position of control point.
         * @param end   Position of end point.
         * @param t     Line thickness.
         * @param steps Line resolution.
         * @param col0  Start color.
         * @param col1  End color.
         */
        void bezier(
            const vec2f& start, const vec2f& b, const vec2f& end, f32 t,
            i32 steps, const color& col0, const color& col1
        )
        {
            f32 add = 1.0f / steps;

            vec2f     prev     = start;
            ShapeQuad prev_seg = {};

            for (i32 i = 1; i < steps; i++)
            {
                vec2f at  = calc::lerp_bezier(start, b, end, add * i);
                color col = calc::lerp(col0, col1, i / (f32)steps);
                auto  seg = get_quad(prev, at, t, col, col);

                if (i > 1) connect_quads(prev_seg, seg);
                draw_shape(seg);
                prev     = at;
                prev_seg = seg;
            }

            auto seg = get_quad(prev, end, t, col1, col1);
            connect_quads(prev_seg, seg);
            draw_shape(seg);
        }

        /**
         * @brief Draws a cubic bezier line.
         *
         * @param start Position of start point.
         * @param b     Position of control point.
         * @param end   Position of end point.
         * @param t     Line thickness.
         * @param steps Line resolution.
         * @param col   Line color.
         */
        void bezier(
            const vec2f& start, const vec2f& b, const vec2f& end, f32 t,
            i32 steps, const color& col
        )
        {
            bezier(start, b, end, t, steps, col, col);
        }

        /**
         * @brief Draws a quadratic bezier line.
         *
         * @param start Position of start point.
         * @param b     Position of first control point.
         * @param c     Position of second control point.
         * @param end   Position of end point.
         * @param t     Line thickness.
         * @param steps Line resolution.
         * @param col0  Start color.
         * @param col1  End color.
         */
        void bezier(
            const vec2f& start, const vec2f& b, const vec2f& c,
            const vec2f& end, f32 t, i32 steps, const color& col0,
            const color& col1
        )
        {
            f32 add = 1.0f / steps;

            vec2f     prev     = start;
            ShapeQuad prev_seg = {};

            for (i32 i = 1; i < steps; i++)
            {
                vec2f at  = calc::lerp_bezier(start, b, c, end, add * i);
                color col = calc::lerp(col0, col1, i / (f32)steps);
                auto  seg = get_quad(prev, at, t, col, col);

                if (i > 1) connect_quads(prev_seg, seg);
                draw_shape(seg);
                prev     = at;
                prev_seg = seg;
            }

            auto seg = get_quad(prev, end, t, col1, col1);
            connect_quads(prev_seg, seg);
            draw_shape(seg);
        }

        /**
         * @brief Draws a quadratic bezier line.
         *
         * @param start Position of start point.
         * @param b     Position of first control point.
         * @param c     Position of second control point.
         * @param end   Position of end point.
         * @param t     Line thickness.
         * @param steps Line resolution.
         * @param col   Color.
         */
        void bezier(
            const vec2f& start, const vec2f& b, const vec2f& c,
            const vec2f& end, f32 t, i32 steps, const color& col
        )
        {
            bezier(start, b, c, end, t, steps, col, col);
        }

        /**
         * @brief Draws a hollow triangle.
         *
         * @param p0  Point one.
         * @param p1  Point two.
         * @param p2  Point three.
         * @param t   Outline thickness.
         * @param col Outline color.
         */
        void empty_triangle(
            const vec2f& p0, const vec2f& p1, const vec2f& p2, const f32 t,
            const color& col
        )
        {
            line(p0, p1, t, col);
            line(p1, p2, t, col);
            line(p2, p0, t, col);
        }

        /**
         * @brief Draws a filled rectangle.
         *
         * @param p0  Point one.
         * @param p1  Point two.
         * @param p2  Point three.
         * @param col Fill color.
         */
        void triangle(
            const vec2f& p0, const vec2f& p1, const vec2f& p2, const color& col
        )
        {
            draw_shape({p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, col, col, col});
        }

        /**
         * @brief Draws a filled rectangle.
         *
         * @param p0  Point one.
         * @param p1  Point two.
         * @param p2  Point three.
         * @param c0  Color near point 0.
         * @param c1  Color near point 1.
         * @param c2  Color near point 2.
         */
        void triangle(
            const vec2f& p0, const vec2f& p1, const vec2f& p2, const color& c0,
            const color& c1, const color& c2
        )
        {
            draw_shape({p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c0, c1, c2});
        }

        /**
         * @brief Draws a hollow rectangle.
         *
         * @param x   Left bounde.
         * @param y   Top bound.
         * @param w   Width.
         * @param h   Height.
         * @param col Outline color.
         */
        void empty_rect(
            const f32 x, const f32 y, const f32 w, const f32 h, f32 t,
            const color& col
        )
        {
            line({x, y + t * 0.5f}, {x + w, y + t * 0.5f}, t, col);
            line({x + w - t * 0.5f, y}, {x + w - t * 0.5f, y + h}, t, col);
            line({x, y + h - t * 0.5f}, {x + w, y + h - t * 0.5f}, t, col);
            line({x + t * 0.5f, y}, {x + t * 0.5f, y + h}, t, col);
        }

        /**
         * @brief Draws a hollow rectangle.
         *
         * @param pos Top Left.
         * @param w   Width.
         * @param h   Height.
         * @param col Outline color.
         */
        void empty_rect(
            const vec2f& pos, const f32 w, const f32 h, f32 t, const color& col
        )
        {
            empty_rect(pos.x, pos.y, w, h, t, col);
        }

        /**
         * @brief Draws a hollow rectangle.
         *
         * @param r   Bounds.
         * @param col Outline color.
         */
        void empty_rect(const rectf& r, f32 t, const color& col)
        {
            empty_rect(r.x, r.y, r.w, r.h, t, col);
        }

        /**
         * @brief Draws a filled rectangle.
         *
         * @param r   Bounds.
         * @param col Fill color.
         */
        void rect(const rectf& r, const color& col)
        {
            draw_shape(
                {r.x, r.y, r.x + r.w, r.y, r.x + r.w, r.y + r.h, r.x, r.y + r.h,
                 col, col, col, col}
            );
        }

        /**
         * @brief Draws a filled rectangle.
         *
         * @param x   Left bound.
         * @param y   Top bound.
         * @param w   Width.
         * @param h   Height.
         * @param col Fill color.
         */
        void rect(
            const f32 x, const f32 y, const f32 w, const f32 h, const color& col
        )
        {
            rect(rectf(x, y, w, h), col);
        }

        /**
         * @brief Draws a filled rectangle.
         *
         * @param pos Top Left bound.
         * @param w   Width.
         * @param h   Height.
         * @param col Fill color.
         */
        void rect(const vec2f& pos, const f32 w, const f32 h, const color& col)
        {
            rect(rectf(pos.x, pos.y, w, h), col);
        }

        /**
         * @brief Draws a hollow circle.
         *
         * @param pos    Center position.
         * @param radius Half size from center.
         * @param steps  Circle resolution (how many steps).
         * @param col    Outline color.
         */
        void empty_circle(
            const vec2f& pos, const f32 radius, f32 t, const i32 steps,
            const color& col
        )
        {
            vec2f last   = vec2f::right * radius;
            vec2f last_p = calc::vec_perpendicular(last);

            ShapeQuad first_seg0 = {};
            ShapeQuad first_seg1 = {};
            ShapeQuad first_seg2 = {};
            ShapeQuad first_seg3 = {};

            ShapeQuad prev_seg0 = {};
            ShapeQuad prev_seg1 = {};
            ShapeQuad prev_seg2 = {};
            ShapeQuad prev_seg3 = {};

            for (i32 i = 1; i <= steps; i++)
            {
                vec2f at =
                    calc::vec_from_angle<f32>(i * (M_PI / 2) / steps) * radius;
                vec2f at_p = calc::vec_perpendicular(at);

                auto seg0 = get_quad(pos + last, pos + at, t, col, col);
                auto seg1 = get_quad(pos - last, pos - at, t, col, col);
                auto seg2 = get_quad(pos + last_p, pos + at_p, t, col, col);
                auto seg3 = get_quad(pos - last_p, pos - at_p, t, col, col);
                if (i > 1)
                {
                    connect_quads(prev_seg0, seg0);
                    connect_quads(prev_seg1, seg1);
                    connect_quads(prev_seg2, seg2);
                    connect_quads(prev_seg3, seg3);
                }
                else
                {
                    first_seg0 = seg0;
                    first_seg1 = seg1;
                    first_seg2 = seg2;
                    first_seg3 = seg3;
                }
                draw_shape(seg0);
                draw_shape(seg1);
                draw_shape(seg2);
                draw_shape(seg3);

                last   = at;
                last_p = at_p;

                prev_seg0 = seg0;
                prev_seg1 = seg1;
                prev_seg2 = seg2;
                prev_seg3 = seg3;
            }

            ShapeQuad seg0 = {prev_seg3.x1,  prev_seg3.y1,  first_seg0.x0,
                              first_seg0.y0, prev_seg3.x2,  prev_seg3.y2,
                              first_seg0.x3, first_seg0.y3, col,
                              col,           col,           col};
            ShapeQuad seg1 = {prev_seg2.x1,  prev_seg2.y1,  first_seg1.x0,
                              first_seg1.y0, prev_seg2.x2,  prev_seg2.y2,
                              first_seg1.x3, first_seg1.y3, col,
                              col,           col,           col};
            ShapeQuad seg2 = {prev_seg0.x1,  prev_seg0.y1,  first_seg2.x0,
                              first_seg2.y0, prev_seg0.x2,  prev_seg0.y2,
                              first_seg2.x3, first_seg2.y3, col,
                              col,           col,           col};
            ShapeQuad seg3 = {prev_seg1.x1,  prev_seg1.y1,  first_seg3.x0,
                              first_seg3.y0, prev_seg1.x2,  prev_seg1.y2,
                              first_seg3.x3, first_seg3.y3, col,
                              col,           col,           col};
            draw_shape(seg0);
            draw_shape(seg1);
            draw_shape(seg2);
            draw_shape(seg3);
        }

        /**
         * @brief Draws a filled circle.
         *
         * @param pos    Center position.
         * @param radius Half size from center.
         * @param steps  Circle resolution (how many steps).
         * @param col    Fill color.
         */
        void circle(
            const vec2f& pos, const f32 radius, const i32 steps,
            const color& col
        )
        {
            vec2f last = {pos.x + radius, pos.y};

            for (i32 i = 1; i <= steps; i++)
            {
                const f32   radians = (i / (f32)steps) * M_PI * 2;
                const vec2f next    = {
                    pos.x + cos(radians) * radius,
                    pos.y + sin(radians) * radius};

                triangle(last, next, pos, col);

                last = next;
            }
        }

        /**
         * @brief Draws a filled circle.
         *
         * @param pos    Center position.
         * @param radius Half size from center.
         * @param steps  Circle resolution (how many steps).
         * @param col0   Start color.
         * @param col1   End color.
         */
        void circle(
            const vec2f& pos, const f32 radius, const i32 steps,
            const color& col0, const color& col1
        )
        {
            vec2f last = {pos.x + radius, pos.y};

            for (i32 i = 1; i <= steps; i++)
            {
                const f32   radians = (i / (f32)steps) * M_PI * 2;
                const vec2f next    = {
                    pos.x + cos(radians) * radius,
                    pos.y + sin(radians) * radius};

                triangle(last, next, pos, col0, col0, col1);

                last = next;
            }
        }

        /**
         * @brief Drawes a hollow ellipse.
         *
         * @param center Center position.
         * @param maj    Semi-major axis length (width).
         * @param min    Semi-minor axis length (height).
         * @param rot    Rotation.
         * @param steps  Circle resolution (how many steps).
         * @param col    Outline color.
         */
        void empty_ellipse(
            const vec2f& center, const i32 maj, const i32 min, const f32 rot,
            f32 t, const i32 steps, const color& col
        )
        {
            f32 max  = M_PI;
            f32 step = 2 * max / (f32)steps;

            vec2f pos = {
                center.x + maj * (f32)cos((f64)-max),
                center.y + min * (f32)sin((f64)-max)};
            if (rot != 0) pos = calc::transform(pos, mat4x4f::rotateZ(rot));
            vec2f last = pos;

            ShapeQuad first_seg = {};
            ShapeQuad prev_seg  = {};
            for (f32 i = -max; i <= max; i += step)
            {
                last = pos;
                pos  = {
                    center.x + maj * (f32)cos((f64)i),
                    center.y + min * (f32)sin((f64)i)};
                if (rot != 0) pos = calc::transform(pos, mat4x4f::rotateZ(rot));
                auto seg = get_quad(last, pos, t, col, col);
                if (i > -max + step)
                    connect_quads(prev_seg, seg);
                else
                    first_seg = seg;
                draw_shape(seg);
                prev_seg = seg;
            }

            last = pos;
            pos  = {
                center.x + maj * (f32)cos((f64)-max),
                center.y + min * (f32)sin((f64)-max)};
            if (rot != 0) pos = calc::transform(pos, mat4x4f::rotateZ(rot));
            auto seg = get_quad(last, pos, t, col, col);
            connect_quads(prev_seg, seg);
            draw_shape(seg);
            prev_seg = seg;

            seg = {prev_seg.x1, prev_seg.y1, first_seg.x0, first_seg.y0,
                   prev_seg.x2, prev_seg.y2, first_seg.x3, first_seg.y3,
                   col,         col,         col,          col};
            draw_shape(seg);
        }
    };

    /** @brief Renders fullscreen quads. */
    class QuadRenderer : impl::SpriteRenderer
    {
    private:
        u32 m_vao;
        u32 m_vbo;
        u32 m_ibo;

        Shader m_shader;

    public:
        QuadRenderer()
        {
            u32 indices[INDEX_PER_QUAD] = {
                0,  //      2
                1,  //    / |
                2,  //  0---1

                2,  //  3---2
                3,  //  | /
                0   //  0
            };

            Vertex vertices[VERTEX_PER_QUAD] = {
                {{0.f, 0.f, 0.f},
                 {0.f, 0.f},
                 color::white.to_vec4(),
                 color::clear.to_vec4()},  //  3-----2
                {{1.f, 0.f, 0.f},
                 {1.f, 0.f},
                 color::white.to_vec4(),
                 color::clear.to_vec4()},  //  |   / |
                {{1.f, 1.f, 0.f},
                 {1.f, 1.f},
                 color::white.to_vec4(),
                 color::clear.to_vec4()},  //  | /   |
                {{0.f, 1.f, 0.f},
                 {0.f, 1.f},
                 color::white.to_vec4(),
                 color::clear.to_vec4()},  //  0-----1
            };

            m_vao = renderer::vertex_array::generate();
            m_vbo = renderer::vertex_buffer::generate(
                vertices, VERTEX_PER_QUAD * sizeof(Vertex)
            );
            m_ibo = renderer::index_buffer::generate(indices, INDEX_PER_QUAD);

            renderer::buffer_layout layout = {
                {renderer::AttributeType::VEC3, "a_pos"},
                {renderer::AttributeType::VEC2, "a_tex"},
                {renderer::AttributeType::VEC4, "a_color"},
                {renderer::AttributeType::VEC4, "a_fill"}};
            renderer::vertex_array::add_buffer(m_vao, m_vbo, layout);

            m_shader = {renderer::shaders::generate(
                renderer::shaders::DEFAULT_2D_VERTEX_SHADER,
                renderer::shaders::DEFAULT_2D_FRAGMENT_SHADER
            )};
        }

        QuadRenderer(const QuadRenderer&)            = delete;
        QuadRenderer& operator=(const QuadRenderer&) = delete;

        ~QuadRenderer()
        {
            renderer::vertex_array::remove(m_vao);
            renderer::vertex_buffer::remove(m_vbo);
            renderer::index_buffer::remove(m_ibo);
        }

        /**
         * @brief Draw a textured quad.
         *
         * @param texture Texture applied to quad.
         * @param shader  Shader effect applied to quad.
         * @param blend   Blend mode applied to quad.
         * @param cull    Culling mode (aka facing direction) of quad.
         */
        void draw(
            u32 texture, Shader shader,
            renderer::BlendMode blend = renderer::BlendMode::NonPremultiplied,
            renderer::Cull      cull  = renderer::Cull::NONE
        )
        {
            renderer::cull(cull);

            renderer::enable_blend();
            renderer::blend(blend.color_eq, blend.alpha_eq);
            renderer::blend(
                blend.color_src, blend.color_dst, blend.alpha_src,
                blend.alpha_dst
            );
            renderer::mask(blend.mask);

            renderer::textures::bind(texture, 0);

            shader.bind();
            shader.apply("u_world", mat4x4f::ortho(1, 1));
            shader.apply(
                "u_flip", renderer::frame_buffer::is_bound() ? -1.f : 1.f
            );
            shader.apply("u_texture", 0);

            renderer::vertex_array::bind(m_vao);
            renderer::vertex_buffer::bind(m_vbo);
            renderer::index_buffer::bind(m_ibo);

            renderer::draw();
        }

        /**
         * @brief Draw a quad without binding any texture.
         *
         * @param shader Shader effect applied to quad.
         * @param matrix Transformation matrix applied to quad.
         * @param blend  Blend mode applied to quad.
         * @param cull   Culling mode (aka facing direction) of quad.
         */
        void draw(
            Shader shader, mat4x4f matrix = mat4x4f::ortho(1, 1),
            renderer::BlendMode blend = renderer::BlendMode::NonPremultiplied,
            renderer::Cull      cull  = renderer::Cull::NONE
        )
        {
            renderer::cull(cull);

            renderer::enable_blend();
            renderer::blend(blend.color_eq, blend.alpha_eq);
            renderer::blend(
                blend.color_src, blend.color_dst, blend.alpha_src,
                blend.alpha_dst
            );
            renderer::mask(blend.mask);

            shader.bind();
            shader.apply("u_world", matrix);
            shader.apply(
                "u_flip", renderer::frame_buffer::is_bound() ? -1.f : 1.f
            );

            renderer::vertex_array::bind(m_vao);
            renderer::vertex_buffer::bind(m_vbo);
            renderer::index_buffer::bind(m_ibo);

            renderer::draw();
        }

        /**
         * @brief Draw a quad without binding any texture.
         *
         * @param texture Texture applied to quad.
         */
        void draw(u32 texture) { draw(texture, m_shader); }

        /**
         * @brief Draw a quad without binding any texture.
         *
         * @param texture Texture applied to quad.
         * @param blend   Blend mode applied to quad.
         */
        void draw(u32 texture, renderer::BlendMode blend)
        {
            draw(texture, m_shader, blend);
        }

        /**
         * @brief Draw a quad without binding any texture.
         *
         * @param texture Texture applied to quad.
         * @param cull    Culling mode (aka facing direction) of quad.
         */
        void draw(u32 texture, renderer::Cull cull)
        {
            draw(
                texture, m_shader, renderer::BlendMode::NonPremultiplied, cull
            );
        }
    };
}
