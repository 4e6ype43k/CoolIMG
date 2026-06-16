//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#define MAIN_CIMG_FUNCTIONALITY
#define CIMG_TYPES
#define CIMG_MANIPULATION
#include "../src/coolIMG.h"
#undef EVIL // SPEAK NO EVIL
// or else there will be a BLOODY DEVASTATION
// because they will be OFF WITH YOUR HEAD

int main() {
    PD fontIMG=decodeCIMGfile("../images/font.cimg");
    PD hi={7,5,NULL}; // the letters are 3 wide and 5 high each
    allocPixelMemory(&hi);
    PD h=cropImage(&fontIMG,(u162) {0,0}, (u162) {2,5}); // uhhh
    PD i=cropImage(&fontIMG,(u162) {3,0}, (u162) {5,5});
    fuseImages(&hi,&h,(u162) {0,0});
    fuseImages(&hi,&i,(u162) {4,0});
    encodeCIMGfile(&hi,"../images/hi.cimg");
    encodeCIMGfile(&h,"../images/h.cimg");

    freePixelMemory(&h); // ah... free, at last
    // freePixelMemory(&i);
    freePixelMemory(&hi);
    freePixelMemory(&fontIMG);
}