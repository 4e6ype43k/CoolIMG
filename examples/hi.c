//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

#include "../src/CIMGmanipulation.h"
#include "../src/CIMGconvenience.h"
#undef EVIL // SPEAK NO EVIL
// or else there will be a BLOODY DEVASTATION
// because they will be OFF WITH YOUR HEAD

int main() {
    PD fontIMG=decodeCIMGfile("../images/font.cimg");
    PD hi={6,5,NULL}; // the letters are 3 wide and 5 high each
    allocPixelMemory(&hi);
    PD h=cropImage(fontIMG,(u162) {0,25}, (u162) {2,29}); // uhhh
    PD i=cropImage(fontIMG,(u162) {3,25}, (u162) {5,29});
    fuseImages(&hi,h,(u162) {0,0});
    fuseImages(&hi,i,(u162) {3,0});
    encodeCIMGfile(hi,"../images/hi.cimg");
    encodeCIMGfile(h,"../images/h.cimg");

    freePixelMemory(&h); // ah... free, at last
    // freePixelMemory(&i);
    freePixelMemory(&hi);
    freePixelMemory(&fontIMG);
}