#pragma once

namespace blocs
{
    /** @brief Texture packer to render sprites from a shared atlas. */
    class SpriteAtlas
    {
    public:
        /**
         * @brief Item in the sprite atlas,
         * and the bounds and position within
         * the texture sheet.
         */
        struct Node
        {
            i32      x, y, w, h;
            u32      buffer_index;
            Texture* texture;
        };

    private:
        using Space = recti;

        // Maximum pixel size of the atlas sheet (8k)
        static constexpr size MAX_ATLAS_SIZE = 8192;

        u8* m_buffer;
        u32 m_bufferIndex;

        Texture* m_texture = nullptr;
        Image*   m_image   = nullptr;

        /**
         * Number of pixels to expand
         * atlas items in each direction
         */
        i32 m_expand = 0;
        /**
         * Number of pixels of empty space
         * inbetween atlas items
         */
        i32 m_border = 0;

        /**
         * Whether or not to apply anti-aliasing to
         * the atlas sheet texture
         */
        bool m_aliased = false;

#ifdef DEBUG
        Stopwatch m_timer;
#endif

    public:
        // Packed texture width.
        i32 width;
        // Packed texture height.
        i32 height;

        // Whether or not sprites have been packed.
        bool packed;
        // Whether or not a texture has been generated.
        bool generated;

        std::vector<Node*> nodes{};

        SpriteAtlas() = delete;
        SpriteAtlas(f32 spacing, f32 padding, bool aliased = true)
            : m_border(spacing), m_expand(padding), m_aliased(aliased)
        {
            m_bufferIndex = 0;
            m_buffer      = new u8[MAX_ATLAS_SIZE * MAX_ATLAS_SIZE * 4];

            packed = false;
        }

        SpriteAtlas(const SpriteAtlas&)            = delete;
        SpriteAtlas& operator=(const SpriteAtlas&) = delete;

        ~SpriteAtlas()
        {
            delete m_texture;
            delete[] m_buffer;
        }

        /**
         * @brief Adds an image to the texture packer's buffer.
         *
         * @param filepath Filepath of image.
         * @return Pointer to Node in texture packer buffer.
         */
        Node* add(const str& filepath)
        {
            Image img = Image(filepath);

            assert(
                img.w < MAX_ATLAS_SIZE && img.h < MAX_ATLAS_SIZE &&
                "ERROR: image too large for atlas"
            );

            Node* node = new Node({0, 0, img.w, img.h, m_bufferIndex, nullptr});
            nodes.push_back(node);

            u32 buffer_length = img.w * img.h * 4;
            memcpy(m_buffer + m_bufferIndex, img.pixels, buffer_length);
            m_bufferIndex += buffer_length;

            return node;
        }

        /**
         * @brief Adds an image to the texture packer's buffer.
         *
         * @param pixels Pixel data buffer.
         * @param w      Width of image.
         * @param h      Height of image.
         * @return Pointer to Node in texture packer buffer.
         */
        Node* add(const u8* pixels, const i32 w, const i32 h)
        {
            if (pixels == nullptr)
            {
                LOG_ERR("could not read texture data");
                return nullptr;
            }

            assert(
                w < MAX_ATLAS_SIZE && h < MAX_ATLAS_SIZE &&
                "ERROR: pixel data too large for atlas"
            );

            Node* node = new Node({0, 0, w, h, m_bufferIndex, nullptr});
            nodes.push_back(node);

            u32 buffer_length = w * h * 4;
            memcpy(m_buffer + m_bufferIndex, pixels, buffer_length);
            m_bufferIndex += buffer_length;

            return node;
        }

        /**
         * @brief Adds a rectangle of solid color to the buffer.
         *
         * @param col Fill color.
         * @param w   Rectangle width.
         * @param h   Rectangle height.
         * @return Pointer to Node in texture packer buffer.
         */
        Node* add(const color& col, const i32 w, const i32 h)
        {
            assert(
                w < MAX_ATLAS_SIZE && h < MAX_ATLAS_SIZE &&
                "ERROR: pixel data too large for atlas"
            );

            Node* node = new Node({0, 0, w, h, m_bufferIndex, nullptr});
            nodes.push_back(node);

            u32    buffer_length = w * h * 4;
            uchar* pixels        = new uchar[buffer_length];
            for (i32 i = 0; i < buffer_length; i += 4)
            {
                pixels[i + 0] = (uchar)(col.r);
                pixels[i + 1] = (uchar)(col.b);
                pixels[i + 2] = (uchar)(col.a);
                pixels[i + 3] = (uchar)255U;
            }
            memcpy(m_buffer + m_bufferIndex, pixels, buffer_length);
            m_bufferIndex += buffer_length;

            delete[] pixels;
            return node;
        }

        /**
         * @brief Adds empty space to the buffer.
         *
         * @param w Width.
         * @param h Height.
         * @return Pointer to Node in texture packer buffer.
         */
        Node* add(const i32 w, const i32 h)
        {
            assert(
                w < MAX_ATLAS_SIZE && h < MAX_ATLAS_SIZE &&
                "ERROR: too large for atlas"
            );

            Node* node = new Node({0, 0, w, h, 0, nullptr});
            nodes.push_back(node);

            return node;
        }

        /**
         * @brief Packs queued up images into a single buffer
         * using a greedy bin packing algorithm.
         */
        void pack()
        {
#ifdef DEBUG
            m_timer.reset();
#endif

            if (nodes.size() < 2)
            {
                LOG_ERR("not enough textures to pack");
                return;
            }

            i32 area  = 0;
            i32 max_w = 0;
            i32 max_h = 0;

            i32 padding = m_expand * 2 + m_border;

            for (auto& node : nodes)
            {
#ifdef __EMSCRIPTEN__
                while (max_w < node->w)
                {
                    max_w *= 2;
                    max_h = max_w;

                    area = max_w * max_h;
                }
#else
                area += (node->w + padding) * (node->h + padding);

                max_w = std::max(max_w, node->w + padding);
                max_h = std::max(max_h, node->h + padding);
#endif
            }

            // assumes sub-100% space utilization
            const f32 SUBOPTIMAL_COEFFICIENT = 0.85f;

            assert(
                max_w < MAX_ATLAS_SIZE && max_h < MAX_ATLAS_SIZE &&
                "ERROR: atlas wider than maximum size"
            );
            assert(
                area <=
                    MAX_ATLAS_SIZE * MAX_ATLAS_SIZE * SUBOPTIMAL_COEFFICIENT &&
                "ERROR: total area needed cannot fit in maximum size"
            );

            i32 start_w = std::max(
                (i32)std::ceil(std::sqrt(area / SUBOPTIMAL_COEFFICIENT)), max_w
            );
            std::vector<Space> spaces = {{0, 0, start_w, MAX_ATLAS_SIZE}};

            std::sort(
                nodes.begin(), nodes.end(),
                [](Node* a, Node* b) { return a->h > b->h; }
            );

            for (auto& node : nodes)
            {
                for (size i = spaces.size() - 1; i >= 0; i--)
                {
                    auto space = spaces[i];

                    i32 w = node->w + m_expand * 2 + m_border;
                    i32 h = node->h + m_expand * 2 + m_border;

                    // Check if image too large for space
                    if (w > space.w || h > space.h) continue;

                    // add image to space's top-left
                    // |-------|-------|
                    // |  box  |       |
                    // |_______|       |
                    // |         space |
                    // |_______________|
                    node->x = space.x + m_expand;
                    node->y = space.y + m_expand;

#ifdef __EMSCRIPTEN__
                    while (max_w < node->x + node->w)
                    {
                        max_w *= 2;
                    }

                    while (max_h < node->y + node->h)
                    {
                        max_h *= 2;
                    }
#else
                    max_w = std::max(max_w, node->x + w);
                    max_h = std::max(max_h, node->y + h);
#endif

                    if (w == space.w && h == space.h)
                    {
                        // Remove space if perfect fit
                        // |---------------|
                        // |               |
                        // |      box      |
                        // |               |
                        // |_______________|
                        Space last = spaces.back();
                        spaces.pop_back();

                        if (i < spaces.size()) spaces[i] = last;
                    }
                    else if (h == space.h)
                    {
                        // Space matches image height
                        // move space right and cut off width
                        // |-------|---------------|
                        // |  box  | updated space |
                        // |_______|_______________|
                        space.x += w;
                        space.w -= w;

                        spaces[i] = space;
                    }
                    else if (w == space.w)
                    {
                        // Space matches image width
                        // move space down and cut off height
                        // |---------------|
                        // |      box      |
                        // |_______________|
                        // | updated space |
                        // |_______________|
                        space.y += h;
                        space.h -= h;

                        spaces[i] = space;
                    }
                    else
                    {
                        // Split width and height
                        // difference into two new spaces
                        // |-------|-----------|
                        // |  box  | new space |
                        // |_______|___________|
                        // | updated space     |
                        // |___________________|
                        spaces.push_back({space.x + w, space.y, space.w - w, h}
                        );
                        space.y += h;
                        space.h -= h;

                        spaces[i] = space;
                    }

                    break;
                }
            }

            width  = max_w;
            height = max_h;

            packed = true;

#ifdef DEBUG
            m_timer.stop();
            LOG_INFO(
                "Packed " << nodes.size() << " textures in "
                          << ((f32)m_timer.get_time_elapsed_ms()) << "ms"
            );
#endif
        }

        /** @brief Generates a texture sheet from packed atlas buffer. */
        void generate_texture()
        {
#ifdef DEBUG
            m_timer.reset();
#endif

            if (!packed)
            {
                LOG_ERR(
                    "cannot generate texture before sprite atlas has been "
                    "packed"
                );
                return;
            }

            i32 w = width, h = height;
            m_image = new Image(w, h);

            for (auto& node : nodes)
            {
                if (m_expand > 0)
                {
                    u8* src = m_buffer + node->buffer_index;
                    for (i32 y = -m_expand * 4; y < (node->h + m_expand) * 4;
                         y += 4)
                    {
                        for (i32 x = -m_expand * 4;
                             x < (node->w + m_expand) * 4; x += 4)
                        {
                            i32 src_x = x < 0 ? 0
                                        : x > (node->w - 1) * 4
                                            ? (node->w - 1) * 4
                                            : x;
                            i32 src_y = y < 0 ? 0
                                        : y > (node->h - 1) * 4
                                            ? (node->h - 1) * 4
                                            : y;

                            m_image->pixels
                                [node->x * 4 + x +
                                 (node->y * 4 + y) * m_image->w + 0] =
                                src[src_x + src_y * node->w + 0];
                            m_image->pixels
                                [node->x * 4 + x +
                                 (node->y * 4 + y) * m_image->w + 1] =
                                src[src_x + src_y * node->w + 1];
                            m_image->pixels
                                [node->x * 4 + x +
                                 (node->y * 4 + y) * m_image->w + 2] =
                                src[src_x + src_y * node->w + 2];
                            m_image->pixels
                                [node->x * 4 + x +
                                 (node->y * 4 + y) * m_image->w + 3] =
                                src[src_x + src_y * node->w + 3];
                        }
                    }
                }
                else
                {
                    m_image->set(
                        m_buffer + node->buffer_index,
                        {node->x, node->y, node->w, node->h}
                    );
                }
            }

            m_texture = new Texture(m_image->pixels, w, h, m_aliased);

            for (auto& node : nodes) node->texture = m_texture;

            delete[] m_buffer;
            delete m_image;

            generated = true;

#ifdef DEBUG
            m_timer.stop();
            LOG_INFO(
                "Generated texture atlas in "
                << ((f32)m_timer.get_time_elapsed_ms()) << "ms"
            );
#endif
        }

        /**
         * @brief Removes all images, unpacks the
         * atlas, deletes the texture and clears the buffer.
         */
        void clear()
        {
            if (packed) nodes.clear();

            if (generated)
            {
                m_bufferIndex = 0;
                m_buffer      = new u8[MAX_ATLAS_SIZE * MAX_ATLAS_SIZE * 4];

                delete m_texture;
            }

            width  = 0;
            height = 0;

            packed    = false;
            generated = false;
        }

        /**
         * @brief Retrieves the atlas texture
         * if it exists (atlas has been packed and generated)
         *
         * @return Texture.
         */
        Texture* get_texture()
        {
            if (!packed)
            {
                LOG_ERR("cannot get texture before sprite atlas has been packed"
                );
                return nullptr;
            }

            if (!generated)
            {
                LOG_ERR(
                    "cannot get texture before sprite atlas has been generated"
                );
                return nullptr;
            }

            return m_texture;
        }
    };

    using AtlasTexture = SpriteAtlas::Node*;
}
