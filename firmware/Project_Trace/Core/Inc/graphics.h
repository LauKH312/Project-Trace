/*
 * graphics.h
 *
 *  Created on: Jun 9, 2026
 *      Author: Laurits
 */

#ifndef INC_GRAPHICS_H_
#define INC_GRAPHICS_H_

#include <bitfont.h>

void graphics_draw_horizontal_line(int x1, int x2, int y);
void graphics_draw_vertical_line(int x, int y1, int y2);

void graphics_draw_line(int x1, int y1, int x2, int y2);
void graphics_draw_rect(int x1, int y1, int x2, int y2);
void graphics_draw_rect_rel(int x, int y, int w, int h);
void graphics_draw_rectf(int x, int y, int w, int h);

void graphics_blit(int x, int y, int w, int h, uint8_t** data);
void graphics_clear(void);

void graphics_draw_glyph(int x, int y, Glyph g);
void graphics_draw_text(int x0, int y0, char* text);

#endif /* INC_GRAPHICS_H_ */
