/*
 * graphics.c
 *
 *  Created on: Jun 9, 2026
 *      Author: Laurits
 */


#include <graphics.h>
#include <fixpoint.h>
#include <hal_oled.h>

#include <mathdefs.h>

extern uint8_t framebuffer[OLED_PAGES][OLED_WIDTH];

// TODO: create optimized routine
void draw_horizontal_line(int x1, int x2, int y) {
	for(int i = MIN(x1,x2); i <= MAX(x1,x2); i++) {
		hal_oled_drawpixel(i,y, HalOledDrawOn);
	}
}

void draw_vertical_line(int x, int y1, int y2) {
	for(int j = MIN(y1,y2); j <= MAX(y1,y2); j++) {
		hal_oled_drawpixel(x,j, HalOledDrawOn);
	}
}


// TODO: Fix missing pixels for steep lines.
void graphics_draw_line(int x1, int y1, int x2, int y2) {
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if (dx == 0) {
		draw_vertical_line(x1,y1,y2);
		return;
	}
	if (dy == 0) {
		draw_horizontal_line(x1,x2,y1);
		return;
	}

	fix9_23 dydx = fix9_23_frac(dy,dx);

	for (int i = MIN(x1,x2); i < MAX(x1,x2); i++) {
		fix9_23 y = fix9_23_add(fix9_23_mul_int(dydx,i), fix9_23_int(y1));
		int32_t yint = fix9_23_truncate(fix9_23_add(y, fix9_23_frac(1,2))); // Add 1/2 to get rounding behaviour.

		hal_oled_drawpixel(i,yint, HalOledDrawOn);
	}
}

void graphics_draw_rect(int x1, int y1, int x2, int y2) {
	draw_horizontal_line(x1,x2,y1);
	draw_horizontal_line(x1,x2,y2);

	draw_vertical_line(x1,y1,y2);
	draw_vertical_line(x2,y1,y2);
}

void graphics_draw_rect_rel(int x, int y, int w, int h) {
	graphics_draw_rect(x, y, x+w, y+h);
}

// TODO create optimized routine
void graphics_draw_rectf(int x, int y, int w, int h) {
	for (int i = x; i < x + w; i++) {
		for (int j = y; j < y + h; j++) {
			hal_oled_drawpixel(i,j, HalOledDrawOn);
		}
	}
}


void graphics_blit(int x, int y, int w, int h, uint8_t** data) {
	for (int i = x; i < x + w; i++) {
		for (int j = y; j < y + h; j++) {
			if ((data[j / 8][i] & (1 << (j % 8))) != 0) {
				hal_oled_drawpixel(i,j,HalOledDrawOn);
			} else {
				hal_oled_drawpixel(i,j,HalOledDrawOff);
			}
		}
	}
}

void graphics_clear(void) {
	memset(framebuffer, 0x0, sizeof(framebuffer));
}

void graphics_draw_glyph(int x, int y, Glyph g) {
	graphics_blit(x,y, 8, 8, (uint8_t**)g.data);
}

void graphics_draw_text(int x0, int y0, char* text) {
	int x = x0; int y = y0;

	const int MAX_ITER = 128;
	for (int i = 0; i < MAX_ITER; i++) {
		const char ch = text[i];
		if (ch == 0) break;
		if (ch == '\n') {
			x = x0;
			y += 8;
		}

		const Glyph gl = bitfont_ascii_to_glyph(ch);
		graphics_draw_glyph(x,y,gl);
	}
}
