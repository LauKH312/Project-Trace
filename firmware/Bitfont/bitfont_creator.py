import json

GLYPH_SLICE_START = 4
GLYPH_SLICE_END = 12
GLYPH_FIRST_BIT = 2
GLYPH_LAST_BIT = 10

with open("MultilloMono.json") as f:
    data = json.load(f)
    keys = [el for el in filter(lambda x: x.isnumeric(), data.keys())]
    # print(keys)
    # print(int(keys[0]))
    
    # print(chr(67))

    # print(data["67"])

    # for key in keys:
    #     data[key] = data[key][GLYPH_SLICE_START:GLYPH_SLICE_END]
    #     print(f"{chr(int(key))}:")
    #     for line in data[key]:
    #         chars = ["-" for i in range(GLYPH_FIRST_BIT, GLYPH_LAST_BIT)]
    #         for i in range(GLYPH_FIRST_BIT, GLYPH_LAST_BIT):
    #             if (line & (1 << i) != 0): chars[i-GLYPH_FIRST_BIT] = "#"

    #         print(''.join(chars))
    #     print()

    with open("bitfont.c", mode="w") as out_src:
        # nglyphs = len(keys)

        byterange = int(keys[-1]) - int(keys[0])
        glyph_arr_len = byterange + 1
        # print(byterange, keys[-1])

        out_src.write(f"""
/*
    MultilloMono Bitfont
        by Laurits Hillemann                     
*/

#include <bitfont.h>
                      
#define GLYPH(...) ((Glyph) {{ .data = {{__VA_ARGS__}}}})


                     
const Glyph GLYPHS[{glyph_arr_len}] = {{\n""")
        for i in range(int(keys[0]), int(keys[-1]) + 1):

            key = f"{i}"
            if key not in keys:
                out_src.write(f"{{0}},\n")
                continue

            # key = keys[f"{i}"]
            out_src.write("GLYPH(" + ", ".join([f"0x{data[key][row] >> GLYPH_FIRST_BIT:02x}" for row in range(GLYPH_SLICE_START,GLYPH_SLICE_END)]) + "), " + f"/* {chr(int(key))} */\n")
            
        out_src.write("};\n")
 
        out_src.write("const Glyph UNKNOWN_CHARACTER = GLYPH(0x7f, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7f);")

    with open("bitfont.h", mode="w") as out_h:
        out_h.write("#pragma once\n")

        out_h.write(f"""
/*
    MultilloMono Bitfont
        by Laurits Hillemann                     
*/
                    
#include <stdint.h>
#define GLYPH_W 8
#define GLYPH_H 8
                    
typedef struct {{
    uint8_t data[GLYPH_H];
}} Glyph;
                    
const char BITFONT_MIN_ASCII = {keys[0]};
const char BITFONT_MAX_ASCII = {keys[-1]};

""")
