//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#include "../src/coolIMG.h"

int main() {
    PixelData pd=decodeCIMGfile("../images/rgb.cimg",0);
    printf("RGB:\n");
    printf("%x,%x,%x\n",pd.pixels[0],pd.pixels[1],pd.pixels[2]);
}