#include <toolkit.h> // one of my other projects

#define BLACK (Color) {0,0,0,255}
#define WHITE (Color) {255,255,255,255}
#define RED (Color) {255,0,0,255}
#define GREEN (Color) {0,255,0,255}
#define BLUE (Color) {0,0,255,255}

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;

// hm i wonder what this does
void printColor(Color clr,int hex) {
    if (hex==0) {
        printf("0x%d%d%d%d",clr.r,clr.g,clr.b,clr.a);
    } else {
        printf("0x%x%x%x%x",clr.r,clr.g,clr.b,clr.a);
    }
}