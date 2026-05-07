/*

Copyright (c) 2026 4e6ype43k

Permission is hereby granted, free of charge,
to any person obtaining a copy of this software
and associated documentation files (the "Software"),
to deal in the Software without restriction,
including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to
whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

*/

#ifndef CIMG_CLR_H
#define CIMG_CLR_H

#include "coolIMG.h"

#define BLANK (Color) {0,0,0,0}
#define BLACK (Color) {0,0,0,255}
#define WHITE (Color) {255,255,255,255}
#define RED (Color) {255,0,0,255}
#define GREEN (Color) {0,255,0,255}
#define BLUE (Color) {0,0,255,255}

typedef enum {
    DENARY, // to print as base10
    HEX
} COLOR_PRINT_TYPE; // only used for printColor()

// hm i wonder what this does
void printColor(Color clr,COLOR_PRINT_TYPE printType) {
    if (!printType) {
        printf("%d,%d,%d,%d",clr.r,clr.g,clr.b,clr.a);
    } else {
        printf("0x%x 0x%x 0x%x 0x%x",clr.r,clr.g,clr.b,clr.a);
    }
}

typedef enum { // TODO add all orders
    RGBA,
    RGAB, // so many spell checks
    RAGB, // 4!=4*3*2*1=... 24... damn
    ARGB,

    GBAR, // we (yes WE) are ALL using GBAR
    GBRA,
    GRBA,
} COLOR_UINT_TYPE; // only used for the function below

// converts Color struct to uint32_t
uint32_t colorToInt(Color clr,COLOR_UINT_TYPE order){
    switch (order) {
        case ARGB:
        return clr.a*16777216+clr.r*65536+clr.g*256+clr.b; // why am i doing this
        break;

        case RGAB:
        return clr.r*16777216+clr.g*65536+clr.a*256+clr.b;
        break;

        case RAGB:
        return clr.r*16777216+clr.a*65536+clr.g*256+clr.b;
        break;

        case GBAR:
        return clr.g*16777216+clr.b*65536+clr.a*256+clr.r;
        break;

        case GBRA:
        return clr.g*16777216+clr.b*65536+clr.r*256+clr.a;
        break;

        case GRBA:
        return clr.g*16777216+clr.r*65536+clr.b*256+clr.a;
        break;

        default: // would return RGBA as default
        return clr.r*16777216+clr.g*65536+clr.b*256+clr.a; // why am i doing this
    }
}

#endif