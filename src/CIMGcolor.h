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

#include "coolIMG.h"

#define BLANK (Color) {0,0,0,0}
#define BLACK (Color) {0,0,0,255}
#define WHITE (Color) {255,255,255,255}
#define RED (Color) {255,0,0,255}
#define GREEN (Color) {0,255,0,255}
#define BLUE (Color) {0,0,255,255}

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