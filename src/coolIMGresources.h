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

// uses some dark c magic to convert uint16_t to charVector2
charVector2 int16ToChar2(uint16_t input) { // my most genius idea yet
    char char1=input; // trust the process
    char char0=(input-char1)/16; // this MUST work
    charVector2 vectorOut={char0,char1}; // this works somehow
    return vectorOut;
}

//! the function below will give a segmentation fault if the path has no file
// it is what it says on the tin
void readFileHeader(char outputString[8],char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r");
    fread(outputString,8,8,pFile); // reads data from pointer to the file in 8 chunks of size of 8 bits and writing the result to the input string
}