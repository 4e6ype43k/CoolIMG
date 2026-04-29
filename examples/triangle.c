//! THIS IS NOT THE FILE YOU SHOULD INCLUDE
//! THIS FILE IS FOR DEMONSTRATIONAL PURPOSES ONLY

// expected output: blue triangle on white background

#include "../src/CIMGcolor.h" // for easy access to colors
#include "../src/CIMGmanipulation.h" // for drawing triangles
#include "../src/CIMGconvenience.h" // for... well... convenience...

// all these headers are extensions to coolIMG.h, so it will be included too

int main() {
    PD triangle={5,5,NULL}; // lets go 5x5 cos why not?
    allocPixelMemory(&triangle); // allocating memory
    u162 pos0={0,0}; //? u162 refers to uint16_t[2]
    u162 pos1={4,4}; // we are drawing a TRIangle, so we need three points to define it
    u162 pos2={4,0}; // again, origin point is (0,0), so max point will be (4,4)

    fill(&triangle,WHITE); // white bg
    drawTriangleFilled(&triangle,BLUE,pos0,pos1,pos2); // filling a triangle with blue
    encodeCIMGfile(triangle,"../images/triangle.cimg"); // time to store the data!
    freePixelMemory(&triangle); // be free my child
} // dont forget -lm when compiling this!