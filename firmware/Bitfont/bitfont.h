#pragma once

/*
    MultilloMono Bitfont
        by Laurits Hillemann                     
*/
                    
#include <stdint.h>
#define GLYPH_W 8
#define GLYPH_H 8
                    
typedef struct {
    uint8_t data[GLYPH_H];
} Glyph;
                    
const char BITFONT_MIN_ASCII = 33;
const char BITFONT_MAX_ASCII = 126;

