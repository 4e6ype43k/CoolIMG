//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#include "../src/CIMGmanipulation.h"
#include "../src/CIMGcolor.h"

int main(){
    PixelData p={3,3,NULL};
    allocPixelMemory(&p);
    drawLine(&p,GREEN, (uint16_t[2]) {0,0}, (uint16_t[2]) {2,2});
    drawLine(&p,GREEN, (uint16_t[2]) {2,0}, (uint16_t[2]) {0,2});
    encodeCIMGfile(p,"../images/greenCross.cimg");
    freePixelMemory(&p);
}