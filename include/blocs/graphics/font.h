#pragma once

#include <stb_truetype.h>

namespace blocs
{
    /** @brief TrueType Font metadata container. */
    struct Font
    {
    private:
        u8*             m_buffer;
        stbtt_fontinfo* m_info;

        /**
         * @brief Get font name from font metadata.
         *
         * @param font   Font info.
         * @param nameId Which name to get (1 = family, 2 = style).
         * @return Font name
         */
        str get_font_name(stbtt_fontinfo* font, i32 nameId)
        {
            i32  length   = 0;
            cstr name_ptr = stbtt_GetFontNameString(
                font, &length, STBTT_PLATFORM_ID_MICROSOFT,
                STBTT_MS_EID_UNICODE_BMP, STBTT_MS_LANG_ENGLISH, nameId
            );

            str name;
            for (i32 i = 0; i < length; i++) name += name_ptr[i];

            return name;
        }

    public:
        /**
         * @brief Individual font character size and
         * spacing metadata.
         */
        struct Character
        {
            i32 glyph    = 0;
            i32 width    = 0;
            i32 height   = 0;
            f32 advance  = 0;
            f32 offset_x = 0;
            f32 offset_y = 0;
            f32 size     = 0;

            bool has_glyph = false;
        };

        str family;
        str style;

        i32 ascent   = 0;
        i32 descent  = 0;
        i32 line_gap = 0;

        i32 height      = 0;
        i32 line_height = 0;

        Font() = delete;
        Font(const str& filename)
        {
            m_info   = new stbtt_fontinfo();
            m_buffer = new u8[1 << 20];

            FILE* file;

            if (!(file = fopen(filename.c_str(), "rb")))
            {
                LOG_ERR("could not load font, bad path");
                delete[] m_buffer;
                return;
            }

            if (fread(m_buffer, 1, 1 << 20, file) == 0)
            {
                LOG_ERR("could not load font, bad data");
                fclose(file);
                delete[] m_buffer;
                return;
            }

            if (stbtt_InitFont(m_info, m_buffer, 0) == 0)
            {
                LOG_ERR("could not parse font data");
                fclose(file);
                delete[] m_buffer;
                return;
            }

            fclose(file);

            family = get_font_name(m_info, 1);
            style  = get_font_name(m_info, 2);
            stbtt_GetFontVMetrics(m_info, &ascent, &descent, &line_gap);

            height      = ascent - descent;
            line_height = height + line_gap;
        }

        Font(const Font&)            = delete;
        Font& operator=(const Font&) = delete;

        ~Font() { delete m_info; }

        /**
         * @brief Get font glyph from unicode codepoint.
         *
         * @param codepoint Codepoint.
         * @return Glyph index.
         */
        i32 get_glyph(u32 codepoint) const
        {
            if (!m_info) return 0;

            return stbtt_FindGlyphIndex(m_info, codepoint);
        }

        /**
         * @brief Get font pixel scale.
         *
         * @param size Font size (as em).
         * @return Scale in pixels.
         */
        f32 get_scale(f32 size) const
        {
            if (!m_info) return 0;

            return stbtt_ScaleForMappingEmToPixels(m_info, size);
        }

        /**
         * @brief Get kerning between two glyphs.
         *
         * @param glyph1 Glyph one.
         * @param glyph2 Glyph two.
         * @param scale  Font pixel scale.
         * @return Kerning value in pixels.
         */
        f32 get_kerning(i32 glyph1, i32 glyph2, f32 scale) const
        {
            if (!m_info) return 0;

            return stbtt_GetGlyphKernAdvance(m_info, glyph1, glyph2) * scale;
        }

        /**
         * @brief Get character data for a glyph.
         *
         * @param glyph Font glyph.
         * @param size  Font pixel scale.
         * @return Character data.
         */
        Character get_character(i32 glyph, f32 size) const
        {
            Character ch;

            if (!m_info) return ch;

            i32 advance, offsetX, x0, y0, x1, y1;

            stbtt_GetGlyphHMetrics(m_info, glyph, &advance, &offsetX);
            stbtt_GetGlyphBitmapBox(
                m_info, glyph, size, size, &x0, &y0, &x1, &y1
            );

            i32 w = (x1 - x0);
            i32 h = (y1 - y0);

            ch.glyph    = glyph;
            ch.width    = w;
            ch.height   = h;
            ch.advance  = advance * size;
            ch.offset_x = offsetX * size;
            ch.offset_y = (f32)y0;
            ch.size     = size;
            ch.has_glyph =
                (w > 0 && h > 0 && stbtt_IsGlyphEmpty(m_info, glyph) == 0);

            return ch;
        }

        /**
         * @brief Write character bitmap data to pixel buffer.
         *
         * @param ch     Character data.
         * @param pixels Pixel buffer pointer.
         */
        bool get_image(const Character& ch, u8* pixels) const
        {
            if (ch.has_glyph)
            {
                stbtt_MakeGlyphBitmap(
                    m_info, pixels, ch.width, ch.height, ch.width, ch.size,
                    ch.size, ch.glyph
                );
                i32 len = ch.width * ch.height;
                for (i32 a = (len - 1) * 4, b = (len - 1); b >= 0;
                     a -= 4, b -= 1)
                {
                    pixels[a + 0] = pixels[b];
                    pixels[a + 1] = pixels[b];
                    pixels[a + 2] = pixels[b];
                    pixels[a + 3] = pixels[b];
                }
                return true;
            }
            return false;
        }

        /**
         * @brief Write character bitmap data to an image.
         *
         * @param ch     Character data.
         * @return Bitmap image of character.
         */
        Image get_image(const Character& ch) const
        {
            i32 w      = ch.width;
            i32 h      = ch.height;
            u8* pixels = new u8[ch.width * ch.height * 4];

            get_image(ch, pixels);
            return Image(pixels, w, h);
        }
    };
}
