//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#define MAIN_CIMG_FUNCTIONALITY
#define CIMG_MORE_COLOR_STUFF
#define CIMG_MANIPULATION
#include "../src/coolIMG.h"

int main(){
    PixelData p={3,3,NULL};
    allocPixelMemory(&p);
    drawLineWidth1(&p,GREEN, (uint16_t[2]) {0,0}, (uint16_t[2]) {2,2});
    drawLineWidth1(&p,GREEN, (uint16_t[2]) {2,0}, (uint16_t[2]) {0,2});
    encodeCIMGfile(&p,"../images/greenCross.cimg");
    freePixelMemory(&p);
}