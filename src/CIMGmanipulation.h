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

//* this file is for manipulating PixelData struct, like drawing shapes

#include "coolIMG.h"

void drawPoint(PixelData* data, Color color,uint16_t pos[2]) {
    data->pixels[posToIndex(*data,pos)]=color; // wow
}

// scales the dimensions of the struct by the scale factor (stretches pixels too)
// ! DOESN'T SUPPORT FLOAT SCALE FACTORS (yet)
PixelData scaleBy(PixelData data, uint16_t scale[2]) {
    PixelData newData={data.width*scale[0],data.height*scale[1],NULL};
    allocPixelMemory(&newData);
    for (uint32_t x=0;x<data.width*scale[0];x+=scale[0]){ // index 0 is x
        for (int16_t countX=scale[0];countX>=0;countX--) { // huh
            for (uint32_t y=0; y<data.height*scale[1];y+=scale[1]) {
                for (int16_t countY=scale[1];countY>=0;countY--) {
                    drawPoint(&newData,data.pixels[posToIndex(data,(uint16_t[2]) {(x/scale[0])+countX,(y/scale[1])+countY})], (uint16_t[2]) {x+countX,y+countY}); // please work
                }
            }
        }
    }
    for (int x=0;x<newData.width*newData.height;x++) {
        printColor(newData.pixels[x],1);
        printf("\n");
    }
    return newData;
}