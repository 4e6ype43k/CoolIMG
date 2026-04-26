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
#include <math.h> // well thats a prob-lm

// if clr.a is 0, then it just won't draw anything
void drawPoint(PixelData* data, Color clr,uint16_t pos[2]) {
    if (clr.a>0){
        data->pixels[posToIndex(*data,pos)]=mixColors(data->pixels[posToIndex(*data,pos)],clr); // wow
    }
}

void fill(PixelData* data,Color clr) {
    for (uint16_t x=0;x<data->width;x++) {
        for (uint16_t y=0; y<data->height; y++) { // note to future me: always remember to swap vars in for loops when you ctrl+c ctrl+v them, otherwise they'll never stop, please
            drawPoint(data,clr,(uint16_t[2]) {x,y}); // its this simple
        }
    }
}

// draws a line between two points
void drawLine(PixelData* data,Color clr,uint16_t pos0[2],uint16_t pos1[2]) {
    if (pos0[1]!=pos1[1]) { // checks if y isn't the same
        if (pos0[1]>pos1[1]) { // going over the height first
            double gradientInverse=(double) (pos0[0]-pos1[0])/(pos0[1]-pos1[1]); // gradient relative to y
            int16_t offset=pos0[0]-pos0[1]*gradientInverse; // uhhhh

            for (uint16_t y=pos1[1];y<=pos0[1];y++) { // trust the process
                uint16_t x=ceil(y*gradientInverse+offset);
                drawPoint(data,clr,(uint16_t[2]) {x,y});
            }
        } else { // through the mirror
            double gradientInverse=(double) (pos1[0]-pos0[0])/(pos1[1]-pos0[1]);
            int16_t offset=pos0[0]-pos0[1]*gradientInverse;

            for (uint16_t y=pos0[1];y<=pos1[1];y++) {
                uint16_t x=ceil(y*gradientInverse+offset);
                drawPoint(data,clr,(uint16_t[2]) {x,y});
            }
        }
    } else { // wide boi
        if (pos0[0]>pos1[0]) {
            double gradient=(double) (pos0[1]-pos1[1])/(pos0[0]-pos1[0]); // gradient relative to x
            int16_t offset=pos0[1]-pos0[0]*gradient; // uhhhh™

            for (uint16_t x=pos1[1];x<=pos0[1];x++) {
                uint16_t y=ceil(x*gradient+offset);
                drawPoint(data,clr,(uint16_t[2]) {x,y});
            }
        } else {
            double gradient=(double) (pos1[1]-pos0[1])/(pos1[0]-pos0[0]);
            int16_t offset=pos0[1]-pos0[0]*gradient;

            for (uint16_t x=pos0[0];x<=pos1[0];x++) {
                uint16_t y=ceil(x*gradient+offset);
                drawPoint(data,clr,(uint16_t[2]) {x,y});
            }
        }
    }
} // theres no way this worked first try
// ^ not anymore tho (well, not for lines where gradient!=1)

// help me
// had to rewrite drawLine() for this btw
// setting fill.a as 0 will not draw the inside
// setting outline.a as 0 will not draw the outline 
void drawTriangle(PixelData* data, uint16_t pos[3][2],Color outline,Color fill) {
    if (fill.a!=0) { // if alpha is 0, why draw the inside?
        uint16_t minY=-1; // trying to find the range of y
        uint16_t minIndex=0; // index of smallest y
        uint16_t maxY=0; // minY is -1 so that every value would (hopefully) be less than it
        for (uint8_t x=0;x<3;x++) { // comparing ys to minY and maxY
            if (minY>pos[x][1]) {minY=pos[x][1]; minIndex=x;}
            if (maxY<pos[x][1]) maxY=pos[x][1];
        }
        

        for (uint16_t y=minY;y<maxY;y++) { // we know the y values, but now we need the x values
        }
    }

    if (outline.a!=0) { // same here
            drawLine(data,outline,pos[0],pos[1]);
            drawLine(data,outline,pos[1],pos[2]);
            drawLine(data,outline,pos[0],pos[2]);
        }
}

// scales the dimensions of the struct by the scale factor (stretches pixels too)
//! DOESN'T SUPPORT FLOAT SCALE FACTORS (yet)
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