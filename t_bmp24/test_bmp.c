#include <stdio.h>
#include "bmc.h"

int main(){
    const char *inputFile = "flowers_colors.bmp"; // Input BMP file
    const char *outputFile = "output_negative.bmp"; // Output file for the negative image

    // Load the BMP image
    t_bmp24 *image = bmp24_loadImage(inputFile);
    if (!image) {
        fprintf(stderr, "Error loading image: %s\n", inputFile);
        return EXIT_FAILURE;
    }

    // Apply negative and outline
    bmp24_negative(image);
    bmp24_outline(image);
    bmp24_saveImage(image, outputFile);
    printf("Negative image saved as %s\n", outputFile);

    // Free
    bmp24_free(image);

    return EXIT_SUCCESS;
}

