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
#include <string.h>

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

// gets CMIG file pixel data
PixelData decodeCMIGfile(char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r"); // read-only cos we won't modify anything

    char rawData[MAXFILESIZE];
    char trueData[MAXFILESIZE]; // this might look like im wasting memory but trust the process
    while (fgets(rawData,MAXFILESIZE,pFile)) {
        sprintf(trueData,"%s%s",trueData,rawData); // rawData gets cut off at 0x0D chars (or simply ENTER) so we use a while loop
        
    }
}

// encodes pixel data into a CMIG file
void encodePixelData(PixelData pd,char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"w"); // if the file doesn't exist in the dir, it'll create itself (i think)

    char rawData[MAXFILESIZE]; // stores all the data in a 1D byte array
    sprintf(rawData,"%s",CIMGheader);
    
}