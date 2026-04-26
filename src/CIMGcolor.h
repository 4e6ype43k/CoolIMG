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
#ifndef CIMGCOLOR
#define CIMGCOLOR

#include "toolkit.h"

#define BLANK (Color) {0,0,0,0}
#define BLACK (Color) {0,0,0,255}
#define WHITE (Color) {255,255,255,255}
#define RED (Color) {255,0,0,255}
#define GREEN (Color) {0,255,0,255}
#define BLUE (Color) {0,0,255,255}

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;

// this feels like reinventing fire
// add is prioritized
Color mixColors(Color base,Color add) {
    if (add.a==255) { // if add.a is full, the returned Color will be add
        return add;
    } else if (add.a==0) { // if it is zero, base will be returned
        return base;
    } else if (base.a==0) { // but if base.a is 0, add wil be returned again
        return add;
    } else { // no
        uint16_t alphaSum=base.a+add.a; // will be useful as we are splitting the Color channels by ratio
        uint16_t r=(base.r*base.a/alphaSum)+(add.r*add.a/alphaSum); // checking if any channels go over 255 (2^8-1)
        uint16_t g=(base.g*base.a/alphaSum)+(add.g*add.a/alphaSum); // all int so no float
        uint16_t b=(base.b*base.a/alphaSum)+(add.b*add.a/alphaSum);

        if (r>255) r=255; // yes, you can just do that
        if (g>255) g=255;
        if (b>255) b=255;

        if (r==254) r=255; // slight error correction
        if (g==254) g=255;
        if (b==254) b=255;

        return (Color) {r,g,b,255}; // next customer
    }
}

// hm i wonder what this does
// hex is either 1 or 0, and if 1, prints the RGBA as hex
void printColor(Color clr,uint8_t hex) {
    if (!hex) {
        printf("%d,%d,%d,%d",clr.r,clr.g,clr.b,clr.a);
    } else {
        printf("0x%x %x %x %x",clr.r,clr.g,clr.b,clr.a);
    }
}

#endif