#include "bmc.h"
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
// BOX BLUR
void bmp24_boxBlur(t_bmp24 *img) {
    float kernel[3][3] = {
        {1.0f / 9, 1.0f / 9, 1.0f / 9},
        {1.0f / 9, 1.0f / 9, 1.0f / 9},
        {1.0f / 9, 1.0f / 9, 1.0f / 9}
    };

    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, (float **)kernel, 3);
        }
    }

    // Copy blurred image back to original
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    bmp24_free(tempImg);
}
// GAUSSIAN
void bmp24_gaussianBlur(t_bmp24 *img) {
    float kernel[5][5] = {
        {1/273.0f, 4/273.0f, 6/273.0f, 4/273.0f, 1/273.0f},
        {4/273.0f, 16/273.0f, 24/273.0f, 16/273.0f, 4/273.0f},
        {6/273.0f, 24/273.0f, 36/273.0f, 24/273.0f, 6/273.0f},
        {4/273.0f, 16/273.0f, 24/273.0f, 16/273.0f, 4/273.0f},
        {1/273.0f, 4/273.0f, 6/273.0f, 4/273.0f, 1/273.0f}
    };

    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, (float **)kernel, 5);
        }
    }

    // Copy blurred image back to original
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    bmp24_free(tempImg);
}
// OUTLINE
void bmp24_outline(t_bmp24 *img) {
    float kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, (float **)kernel, 3);
        }
    }

    // Copy outlined image back to original
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    bmp24_free(tempImg);
}
// EMBROSS
void bmp24_emboss(t_bmp24 *img) {
    float kernel[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };

    t_bmp24 *tempImg = bmp24_allocate(img->width, img->height, img->colorDepth);

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            tempImg->data[y][x] = bmp24_convolution(img, x, y, (float **)kernel, 3);
        }
    }

    // Copy embossed image back to original
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x] = tempImg->data[y][x];
        }
    }

    bmp24_free(tempImg);
}