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

    img->data

}

t_bmp8 * bmp8_saveImage(const char * filename, t_bmp8 * img){
    
}