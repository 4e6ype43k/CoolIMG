//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#include "../src/coolIMG.h"

//* Expected output (hex):
/*

images/red.cimg:

43 49 4d 47 0d 0a 1a 0a 00 01 00 01 ff 00 00 ff

*/

int main() {
    PixelData red;
    red.width=1;
    red.height=1;
    allocPixelMemory(&red);
    red.pixels[0]=(Color) {255,0,0,255}; // opaque red
    encodeCIMGfile(red,"../images/red.cimg");
    freePixelMemory(&red);
}