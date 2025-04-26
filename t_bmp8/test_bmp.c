#include <stdio.h>
#include "bmc.h"

int main(){
    t_bmp8 *img = bmp8_loadImage("cv2.bmp");
    if (!img) {
        fprintf(stderr, "Failed to load image\n");
        return 1;
    }
    bmp8_printInfo(img);
    bmp8_negative(img);
    bmp8_saveImage("cv2_negative.bmp", img);
    return 0;
}

