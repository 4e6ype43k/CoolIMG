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

//! THIS FILE ONLY INCLUDES THINGMABOB TOOLKIT USED BY coolIMG.h, YOU WON'T BE ABLE TO CREATE/READ IMAGES WITH THIS FILE

#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef CIMGR
#define CIMGR
#endif

#ifdef EVIL //! DONT
#pragma GCC poison printf
#endif

typedef struct {
    char value[2];
} charVector2;

typedef struct {
    char value[4];
} charVector4;

// uses some dark c magic to convert uint16_t to charVector2
charVector2 int16ToChar2(uint16_t input) { // my most genius idea yet
    uint8_t char1=input; // trust the process
    uint8_t char0=(input-char1)/256; // this MUST work
    printf("%d,%d\n",char1,char0);
    charVector2 vectorOut={char0,char1}; // this works somehow
    return vectorOut;
}

// idk
charVector4 int32ToChar4(uint32_t input) {
    uint16_t yes1=input;
    uint16_t yes0=(input-yes1)/65536;
    charVector2 a0=int16ToChar2(yes0);
    charVector2 a1=int16ToChar2(yes1);
    uint8_t halt=a0.value[0];
    uint8_t where=a0.value[1];
    uint8_t is=a1.value[0];
    uint8_t gabriel=a1.value[1];
    charVector4 what={halt,where,is,gabriel}; //! AND WHAT HAVE YOU DONE TO HIM?
    return what; //? ... how can this be? Bested by this... this, THING?!
}

//! the function below will give a segmentation fault if the path has no file
// it is what it says on the tin
void readFileHeader(char outputString[8],char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r");
    fread(outputString,8,8,pFile); // reads data from pointer to the file in 8 chunks of size of 8 bits and writing the result to the input string
}