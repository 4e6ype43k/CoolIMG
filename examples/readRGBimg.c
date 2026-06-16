//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#define MAIN_CIMG_FUNCTIONALITY // for image IO
#define CIMG_MORE_COLOR_STUFF // color functionality
#include "../src/coolIMG.h"

int main() {
    PixelData pd=decodeCIMGfile("../images/rgb.cimg");
    for (char x=0; x<3; x++) { // loops over the three pixels
        printColor(pd.pixels[x],HEX);
        printf("\n");
    }
    freePixelMemory(&pd);
}