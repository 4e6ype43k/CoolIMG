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

header|size (wh order)|2 empty bytes|array of pixel colour data in human-readable, ARGB format

*/

//* this is the file that allows you to read/write CIMG files

#pragma once
#include "coolIMGresources.h"

#ifndef CIMG_H
#define CIMG_H
#endif

char CIMGheader[]={0x43,0x49,0x4d,0x47,0x0d,0x0a,0x1a,0x0a}; // pls dont modify this pls pls

typedef struct {
    uint16_t width;
    uint16_t height;
    uint32_t* pixels; //? color is stored as RGBA
} PixelData;

// the array doesn't magically allocate memory in a struct because the w and h aren't always initiated
void allocPixelMemory(PixelData* pd) {
    pd->pixels=(uint32_t*) malloc(pd->width*pd->height*4); //? the 4 is the bytes required to store a color
}

// frees memory after you're done with the pixel data
void freePixelMemory(PixelData* pd) {
    free(pd->pixels);
}

//! the function below will give a segmentation fault if the path has no file
// checks first 8 bytes of the file to check if they match the CIMG header. directory could also be an absolute path
int isCIMG(char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r");

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

PixelData decodeCIMGfile(char* directory) {
    // TODO continue this tomorrow cos im way too exhausted with all that happened with this method
}

// encodes pixel data into a CIMG file
void encodeCIMGfile(PixelData pd,char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"wb"); // if the file doesn't exist in the dir, it'll create itself (i think)
    uint32_t magicNumber=pd.width*pd.height*4+12; // it's different now (12 bytes for header+dimensions (2 for each) and 4 for each pixel)

    char rawData[magicNumber]; // stores all the data in a 1D byte array
    sprintf(rawData,"%s",CIMGheader); // adding the header
    charVector2 width=int16ToChar2(pd.width);
    charVector2 height=int16ToChar2(pd.height);
    rawData[8]=width.value[0]; // is there a more efficient way to do thiiiiiiiiiiiiiiiiiis
    rawData[9]=width.value[1];
    rawData[10]=height.value[0];
    rawData[11]=height.value[1];
    
    
    for (uint32_t index=12; index<magicNumber; index+=4) { // oh boy
        charVector4 char4=int32ToChar4(pd.pixels[index/4-3]); // what
        rawData[index]=char4.value[0];
        rawData[index+1]=char4.value[1];
        rawData[index+2]=char4.value[2];
        rawData[index+3]=char4.value[3];
    }
    
    fwrite(rawData,1,magicNumber,pFile);
    fclose(pFile); // hope this works
}