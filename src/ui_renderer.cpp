
#include "ui_renderer.hpp"
#include "pros/screen.hpp"

#include <cstdint>
#include <cstring>
#include <cctype>

namespace ui {

static void setPixel(int x, int y, pros::Color color) {
    if (x < 0 || x >= width() || y < 0 || y >= height()) {
        return;
    }

    const Point p = toPhysical(x, y);

    pros::screen::set_pen(color);
    pros::screen::draw_pixel(
        static_cast<std::int16_t>(p.x),
        static_cast<std::int16_t>(p.y)
    );
}

void clear() {
    pros::screen::erase();
}

void fillRect(
    int x1, int y1, int x2, int y2,
    pros::Color color
) {
    if (x2 < x1) {
        const int t = x1;
        x1 = x2;
        x2 = t;
    }

    if (y2 < y1) {
        const int t = y1;
        y1 = y2;
        y2 = t;
    }

    x1 = x1 < 0 ? 0 : x1;
    y1 = y1 < 0 ? 0 : y1;
    x2 = x2 >= width() ? width() - 1 : x2;
    y2 = y2 >= height() ? height() - 1 : y2;

    if (x1 > x2 || y1 > y2) {
        return;
    }

    pros::screen::set_pen(color);

    if (currentOrientation == Orientation::Landscape) {
        pros::screen::fill_rect(x1, y1, x2, y2);
    } else {
        // Transform all four rectangle edges.
        const int physicalLeft = PHYSICAL_WIDTH - 1 - y2;
        const int physicalRight = PHYSICAL_WIDTH - 1 - y1;
        const int physicalTop = x1;
        const int physicalBottom = x2;

        pros::screen::fill_rect(
            physicalLeft, physicalTop,
            physicalRight, physicalBottom
        );
    }
}

void drawRect(
    int x1, int y1, int x2, int y2,
    pros::Color color
) {
    for (int x = x1; x <= x2; ++x) {
        setPixel(x, y1, color);
        setPixel(x, y2, color);
    }

    for (int y = y1; y <= y2; ++y) {
        setPixel(x1, y, color);
        setPixel(x2, y, color);
    }
}

// 5-column bitmap font. Bit 0 is the top pixel.
static void glyph(char input, unsigned char out[5]) {
    const char c = static_cast<char>(
        std::toupper(static_cast<unsigned char>(input))
    );

    switch (c) {
        case 'A': { const unsigned char g[5]={0x7E,0x11,0x11,0x11,0x7E}; std::memcpy(out,g,5); return; }
        case 'B': { const unsigned char g[5]={0x7F,0x49,0x49,0x49,0x36}; std::memcpy(out,g,5); return; }
        case 'C': { const unsigned char g[5]={0x3E,0x41,0x41,0x41,0x22}; std::memcpy(out,g,5); return; }
        case 'D': { const unsigned char g[5]={0x7F,0x41,0x41,0x22,0x1C}; std::memcpy(out,g,5); return; }
        case 'E': { const unsigned char g[5]={0x7F,0x49,0x49,0x49,0x41}; std::memcpy(out,g,5); return; }
        case 'F': { const unsigned char g[5]={0x7F,0x09,0x09,0x09,0x01}; std::memcpy(out,g,5); return; }
        case 'G': { const unsigned char g[5]={0x3E,0x41,0x49,0x49,0x7A}; std::memcpy(out,g,5); return; }
        case 'H': { const unsigned char g[5]={0x7F,0x08,0x08,0x08,0x7F}; std::memcpy(out,g,5); return; }
        case 'I': { const unsigned char g[5]={0x00,0x41,0x7F,0x41,0x00}; std::memcpy(out,g,5); return; }
        case 'J': { const unsigned char g[5]={0x20,0x40,0x41,0x3F,0x01}; std::memcpy(out,g,5); return; }
        case 'K': { const unsigned char g[5]={0x7F,0x08,0x14,0x22,0x41}; std::memcpy(out,g,5); return; }
        case 'L': { const unsigned char g[5]={0x7F,0x40,0x40,0x40,0x40}; std::memcpy(out,g,5); return; }
        case 'M': { const unsigned char g[5]={0x7F,0x02,0x0C,0x02,0x7F}; std::memcpy(out,g,5); return; }
        case 'N': { const unsigned char g[5]={0x7F,0x04,0x08,0x10,0x7F}; std::memcpy(out,g,5); return; }
        case 'O': { const unsigned char g[5]={0x3E,0x41,0x41,0x41,0x3E}; std::memcpy(out,g,5); return; }
        case 'P': { const unsigned char g[5]={0x7F,0x09,0x09,0x09,0x06}; std::memcpy(out,g,5); return; }
        case 'Q': { const unsigned char g[5]={0x3E,0x41,0x51,0x21,0x5E}; std::memcpy(out,g,5); return; }
        case 'R': { const unsigned char g[5]={0x7F,0x09,0x19,0x29,0x46}; std::memcpy(out,g,5); return; }
        case 'S': { const unsigned char g[5]={0x46,0x49,0x49,0x49,0x31}; std::memcpy(out,g,5); return; }
        case 'T': { const unsigned char g[5]={0x01,0x01,0x7F,0x01,0x01}; std::memcpy(out,g,5); return; }
        case 'U': { const unsigned char g[5]={0x3F,0x40,0x40,0x40,0x3F}; std::memcpy(out,g,5); return; }
        case 'V': { const unsigned char g[5]={0x1F,0x20,0x40,0x20,0x1F}; std::memcpy(out,g,5); return; }
        case 'W': { const unsigned char g[5]={0x3F,0x40,0x30,0x40,0x3F}; std::memcpy(out,g,5); return; }
        case 'X': { const unsigned char g[5]={0x63,0x14,0x08,0x14,0x63}; std::memcpy(out,g,5); return; }
        case 'Y': { const unsigned char g[5]={0x07,0x08,0x70,0x08,0x07}; std::memcpy(out,g,5); return; }
        case 'Z': { const unsigned char g[5]={0x61,0x51,0x49,0x45,0x43}; std::memcpy(out,g,5); return; }
        case '0': { const unsigned char g[5]={0x3E,0x51,0x49,0x45,0x3E}; std::memcpy(out,g,5); return; }
        case '1': { const unsigned char g[5]={0x00,0x42,0x7F,0x40,0x00}; std::memcpy(out,g,5); return; }
        case '2': { const unsigned char g[5]={0x42,0x61,0x51,0x49,0x46}; std::memcpy(out,g,5); return; }
        case '3': { const unsigned char g[5]={0x21,0x41,0x45,0x4B,0x31}; std::memcpy(out,g,5); return; }
        case '4': { const unsigned char g[5]={0x18,0x14,0x12,0x7F,0x10}; std::memcpy(out,g,5); return; }
        case '5': { const unsigned char g[5]={0x27,0x45,0x45,0x45,0x39}; std::memcpy(out,g,5); return; }
        case '6': { const unsigned char g[5]={0x3C,0x4A,0x49,0x49,0x30}; std::memcpy(out,g,5); return; }
        case '7': { const unsigned char g[5]={0x01,0x71,0x09,0x05,0x03}; std::memcpy(out,g,5); return; }
        case '8': { const unsigned char g[5]={0x36,0x49,0x49,0x49,0x36}; std::memcpy(out,g,5); return; }
        case '9': { const unsigned char g[5]={0x06,0x49,0x49,0x29,0x1E}; std::memcpy(out,g,5); return; }
        case '-': { const unsigned char g[5]={0x08,0x08,0x08,0x08,0x08}; std::memcpy(out,g,5); return; }
        case '_': { const unsigned char g[5]={0x40,0x40,0x40,0x40,0x40}; std::memcpy(out,g,5); return; }
        case ':': { const unsigned char g[5]={0x00,0x36,0x36,0x00,0x00}; std::memcpy(out,g,5); return; }
        case '.': { const unsigned char g[5]={0x00,0x60,0x60,0x00,0x00}; std::memcpy(out,g,5); return; }
        case '%': { const unsigned char g[5]={0x63,0x13,0x08,0x64,0x63}; std::memcpy(out,g,5); return; }
        case '^': { const unsigned char g[5]={0x04,0x02,0x01,0x02,0x04}; std::memcpy(out,g,5); return; }
        case ' ': { const unsigned char g[5]={0,0,0,0,0}; std::memcpy(out,g,5); return; }
        default: { const unsigned char g[5]={0x7F,0x41,0x5D,0x41,0x7F}; std::memcpy(out,g,5); return; }
    }
}

void drawText(
    int x, int y,
    const char* text,
    pros::Color color,
    int scale
) {
    if (text == nullptr || scale < 1) {
        return;
    }

    int cursorX = x;

    for (const char* c = text; *c != '\0'; ++c) {
        unsigned char columns[5] = {};
        glyph(*c, columns);

        for (int col = 0; col < 5; ++col) {
            for (int row = 0; row < 7; ++row) {
                if ((columns[col] & (1u << row)) == 0) {
                    continue;
                }

                for (int sx = 0; sx < scale; ++sx) {
                    for (int sy = 0; sy < scale; ++sy) {
                        setPixel(
                            cursorX + col * scale + sx,
                            y + row * scale + sy,
                            color
                        );
                    }
                }
            }
        }

        cursorX += 6 * scale;
    }
}

} // namespace ui
