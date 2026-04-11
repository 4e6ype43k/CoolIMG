//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#include "../src/coolIMG.h"

int main() {
    PixelData red;
    red.width=1;
    red.height=1;
    allocPixelMemory(&red);
    red.pixels[0]=0xFF0000FF; // opaque red
    encodePixelData(red,"../images/red.cimg");
}