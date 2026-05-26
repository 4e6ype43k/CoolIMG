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

#pragma region SET_COLOR

// if clr.a is 0, then it just won't draw anything
void drawPoint(PixelData* data, Color clr,uint16_t pos[2]) {
    if (clr.a>0){
        data->pixels[posToIndex(*data,pos)]=mixColors(data->pixels[posToIndex(*data,pos)],clr); // wow
    }
}

// sets all Color structs in data to be of the same color
void fill(PixelData* data,Color clr) {
    for (uint16_t x=0;x<data->width;x++) {
        for (uint16_t y=0; y<data->height; y++) { // note to future me: always remember to swap vars in for loops when you ctrl+c ctrl+v them, otherwise they'll never stop, please
            drawPoint(data,clr,(uint16_t[2]) {x,y}); // its this simple
        }
    }
}

// replaces all instances of clr0 with clr1
void replaceColor(PixelData* data,Color clr0,Color clr1) {
    for (uint16_t x=0;x<data->width;x++) {
        for (uint16_t y=0;y<data->height;y++) {
            // HERE IT COMES!
            if (data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].r==clr0.r||data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].g==clr0.g||data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].b==clr0.b||data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].a==clr0.a){
                drawPoint(data,clr1,(uint16_t[2]) {x,y});
            }
        }
    }
}

// replaceColor() but for an area
void replaceColorArea(PixelData* data,Color clr0,Color clr1, uint16_t pos0[2], uint16_t pos1[2]) {
    uint16_t minX; // reduce, reuse, recycle
    uint16_t maxX;
    uint16_t maxY;
    uint16_t minY;

    if (pos0[0]>pos1[0]) {
        maxX=pos0[0];
        minX=pos1[0];
    } else {
        maxX=pos1[0];
        minX=pos0[0];
    }

    if (pos0[1]>pos1[1]) {
        maxY=pos0[1];
        minY=pos1[1];
    } else {
        maxY=pos1[1];
        minY=pos0[1];
    }

    for (uint16_t x=minX;x<=maxX;x++) {
        for (uint16_t y=minY;y<=maxY;y++) { // hope this works
            if (data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].r==clr0.r||data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].g==clr0.g||data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].b==clr0.b||data->pixels[posToIndex(*data,(uint16_t[2]) {x,y})].a==clr0.a){
                drawPoint(data,clr1,(uint16_t[2]) {x,y});
            }
        }
    }
}

#pragma endregion

#pragma region INVERT

// inverts color at pos
void invertPoint(PixelData* data, uint16_t pos[2]) {
    Color clr=data->pixels[posToIndex(*data,pos)]; // storing the color for processing
    Color newColor={255-clr.r,255-clr.g,255-clr.b,clr.a}; // 255-rgb is pretty much inverting the color
    //* a is the same cos... we dont invert alpha ok?
    drawPoint(data,newColor,pos);
}

// inverts all pixels between p0 and p1
void invertArea(PixelData* data, uint16_t pos0[2], uint16_t pos1[2]) {
    uint16_t minX; // reduce, reuse, recycle
    uint16_t maxX;
    uint16_t maxY;
    uint16_t minY;

    if (pos0[0]>pos1[0]) {
        maxX=pos0[0];
        minX=pos1[0];
    } else {
        maxX=pos1[0];
        minX=pos0[0];
    }

    if (pos0[1]>pos1[1]) {
        maxY=pos0[1];
        minY=pos1[1];
    } else {
        maxY=pos1[1];
        minY=pos0[1];
    }

    for (uint16_t x=minX;x<=maxX;x++) {
        for (uint16_t y=minY;y<=maxY;y++) {
            invertPoint(data,(uint16_t[2]) {x,y}); // why
        }
    }
}

// inverts all pixels in PD
void invertFill(PixelData* data){
    invertArea(data,(uint16_t[2]) {0,0},(uint16_t[2]) {data->width-1,data->height-1}); // wow
}

#pragma endregion

#pragma region SHAPE_DRAW

// draws a line between two points
// TODO add width
void drawLine(PixelData* data,Color clr,uint16_t pos0[2],uint16_t pos1[2]) {
    if (pos0[1]!=pos1[1]) { // checks if y isn't the same
        if (pos0[1]>pos1[1]) { // going over the height first
            double gradientInverse=(double) (pos0[0]-pos1[0])/(pos0[1]-pos1[1]); // gradient relative to y
            int16_t offset=pos0[0]-pos0[1]*gradientInverse; // uhhhh

            for (uint16_t y=pos1[1];y<=pos0[1];y++) { // trust the process
                uint16_t x=ceil(y*gradientInverse+offset); //? why ceil? because I WANT IT, I WANT IT QUITE A LOT! I'M IN IT COS I CAN BE, YOU CAN'T TELL ME THAT I'M NOT!
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

            for (uint16_t x=pos1[0];x<=pos0[0];x++) {
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
// TODO add width
void drawTriangleWireframe(PixelData* data,Color clr, uint16_t pos[3][2]) {
    drawLine(data,clr,pos[0],pos[1]); // this is horrendously easy
    drawLine(data,clr,pos[1],pos[2]);
    drawLine(data,clr,pos[0],pos[2]);
}

// uses a new triangle draw algorithm cos the last one SUCKED
void drawTriangleFilled(PixelData* data,Color clr,uint16_t pos[3][2]){
    /*
    * new algorithm in pseudocode:

    find min, mid and max ys

    find the gradients of all lines

    for y between minY and maxY
        find minX and maxX

        draw a line between (y,minX) and (y,maxX) on data of color clr
    */

    // 1. find min, mid and max ys

    uint16_t minY=0; // first, we will store indices of the ys
    uint16_t maxY=0;

    for (uint8_t x=0; x<3; x++) { // comparison.
        if (pos[x][1]<pos[minY][1]) minY=x;
        if (pos[x][1]>pos[minY][1]) maxY=x;
    }

    uint16_t midY=3-minY-maxY; // will work in basically every case (except where all ys are the same)

    // 2. find the gradients of all lines

    int16_t minMidGradient;
    int16_t midMaxGradient;
    int16_t minMaxGradient;

    if (pos[minY][1]-pos[midY][1]==0) minMidGradient=0;
    else minMidGradient=ceil((pos[minY][0]-pos[midY][0])/(pos[minY][1]-pos[midY][1])); // gradient of line between the points with lowest and middle ys

    if (pos[midY][1]-pos[maxY][1]==0) midMaxGradient=0;
    else midMaxGradient=ceil((pos[midY][0]-pos[maxY][0])/(pos[midY][1]-pos[maxY][1]));

    if (pos[minY][1]-pos[maxY][1]==0) minMaxGradient=0; //* You found the gradients!
    else minMaxGradient=ceil((pos[minY][0]-pos[maxY][0])/(pos[minY][1]-pos[maxY][1])); //! They are inverse gradients

    int16_t minMidOffset=pos[minY][0]-pos[minY][1]*minMidGradient; // offset
    int16_t midMaxOffset=pos[midY][0]-pos[midY][1]*midMaxGradient;
    int16_t minMaxOffset=pos[maxY][0]-pos[maxY][1]*minMaxGradient;

    minY=pos[minY][1]; // now we can make it actual y
    midY=pos[midY][1];
    maxY=pos[maxY][1];

    // 3. for y between minY and maxY
    for (uint16_t y=minY; y<=maxY; y++) {
        
        // 3.1. find minX and maxX
        uint16_t minX;
        uint16_t maxX;

        if (y>=midY) {
            minX=minMaxGradient*y+minMaxOffset; // yes
            maxX=midMaxGradient*y+midMaxOffset;
        } else {
            minX=minMidGradient*y+minMidOffset;
            maxX=minMaxGradient*y+minMaxOffset;
        }

        // 3.2. draw a line between (y,minX) and (y,maxX) on data of color clr
        drawLine(data,clr,(uint16_t[2]) {y,minX},(uint16_t[2]) {y,maxX}); // this isn't going to work
    }
}

// draws a rect outline with given xywh
// TODO you know what to add
void drawRectWireframe(PixelData* data,Color clr,uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    uint16_t pos0[]={x,y}; // first pos
    uint16_t pos1[]={x+w-1,y}; //? if w or h == data->w or h then ut will just be bigger than it by a pixel
    uint16_t pos2[]={x+w-1,y+h-1};
    uint16_t pos3[]={x,y+h-1};

    drawLine(data,clr,pos0,pos1);
    drawLine(data,clr,pos1,pos2);
    drawLine(data,clr,pos2,pos3);
    drawLine(data,clr,pos3,pos0);
}

// its literally in the name
void drawRectFilled(PixelData* data,Color clr,uint16_t x,uint16_t y,uint16_t w,uint16_t h) {
    for (uint16_t x1=x; x1<w+x; x1++) { // not loops again
        drawLine(data,clr,(uint16_t[2]) {x1,y},(uint16_t[2]) {x1,y+h});
    }
}

#pragma endregion

#pragma region SIZE_MANIPULATION

// scales the dimensions of the struct by the scale factor (stretches pixels too) and returns new PD
PixelData scaleBy(PixelData data, float scale[2]) {
    PixelData newData={ceil(data.width*scale[0]),ceil(data.height*scale[1]),NULL};
    allocPixelMemory(&newData);
    for (uint16_t x=0;x<newData.width;x++) { // goes over the width of the new struct
        uint16_t actualX=floor(x/scale[0]); // the x of the input PD
        if (actualX>=data.width) actualX=data.width-1;
        for (uint16_t y=0;y<newData.height;y++) { // the height
            uint16_t actualY=floor(y/scale[1]); // same but for y
            if (actualY>=data.height) actualY=data.height-1;
            drawPoint(&newData,data.pixels[posToIndex(data,(uint16_t[2]) {actualX,actualY})],(uint16_t[2]) {x,y}); // theres no way this actually works
        }
    }
    return newData;
}

// same as scaleBy() but scales the PD to the dimensions given
PixelData scaleTo(PixelData data, uint16_t dimensions[2]) {
    float scale[2]={dimensions[0]/data.width,dimensions[1]/data.height}; // finding out the scale factor
    return scaleBy(data,scale); // "yeah, i'll just make it a bit different"
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
void fuseImages(PixelData* base, PixelData add, uint16_t pos[2]) {
    uint16_t maxX=pos[0]+add.width; // the max x we will reach in the loop... a disintegration loop...
    uint16_t maxY=pos[1]+add.height; // same but for y

    for (uint16_t x=pos[0];x<maxX;x++) {
        uint16_t addX=x-pos[0]; // x of add
        for (uint16_t y=pos[1];y<maxY;y++) {
            // dont really need addY, cos it'll be used only once
            drawPoint(base,add.pixels[posToIndex(add,(uint16_t[2]) {addX,y-pos[1]})],(uint16_t[2]) {x,y}); // hmmm, what should i reference now
        }
    }
}

#pragma endregion

#endif
