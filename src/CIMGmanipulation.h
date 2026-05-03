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

// draws something but i cant really figure out what exactly
void drawTriangleWireframe(PixelData* data,Color clr, uint16_t pos0[2],uint16_t pos1[2],uint16_t pos2[2]) {
    drawLine(data,clr,pos0,pos1); // this is horrendously easy
    drawLine(data,clr,pos1,pos2);
    drawLine(data,clr,pos0,pos2);
}

// help me
void drawTriangleFilled(PixelData* data,Color clr,uint16_t pos0[2],uint16_t pos1[2],uint16_t pos2[2]) {
    uint16_t centerX=ceil((double) (pos0[0]+pos1[0]+pos2[0])/3); // all points will slowly approach the center and on each iteration, will draw a wireframe
    uint16_t centerY=ceil((double) (pos0[1]+pos1[1]+pos2[1])/3); // i know there are other methods to do this but they are kind of complicated

    int16_t x0add=pos0[0]<centerX ? 1:-1; // if x0<centerX, we will increase it, otherwise, we will decrease it
    int16_t y0add=pos0[1]<centerY ? 1:-1;
    int16_t x1add=pos1[0]<centerX ? 1:-1;
    int16_t y1add=pos1[1]<centerY ? 1:-1;
    int16_t x2add=pos2[0]<centerX ? 1:-1;
    int16_t y2add=pos2[1]<centerY ? 1:-1;

    uint8_t x0stop=0; // to check if x0==centerX
    uint8_t y0stop=0;
    uint8_t x1stop=0;
    uint8_t y1stop=0;
    uint8_t x2stop=0;
    uint8_t y2stop=0;

    while (!(x0stop&&y0stop&&x1stop&&y1stop&&x2stop&&y2stop)) { // why
        drawTriangleWireframe(data,clr,pos0,pos1,pos2); // one of many
        if (pos0[0]==centerX) { // wow
            x0stop=1;
        } else {
            pos0[0]+=x0add;
        }
        if (pos0[1]==centerY) {
            y0stop=1;
        } else {
            pos0[1]+=y0add;
        }

        if (pos1[0]==centerX) {
            x1stop=1;
        } else {
            pos1[0]+=x1add;
        }
        if (pos1[1]==centerY) {
            y1stop=1;
        } else {
            pos1[1]+=y1add;
        }

        if (pos2[0]==centerX) {
            x2stop=1;
        } else {
            pos2[0]+=x2add;
        }
        if (pos2[1]==centerY) {
            y2stop=1;
        } else {
            pos2[1]+=y2add;
        }
    }
}

// scales the dimensions of the struct by the scale factor (stretches pixels too) and returns new PD
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

// takes a chunk of the struct between two points and returns the pixel data of the area (if this makes any sense)
PixelData cropImage(PixelData data,uint16_t pos0[2],uint16_t pos1[2]) {
    uint16_t minX; // setting the range for the function to work on
    uint16_t maxX;
    uint16_t maxY;
    uint16_t minY;

    if (pos0[0]>pos1[0]) { // i could do a fancy-shmancy ternary operator thingy but then i would need to do it twice for each axis
        maxX=pos0[0];
        minX=pos1[0];
    } else {
        maxX=pos1[0];
        minX=pos0[0];
    }

    if (pos0[1]>pos1[1]) { // again
        maxY=pos0[1];
        minY=pos1[1];
    } else {
        maxY=pos1[1];
        minY=pos0[1];
    }

    PixelData newData={maxX-minX+1,maxY-minY+1,NULL}; //? +1 because i dont want 0-wide arrays
    allocPixelMemory(&newData);

    for (uint16_t x=minX;x<=maxX;x++) { // from smallest to largest INCLUSIVE (because otherwise if any of the axis are the same for p0 and p1, the func will output nothing)
        uint16_t newX=x-minX; // the x of newData
        for (uint16_t y=minY;y<=maxY;y++) {
            uint16_t newY=y-minY;
            drawPoint(&newData,data.pixels[posToIndex(data,(uint16_t[2]) {x,y})],(uint16_t[2]) {newX,newY}); // im so sorry to PUSH UR T3MPRR so much with this bad code
        }                                                                                                    // i need to ACT RIGHT more often
    }                                                                                                        // this is probably because I MIGHT B3 SICK
    
    return newData;
}

// adds add pixels to base, starting from pos
PixelData fuseImages(PixelData* base, PixelData add, uint16_t pos[2]) {
    uint16_t maxX=pos[0]+add.width; // the max x we will reach in the loop
    uint16_t maxY=pos[1]+add.height; // same but for y

    for (uint16_t x=pos[0];x<maxX;x++) {
        uint16_t addX=x-pos[0]; // x of add
        for (uint16_t y=pos[1];y<maxY;y++) {
            // dont really need addY, cos it'll be used only once
            drawPoint(base,add.pixels[posToIndex(add,(uint16_t[2]) {addX,y-pos[1]})],(uint16_t[2]) {x,y}); // hmmm, what should i reference now
        }
    }
}

#endif