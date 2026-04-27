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

//* ^ blah blah blah

/* CIMG file layout (| indicates separation between words):

header|size (wh order)|array of pixel colour data in human-readable, RGBA format

*/

//* this is the file that allows you to read/write CIMG files

#pragma once
#include "CIMGtoolkit.h"

#ifndef CIMG_H
#define CIMG_H

char CIMGheader[]={0x43,0x49,0x4d,0x47,0x0d,0x0a,0x1a,0x0a}; // pls dont modify this pls pls

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

// stores the actual data from/for a CIMG file
typedef struct {
    uint16_t width;
    uint16_t height;
    Color* pixels; //? color is stored as RGBA in the file
} PixelData;

// the array doesn't magically allocate memory in a struct because the w and h aren't always initiated
void allocPixelMemory(PixelData* data) {
    data->pixels=(Color*) calloc(data->width*data->height,4); //? the 4 is the bytes required to store a color
} //? i use calloc instead of malloc so that the script won't crash when there is an undefined color (which there won't be if i calloc)

// frees memory after you're done with the pixel data
void freePixelMemory(PixelData* data) {
    free(data->pixels);
}

// converts a position on a PD and returns an index matching the pos
// returns 0 if the pos is out of range
uint32_t posToIndex(PixelData data,uint16_t pos[2]) {
    if ((pos[0]<data.width&&pos[1]<data.height)==1) { // index 0 is x, 1 is y
        return data.height*pos[1]+pos[0]; // there is y lots of height and there is x
    }
}

//! the function below will give a segmentation fault if the path has no file
// checks first 8 bytes of the file to check if they match the CIMG header. path could also be an absolute path
int isCIMG(char* path) {
    FILE* pFile;
    pFile=fopen(path,"r");

    char header[8];
    fread(header,1,8,pFile); 

    int count=0;
    for (uint8_t index=0; index<8; index++) { // compares the two because strcmp() doesn't want to do its allocated job
        if (header[index]==CIMGheader[index]) {
            count++; // adds 1 to counter for each match
        } //? why? because if the input header is [0x43 0x49], the algorithm won't find a mismatch and will output true
    }
    
    return count==8;
}

PixelData decodeCIMGfile(char* path) {
    if (isCIMG(path)) {
        FILE* pFile; // you know the drill
        pFile=fopen(path,"rb"); // reading the BINARY
        uint16_t sizeData[6]; // there are two axis of size (both take up 2 bytes each) + the header (8 bytes/2 bytes=4)
        fread(sizeData,2,6,pFile);

        uint16_t width=sizeData[4]/256; // width is stored first
        uint16_t height=sizeData[5]/256;
        PixelData data={width,height,NULL}; // null because the third arg will be an array on the next line
        allocPixelMemory(&data);
        uint32_t magicNumber=width*height*4+12; // 12 bytes for header+dimensions (2 for each) and 4 for each pixel

        uint8_t fileData[magicNumber]; // ... just dont look at the memory usage... please
        fread(fileData,1,magicNumber,pFile);

        for (uint32_t x=0; x<magicNumber-12; x+=4) {
            data.pixels[x/4]=(Color){fileData[x],fileData[x+1],fileData[x+2],fileData[x+3]}; // x is the red value, x+1 is green and etc
        }

        return data;
    }
}

// encodes pixel data into a CIMG file
void encodeCIMGfile(PixelData data,char* path) {
    FILE* pFile;
    pFile=fopen(path,"wb"); // if the file doesn't exist in the dir, it'll create itself (i think)
    uint32_t magicNumber=data.width*data.height*4+12;  // same as in decodeCIMGfile()

    char rawData[magicNumber]; // stores all the data in a 1D byte array
    sprintf(rawData,"%s",CIMGheader); // adding the header
    charVector2 width=int16ToChar2(data.width);
    charVector2 height=int16ToChar2(data.height);
    rawData[8]=width.value[0]; // is there a more efficient way to do thiiiiiiiiiiiiiiiiiis
    rawData[9]=width.value[1];
    rawData[10]=height.value[0];
    rawData[11]=height.value[1];
    
    
    for (uint32_t index=12; index<magicNumber; index+=4) { // oh boy
        Color temp=data.pixels[index/4-3];
        rawData[index]=temp.r;
        rawData[index+1]=temp.g;
        rawData[index+2]=temp.b;
        rawData[index+3]=temp.a;
    }
    
    fwrite(rawData,1,magicNumber,pFile);
    fclose(pFile); // hope this works
}

#endif