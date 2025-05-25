#include <stdio.h>
#include <stdlib.h>
#include "bmc.h"

int main() {
    const char *inputFile = "1697307652197.bmp";  // Replace with your BMP file path
    t_bmp8 *img = bmp8_loadImage(inputFile);
    if (!img) {
        printf("Failed to load image: %s\n", inputFile);
        return 1;
    }

    printf("Original Image Info:\n");
    bmp8_printInfo(img);

    printf("\nApplying negative filter...\n");
    bmp8_negative(img);

    bmp8_saveImage("negative.bmp", img);
    printf("Negative image saved as 'negative.bmp'.\n");

    printf("\nAdjusting brightness by +50...\n");
    bmp8_brightness(img, 50);

    bmp8_saveImage("brightness.bmp", img);
    printf("Brightness-adjusted image saved as 'brightness.bmp'.\n");

    printf("\nApplying thresholding with threshold = 128...\n");
    bmp8_threshold(img, 128);

    bmp8_saveImage("threshold.bmp", img);
    printf("Thresholded image saved as 'threshold.bmp'.\n");

    bmp8_free(img);

    printf("\nEverything dooone\n");
    return 0;
}