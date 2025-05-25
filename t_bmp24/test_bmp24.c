#include <stdio.h>
#include <stdlib.h>
#include "bmc.h"

int main() {
    const char *inputFile = "flowers_color.bmp";  // Replace with your BMP file path

    // Load the BMP image
    t_bmp24 *img = bmp24_loadImage(inputFile);
    if (!img) {
        printf("Failed to load image: %s\n", inputFile);
        return 1;
    }

    printf("Original Image Info:\n");
    printf("Width: %d\n", img->width);
    printf("Height: %d\n", img->height);
    printf("Color Depth: %d\n", img->colorDepth);

    printf("\nApplying negative filter...\n");
    bmp24_negative(img);
    bmp24_saveImage(img, "negative24.bmp");

    printf("\nApplying grayscale filter...\n");
    bmp24_grayscale(img);
    bmp24_saveImage(img, "grayscale24.bmp");

    printf("\nAdjusting brightness by +50...\n");
    bmp24_brightness(img, 50);
    bmp24_saveImage(img, "brightness24.bmp");

    printf("\nApplying box blur...\n");
    bmp24_boxBlur(img);
    bmp24_saveImage(img, "boxblur24.bmp");

    printf("\nApplying Gaussian blur...\n");
    bmp24_gaussianBlur(img);
    bmp24_saveImage(img, "gaussian24.bmp");

    printf("\nApplying outline filter...\n");
    bmp24_outline(img);
    bmp24_saveImage(img, "outline24.bmp");

    printf("\nApplying emboss filter...\n");
    bmp24_emboss(img);
    bmp24_saveImage(img, "emboss24.bmp");

    bmp24_free(img);

    printf("\nAll tests arrrre done\n");
    return 0;
}