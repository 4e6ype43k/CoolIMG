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

typedef struct {
    char value[2];
} charVector2;

typedef struct {
    char value[4];
} charVector4;

charVector2 int16ToChar2(uint16_t input) {
    char tempHex[5]; // used for storing hex value of the input to convert it to char[2] later
    // 4 chars is enough because 2 bits can be represented as 4 hex digits max but using 4 gives a warning for some reason
    char inputChars[2]={0,0}; // yes
    sprintf(tempHex,"%x",input);
    for (uint8_t index=1; index<5; index++) {
        if ((tempHex[index]-'0')<10) { // uhhh, dont really know how to explain this. ascii table ig? does that help?

        } else {
            printf("%c",tempHex[index]);
        }
    }
}

//! the function below will give a segmentation fault if the path has no file
// it is what it says on the tin
void readFileHeader(char outputString[8],char* directory) {
    FILE* pFile;
    pFile=fopen(directory,"r");
    fread(outputString,8,8,pFile); // reads data from pointer to the file in 8 chunks of size of 8 bits and writing the result to the input string
}