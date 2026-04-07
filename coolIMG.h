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

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef CIMG_H
#define CIMG_H
#endif

#ifndef MAXFILESIZE
#define MAXFILESIZE 65535 // change this if you want (don't make it too high or too low)
#endif

#ifdef EVIL //! DONT
#pragma GCC poison printf
#endif

char CIMGheader[8]={0x43,0x49,0x4d,0x47,0x0d,0x0a,0x1a,0x0a}; // pls dont modify this pls pls

typedef struct {
    uint16_t width;
    uint16_t height;
    uint32_t* pixels; //? ARGB, not RGBA (same in the file)
} PixelData;

// the array doesn't magically allocate memory in a struct
void allocPixelMemory(PixelData* pd) {
    pd->pixels=(uint32_t*) malloc(pd->width*pd->height);
}

// frees memory after you're done with the pixel data
void freePixelMemory(PixelData* pd) {
    free(pd->pixels);
}

void readFileHeader(char outputString[8],char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r");
    fread(outputString,8,8,pFile); // reads data from pointer to the file in 8 chunks of size of 8 bits and writing the result to the input string
}

// checks first 8 bytes of the file to check if they match the 
bool isCIMG(char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r");

    char header[8];
    fread(header,8,8,pFile); 
    bool match=1;

    for (uint8_t index=0; index<8; index++) { // compares the two because strcmp() doesn't want to do its allocated job
        if (header[index]!=CIMGheader[index]) {
            match=0;
        }
    }
    
    return match;
}

// gets CMIG file pixel data
PixelData decodeCMIGfile(char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r"); // read-only cos we won't modify anything


}

// encodes pixel data into a CMIG file
void encodePixelData(PixelData pd,char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"w"); // if the file doesn't exist in the dir, it'll create itself (i think)

    char rawData[MAXFILESIZE]; // stores all the data in a 1D byte array
    sprintf(rawData,"%s",CIMGheader);
    
}