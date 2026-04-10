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

#ifndef MAXFILEPIXELS // this is the thing you can change, NOT THE BOTTOM ONE
#define MAXFILEPIXELS 16384 // this refers to the max product of file w and h that will be accepted by readCMIGfile()
#endif // you can change this with #define MAXFILEPIXELS in your scripts

#ifndef MAXFILESIZE
#define MAXFILESIZE MAXFILEPIXELS*32+96 //! DO NOT CHANGE THIS, PLEASE CHANGE THE ONE ABOVE
#endif

char CIMGheader[8]={0x43,0x49,0x4d,0x47,0x0d,0x0a,0x1a,0x0a}; // pls dont modify this pls pls



typedef struct {
    uint8_t r; // red
    uint8_t g; // green
    uint8_t b; // blue
    uint8_t a; // alpha
} Color;

Color int32ToColor(uint32_t colorInt) {
    // TODO
}

typedef struct {
    uint16_t width;
    uint16_t height;
    Color* pixels; //? color is stored as RGBA in the file
} PixelData;

// the array doesn't magically allocate memory in a struct because the w and h aren't always initiated
void allocPixelMemory(PixelData* pd) {
    pd->pixels=(Color*) malloc(pd->width*pd->height*4); //? the 4 is the bytes required to store a Color struct
}

// frees memory after you're done with the pixel data
void freePixelMemory(PixelData* pd) {
    free(pd->pixels);
}

//! the function below will give a segmentation fault if the path has no file
// checks first 8 bytes of the file to check if they match the CIMG header. directory could also be an absolute path
bool isCIMG(char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r");

    char header[8];
    fread(header,8,8,pFile); 
    bool match=1;

    for (uint8_t index=0; index<8; index++) { // compares the two because strcmp() doesn't want to do its allocated job
        if (header[index]!=CIMGheader[index]) {
            match=0; // sets output to 0 (false) if there is 1 mismatch
        }
    }
    
    return match;
}

//! the function below will give a segmentation fault if the path has no file
// gets CIMG file pixel data
PixelData decodeCMIGfile(char* directory,bool printErrors) {
    if (isCIMG(directory)) {
        FILE* pFile;
        pFile=fopen(directory,"r"); // read-only cos we won't modify anything

        PixelData data;
        uint16_t sizeData[6]; // the size info is 2 bytes (16 bits long)
        fread(sizeData,16,6,pFile); // same as usual but read in chunks of 16 bits instead
        data.width=sizeData[4]; // size data is stored in the order of wh, allowing 2^32 different size combinations (2^16 max size for each axis)
        data.height=sizeData[5];

        // TODO img type and channel bit depth
        // TODO? invent some compression method
        //* ^ not too keen on any of these tbh

        allocPixelMemory(&data); //* here comes the fun(?) part :)
        uint32_t readPdata[MAXFILESIZE/32]; // TODO please find a better way to do lines 111- to not waste so much ram
        fread(readPdata,32,MAXFILESIZE/32,pFile); //? i think this should work (will clarify after i finish encodePixelData())
    } else {
        if (printErrors) {
            printf("coolIMG.h: File at %s is not a CMIG file",directory);
        }
    }
}

// encodes pixel data into a CIMG file
void encodePixelData(PixelData pd,char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"w"); // if the file doesn't exist in the dir, it'll create itself (i think)

    char rawData[MAXFILESIZE/8]; // stores all the data in a 1D byte array
    sprintf("%s%s%s",CIMGheader,int16ToChar2(pd.width),int16ToChar2(pd.height)); // damn
    
    for (uint32_t index=12; index<pd.height*pd.width+12; index+=4) { // oh boy
        rawData[index]=pd.pixels[index].r; // "yeah bro, c is very readable"
        rawData[index+1]=pd.pixels[index].g;
        rawData[index+2]=pd.pixels[index].b;
        rawData[index+3]=pd.pixels[index].a;
    }

    fprintf(pFile,"%s",rawData);
    fclose(pFile); // hope this works
}