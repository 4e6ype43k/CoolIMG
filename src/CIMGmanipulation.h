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

#ifndef CIMG_M_H
#define CIMG_M_H

#include "coolIMG.h" // this current header (NOT coolIMG.h) acts as an extension, so is dependant on the included header

void drawPoint(PixelData* data, Color color,uint16_t pos[2]) {
    data->pixels[posToIndex(*data,pos)]=color; // wow
}

// scales the dimensions of the struct by the scale factor (stretches pixels too)
// ! DOESN'T SUPPORT FLOAT SCALE FACTORS (yet)
PixelData scaleBy(PixelData data, uint16_t scale[2]) {
    PixelData newData={data.width*scale[0],data.height*scale[1],NULL};
    allocPixelMemory(&newData);
    for (uint16_t x=0;x<newData.width;x++) { // goes over the width of the new struct
        uint16_t actualX=(x-(x%scale[0]))/scale[0]; // the x of the input PD
        for (uint16_t y=0;y<newData.height;y++) { // the height
            uint16_t actualY=(y-(y%scale[1]))/scale[1]; // same but for y
            drawPoint(&newData,data.pixels[posToIndex(data,(uint16_t[2]) {actualX,actualY})],(uint16_t[2]) {x,y}); // theres no way this actually works
        }
    }
    return newData;
}

#endif