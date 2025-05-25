#include "bmc.h"
#include <stdio.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////////////// 2.3
t_pixel** bmp24_allocateDataPixels(int width, int height) {
    t_pixel** data = (t_pixel**)malloc(height * sizeof(t_pixel*));
    if (!data) return NULL;

    for (int i = 0; i < height; i++) {
        data[i] = (t_pixel*)malloc(width * sizeof(t_pixel));
        if (!data[i]) {
            // Free previously allocated rows in case of failure
            for (int j = 0; j < i; j++) {
                free(data[j]);
            }
            free(data);
            return NULL;
        }
    }
    return data;
}

void bmp24_printInfo(t_bmp24 *img) {
    if (!img) {
        fprintf(stderr, "Invalid BMP image.\n");
        return;
    }

    printf("BMP Image Information:\n");
    printf("-----------------------\n");
    printf("Width: %d pixels\n", img->width);
    printf("Height: %d pixels\n", img->height);
    printf("Color Depth: %d bits per pixel\n", img->colorDepth);
    printf("File Size: %u bytes\n", img->header.size);
    printf("Image Data Offset: %u bytes\n", img->header.offset);
    printf("Compression: %u\n", img->header_info.compression);
    printf("Image Size (raw data): %u bytes\n", img->header_info.imagesize);
    printf("Horizontal Resolution: %d pixels/meter\n", img->header_info.xresolution);
    printf("Vertical Resolution: %d pixels/meter\n", img->header_info.yresolution);
    printf("Number of Colors: %u\n", img->header_info.ncolors);
    printf("Important Colors: %u\n", img->header_info.importantcolors);
    printf("-----------------------\n");
}

void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels) {
        for (int i = 0; i < height; i++) {
            free(pixels[i]);
        }
        free(pixels);
    }
}

t_bmp24* bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24* bmp = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (!bmp) return NULL;

    bmp->width = width;
    bmp->height = height;
    bmp->colorDepth = colorDepth;

    bmp->header.type = BMP_TYPE;
    bmp->header.size = sizeof(t_bmp_header) + sizeof(t_bmp_info) + width * height * sizeof(t_pixel);
    bmp->header.offset = sizeof(t_bmp_header) + sizeof(t_bmp_info);

    bmp->header_info.size = sizeof(t_bmp_info);
    bmp->header_info.width = width;
    bmp->header_info.height = height;
    bmp->header_info.planes = 1;
    bmp->header_info.bits = colorDepth;
    bmp->header_info.compression = 0;
    bmp->header_info.imagesize = width * height * sizeof(t_pixel);
    bmp->header_info.xresolution = 0;
    bmp->header_info.yresolution = 0;
    bmp->header_info.ncolors = 0;
    bmp->header_info.importantcolors = 0;

    bmp->data = bmp24_allocateDataPixels(width, height);
    if (!bmp->data) {
        free(bmp);
        return NULL;
    }

    return bmp;
}

void bmp24_free(t_bmp24 *img) {
    if (img) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}
/////////////////////////////////////////////////////////////////////////////////// 2.4.1
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}
/////////////////////////////////////////////////////////////////////////////////// 2.4.2
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    if (x < 0 || x >= image->width || y < 0 || y >= image->height) {
        fprintf(stderr, "Pixel coordinates out of bounds.\n");
        return;
    }
    fread(&image->data[y][x], sizeof(t_pixel), 1, file);
    //printf("Read pixel at (%d, %d): R=%d, G=%d, B=%d\n", x, y, image->data[y][x].red, image->data[y][x].green, image->data[y][x].blue);
}
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file);
        }
    }
}
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    if (x < 0 || x >= image->width || y < 0 || y >= image->height) {
        fprintf(stderr, "Pixel coordinates out of bounds.\n");
        return;
    }
    fwrite(&image->data[y][x], sizeof(t_pixel), 1, file);
}
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////// 2.4 ^^ need these
////////////////////////////////////////////////////////////////////////////////// 2.4.3
t_bmp24* bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return NULL;
    }

    // Read BMP headers
    t_bmp_header header;
    t_bmp_info header_info;

    // Read the BMP header
    file_rawRead(0, &header, sizeof(t_bmp_header), 1, file);
    // Read the BMP info header
    file_rawRead(sizeof(t_bmp_header), &header_info, sizeof(t_bmp_info), 1, file);
    //printf("BMP Header: type=%d, size=%d, offset=%d\n", header.type, header.size, header.offset);
    //printf("BMP Info: size=%d, width=%d, height=%d, bits=%d\n", header_info.size, header_info.width, header_info.height, header_info.bits);
    // Allocate the BMP structure based on width, height, and color depth
    t_bmp24 *bmp = bmp24_allocate(header_info.width, header_info.height, header_info.bits);
    if (!bmp) {
        fclose(file);
        return NULL;
    }

    // Store the headers in the image structure
    bmp->header = header;
    bmp->header_info = header_info;

    // Read pixel data
    bmp24_readPixelData(bmp, file);
    printf("Pixel data read successfully.\n");


    fclose(file);
    return bmp;
}
////////////////////////////////////////////////////////////////////////////////// 2.4.4
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        return;
    }

    // Write headers
    file_rawWrite(0, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(sizeof(t_bmp_header), &img->header_info, sizeof(t_bmp_info), 1, file);

    // Write pixel data
    bmp24_writePixelData(img, file);

    fclose(file);
}
//////////////////////////////////////////////////////////////////////////////// 2.5
// NEGATIVE 255 - pixel
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *pixel = &img->data[y][x];
            // Invert the colors
            pixel->red = 255 - pixel->red;
            pixel->green = 255 - pixel->green;
            pixel->blue = 255 - pixel->blue;
        }
    }
}
// AVERAGE TO GRAY
void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *pixel = &img->data[y][x];
            // Calculate the average
            uint8_t grayValue = (pixel->red + pixel->green + pixel->blue) / 3;
            // Assign the average to gray
            pixel->red = grayValue;
            pixel->green = grayValue;
            pixel->blue = grayValue;
        }
    }
}
// BRIGHT UP TO INT
void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel *pixel = &img->data[y][x];
            // Adjust brightness and cap at 255 (?255)
            pixel->red = (pixel->red + value > 255) ? 255 : pixel->red + value;
            pixel->green = (pixel->green + value > 255) ? 255 : pixel->green + value;
            pixel->blue = (pixel->blue + value > 255) ? 255 : pixel->blue + value;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////// 2.6
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
    t_pixel newPixel = {0, 0, 0}; // Initialize new pixel values
    int halfSize = kernelSize / 2;

    // kernel
    for (int ky = -halfSize; ky <= halfSize; ky++) {
        for (int kx = -halfSize; kx <= halfSize; kx++) {
            int pixelX = x + kx;
            int pixelY = y + ky;

            // Check if the pixel is in
            if (pixelX >= 0 && pixelX < img->width && pixelY >= 0 && pixelY < img->height) {
                t_pixel currentPixel = img->data[pixelY][pixelX];
                newPixel.red += (int)(currentPixel.red * kernel[ky + halfSize][kx + halfSize]);
                newPixel.green += (int)(currentPixel.green * kernel[ky + halfSize][kx + halfSize]);
                newPixel.blue += (int)(currentPixel.blue * kernel[ky + halfSize][kx + halfSize]);
            }
        }
    }

    // Clamp
    newPixel.red = newPixel.red < 0 ? 0 : (newPixel.red > 255 ? 255 : newPixel.red);
    newPixel.green = newPixel.green < 0 ? 0 : (newPixel.green > 255 ? 255 : newPixel.green);
    newPixel.blue = newPixel.blue < 0 ? 0 : (newPixel.blue > 255 ? 255 : newPixel.blue);

    return newPixel;
}

// Dynamically allocate a kernel with a given size
float **allocateKernel(int kernelSize) {
    float **kernel = (float **)malloc(kernelSize * sizeof(float *));
    if (!kernel) {
        fprintf(stderr, "Failed to allocate memory for kernel rows.\n");
        return NULL;
    }

    for (int i = 0; i < kernelSize; i++) {
        kernel[i] = (float *)malloc(kernelSize * sizeof(float));
        if (!kernel[i]) {
            fprintf(stderr, "Failed to allocate memory for kernel row %d.\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(kernel[j]);
            }
            free(kernel);
            return NULL;
        }
    }

    return kernel;
}

// Free the dynamically allocated kernel
void freeKernel(float **kernel, int kernelSize) {
    for (int i = 0; i < kernelSize; i++) {
        free(kernel[i]);
    }
    free(kernel);
}


// BOX BLUR
void bmp24_boxBlur(t_bmp24 *img) {
    int kernelSize = 3;

    // Allocate the kernel
    float **kernel = allocateKernel(kernelSize);
    if (!kernel) {
        return;
    }

    // Initialize the kernel values (box blur kernel)
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] = 1.0f / (kernelSize * kernelSize);
        }
    }

    // Allocate a temporary image for storing the blurred result
    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);
    // Apply the convolution using the kernel
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    // Copy the blurred image back to the original image
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    // Free resources
    bmp24_free(tempImg);
    freeKernel(kernel, kernelSize);
}
// GAUSSIAN
void bmp24_gaussianBlur(t_bmp24 *img) {
    int kernelSize = 5;

    // Allocate the kernel
    float **kernel = allocateKernel(kernelSize);
    if (!kernel) {
        return;
    }

    // Initialize the kernel values (Gaussian blur kernel)
    float gaussianValues[5][5] = {
        {1 / 273.0f, 4 / 273.0f, 7 / 273.0f, 4 / 273.0f, 1 / 273.0f},
        {4 / 273.0f, 16 / 273.0f, 26 / 273.0f, 16 / 273.0f, 4 / 273.0f},
        {7 / 273.0f, 26 / 273.0f, 41 / 273.0f, 26 / 273.0f, 7 / 273.0f},
        {4 / 273.0f, 16 / 273.0f, 26 / 273.0f, 16 / 273.0f, 4 / 273.0f},
        {1 / 273.0f, 4 / 273.0f, 7 / 273.0f, 4 / 273.0f, 1 / 273.0f}
    };

    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] = gaussianValues[i][j];
        }
    }

    // Allocate a temporary image for storing the blurred result
    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);

    // Apply the convolution using the kernel
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    // Copy the blurred image back to the original image
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    // Free resources
    bmp24_free(tempImg);
    freeKernel(kernel, kernelSize);
}
// OUTLINE
void bmp24_outline(t_bmp24 *img) {
    int kernelSize = 3;

    // Dynamically allocate the kernel
    float **kernel = allocateKernel(kernelSize);
    if (!kernel) {
        return;
    }

    // Initialize the kernel values (outline kernel)
    float outlineValues[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] = outlineValues[i][j];
        }
    }

    // Allocate a temporary image for storing the outlined result
    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);

    // Apply the convolution using the kernel
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    // Copy the outlined image back to the original image
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    // Free resources
    bmp24_free(tempImg);
    freeKernel(kernel, kernelSize);
}
// EMBROSS
void bmp24_emboss(t_bmp24 *img) {
    int kernelSize = 3;

    // Dynamically allocate the kernel
    float **kernel = allocateKernel(kernelSize);
    if (!kernel) {
        return;
    }

    // Initialize the kernel values (emboss kernel)
    float embossValues[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };

    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] = embossValues[i][j];
        }
    }

    // Allocate a temporary image for storing the embossed result
    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);
    // Apply the convolution using the kernel
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
        }
    }

    // Copy the embossed image back to the original image
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    // Free resources
    bmp24_free(tempImg);
    freeKernel(kernel, kernelSize);
}