#include "bmc.h"

t_bmp8 * bmp8_loadImage(const char * filename){
    FILE *file= fopen(filename, "rb");
    if (!file) {
        fclose(file);
        perror("There were an error to open the file");
        return NULL;
    }
    t_bmp8 *img=(t_bmp8 *)malloc(sizeof(t_bmp8));
    if (!img){
        fclose(file);
        perror("There where an error in allocating memory to the image");
        return NULL;
    }

    //read the header
    fread(img->header, 1, 54, file);

    img->width = *(unsigned int *)&img->header[18];
    img->height= *(unsigned int *)&img->header[22];
    img->colorDepth= *(unsigned int *)&img->header[28];
    img->dataSize= *(unsigned int *)&img->header[34];

    fread(img->colorTable, 1, 1024, file);

    img->data = (unsigned char *)malloc(img->dataSize);
    if (!img->data) {
        free(img);
        fclose(file);
        perror("There were an issue allocating memory for the image data");
        return NULL;
    }
    fread(img->data, 1, img->dataSize, file);
    fclose(file);
    return img;

}

void bmp8_saveImage(const char * filename, t_bmp8 * img){
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("There where an error opening file");
        return;
    }
    fwrite(img->header, 1, 54, file);
    fwrite(img->colorTable, 1, 1024, file);
    fwrite(img->data, 1, img->dataSize, file);
    fclose(file);
    return;
}

void bmp8_free(t_bmp8 * img){
    if (img) {
        if (img->data) {
            free(img->data);
        }
        free(img);
    }
}

void bmp8_printInfo(t_bmp8 * img){
    if (img) {
        printf("Image Information:\n");
        printf("Width: %u\n", img->width);
        printf("Height: %u\n", img->height);
        printf("Color Depth: %u\n", img->colorDepth);
        printf("Data Size: %u\n", img->dataSize);
    } else {
        printf("Image is NULL\n");
    }
}


// IMAGE PROCESSING FUNCTIONS


void bmp8_negative(t_bmp8 * img){
    if (img) {
        for (unsigned int i = 0; i < img->dataSize; i++) {
            img->data[i] = 255 - img->data[i];
        }
    }
}



//testing

// Load the image and store the result in a variable
