#pragma once

#include <vector>

#include <blocs/graphics/font.h>
#include <blocs/graphics/spriteatlas.h>

namespace blocs
{
    /**
     * @brief Texture atlas of TrueType Font glyphs
     * for a fixed font size.
     */
    class SpriteFont
    {
    public:
        /**
         * @brief Individual character
         * metadata for size, positioning,
         * and source in atlas sheet.
         */
        struct Character
        {
            AtlasTexture node      = nullptr;
            u32          codepoint = 0;
            i32          glyph     = 0;
            f32          advance   = 0;
            i32          height    = 0;
            vec2f        offset    = vec2f::zero;
        };

    private:
        SpriteAtlas*                   m_atlas      = nullptr;
        Character*                     m_characters = nullptr;
        std::unordered_map<char, f32*> m_kerning{};

        bool has_atlas = false;

    public:
        str name;
        f32 size     = 0;
        f32 ascent   = 0;
        f32 descent  = 0;
        f32 line_gap = 0;

        f32 height      = 0;
        f32 line_height = 0;

        SpriteFont() = delete;
        SpriteFont(
            const str& filepath, f32 size, SpriteAtlas* atlas = nullptr,
            bool aliased = true
        )
        {
            Font font = Font(filepath);
            pack_glyphs(font, size, atlas);
        }

        SpriteFont(
            const Font& font, f32 size, SpriteAtlas* atlas = nullptr,
            bool aliased = true
        )
        {
            pack_glyphs(font, size, atlas);
        }

        SpriteFont(const SpriteFont&)            = delete;
        SpriteFont& operator=(const SpriteFont&) = delete;

        ~SpriteFont() { delete[] m_characters; }

        /**
         * @brief Get character size and position data
         * from unicode codepoint value.
         *
         * @param codepoint Codepoint.
         * @return Character data.
         */
        Character& get_character(u32 codepoint)
        {
            return m_characters[codepoint - 32];
        }

        /**
         * @brief Get character size and position data
         * from unicode codepoint value.
         *
         * @param codepoint Codepoint.
         * @return Character data.
         */
        const Character get_character(u32 codepoint) const
        {
            return m_characters[codepoint - 32];
        }

        /**
         * @brief Get kerning between two characters.
         *
         * @param a Character one.
         * @param b Character two.
         * @return const f32
         */
        const f32 get_kerning(u32 a, u32 b) const
        {
            if (m_kerning.find(a) != m_kerning.end())
                return 0;
            else
                return m_kerning.at(a)[b + 32];
        }

        /**
         * @brief Creates a font sheet by packing all English
         * unicode characters into an atlas (either a new one
         * or provided).
         *
         * @param font    Font data.
         * @param size    Font size (em).
         * @param atlas   Atlas to pack glyph bitmaps (default makes a new one).
         * @param aliased Enables anti-aliasing (default true).
         */
        void pack_glyphs(
            const Font& font, f32 size, SpriteAtlas* atlas = nullptr,
            bool aliased = true
        )
        {
            f32 scale = font.get_scale(size);

            name     = font.family;
            ascent   = font.ascent * scale;
            descent  = font.descent * scale;
            line_gap = font.line_gap * scale;

            height      = ascent - descent;
            line_height = height + line_gap;

            this->size = size;

            if (!atlas)
            {
                has_atlas = true;
                m_atlas   = new SpriteAtlas(0, 0, aliased);
            }

            m_characters = new Character[96];

            for (i32 i = 32; i < 128; i++)
            {
                m_kerning[i] = new f32[96];

                auto glyph = font.get_glyph(i);
                if (glyph <= 0) continue;

                auto  ch    = font.get_character(glyph, scale);
                auto& c     = get_character(i);
                c.codepoint = i;
                c.advance   = ch.advance;
                c.height    = ch.height;
                c.offset    = {ch.offset_x, ch.offset_y};

                if (ch.has_glyph)
                {
                    u8* pixels = new u8[ch.width * ch.height * 4];
                    if (font.get_image(ch, pixels))
                    {
                        if (has_atlas)
                            c.node = m_atlas->add(pixels, ch.width, ch.height);
                        else
                            c.node = atlas->add(pixels, ch.width, ch.height);
                    }
                    delete[] pixels;
                }
            }

            for (i32 a = 0; a < 96; a++)
            {
                for (i32 b = 0; b < 96; b++)
                {
                    auto ch_a = get_character(a + 32);
                    auto ch_b = get_character(b + 32);
                    auto kerning_value =
                        font.get_kerning(ch_a.glyph, ch_b.glyph, size);
                    if (kerning_value != 0)
                        m_kerning[ch_a.codepoint][b] = kerning_value;
                }
            }

            if (has_atlas)
            {
                m_atlas->pack();
                m_atlas->generate_texture();
            }
        }

        /**
         * @brief Calculates the area taken up by
         * a string with the current font and scale.
         *
         * @param text Text displayed.
         * @return Width and height.
         */
        const vec2f measure_text(cstr text) const
        {
            f32 w = 0.0f;
            f32 h = 0.0f;

            u32 prev = 0;
            while (*text)
            {
                if (*text >= 32 && *text <= 127)
                {
                    if (prev > 0) w += get_kerning(*text, prev);
                    auto ch = get_character(*text);
                    w += ch.advance;
                    h = std::max(h, ch.height + ascent + descent + ch.offset.y);
                }
                prev = (u32)*text;
                ++text;
            }

            return {w, h};
        }

        /**
         * @brief Calculates the area taken up by
         * a string with the current font and scale.
         *
         * @param text Text displayed.
         * @return Width and height.
         */
        const vec2f measure_text(str text) const
        {
            return measure_text(text.c_str());
        }
    };
}
