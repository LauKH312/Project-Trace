/*
 * bitfont.h
 *
 *  Created on: May 9, 2026
 *      Author: Laurits
 */

#ifndef INC_BITFONT_H_
#define INC_BITFONT_H_
#include <stdint.h>

#define GLYPH_W 8
#define GLYPH_H 8
#define GLYPH(...) ((Glyph) { .data = {__VA_ARGS__}})


typedef struct {
	uint8_t data[GLYPH_H];
} Glyph;

extern const Glyph GLYPHS[94];

Glyph bitfont_ascii_to_glyph(char ch);

#endif /* INC_BITFONT_H_ */
