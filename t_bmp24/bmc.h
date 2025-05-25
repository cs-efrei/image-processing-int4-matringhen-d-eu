#include <stdio.h>
#include <stdint.h>
// 21/04/2025
// main

// for header
#pragma pack(push, 1) // Ensure 1-byte alignment
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

// for info
typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;
#pragma pack(pop) // Restore previous alignment

// single pixel
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;

// Offsets for the BMP header
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34
// Magical number for BMP files
#define BMP_TYPE 0x4D42 // 'BM' in hexadecimal
// Header sizes
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets
// Constant for the color depth
#define DEFAULT_DEPTH 0x18 // 24

////////////////////////////////////////////////////////////////////// 2.3
// Function to allocate memory for pixel data
t_pixel** bmp24_allocateDataPixels(int width, int height);
// Function to free allocated pixel data
void bmp24_freeDataPixels(t_pixel **pixels, int height);
// Function to allocate a new BMP image
t_bmp24* bmp24_allocate(int width, int height, int colorDepth);
// Function to free a BMP image
void bmp24_free(t_bmp24 *img);
/////////////////////////////////////////////////////////////////////// 2.4.1
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);
/////////////////////////////////////////////////////////////////////// 2.4.2
// Function to read the pixel value at position (x, y) in the image
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file);
// Function to read all pixel data from the file into the image
void bmp24_readPixelData(t_bmp24 *image, FILE *file);
    // Function to write the pixel value at position (x, y) in the image to the file
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file);
// Function to write all pixel data from the image to the file
void bmp24_writePixelData(t_bmp24 *image, FILE *file);
/////////////////////////////////////////////////////////////////////// 2.4 ^^ need these
t_bmp24 * bmp24_loadImage (const char * filename);
void bmp24_saveImage (t_bmp24 * img, const char * filename);
/////////////////////////////////////////////////////////////////////// 2.5
void bmp24_negative (t_bmp24 * img);
void bmp24_grayscale (t_bmp24 * img);
void bmp24_brightness (t_bmp24* img, int value);
 /////////////////////////////////////////////////////////////////////// 2.6
t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize);
void bmp24_boxBlur(t_bmp24 *img);
void bmp24_gaussianBlur(t_bmp24 *img);
void bmp24_outline(t_bmp24 *img);
void bmp24_emboss(t_bmp24 *img);
void bmp24_printInfo(t_bmp24 *img);

// 27/04/2025