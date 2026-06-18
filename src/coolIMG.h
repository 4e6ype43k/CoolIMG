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

#ifndef CIMG
#define CIMG

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef EVIL //! DONT
#pragma GCC poison printf
#endif

// the following is borrowed from CIMGtoolkit.h

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

// it is what it says on the tin
// returns 0 if the file was read successfully or 1 in any other case
int8_t readFileHeader(int8_t outputString[8],int8_t* path) {
    FILE* pFile;
    pFile=fopen(path,"r");
    if (pFile) {fread(outputString,1,8,pFile);fclose(pFile); return 0;} // reads data from pointer to the file in 8 chunks of size of 8 bits and writing the result to the input string
    else {fclose(pFile);return 1;}
}

#ifdef MAIN_CIMG_FUNCTIONALITY // this is the true CIMG functionality (PD, file IO)

typedef struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;

// this feels like reinventing fire
// add is prioritized
Color mixColors(Color base,Color add) {
    if (add.a==255) { // if add.a is full, the returned Color will be add
        return add;
    } else if (add.a==0) { // if it is zero, base will be returned
        return (Color) {base.r,base.g,base.b,255}; // in this land, alpha is always 255
    } else if (base.a==0) { // but if base.a is 0, add wil be returned again
        return (Color) {add.r,add.g,add.b,255};
    } else { // no
        uint16_t alphaSum=base.a+add.a; // will be useful as we are splitting the Color channels by ratio
        uint16_t r=(base.r*base.a/alphaSum)+(add.r*add.a/alphaSum); // checking if any channels go over 255 (2^8-1)
        uint16_t g=(base.g*base.a/alphaSum)+(add.g*add.a/alphaSum); // all int so no float
        uint16_t b=(base.b*base.a/alphaSum)+(add.b*add.a/alphaSum);

        if (r>255) r=255; // yes, you can just do that
        if (g>255) g=255;
        if (b>255) b=255;

        if (r==254) r=255; // slight error correction
        if (g==254) g=255;
        if (b==254) b=255;

        return (Color) {r,g,b,255}; // next customer
    }
}

// stores the actual data from/for a CIMG file
typedef struct PixelData { // uh oh, it seems like having any of the two different WILL give an error
    uint16_t width;
    uint16_t height;
    Color* pixels; //? color is stored as RGBA in the file
} PixelData;

// the array doesn't magically allocate memory in a struct because the w and h aren't always initiated
void allocPixelMemory(PixelData* data) {
    data->pixels=(Color*) calloc(data->width*data->height,4); //? the 4 is the bytes required to store a color
} //? use calloc instead of malloc so that the script won't crash when there is an undefined color (which there won't be if i calloc)

// frees memory after you're done with the pixel data
void freePixelMemory(PixelData* data) {
    free(data->pixels); //? actually, you don't really need neither this function nor APM as you can just pass in a pointer to an array on the stack for faster memory
}

// converts a position on a PD and returns an index matching the pos
// returns 0 if the pos is out of range
uint32_t posToIndex(PixelData* data,uint16_t pos[2]) {
    if (pos[0]<data->width&&pos[1]<data->height) { // index 0 is x, 1 is y
        return data->width*pos[1]+pos[0]; // there is y lots of width (as the colors are stored in a 1d array from (0,0) to max pos) and there is x
    } // sounds counter-intuitive but it works
}

// if you just assign 1 PD to another, they will point to the same array, so changing one will change the other
// this is where this function comes in
PixelData copyData(PixelData* data){
    uint16_t width=data->width; // we will reference it quite a few times
    uint16_t height=data->height;
    PixelData newData={width,height,NULL}; // we do NOT assign pixels to point to the same array
    allocPixelMemory(&newData); // nearly forgot this

    for (uint32_t x=0; x<width*height; x++) {
        newData.pixels[x]=data->pixels[x]; // going over each pixel and yeah
    }
    return newData;
}

int8_t CIMGheader[]={0x43,0x49,0x4d,0x47,0x0d,0x0a,0x1a,0x0a}; // pls dont modify this pls pls

// checks first 8 bytes of the file to check if they match the CIMG header. path could also be an absolute path
// returns 0 if file isn't CIMG, 1 if it is and -1 if file doesn't exist
int8_t isCIMG(int8_t* path) {
    int8_t header[8];
    int8_t fileFound=readFileHeader(header,path); 
    if (fileFound) return -1; // above func returns 1 if the file was read successfully

    int32_t count=0;
    for (uint8_t index=0; index<8; index++) { // compares the two because strcmp() doesn't want to do its allocated job
        if (header[index]==CIMGheader[index]) {
            count++; // adds 1 to counter for each match
        } //? why? because if the input header is (ie) [0x43 0x49], the algorithm won't find a mismatch and will output true
    }
    
    return count==8;
}

// returns 0 size PD if file wasn't CIMG
PixelData decodeCIMGfile(int8_t* path) {
    uint8_t cimgCheck=isCIMG(path);
    if (cimgCheck==1) {
        FILE* pFile; // you know the drill
        pFile=fopen(path,"rb"); // reading the BINARY
        uint16_t sizeData[6]; // there are two axis of size (both take up 2 bytes each) + the header (8 bytes/2 bytes=4)
        fread(sizeData,2,6,pFile);

        uint16_t width=sizeData[4]/256; // width is stored first
        width+=(sizeData[4]-width*256)*256; // more magic
        uint16_t height=sizeData[5]/256;
        height+=(sizeData[5]-height*256)*256;
        
        PixelData data={width,height,NULL}; // null because the third arg will be an array on the next line
        allocPixelMemory(&data);
        uint32_t magicNumber=width*height*4+12; // 12 bytes for header+dimensions (2 for each) and 4 for each pixel

        uint8_t fileData[magicNumber]; // ... just dont look at the memory usage... please
        fread(fileData,1,magicNumber,pFile);

        for (uint32_t x=0; x<magicNumber-12; x+=4) {
            data.pixels[x/4]=(Color){fileData[x],fileData[x+1],fileData[x+2],fileData[x+3]}; // x is the red value, x+1 is green and etc
        }
        fclose(pFile);
        return data;
    } else return (PixelData) {0,0,NULL};
}

// decodes CIMG file into a struct...
void decodeCIMGfileIntoStruct(int8_t* path,PixelData* data) {
    uint8_t cimgCheck=isCIMG(path);
    if (cimgCheck==1) {
        FILE* pFile;
        pFile=fopen(path,"rb");
        uint16_t sizeData[6];
        fread(sizeData,2,6,pFile);

        uint16_t width=sizeData[4]/256;
        width+=(sizeData[4]-width*256)*256;
        uint16_t height=sizeData[5]/256;
        height+=(sizeData[5]-height*256)*256;
        
        allocPixelMemory(data);
        uint32_t magicNumber=width*height*4+12;

        uint8_t fileData[magicNumber];
        fread(fileData,1,magicNumber,pFile);

        for (uint32_t x=0; x<magicNumber-12; x+=4) {
            data->pixels[x/4]=(Color){fileData[x],fileData[x+1],fileData[x+2],fileData[x+3]};
        }
        fclose(pFile);
    } else return (PixelData) {0,0,NULL};
}

// encodes pixel data into a CIMG file
void encodeCIMGfile(PixelData* data,int8_t* path) {
    FILE* pFile;
    pFile=fopen(path,"wb"); // if the file doesn't exist in the dir, it'll create itself (i think)
    uint32_t magicNumber=data->width*data->height*4+12;  // same as in decodeCIMGfile()

    int8_t rawData[magicNumber]; // stores all the data in a 1D byte array
    for (int8_t x=0;x<8;x++) { // no sprintf() this time
        rawData[x]=CIMGheader[x];
    }
    charVector2 width=int16ToChar2(data->width);
    charVector2 height=int16ToChar2(data->height);
    rawData[8]=width.value[0]; // is there a more efficient way to do thiiiiiiiiiiiiiiiiiis
    rawData[9]=width.value[1];
    rawData[10]=height.value[0];
    rawData[11]=height.value[1];
    
    
    for (uint32_t index=12; index<magicNumber; index+=4) { // oh boy
        Color temp=data->pixels[index/4-3];
        rawData[index]=temp.r;
        rawData[index+1]=temp.g;
        rawData[index+2]=temp.b;
        rawData[index+3]=temp.a;
    }
    
    fwrite(rawData,1,magicNumber,pFile);
    fclose(pFile); // hope this works
}

#ifdef CIMG_MORE_COLOR_STUFF // colors

#define BLANK (Color) {0,0,0,0}
#define BLACK (Color) {0,0,0,255}
#define WHITE (Color) {255,255,255,255}
#define RED (Color) {255,0,0,255}
#define GREEN (Color) {0,255,0,255}
#define BLUE (Color) {0,0,255,255}
#define CYAN (Color) {0,255,255,255}
#define MAGENTA (Color) {255,0,255,255}
#define YELLOW (Color) {255,255,0,255}

// only used for printColor()
typedef enum COLOR_PRINT_TYPE {
    DENARY, // to print as base10
    HEX
} COLOR_PRINT_TYPE;

// hm i wonder what this does
void printColor(Color clr,COLOR_PRINT_TYPE printType) {
    if (!printType) {
        printf("%d,%d,%d,%d",clr.r,clr.g,clr.b,clr.a);
    } else {
        printf("0x%x 0x%x 0x%x 0x%x",clr.r,clr.g,clr.b,clr.a);
    }
}

// inverts clr rgb
Color invertColor(Color clr){
    return (Color) {255-clr.r,255-clr.g,255-clr.b,clr.a}; // yeah
}

// only used for the function below
typedef enum COLOR_UINT_TYPE {
    RGBA,
    RGAB, // so many spell checks
    RAGB, // 4!=4*3*2*1=... 24... damn
    RBAG,
    RBGA,
    RABG,

    GBAR, // we (yes WE) are ALL using GBAR
    GBRA,
    GRBA,
    GRAB,
    GARB,
    GABR, //iel, now dawns thy reckoning...

    BRGA,
    BRAG,
    BGAR,
    BGRA,
    BAGR,
    BARG,

    ARGB,
    ARBG,
    ABRG,
    ABGR,
    AGRB,
    AGBR //! YES! THAT'S IT!
} COLOR_UINT_TYPE; // no unnamed structs or enums

// converts Color struct to uint32_t
uint32_t colorToInt(Color clr,COLOR_UINT_TYPE order){
    switch (order) { //? ARE YOU SURE THIS GONNA WORK?
        case ARGB: //! I HAVE NO IDEA!
        return clr.a*16777216+clr.r*65536+clr.g*256+clr.b; // why am i doing this
        break;

        case RGAB:
        return clr.r*16777216+clr.g*65536+clr.a*256+clr.b;
        break;

        case RAGB:
        return clr.r*16777216+clr.a*65536+clr.g*256+clr.b;
        break;

        case RBAG:
        return clr.r*16777216+clr.b*65536+clr.a*256+clr.g;
        break;

        case RBGA:
        return clr.r*16777216+clr.b*65536+clr.g*256+clr.a;
        break;

        case RABG:
        return clr.r*16777216+clr.a*65536+clr.b*256+clr.g;
        break;

        case GBAR:
        return clr.g*16777216+clr.b*65536+clr.a*256+clr.r;
        break;

        case GBRA:
        return clr.g*16777216+clr.b*65536+clr.r*256+clr.a;
        break;

        case GRBA:
        return clr.g*16777216+clr.r*65536+clr.b*256+clr.a;
        break;

        case GRAB:
        return clr.g*16777216+clr.r*65536+clr.a*256+clr.b;
        break;

        case GARB:
        return clr.g*16777216+clr.a*65536+clr.r*256+clr.b;
        break;

        case GABR:
        return clr.g*16777216+clr.a*65536+clr.b*256+clr.r;
        break;

        case BRGA:
        return clr.b*16777216+clr.r*65536+clr.g*256+clr.a;
        break;

        case BRAG:
        return clr.b*16777216+clr.r*65536+clr.a*256+clr.g;
        break;

        case BGAR:
        return clr.b*16777216+clr.g*65536+clr.a*256+clr.r;
        break;

        case BGRA:
        return clr.b*16777216+clr.g*65536+clr.r*256+clr.a;
        break;

        case BAGR:
        return clr.b*16777216+clr.a*65536+clr.g*256+clr.r;
        break;

        case BARG:
        return clr.b*16777216+clr.a*65536+clr.r*256+clr.g;
        break;

        case ARBG:
        return clr.a*16777216+clr.r*65536+clr.b*256+clr.g;
        break;

        case ABRG:
        return clr.a*16777216+clr.b*65536+clr.r*256+clr.g;
        break;

        case ABGR:
        return clr.a*16777216+clr.b*65536+clr.g*256+clr.r;
        break;

        case AGRB:
        return clr.a*16777216+clr.g*65536+clr.r*256+clr.b;
        break;

        case AGBR:
        return clr.a*16777216+clr.g*65536+clr.b*256+clr.r;
        break;

        default: // would return RGBA as default
        return clr.r*16777216+clr.g*65536+clr.b*256+clr.a;
    }
}

// converts uint32_t to Color
Color intToColor(uint32_t intColor, COLOR_UINT_TYPE order) {
    uint32_t remainder=intColor%16777216;
    uint8_t r=(intColor-remainder)/16777216; // the color r channel if it was rgba
    remainder%=65536;
    uint8_t g=(intColor-remainder)/65536;
    remainder%=256;
    uint8_t b=(intColor-remainder)/256;
    uint8_t a=intColor;
	
    switch(order) {
        case ARGB: 
        return (Color) {a,r,g,b};
        break;

        case RGAB:
        return (Color) {r,g,a,b};
        break;

        case RAGB:
        return (Color) {r,a,g,b};
        break;

        case RBAG:
        return (Color) {r,b,a,g};
        break;

        case RBGA:
        return (Color) {r,b,g,a};
        break;

        case RABG:
        return (Color) {r,a,b,g};
        break;

        case GBAR:
        return (Color) {g,b,a,r};
        break;

        case GBRA:
        return (Color) {g,b,r,a};
        break;

        case GRBA:
        return (Color) {g,r,b,a};
        break;

        case GRAB:
        return (Color) {g,r,a,b};
        break;

        case GARB:
        return (Color) {g,a,r,b};
        break;

        case GABR:
        return (Color) {g,a,b,r};
        break;

        case BRGA:
        return (Color) {b,r,g,a};
        break;

        case BRAG:
        return (Color) {b,r,a,g};
        break;

        case BGAR:
        return (Color) {b,g,a,r};
        break;

        case BGRA:
        return (Color) {b,g,r,a};
        break;

        case BAGR:
        return (Color) {b,a,g,r};
        break;

        case BARG:
        return (Color) {b,a,r,g};
        break;

        case ARBG:
        return (Color) {a,r,b,g};
        break;

        case ABRG:
        return (Color) {a,b,r,g};
        break;

        case ABGR:
        return (Color) {a,b,g,r};
        break;

        case AGRB:
        return (Color) {a,g,r,b};
        break;

        case AGBR:
        return (Color) {a,g,b,r};
        break;

        default: // would return RGBA as default
        return (Color) {r,g,b,a};
    }
}

// compares two colors and outputs 1 if they are the same
uint8_t compareColors(Color clr1, Color clr2) {
    return clr1.r==clr2.r&&clr1.g==clr2.g&&clr1.b==clr2.b&&clr1.a==clr2.a;
}

#endif

#ifdef CIMG_MANIPULATION // AKA CIMGmanipulation.h

#include <math.h> // well thats a prob-lm

// if clr.a is 0, then it just won't draw anything
void drawPoint(PixelData* data, Color clr,uint16_t pos[2]) {
    if (clr.a>0){
        data->pixels[posToIndex(data,pos)]=mixColors(data->pixels[posToIndex(data,pos)],clr); // wow
    }
}

// sets all Color structs in data to be of the same color
void fill(PixelData* data,Color clr) {
    for (uint16_t x=0;x<data->width;x++) {
        for (uint16_t y=0; y<data->height; y++) { // note to future me: always remember to swap vars in for loops when you ctrl+c ctrl+v them, otherwise they'll never stop, please
            drawPoint(data,clr,(uint16_t[2]) {x,y}); // its this simple
        }
    }
}

// replaces all instances of clr0 with clr1
void replaceColor(PixelData* data,Color clr0,Color clr1) {
    for (uint16_t x=0;x<data->width;x++) {
        for (uint16_t y=0;y<data->height;y++) {
            // HERE IT COMES!
            if (data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].r==clr0.r||data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].g==clr0.g||data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].b==clr0.b||data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].a==clr0.a){
                drawPoint(data,clr1,(uint16_t[2]) {x,y});
            }
        }
    }
}

// replaceColor() but for an area
void replaceColorArea(PixelData* data,Color clr0,Color clr1, uint16_t pos0[2], uint16_t pos1[2]) {
    uint16_t minX; // reduce, reuse, recycle
    uint16_t maxX;
    uint16_t maxY;
    uint16_t minY;

    if (pos0[0]>pos1[0]) {
        maxX=pos0[0];
        minX=pos1[0];
    } else {
        maxX=pos1[0];
        minX=pos0[0];
    }

    if (pos0[1]>pos1[1]) {
        maxY=pos0[1];
        minY=pos1[1];
    } else {
        maxY=pos1[1];
        minY=pos0[1];
    }

    for (uint16_t x=minX;x<=maxX;x++) {
        for (uint16_t y=minY;y<=maxY;y++) { // hope this works
            if (data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].r==clr0.r||data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].g==clr0.g||data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].b==clr0.b||data->pixels[posToIndex(data,(uint16_t[2]) {x,y})].a==clr0.a){
                drawPoint(data,clr1,(uint16_t[2]) {x,y});
            }
        }
    }
}

// inverts color at pos
void invertPoint(PixelData* data, uint16_t pos[2]) {
    Color clr=data->pixels[posToIndex(data,pos)]; // storing the color for processing
    Color newColor={255-clr.r,255-clr.g,255-clr.b,clr.a}; // 255-rgb is pretty much inverting the color
    //* a is the same cos... we dont invert alpha ok?
    drawPoint(data,newColor,pos);
}

// inverts all pixels between p0 and p1
void invertArea(PixelData* data, uint16_t pos0[2], uint16_t pos1[2]) {
    uint16_t minX; // reduce, reuse, recycle
    uint16_t maxX;
    uint16_t maxY;
    uint16_t minY;

    if (pos0[0]>pos1[0]) {
        maxX=pos0[0];
        minX=pos1[0];
    } else {
        maxX=pos1[0];
        minX=pos0[0];
    }

    if (pos0[1]>pos1[1]) {
        maxY=pos0[1];
        minY=pos1[1];
    } else {
        maxY=pos1[1];
        minY=pos0[1];
    }

    for (uint16_t x=minX;x<=maxX;x++) {
        for (uint16_t y=minY;y<=maxY;y++) {
            invertPoint(data,(uint16_t[2]) {x,y}); // why
        }
    }
}

// inverts all pixels in PD
void invertFill(PixelData* data){
    invertArea(data,(uint16_t[2]) {0,0},(uint16_t[2]) {data->width-1,data->height-1}); // wow
}

// draws a line with a width of 1
void drawLineWidth1(PixelData* data,Color clr,uint16_t pos0[2],uint16_t pos1[2]) {
    if (pos0[1]!=pos1[1]) { // checks if y isn't the same
        if (pos0[1]>pos1[1]) { // going over the height first
                double gradientInverse=(double) (pos0[0]-pos1[0])/(pos0[1]-pos1[1]); // gradient relative to y
                int16_t offset=pos0[0]-pos0[1]*gradientInverse; // uhhhh

                for (int16_t y=pos1[1];y<=pos0[1];y++) { // trust the process
                    int16_t x=ceil(y*gradientInverse+offset); //? why ceil? because I WANT IT, I WANT IT QUITE A LOT! I'M IN IT COS I CAN BE, YOU CAN'T TELL ME THAT I'M NOT!
                    drawPoint(data,clr,(uint16_t[2]) {x,y});
                }
        } else { // through the mirror
            double gradientInverse=(double) (pos1[0]-pos0[0])/(pos1[1]-pos0[1]);
            int16_t offset=pos0[0]-pos0[1]*gradientInverse;

            for (int16_t y=pos0[1];y<=pos1[1];y++) {
                int16_t x=ceil(y*gradientInverse+offset);
                drawPoint(data,clr,(uint16_t[2]) {x,y});
            }
        }
    } else { // wide boi
        if (pos0[0]>pos1[0]) {
            double gradient=(double) (pos0[1]-pos1[1])/(pos0[0]-pos1[0]); // gradient relative to x
            int16_t offset=pos0[1]-pos0[0]*gradient; // uhhhh™

            for (int16_t x=pos1[0];x<=pos0[0];x++) {
                int16_t y=ceil(x*gradient+offset);
                drawPoint(data,clr,(uint16_t[2]) {x,y});
            }
        } else {
            double gradient=(double) (pos1[1]-pos0[1])/(pos1[0]-pos0[0]);
            int16_t offset=pos0[1]-pos0[0]*gradient;

            for (int16_t x=pos0[0];x<=pos1[0];x++) {
                int16_t y=ceil(x*gradient+offset);
                drawPoint(data,clr,(uint16_t[2]) {x,y});
            }
        }
    }
}

// draws a line between two points. now with width!1!!!
void drawLine(PixelData* data,Color clr,uint16_t pos0[2],uint16_t pos1[2],uint16_t width) {
    int16_t lineMin=width%2!=0?-(width-1)/2:-width/2; // minimum offset for line based on width (if that makes sense)
    int16_t lineMax=width%2!=0?(width-1)/2+1:width/2;

    for (int16_t x=lineMin; x<lineMax; x++) {
        uint16_t y0=pos0[1]+x<0?pos0[1]:pos0[1]+x; //? if the result<0, it will be gigantic if unsigned
        uint16_t y1=pos1[1]+x<0?pos1[1]:pos1[1]+x;
        drawLineWidth1(data,clr,(uint16_t[2]) {pos0[0],y0},(uint16_t[2]) {pos1[0],y1});
    }
}

// draws something but i cant really figure out what exactly
void drawTriangleWireframe(PixelData* data,Color clr, uint16_t pos[3][2],uint16_t width) {
    drawLine(data,clr,pos[0],pos[1],width); // this is horrendously easy
    drawLine(data,clr,pos[1],pos[2],width);
    drawLine(data,clr,pos[0],pos[2],width);
}

// uses a new triangle draw algorithm cos the last one SUCKED
void drawTriangleFilled(PixelData* data,Color clr,uint16_t pos[3][2]){
    /*
    * new algorithm in pseudocode:

    find min, mid and max ys

    find the gradients of all lines

    for y between minY and maxY
        find minX and maxX

        draw a line between (y,minX) and (y,maxX) on data of color clr
    */

    // 1. find min, mid and max ys

    uint16_t minY=0; // first, we will store indices of the ys
    uint16_t maxY=0;

    for (uint8_t x=0; x<3; x++) { // comparison.
        if (pos[x][1]<pos[minY][1]) minY=x;
        if (pos[x][1]>pos[minY][1]) maxY=x;
    }

    uint16_t midY=3-minY-maxY; // will work in basically every case (except where all ys are the same)

    // 2. find the gradients of all lines

    int16_t minMidGradient;
    int16_t midMaxGradient;
    int16_t minMaxGradient;

    if (pos[minY][1]-pos[midY][1]==0) minMidGradient=0;
    else minMidGradient=ceil((pos[minY][0]-pos[midY][0])/(pos[minY][1]-pos[midY][1])); // gradient of line between the points with lowest and middle ys

    if (pos[midY][1]-pos[maxY][1]==0) midMaxGradient=0;
    else midMaxGradient=ceil((pos[midY][0]-pos[maxY][0])/(pos[midY][1]-pos[maxY][1]));

    if (pos[minY][1]-pos[maxY][1]==0) minMaxGradient=0; //* You found the gradients!
    else minMaxGradient=ceil((pos[minY][0]-pos[maxY][0])/(pos[minY][1]-pos[maxY][1])); //! They are inverse gradients

    int16_t minMidOffset=pos[minY][0]-pos[minY][1]*minMidGradient; // offset
    int16_t midMaxOffset=pos[midY][0]-pos[midY][1]*midMaxGradient;
    int16_t minMaxOffset=pos[maxY][0]-pos[maxY][1]*minMaxGradient;

    minY=pos[minY][1]; // now we can make it actual y
    midY=pos[midY][1];
    maxY=pos[maxY][1];

    // 3. for y between minY and maxY
    for (uint16_t y=minY; y<=maxY; y++) {
        
        // 3.1. find minX and maxX
        uint16_t minX;
        uint16_t maxX;

        if (y>=midY) {
            minX=minMaxGradient*y+minMaxOffset; // yes
            maxX=midMaxGradient*y+midMaxOffset;
        } else {
            minX=minMidGradient*y+minMidOffset;
            maxX=minMaxGradient*y+minMaxOffset;
        }

        // 3.2. draw a line between (y,minX) and (y,maxX) on data of color clr
        drawLine(data,clr,(uint16_t[2]) {y,minX},(uint16_t[2]) {y,maxX},1); // this isn't going to work
    }
}

// draws a rect outline with given xywh
void drawRectWireframe(PixelData* data,Color clr,uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t width) {
    uint16_t pos0[]={x,y}; // first pos
    uint16_t pos1[]={x+w-1,y}; //? if w or h == data->w or h then ut will just be bigger than it by a pixel
    uint16_t pos2[]={x+w-1,y+h-1};
    uint16_t pos3[]={x,y+h-1};

    drawLine(data,clr,pos0,pos1,width);
    drawLine(data,clr,pos1,pos2,width);
    drawLine(data,clr,pos2,pos3,width);
    drawLine(data,clr,pos3,pos0,width);
}

// its literally in the name
void drawRectFilled(PixelData* data,Color clr,uint16_t x,uint16_t y,uint16_t w,uint16_t h) {
    for (uint16_t x1=x; x1<w+x; x1++) { // not loops again
        drawLine(data,clr,(uint16_t[2]) {x1,y},(uint16_t[2]) {x1,y+h},1);
    }
}

// scales the dimensions of the struct by the scale factor (stretches pixels too) and returns a new PD
PixelData scaleBy(PixelData* data, float scale[2]) {
    PixelData newData={ceil(data->width*scale[0]),ceil(data->height*scale[1]),NULL};
    allocPixelMemory(&newData);
    for (uint16_t x=0;x<newData.width;x++) { // goes over the width of the new struct
        uint16_t actualX=floor(x/scale[0]); // the x of the input PD
        if (actualX>=data->width) actualX=data->width-1;
        for (uint16_t y=0;y<newData.height;y++) { // the height
            uint16_t actualY=floor(y/scale[1]); // same but for y
            if (actualY>=data->height) actualY=data->height-1;
            drawPoint(&newData,data->pixels[posToIndex(data,(uint16_t[2]) {actualX,actualY})],(uint16_t[2]) {x,y}); // theres no way this actually works
        }
    }
    return newData;
}

// same as scaleBy() but scales the PD to the dimensions given
PixelData scaleTo(PixelData* data, uint16_t dimensions[2]) {
    float scale[2]={dimensions[0]/data->width,dimensions[1]/data->height}; // finding out the scale factor
    return scaleBy(data,scale); // "yeah, i'll just make it a bit different"
}

// takes a chunk of the struct between two points and returns the pixel data of the area (if this makes any sense)
PixelData cropImage(PixelData* data,uint16_t pos0[2],uint16_t pos1[2]) {
    uint16_t minX; // setting the range for the function to work on
    uint16_t maxX;
    uint16_t maxY;
    uint16_t minY;

    if (pos0[0]>pos1[0]) { // i could do a fancy-shmancy ternary operator thingy but then i would need to do it twice for each axis
        maxX=pos0[0];
        minX=pos1[0];
    } else {
        maxX=pos1[0];
        minX=pos0[0];
    }

    if (pos0[1]>pos1[1]) { // again
        maxY=pos0[1];
        minY=pos1[1];
    } else {
        maxY=pos1[1];
        minY=pos0[1];
    }

    PixelData newData={maxX-minX+1,maxY-minY+1,NULL}; //? +1 because i dont want 0-wide arrays
    allocPixelMemory(&newData);

    for (uint16_t x=minX;x<=maxX;x++) { // from smallest to largest INCLUSIVE (because otherwise if any of the axis are the same for p0 and p1, the func will output nothing)
        uint16_t newX=x-minX; // the x of newData
        for (uint16_t y=minY;y<=maxY;y++) {
            uint16_t newY=y-minY;
            drawPoint(&newData,data->pixels[posToIndex(data,(uint16_t[2]) {x,y})],(uint16_t[2]) {newX,newY}); // im so sorry to PUSH UR T3MPRR so much with this bad code
        }                                                                                                    // i need to ACT RIGHT more often
    }                                                                                                        // this is probably because I MIGHT B3 SICK
    
    return newData;
}

// adds add pixels to base, starting from pos
void fuseImages(PixelData* base, PixelData* add, uint16_t pos[2]) {
    uint16_t maxX=pos[0]+add->width; // the max x we will reach in the loop... a disintegration loop...
    uint16_t maxY=pos[1]+add->height; // same but for y

    for (uint16_t x=pos[0];x<maxX;x++) {
        uint16_t addX=x-pos[0]; // x of add
        for (uint16_t y=pos[1];y<maxY;y++) {
            // dont really need addY, cos it'll be used only once
            drawPoint(base,add->pixels[posToIndex(add,(uint16_t[2]) {addX,y-pos[1]})],(uint16_t[2]) {x,y}); // hmmm, what should i reference now
        }
    }
}

#endif

#ifdef CIMG_TYPES

typedef uint8_t u8;

typedef uint16_t u16;
typedef uint16_t ui16_2[2]; // useful when converting to coord type (ie as in (ui16_2) {x,y})
typedef uint16_t u162[2]; // even shorter
typedef uint16_t u1632[3][2]; // triangle
typedef int16_t i16;

typedef uint32_t u32;

typedef Color cl;
typedef PixelData PD;

#endif

#endif

#endif