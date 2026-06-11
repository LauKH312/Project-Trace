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
void graphics_draw_horizontal_line(int x1, int x2, int y) {
	if (y < 0 || y >= OLED_HEIGHT) return;

	for(int i = MIN(x1,x2); i <= MAX(x1,x2); i++) {
		hal_oled_drawpixel(i,y, HalOledDrawOn);
	}
}

void graphics_draw_vertical_line(int x, int y1, int y2) {
	if (x < 0 || x >= OLED_WIDTH) return;

	for(int j = MIN(y1,y2); j <= MAX(y1,y2); j++) {
		hal_oled_drawpixel(x,j, HalOledDrawOn);
	}
}


void graphics_draw_line(int x1, int y1, int x2, int y2) {
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if (dx == 0) {
		graphics_draw_vertical_line(x1,y1,y2);
		return;
	}
	if (dy == 0) {
		graphics_draw_horizontal_line(x1,x2,y1);
		return;
	}

	//fix9_23 dist = fix9_23_sqrt(fix9_23_add(fix9_23_sqr(fix9_23_int(dx)), fix9_23_sqr(fix9_23_int(dy))));
	//int32_t distd = fix9_23_trunc(dist) + 1;

	//int32_t distd2 = dx*dx + dy*dy;
	//fix9_23 dt = fix9_23_frac(1, distd);

	//fix9_23 dydx = fix9_23_frac(dy,dx);

	int32_t minimum_steps = dy + dx;
	fix9_23 dt = fix9_23_frac(1,minimum_steps);

	fix9_23 dxdt = fix9_23_mul_int(dt, dx);
	fix9_23 dydt = fix9_23_mul_int(dt, dy);

	for (int i = 0; i < minimum_steps; i++) {

		fix9_23 x = fix9_23_add(fix9_23_int(x1), fix9_23_mul_int(dxdt,i));
		fix9_23 y = fix9_23_add(fix9_23_int(y1), fix9_23_mul_int(dydt,i));

		hal_oled_drawpixel(fix9_23_round(x),fix9_23_round(y), HalOledDrawOn);
	}

	/*for (int i = MIN(x1,x2); i < MAX(x1,x2); i++) {
		fix9_23 y = fix9_23_add(fix9_23_mul_int(dydx,i), fix9_23_int(y1));
		int32_t yint = fix9_23_round(y); // Add 1/2 to get rounding behaviour.

		hal_oled_drawpixel(i,yint, HalOledDrawOn);
	}*/
}

void graphics_draw_rect(int x1, int y1, int x2, int y2) {
	graphics_draw_horizontal_line(x1,x2,y1);
	graphics_draw_horizontal_line(x1,x2,y2);

	graphics_draw_vertical_line(x1,y1,y2);
	graphics_draw_vertical_line(x2,y1,y2);
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


// TODO: TESTME
void graphics_blit(int x, int y, int w, int h, uint8_t** data) {
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if ((data[j / 8][i] & (1 << (j % 8))) != 0) {
				hal_oled_drawpixel(x+i,y+j,HalOledDrawOn);
			} else {
				hal_oled_drawpixel(x+i,y+j,HalOledDrawOff);
			}
		}
	}
}

void graphics_clear(void) {
	memset(framebuffer, 0x0, sizeof(framebuffer));
}

void graphics_draw_glyph(int x, int y, Glyph g) {
	//graphics_blit(x,y, 8, 8, (uint8_t**)g.data);
	const int w = 8;
	const int h = 8;

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			//if ((g.data[i] & (1 << (j % 8))) != 0) {

			// NOTE: USES TRANSPOSE, SINCE BITFONT IS IN WRONG FORMAT
			if ((g.data[i] & (1 << (j % 8))) != 0) {
				hal_oled_drawpixel(x+j,y+i,HalOledDrawOn);
			} else {
				hal_oled_drawpixel(x+j,y+i,HalOledDrawOff);
			}
		}
	}
}

void graphics_draw_text(int x0, int y0, char* text) {
	int x = x0; int y = y0;

	const int MAX_ITER = 128;
	for (int i = 0; i < MAX_ITER; i++) {
		const char ch = text[i];
		if (ch == 0) break;
		if (ch == ' ') {
			x += 8;
			continue;
		}
		if (ch == '\n') {
			x = x0;
			y += 8;
			continue;
		}

		const Glyph gl = bitfont_ascii_to_glyph(ch);
		graphics_draw_glyph(x,y,gl);
		x += 8;
	}
}
