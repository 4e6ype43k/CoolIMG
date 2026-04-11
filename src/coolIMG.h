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

char CIMGheader[8]={0x43,0x49,0x4d,0x47,0x0d,0x0a,0x1a,0x0a}; // pls dont modify this pls pls

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
        uint32_t magicNumber=data.width*data.height+3; // trust it works

        // TODO img type and channel bit depth
        // TODO? invent some compression method
        //* ^ not too keen on any of these tbh

        allocPixelMemory(&data); //* here comes the fun(?) part :)
        uint32_t readPdata[magicNumber]; // TODO please find a better way to do lines 111- to not waste so much ram
        fread(readPdata,32,magicNumber,pFile); //? i think this should work (will clarify after i finish encodePixelData())
    } else {
        if (printErrors) {
            printf("coolIMG.h: File at %s is not a CMIG file",directory);
        }
    }
}

// encodes pixel data into a CIMG file
void encodePixelData(PixelData pd,char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"wb"); // if the file doesn't exist in the dir, it'll create itself (i think)
    uint32_t magicNumber=pd.width*pd.height*4+12; // it's different now (12 bytes for header+dimensions+2 empty bytes and 4 for each pixel)

    char rawData[magicNumber]; // stores all the data in a 1D byte array
    sprintf(rawData,"%s",CIMGheader); // adding the header
    charVector2 width=int16ToChar2(pd.width);
    charVector2 height=int16ToChar2(pd.height);
    rawData[8]=width.value[0]; // is there a more efficient way to do thiiiiiiiiiiiiiiiiiis
    rawData[9]=width.value[1];
    rawData[10]=height.value[0];
    rawData[11]=height.value[1];
    
    
    for (uint32_t index=12; index<pd.height*pd.width+12; index+=4) { // oh boy
        charVector4 char4=int32ToChar4(pd.pixels[index/4-3]); // what
        rawData[index]=char4.value[0];
        rawData[index+1]=char4.value[1];
        rawData[index+2]=char4.value[2];
        rawData[index+3]=char4.value[3];
    }
    
    fwrite(rawData,8,magicNumber,pFile);
    fclose(pFile); // hope this works
}