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
#include "CIMGcolor.h"

#ifndef CIMG_H
#define CIMG_H
#endif

char CIMGheader[]={0x43,0x49,0x4d,0x47,0x0d,0x0a,0x1a,0x0a}; // pls dont modify this pls pls

// stores the actual data from/for a CIMG file
typedef struct {
    uint16_t width;
    uint16_t height;
    Color* pixels; //? color is stored as RGBA in the file
} PixelData;

// the array doesn't magically allocate memory in a struct because the w and h aren't always initiated
void allocPixelMemory(PixelData* pd) {
    pd->pixels=(Color*) calloc(pd->width*pd->height,4); //? the 4 is the bytes required to store a color
} //? i use calloc instead of malloc so that the script won't crash when there is an undefined color (which there won't be if i calloc)

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
    if (isCIMG(directory)) {
        FILE* pFile; // you know the drill
        pFile=fopen(directory,"rb"); // reading the BINARY
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
void encodeCIMGfile(PixelData pd,char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"wb"); // if the file doesn't exist in the dir, it'll create itself (i think)
    uint32_t magicNumber=pd.width*pd.height*4+12;  // same as in decodeCIMGfile()

    char rawData[magicNumber]; // stores all the data in a 1D byte array
    sprintf(rawData,"%s",CIMGheader); // adding the header
    charVector2 width=int16ToChar2(pd.width);
    charVector2 height=int16ToChar2(pd.height);
    rawData[8]=width.value[0]; // is there a more efficient way to do thiiiiiiiiiiiiiiiiiis
    rawData[9]=width.value[1];
    rawData[10]=height.value[0];
    rawData[11]=height.value[1];
    
    
    for (uint32_t index=12; index<magicNumber; index+=4) { // oh boy
        Color temp=pd.pixels[index/4-3];
        rawData[index]=temp.r;
        rawData[index+1]=temp.g;
        rawData[index+2]=temp.b;
        rawData[index+3]=temp.a;
    }
    
    fwrite(rawData,1,magicNumber,pFile);
    fclose(pFile); // hope this works
}