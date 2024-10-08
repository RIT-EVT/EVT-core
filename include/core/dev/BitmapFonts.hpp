#ifndef EVT_BITMAPFONTS_HPP
#define EVT_BITMAPFONTS_HPP

#include <cstdint>

/**
 * BitMapFont is a class that holds a representation of a bitmap font. The class can hold multiple fonts, however
 * currently it is only holding a single 4x6 font.
 */
class BitmapFont {
public:
    constexpr static const uint8_t font6x13[235][14]{
        {0x00, 0x00, 0x00, 0x00},                                                  // U+0000 (nul)
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0001
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0002
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0003
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0004
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0005
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0006
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0007
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0008
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0009
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+000A
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+000B
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+000C
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+000D
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+000E
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+000F
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0010
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0011
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0012
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0013
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0014
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0015
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0016
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0017
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0018
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+0019
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+001A
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+001B
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+001C
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+001D
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+001E
        {0x00, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // U+001F
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},         // Character: space
        {0x0, 0x0, 0xFC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0},        // Character: exclam
        {0x0, 0x1C, 0x0, 0x1C, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},       // Character: quotedbl
        {0xA0, 0xF8, 0xA0, 0xF8, 0xA0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x3, 0x0},    // Character: numbersign
        {0x30, 0x48, 0xFC, 0x48, 0x88, 0x0, 0x0, 0x0, 0x2, 0x2, 0x7, 0x2, 0x1},    // Character: dollar
        {0x8, 0x94, 0x48, 0x30, 0xC, 0x0, 0x0, 0x0, 0x6, 0x1, 0x2, 0x5, 0x2},      // Character: percent
        {0xB0, 0x48, 0xB0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x3, 0x5},      // Character: ampersand
        {0x0, 0x0, 0x1C, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},        // Character: quotesingle
        {0x0, 0xF0, 0xC, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x6, 0x8, 0x0},        // Character: parenleft
        {0x0, 0x2, 0xC, 0xF0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x6, 0x1, 0x0},        // Character: parenright
        {0x28, 0x10, 0x7C, 0x10, 0x28, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},    // Character: asterisk
        {0x40, 0x40, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0},    // Character: plus
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x6, 0x2, 0x0},         // Character: comma
        {0x40, 0x40, 0x40, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},    // Character: hyphen
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xE, 0x4, 0x0},         // Character: period
        {0x0, 0x80, 0x40, 0x30, 0xC, 0x0, 0x0, 0x0, 0x6, 0x1, 0x0, 0x0, 0x0},      // Character: slash
        {0xF0, 0x8, 0x4, 0x8, 0xF0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x4, 0x2, 0x1},       // Character: zero
        {0x10, 0x8, 0xFC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x4, 0x7, 0x4, 0x4},       // Character: one
        {0x18, 0x4, 0x84, 0x44, 0x38, 0x0, 0x0, 0x0, 0x6, 0x5, 0x4, 0x4, 0x4},     // Character: two
        {0x4, 0x44, 0x64, 0x54, 0x8C, 0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x4, 0x3},     // Character: three
        {0x80, 0x60, 0x10, 0xFC, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x7, 0x1},     // Character: four
        {0x7C, 0x44, 0x24, 0x24, 0xC4, 0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x4, 0x3},    // Character: five
        {0xF8, 0x44, 0x44, 0x44, 0x88, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: six
        {0x4, 0x4, 0xC4, 0x34, 0xC, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0},       // Character: seven
        {0xB8, 0x44, 0x44, 0x44, 0xB8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: eight
        {0x38, 0x44, 0x44, 0x44, 0xF8, 0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x4, 0x3},    // Character: nine
        {0x0, 0x20, 0x70, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0xE, 0x4, 0x0},      // Character: colon
        {0x0, 0x20, 0x70, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x6, 0x2, 0x0},      // Character: semicolon
        {0x40, 0xA0, 0x10, 0x8, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x4},      // Character: less
        {0x20, 0x20, 0x20, 0x20, 0x20, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1},    // Character: equal
        {0x4, 0x8, 0x10, 0xA0, 0x40, 0x0, 0x0, 0x0, 0x4, 0x2, 0x1, 0x0, 0x0},      // Character: greater
        {0x18, 0x4, 0x84, 0x44, 0x38, 0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0},     // Character: question
        {0xF8, 0x4, 0xC4, 0x24, 0xF8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x5, 0x5, 0x4},     // Character: at
        {0xF0, 0x88, 0x84, 0x88, 0xF0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x7},    // Character: A
        {0x4, 0xFC, 0x44, 0x44, 0xB8, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x4, 0x3},     // Character: B
        {0xF8, 0x4, 0x4, 0x4, 0x8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},        // Character: C
        {0x4, 0xFC, 0x4, 0x4, 0xF8, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x4, 0x3},       // Character: D
        {0xFC, 0x44, 0x44, 0x44, 0x4, 0x0, 0x0, 0x0, 0x7, 0x4, 0x4, 0x4, 0x4},     // Character: E
        {0xFC, 0x44, 0x44, 0x44, 0x4, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0},     // Character: F
        {0xF8, 0x4, 0x4, 0x84, 0x88, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},      // Character: G
        {0xFC, 0x40, 0x40, 0x40, 0xFC, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x7},    // Character: H
        {0x0, 0x4, 0xFC, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},        // Character: I
        {0x0, 0x0, 0x4, 0xFC, 0x4, 0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x3, 0x0},        // Character: J
        {0xFC, 0x40, 0xA0, 0x10, 0xC, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x1, 0x6},     // Character: K
        {0xFC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x4, 0x4, 0x4, 0x4},        // Character: L
        {0xFC, 0x10, 0x60, 0x10, 0xFC, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x7},    // Character: M
        {0xFC, 0x18, 0x60, 0x80, 0xFC, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x1, 0x7},    // Character: N
        {0xF8, 0x4, 0x4, 0x4, 0xF8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},       // Character: O
        {0xFC, 0x44, 0x44, 0x44, 0x38, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0},    // Character: P
        {0xF8, 0x4, 0x4, 0x4, 0xF8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x6, 0x4, 0xB},       // Character: Q
        {0xFC, 0x44, 0xC4, 0x44, 0x38, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x1, 0x6},    // Character: R
        {0x38, 0x44, 0x44, 0x44, 0x88, 0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x4, 0x3},    // Character: S
        {0x4, 0x4, 0xFC, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0},        // Character: T
        {0xFC, 0x0, 0x0, 0x0, 0xFC, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},       // Character: U
        {0x3C, 0xC0, 0x0, 0xC0, 0x3C, 0x0, 0x0, 0x0, 0x0, 0x1, 0x6, 0x1, 0x0},     // Character: V
        {0xFC, 0x0, 0xC0, 0x0, 0xFC, 0x0, 0x0, 0x0, 0x3, 0x4, 0x3, 0x4, 0x3},      // Character: W
        {0xC, 0xB0, 0x40, 0xB0, 0xC, 0x0, 0x0, 0x0, 0x6, 0x1, 0x0, 0x1, 0x6},      // Character: X
        {0xC, 0x30, 0xC0, 0x30, 0xC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0},      // Character: Y
        {0x4, 0x84, 0x44, 0x34, 0xC, 0x0, 0x0, 0x0, 0x6, 0x5, 0x4, 0x4, 0x4},      // Character: Z
        {0x0, 0xFE, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0xF, 0x8, 0x8, 0x0},        // Character: bracketleft
        {0xC, 0x30, 0x40, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x6},      // Character: backslash
        {0x0, 0x2, 0x2, 0xFE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x8, 0xF, 0x0},        // Character: bracketright
        {0x10, 0x8, 0x4, 0x8, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},       // Character: asciicircum
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x8, 0x8, 0x8, 0x8},         // Character: underscore
        {0x0, 0x0, 0x2, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},         // Character: grave
        {0x0, 0xA0, 0xA0, 0xA0, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: a
        {0xFC, 0x20, 0x20, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x4, 0x4, 0x4, 0x3},    // Character: b
        {0xC0, 0x20, 0x20, 0x20, 0x40, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: c
        {0xC0, 0x20, 0x20, 0x20, 0xFC, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x7},    // Character: d
        {0xC0, 0xA0, 0xA0, 0xA0, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: e
        {0x40, 0xF8, 0x44, 0x44, 0x8, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0},     // Character: f
        {0xC0, 0x20, 0x20, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x9, 0x12, 0x12, 0x12, 0xF}, // Character: g
        {0xFC, 0x40, 0x20, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x7},    // Character: h
        {0x0, 0x20, 0xE8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},       // Character: i
        {0x0, 0x0, 0x20, 0xE8, 0x0, 0x0, 0x0, 0x0, 0xC, 0x10, 0x10, 0xF, 0x0},     // Character: j
        {0xFC, 0x80, 0x40, 0x20, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x1, 0x2, 0x4},     // Character: k
        {0x0, 0x4, 0xFC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},        // Character: l
        {0xE0, 0x20, 0xC0, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x3, 0x0, 0x7},    // Character: m
        {0xE0, 0x40, 0x20, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x7},    // Character: n
        {0xC0, 0x20, 0x20, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: o
        {0xE0, 0x20, 0x20, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x1F, 0x2, 0x2, 0x2, 0x1},   // Character: p
        {0xC0, 0x20, 0x20, 0x20, 0xE0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x2, 0x2, 0x1F},   // Character: q
        {0xE0, 0x40, 0x20, 0x20, 0x40, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0},    // Character: r
        {0x40, 0xA0, 0xA0, 0x20, 0x40, 0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x5, 0x2},    // Character: s
        {0x20, 0xF8, 0x20, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2},     // Character: t
        {0xE0, 0x0, 0x0, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},       // Character: u
        {0xE0, 0x0, 0x0, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x3, 0x0},       // Character: v
        {0xE0, 0x0, 0x80, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x3, 0x4, 0x3},      // Character: w
        {0x20, 0x40, 0x80, 0x40, 0x20, 0x0, 0x0, 0x0, 0x4, 0x2, 0x1, 0x2, 0x4},    // Character: x
        {0xE0, 0x0, 0x0, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x9, 0x12, 0x12, 0x11, 0xF},    // Character: y
        {0x20, 0x20, 0xA0, 0x60, 0x20, 0x0, 0x0, 0x0, 0x6, 0x5, 0x4, 0x4, 0x4},    // Character: z
        {0x40, 0x40, 0xBC, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x8, 0x8},      // Character: braceleft
        {0x0, 0x0, 0xFC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0},        // Character: bar
        {0x2, 0x2, 0xBC, 0x40, 0x40, 0x0, 0x0, 0x0, 0x8, 0x8, 0x7, 0x0, 0x0},      // Character: braceright
        {0x18, 0x4, 0x8, 0x10, 0xC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},       // Character: asciitilde
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},         // Character: space
        {0x0, 0x0, 0xF4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0},        // Character: exclamdown
        {0xF0, 0x8, 0xFC, 0x8, 0x90, 0x0, 0x0, 0x0, 0x0, 0x1, 0x3, 0x1, 0x0},      // Character: cent
        {0x40, 0xF8, 0x44, 0x4, 0x8, 0x0, 0x0, 0x0, 0x4, 0x3, 0x4, 0x4, 0x2},      // Character: sterling
        {0x10, 0xE0, 0x20, 0xE0, 0x10, 0x0, 0x0, 0x0, 0x2, 0x1, 0x1, 0x1, 0x2},    // Character: currency
        {0x4C, 0x70, 0xC0, 0x70, 0x4C, 0x0, 0x0, 0x0, 0x1, 0x1, 0x7, 0x1, 0x1},    // Character: yen
        {0x0, 0x0, 0xBC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0},        // Character: brokenbar
        {0x0, 0x6C, 0x92, 0x92, 0x64, 0x0, 0x0, 0x0, 0x0, 0x2, 0x4, 0x4, 0x3},     // Character: section
        {0x0, 0x6, 0x0, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},         // Character: dieresis
        {0xFC, 0x72, 0x8A, 0x52, 0xFC, 0x0, 0x0, 0x0, 0x1, 0x2, 0x2, 0x2, 0x1},    // Character: copyright
        {0x20, 0x54, 0x54, 0x54, 0x78, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1},    // Character: ordfeminine
        {0xC0, 0x20, 0xD0, 0x20, 0x10, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x1, 0x2},    // Character: guillemotleft
        {0x40, 0x40, 0x40, 0x40, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1},    // Character: logicalnot
        {0x0, 0x40, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},      // Character: hyphen
        {0xFC, 0xFA, 0x4A, 0xB2, 0xFC, 0x0, 0x0, 0x0, 0x1, 0x2, 0x2, 0x2, 0x1},    // Character: registered
        {0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},         // Character: macron
        {0x0, 0x18, 0x24, 0x24, 0x18, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},     // Character: degree
        {0x20, 0x20, 0xF8, 0x20, 0x20, 0x0, 0x0, 0x0, 0x2, 0x2, 0x2, 0x2, 0x2},    // Character: plusminus
        {0x24, 0x32, 0x2C, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},      // Character: twosuperior
        {0x24, 0x2A, 0x14, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},      // Character: threesuperior
        {0x0, 0x0, 0x4, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},         // Character: acute
        {0xE0, 0x0, 0x0, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x1F, 0x4, 0x4, 0x2, 0x7},      // Character: mu
        {0x78, 0xFC, 0xFC, 0x4, 0xFC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x7},     // Character: paragraph
        {0x0, 0x0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},       // Character: periodcentered
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x8, 0x0},        // Character: cedilla
        {0x24, 0x3E, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},      // Character: onesuperior
        {0x38, 0x44, 0x44, 0x44, 0x38, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1},    // Character: ordmasculine
        {0x10, 0x20, 0xD0, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x2, 0x1, 0x2, 0x1, 0x0},    // Character: guillemotright
        {0x24, 0x3E, 0x20, 0x80, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x2, 0x7},    // Character: onequarter
        {0x24, 0x3E, 0xA0, 0x40, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x6, 0x5},    // Character: onehalf
        {0x24, 0x4A, 0x34, 0x80, 0xC0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x2, 0x7},    // Character: threequarters
        {0x80, 0x40, 0x34, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},      // Character: questiondown
        {0xC0, 0x22, 0x14, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x1, 0x1, 0x1, 0x7},    // Character: Agrave
        {0xC0, 0x20, 0x14, 0x22, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x1, 0x1, 0x1, 0x7},    // Character: Aacute
        {0xC0, 0x24, 0x12, 0x22, 0xC4, 0x0, 0x0, 0x0, 0x7, 0x1, 0x1, 0x1, 0x7},    // Character: Acircumflex
        {0xC0, 0x24, 0x12, 0x24, 0xC2, 0x0, 0x0, 0x0, 0x7, 0x1, 0x1, 0x1, 0x7},    // Character: Atilde
        {0xC0, 0x26, 0x10, 0x26, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x1, 0x1, 0x1, 0x7},    // Character: Adieresis
        {0xC0, 0x24, 0x1A, 0x24, 0xC0, 0x0, 0x0, 0x0, 0x7, 0x1, 0x1, 0x1, 0x7},    // Character: Aring
        {0xF8, 0x84, 0xF8, 0x44, 0x4, 0x0, 0x0, 0x0, 0x7, 0x0, 0x7, 0x4, 0x4},     // Character: AE
        {0xF8, 0x4, 0x4, 0x4, 0x8, 0x0, 0x0, 0x0, 0x3, 0x14, 0xC, 0x4, 0x2},       // Character: Ccedilla
        {0xF0, 0x92, 0x94, 0x90, 0x10, 0x0, 0x0, 0x0, 0x7, 0x4, 0x4, 0x4, 0x4},    // Character: Egrave
        {0xF0, 0x90, 0x94, 0x92, 0x10, 0x0, 0x0, 0x0, 0x7, 0x4, 0x4, 0x4, 0x4},    // Character: Eacute
        {0xF0, 0x94, 0x92, 0x92, 0x14, 0x0, 0x0, 0x0, 0x7, 0x4, 0x4, 0x4, 0x4},    // Character: Ecircumflex
        {0xF0, 0x96, 0x90, 0x96, 0x10, 0x0, 0x0, 0x0, 0x7, 0x4, 0x4, 0x4, 0x4},    // Character: Edieresis
        {0x0, 0x12, 0xF4, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},      // Character: Igrave
        {0x0, 0x10, 0xF4, 0x12, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},      // Character: Iacute
        {0x0, 0x14, 0xF2, 0x12, 0x4, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},      // Character: Icircumflex
        {0x0, 0x16, 0xF0, 0x16, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},      // Character: Idieresis
        {0x44, 0xFC, 0x44, 0x4, 0xF8, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x4, 0x3},     // Character: Eth
        {0xF0, 0x44, 0x82, 0x4, 0xF2, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x1, 0x7},     // Character: Ntilde
        {0xE0, 0x12, 0x14, 0x10, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: Ograve
        {0xE0, 0x10, 0x14, 0x12, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: Oacute
        {0xE0, 0x14, 0x12, 0x12, 0xE4, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: Ocircumflex
        {0xE0, 0x14, 0x12, 0x14, 0xE2, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: Otilde
        {0xE0, 0x16, 0x10, 0x16, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: Odieresis
        {0x20, 0x40, 0x80, 0x40, 0x20, 0x0, 0x0, 0x0, 0x2, 0x1, 0x0, 0x1, 0x2},    // Character: multiply
        {0xF8, 0x4, 0xE4, 0x1C, 0xFA, 0x0, 0x0, 0x0, 0xB, 0x7, 0x4, 0x4, 0x3},     // Character: Oslash
        {0xF0, 0x2, 0x4, 0x0, 0xF0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},       // Character: Ugrave
        {0xF0, 0x0, 0x4, 0x2, 0xF0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},       // Character: Uacute
        {0xF0, 0x4, 0x2, 0x2, 0xF4, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},       // Character: Ucircumflex
        {0xF0, 0x6, 0x0, 0x6, 0xF0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},       // Character: Udieresis
        {0x30, 0x40, 0x84, 0x42, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0},    // Character: Yacute
        {0xFC, 0x88, 0x88, 0x88, 0x70, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0},    // Character: Thorn
        {0xF8, 0x4, 0x64, 0x98, 0x0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x4, 0x4, 0x3},      // Character: germandbls
        {0x0, 0xA4, 0xA8, 0xA0, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: agrave
        {0x0, 0xA0, 0xA8, 0xA4, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: aacute
        {0x0, 0xA8, 0xA4, 0xA4, 0xC8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: acircumflex
        {0x0, 0xA8, 0xA4, 0xA8, 0xC4, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: atilde
        {0x0, 0xAC, 0xA0, 0xAC, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: adieresis
        {0x0, 0xA4, 0xAA, 0xAA, 0xC4, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: aring
        {0x0, 0xA0, 0xE0, 0xA0, 0x40, 0x0, 0x0, 0x0, 0x3, 0x4, 0x3, 0x4, 0x2},     // Character: ae
        {0xC0, 0x20, 0x20, 0x20, 0x40, 0x0, 0x0, 0x0, 0x3, 0x14, 0xC, 0x4, 0x2},   // Character: ccedilla
        {0xC0, 0xA4, 0xA8, 0xA0, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: egrave
        {0xC0, 0xA0, 0xA8, 0xA4, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: eacute
        {0xC0, 0xA8, 0xA4, 0xA4, 0xC8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: ecircumflex
        {0xC0, 0xAC, 0xA0, 0xAC, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: edieresis
        {0x0, 0x24, 0xE8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},       // Character: igrave
        {0x0, 0x20, 0xE8, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},       // Character: iacute
        {0x0, 0x28, 0xE4, 0x4, 0x8, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},       // Character: icircumflex
        {0x0, 0x2C, 0xE0, 0xC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x7, 0x4, 0x0},       // Character: idieresis
        {0xC0, 0x2A, 0x2C, 0x32, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: eth
        {0xE0, 0x48, 0x24, 0x28, 0xC4, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x7},    // Character: ntilde
        {0xC0, 0x24, 0x28, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: ograve
        {0xC0, 0x20, 0x28, 0x24, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: oacute
        {0xC0, 0x28, 0x24, 0x24, 0xC8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: ocircumflex
        {0xC0, 0x28, 0x24, 0x28, 0xC4, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: otilde
        {0xC0, 0x2C, 0x20, 0x2C, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x3},    // Character: odieresis
        {0x40, 0x40, 0x58, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0},    // Character: divide
        {0xC0, 0x20, 0xA0, 0x60, 0xD0, 0x0, 0x0, 0x0, 0xB, 0x6, 0x5, 0x4, 0x3},    // Character: oslash
        {0xE0, 0x4, 0x8, 0x0, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},       // Character: ugrave
        {0xE0, 0x0, 0x8, 0x4, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},       // Character: uacute
        {0xE0, 0x8, 0x4, 0x4, 0xE8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},       // Character: ucircumflex
        {0xE0, 0xC, 0x0, 0xC, 0xE0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},       // Character: udieresis
        {0xE0, 0x0, 0x8, 0x4, 0xE0, 0x0, 0x0, 0x0, 0x9, 0x12, 0x12, 0x11, 0xF},    // Character: yacute
        {0xF8, 0x40, 0x20, 0x20, 0xC0, 0x0, 0x0, 0x0, 0x1F, 0x2, 0x4, 0x4, 0x3},   // Character: thorn
        {0xE0, 0xC, 0x0, 0xC, 0xE0, 0x0, 0x0, 0x0, 0x9, 0x12, 0x12, 0x11, 0xF},    // Character: ydieresis
        {0xE2, 0x92, 0x8A, 0x92, 0xE2, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x7},    // Character: Amacron
        {0x8, 0xA8, 0xA8, 0xA8, 0xC8, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: amacron
        {0xC2, 0x24, 0x14, 0x24, 0xC2, 0x0, 0x0, 0x0, 0x7, 0x1, 0x1, 0x1, 0x7},    // Character: Abreve
        {0x4, 0xA8, 0xA8, 0xA8, 0xC4, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0x7},     // Character: abreve
        {0xF0, 0x88, 0x84, 0x88, 0xF0, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0xF},    // Character: Aogonek
        {0x0, 0xA0, 0xA0, 0xA0, 0xC0, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x2, 0xF},     // Character: aogonek
        {0xE0, 0x10, 0x14, 0x12, 0x20, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: Cacute
        {0xC0, 0x20, 0x28, 0x24, 0x40, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: cacute
        {0xE0, 0x14, 0x12, 0x12, 0x24, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: Ccircumflex
        {0xC0, 0x28, 0x24, 0x24, 0x48, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: ccircumflex
        {0xE0, 0x10, 0x16, 0x10, 0x20, 0x0, 0x0, 0x0, 0x3, 0x4, 0x4, 0x4, 0x2},    // Character: Cdotaccent
    };

    constexpr static const uint8_t font4x6[235][8]{
        {0x00, 0x00, 0x00, 0x00}, // U+0000 (nul)
        {0x00, 0x00, 0x00, 0x00}, // U+0001
        {0x00, 0x00, 0x00, 0x00}, // U+0002
        {0x00, 0x00, 0x00, 0x00}, // U+0003
        {0x00, 0x00, 0x00, 0x00}, // U+0004
        {0x00, 0x00, 0x00, 0x00}, // U+0005
        {0x00, 0x00, 0x00, 0x00}, // U+0006
        {0x00, 0x00, 0x00, 0x00}, // U+0007
        {0x00, 0x00, 0x00, 0x00}, // U+0008
        {0x00, 0x00, 0x00, 0x00}, // U+0009
        {0x00, 0x00, 0x00, 0x00}, // U+000A
        {0x00, 0x00, 0x00, 0x00}, // U+000B
        {0x00, 0x00, 0x00, 0x00}, // U+000C
        {0x00, 0x00, 0x00, 0x00}, // U+000D
        {0x00, 0x00, 0x00, 0x00}, // U+000E
        {0x00, 0x00, 0x00, 0x00}, // U+000F
        {0x00, 0x00, 0x00, 0x00}, // U+0010
        {0x00, 0x00, 0x00, 0x00}, // U+0011
        {0x00, 0x00, 0x00, 0x00}, // U+0012
        {0x00, 0x00, 0x00, 0x00}, // U+0013
        {0x00, 0x00, 0x00, 0x00}, // U+0014
        {0x00, 0x00, 0x00, 0x00}, // U+0015
        {0x00, 0x00, 0x00, 0x00}, // U+0016
        {0x00, 0x00, 0x00, 0x00}, // U+0017
        {0x00, 0x00, 0x00, 0x00}, // U+0018
        {0x00, 0x00, 0x00, 0x00}, // U+0019
        {0x00, 0x00, 0x00, 0x00}, // U+001A
        {0x00, 0x00, 0x00, 0x00}, // U+001B
        {0x00, 0x00, 0x00, 0x00}, // U+001C
        {0x00, 0x00, 0x00, 0x00}, // U+001D
        {0x00, 0x00, 0x00, 0x00}, // U+001E
        {0x00, 0x00, 0x00, 0x00}, // U+001F
        {0x0, 0x0, 0x0, 0x0},     // Character: space
        {0x17, 0x0, 0x0, 0x0},    // Character: exclam
        {0xC, 0x0, 0xC, 0x0},     // Character: quotedbl
        {0x1F, 0xA, 0x1F, 0x0},   // Character: numbersign
        {0xA, 0x1F, 0x5, 0x0},    // Character: dollar
        {0x9, 0x4, 0x12, 0x0},    // Character: percent
        {0xF, 0x17, 0x1C, 0x0},   // Character: ampersand
        {0xC, 0x0, 0x0, 0x0},     // Character: quotesingle
        {0xE, 0x11, 0x0, 0x0},    // Character: parenleft
        {0x11, 0xE, 0x0, 0x0},    // Character: parenright
        {0xA, 0x4, 0xA, 0x0},     // Character: asterisk
        {0x4, 0xE, 0x4, 0x0},     // Character: plus
        {0x8, 0x4, 0x0, 0x0},     // Character: comma
        {0x4, 0x4, 0x4, 0x0},     // Character: hyphen
        {0x4, 0x0, 0x0, 0x0},     // Character: period
        {0x18, 0x4, 0x3, 0x0},    // Character: slash
        {0x1E, 0x11, 0xF, 0x0},   // Character: zero
        {0x2, 0x1F, 0x0, 0x0},    // Character: one
        {0x19, 0x15, 0x12, 0x0},  // Character: two
        {0x11, 0x15, 0xA, 0x0},   // Character: three
        {0x7, 0x4, 0x1F, 0x0},    // Character: four
        {0x17, 0x15, 0x9, 0x0},   // Character: five
        {0x1E, 0x15, 0x1D, 0x0},  // Character: six
        {0x19, 0x5, 0x3, 0x0},    // Character: seven
        {0x1F, 0x15, 0x1F, 0x0},  // Character: eight
        {0x17, 0x15, 0xF, 0x0},   // Character: nine
        {0xA, 0x0, 0x0, 0x0},     // Character: colon
        {0x10, 0xA, 0x0, 0x0},    // Character: semicolon
        {0x4, 0xA, 0x11, 0x0},    // Character: less
        {0xA, 0xA, 0xA, 0x0},     // Character: equal
        {0x11, 0xA, 0x4, 0x0},    // Character: greater
        {0x1, 0x15, 0x3, 0x0},    // Character: question
        {0xE, 0x15, 0x16, 0x0},   // Character: at
        {0x1E, 0x5, 0x1E, 0x0},   // Character: A
        {0x1F, 0x15, 0xA, 0x0},   // Character: B
        {0xE, 0x11, 0x11, 0x0},   // Character: C
        {0x1F, 0x11, 0xE, 0x0},   // Character: D
        {0x1F, 0x15, 0x15, 0x0},  // Character: E
        {0x1F, 0x5, 0x5, 0x0},    // Character: F
        {0xE, 0x15, 0x1D, 0x0},   // Character: G
        {0x1F, 0x4, 0x1F, 0x0},   // Character: H
        {0x11, 0x1F, 0x11, 0x0},  // Character: I
        {0x8, 0x10, 0xF, 0x0},    // Character: J
        {0x1F, 0x4, 0x1B, 0x0},   // Character: K
        {0x1F, 0x10, 0x10, 0x0},  // Character: L
        {0x1F, 0x6, 0x1F, 0x0},   // Character: M
        {0x1F, 0xE, 0x1F, 0x0},   // Character: N
        {0xE, 0x11, 0xE, 0x0},    // Character: O
        {0x1F, 0x5, 0x2, 0x0},    // Character: P
        {0xE, 0x19, 0x1E, 0x0},   // Character: Q
        {0x1F, 0xD, 0x16, 0x0},   // Character: R
        {0x12, 0x15, 0x9, 0x0},   // Character: S
        {0x1, 0x1F, 0x1, 0x0},    // Character: T
        {0xF, 0x10, 0x1F, 0x0},   // Character: U
        {0x7, 0x18, 0x7, 0x0},    // Character: V
        {0x1F, 0xC, 0x1F, 0x0},   // Character: W
        {0x1B, 0x4, 0x1B, 0x0},   // Character: X
        {0x3, 0x1C, 0x3, 0x0},    // Character: Y
        {0x19, 0x15, 0x13, 0x0},  // Character: Z
        {0x1F, 0x11, 0x11, 0x0},  // Character: bracketleft
        {0x2, 0x4, 0x8, 0x0},     // Character: backslash
        {0x11, 0x11, 0x1F, 0x0},  // Character: bracketright
        {0x8, 0x4, 0x8, 0x0},     // Character: asciicircum
        {0x4, 0x4, 0x4, 0x0},     // Character: underscore
        {0x4, 0x8, 0x0, 0x0},     // Character: grave
        {0x1A, 0x16, 0x1C, 0x0},  // Character: a
        {0x1F, 0x12, 0xC, 0x0},   // Character: b
        {0xC, 0x12, 0x12, 0x0},   // Character: c
        {0xC, 0x12, 0x1F, 0x0},   // Character: d
        {0xC, 0x1A, 0x16, 0x0},   // Character: e
        {0x4, 0x1E, 0x5, 0x0},    // Character: f
        {0x6, 0x15, 0xF, 0x0},    // Character: g
        {0x1F, 0x2, 0x1C, 0x0},   // Character: h
        {0x1D, 0x0, 0x0, 0x0},    // Character: i
        {0x10, 0x20, 0x1D, 0x0},  // Character: j
        {0x1F, 0xC, 0x12, 0x0},   // Character: k
        {0x11, 0x1F, 0x10, 0x0},  // Character: l
        {0x1E, 0xE, 0x1E, 0x0},   // Character: m
        {0x1E, 0x2, 0x1C, 0x0},   // Character: n
        {0xC, 0x12, 0xC, 0x0},    // Character: o
        {0x1F, 0x9, 0x6, 0x0},    // Character: p
        {0x6, 0x9, 0x1F, 0x0},    // Character: q
        {0x1C, 0x2, 0x2, 0x0},    // Character: r
        {0x14, 0x1E, 0xA, 0x0},   // Character: s
        {0x2, 0x1F, 0x12, 0x0},   // Character: t
        {0xE, 0x10, 0x1E, 0x0},   // Character: u
        {0xE, 0x18, 0xE, 0x0},    // Character: v
        {0x1E, 0x1C, 0x1E, 0x0},  // Character: w
        {0x12, 0xC, 0x12, 0x0},   // Character: x
        {0x3, 0x14, 0xF, 0x0},    // Character: y
        {0x1A, 0x1E, 0x16, 0x0},  // Character: z
        {0x4, 0x1B, 0x11, 0x0},   // Character: braceleft
        {0x1B, 0x0, 0x0, 0x0},    // Character: bar
        {0x11, 0x1B, 0x4, 0x0},   // Character: braceright
        {0x8, 0xC, 0x4, 0x0},     // Character: asciitilde
        {0x1D, 0x0, 0x0, 0x0},    // Character: exclamdown
        {0xE, 0x1B, 0xA, 0x0},    // Character: cent
        {0x14, 0x1F, 0x15, 0x0},  // Character: sterling
        {0x15, 0xE, 0x15, 0x0},   // Character: currency
        {0xB, 0x1C, 0xB, 0x0},    // Character: yen
        {0x1B, 0x0, 0x0, 0x0},    // Character: brokenbar
        {0x14, 0x1B, 0x5, 0x0},   // Character: section
        {0x4, 0x0, 0x4, 0x0},     // Character: dieresis
        {0x4, 0xA, 0xA, 0x0},     // Character: copyright
        {0x16, 0x15, 0x17, 0x0},  // Character: ordfeminine
        {0x4, 0xA, 0x0, 0x0},     // Character: guillemotleft
        {0x4, 0x4, 0xC, 0x0},     // Character: logicalnot
        {0x4, 0x4, 0x0, 0x0},     // Character: softhyphen
        {0xE, 0x6, 0x8, 0x0},     // Character: registered
        {0x4, 0x4, 0x4, 0x0},     // Character: macron
        {0x4, 0xA, 0x4, 0x0},     // Character: degree
        {0x12, 0x17, 0x12, 0x0},  // Character: plusminus
        {0x2, 0xE, 0x8, 0x0},     // Character: twosuperior
        {0xA, 0xE, 0xE, 0x0},     // Character: threesuperior
        {0x8, 0x4, 0x0, 0x0},     // Character: acute
        {0x1F, 0x8, 0x7, 0x0},    // Character: mu
        {0x2, 0x1D, 0x1F, 0x0},   // Character: paragraph
        {0xE, 0xE, 0xE, 0x0},     // Character: periodcentered
        {0x8, 0xA, 0x4, 0x0},     // Character: cedilla
        {0xE, 0x0, 0x0, 0x0},     // Character: onesuperior
        {0x12, 0x15, 0x12, 0x0},  // Character: ordmasculine
        {0xA, 0x4, 0x0, 0x0},     // Character: guillemotright
        {0x3, 0x8, 0x18, 0x0},    // Character: onequarter
        {0xB, 0x18, 0x10, 0x0},   // Character: onehalf
        {0x3, 0xB, 0x18, 0x0},    // Character: threequarters
        {0x18, 0x15, 0x10, 0x0},  // Character: questiondown
        {0x18, 0xD, 0x1A, 0x0},   // Character: Agrave
        {0x1A, 0xD, 0x18, 0x0},   // Character: Aacute
        {0x19, 0xD, 0x19, 0x0},   // Character: Acircumflex
        {0x1A, 0xF, 0x19, 0x0},   // Character: Atilde
        {0x1D, 0xA, 0x1D, 0x0},   // Character: Adieresis
        {0x1F, 0xB, 0x1C, 0x0},   // Character: Aring
        {0x1E, 0x1F, 0x15, 0x0},  // Character: AE
        {0x6, 0x29, 0x19, 0x0},   // Character: Ccedilla
        {0x1C, 0x1D, 0x16, 0x0},  // Character: Egrave
        {0x1E, 0x1D, 0x14, 0x0},  // Character: Eacute
        {0x1D, 0x1D, 0x15, 0x0},  // Character: Ecircumflex
        {0x1D, 0x1C, 0x15, 0x0},  // Character: Edieresis
        {0x14, 0x1D, 0x16, 0x0},  // Character: Igrave
        {0x16, 0x1D, 0x14, 0x0},  // Character: Iacute
        {0x15, 0x1D, 0x15, 0x0},  // Character: Icircumflex
        {0x15, 0x1C, 0x15, 0x0},  // Character: Idieresis
        {0x1F, 0x15, 0xE, 0x0},   // Character: Eth
        {0x1D, 0xB, 0x1E, 0x0},   // Character: Ntilde
        {0x1C, 0x15, 0x1E, 0x0},  // Character: Ograve
        {0x1E, 0x15, 0x1C, 0x0},  // Character: Oacute
        {0x1D, 0x15, 0x1D, 0x0},  // Character: Ocircumflex
        {0x1D, 0x17, 0x1E, 0x0},  // Character: Otilde
        {0x1D, 0x14, 0x1D, 0x0},  // Character: Odieresis
        {0xA, 0x4, 0xA, 0x0},     // Character: multiply
        {0x1E, 0x15, 0xF, 0x0},   // Character: Oslash
        {0x1D, 0x12, 0x1C, 0x0},  // Character: Ugrave
        {0x1C, 0x12, 0x1D, 0x0},  // Character: Uacute
        {0x1D, 0x11, 0x1D, 0x0},  // Character: Ucircumflex
        {0x1D, 0x10, 0x1D, 0x0},  // Character: Udieresis
        {0xC, 0x1A, 0xD, 0x0},    // Character: Yacute
        {0x1F, 0xA, 0xE, 0x0},    // Character: Thorn
        {0x3E, 0x15, 0xB, 0x0},   // Character: germandbls
        {0x18, 0x15, 0x1E, 0x0},  // Character: agrave
        {0x1A, 0x15, 0x1C, 0x0},  // Character: aacute
        {0x19, 0x15, 0x1D, 0x0},  // Character: acircumflex
        {0x1A, 0x17, 0x1D, 0x0},  // Character: atilde
        {0x19, 0x14, 0x1D, 0x0},  // Character: adieresis
        {0x18, 0x17, 0x1F, 0x0},  // Character: aring
        {0x1C, 0x1E, 0xE, 0x0},   // Character: ae
        {0x2, 0x15, 0xD, 0x0},    // Character: ccedilla
        {0x8, 0x1D, 0x1E, 0x0},   // Character: egrave
        {0xA, 0x1D, 0x1C, 0x0},   // Character: eacute
        {0x9, 0x1D, 0x1D, 0x0},   // Character: ecircumflex
        {0x9, 0x1C, 0x1D, 0x0},   // Character: edieresis
        {0x1D, 0x2, 0x0, 0x0},    // Character: igrave
        {0x2, 0x1D, 0x0, 0x0},    // Character: iacute
        {0x1, 0x1D, 0x1, 0x0},    // Character: icircumflex
        {0x1, 0x1C, 0x1, 0x0},    // Character: idieresis
        {0xA, 0x17, 0x1D, 0x0},   // Character: eth
        {0x1D, 0x7, 0x1A, 0x0},   // Character: ntilde
        {0x8, 0x15, 0xA, 0x0},    // Character: ograve
        {0xA, 0x15, 0x8, 0x0},    // Character: oacute
        {0x9, 0x15, 0x9, 0x0},    // Character: ocircumflex
        {0x9, 0x17, 0xA, 0x0},    // Character: otilde
        {0x9, 0x14, 0x9, 0x0},    // Character: odieresis
        {0x4, 0x15, 0x4, 0x0},    // Character: divide
        {0x1C, 0x16, 0xE, 0x0},   // Character: oslash
        {0xD, 0x12, 0x1C, 0x0},   // Character: ugrave
        {0xC, 0x12, 0x1D, 0x0},   // Character: uacute
        {0xD, 0x11, 0x1D, 0x0},   // Character: ucircumflex
        {0xD, 0x10, 0x1D, 0x0},   // Character: udieresis
        {0x4, 0x2A, 0x1D, 0x0},   // Character: yacute
        {0x1F, 0xA, 0x4, 0x0},    // Character: thorn
        {0x5, 0x28, 0x1D, 0x0},   // Character: ydieresis
        {0x0, 0x0, 0x0, 0x0},     // Character: gcircumflex
        {0xE, 0x1F, 0x15, 0x0},   // Character: OE
        {0x1C, 0x1E, 0x16, 0x0},  // Character: oe
        {0x15, 0x1E, 0xB, 0x0},   // Character: Scaron
        {0x15, 0x1E, 0xB, 0x0},   // Character: scaron
        {0x5, 0x18, 0x5, 0x0},    // Character: Ydieresis
        {0x1B, 0x1E, 0x17, 0x0},  // Character: Zcaron
        {0x1B, 0x1E, 0x17, 0x0},  // Character: zcaron
        {0x0, 0x0, 0x0, 0x0},     // Character: uni0EA4
        {0x0, 0x0, 0x0, 0x0},     // Character: uni13A0
        {0x4, 0x0, 0x0, 0x0},     // Character: bullet
        {0x4, 0x0, 0x4, 0x0},     // Character: ellipsis
        {0xE, 0x1F, 0x17, 0x0},   // Character: Euro
    };
};

#endif // EVT_BITMAPFONTS_HPP
