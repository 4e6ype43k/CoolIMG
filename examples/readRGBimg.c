//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#include "../src/coolIMG.h"

int main() {
    PixelData pd=decodeCIMGfile("../images/rgb.cimg");
    for (char x=0; x<3; x++) {
        printColor(pd.pixels[x],0);
        printf("\n");
    }
}