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

#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef TOOLKIT
#define TOOLKIT

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
    charVector2 vectorOut={char0,char1}; // this works somehow
    return vectorOut;
}

//! the function below will give a segmentation fault if the path has no file
// it is what it says on the tin
void readFileHeader(char outputString[8],char* path) {
    FILE* pFile;
    pFile=fopen(path,"r");
    fread(outputString,1,8,pFile); // reads data from pointer to the file in 8 chunks of size of 8 bits and writing the result to the input string
}

// returns x as a positive number
uint16_t absoluteValue(int16_t x) {
    if (x>=0) {
        return x;
    } else {
        return -x;
    }
}

#endif