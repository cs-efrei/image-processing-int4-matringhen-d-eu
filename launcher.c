#include <stdio.h>
#include "launcher_func.h"
#include "t_bmp8/bmc.h"
#include "t_bmp24/bmc.h"

int main(){
    char name[21];
    int type = 0;
    char full_path[50];
    int val = load_menu();
    t_bmp8 *img8 = NULL;
    t_bmp24 *img24 = NULL;

switch (val) {
    //////////////////////////////////////////// LOAD
        case 1:
            printf("We will need you to say if it is a bmp8 or bmp24. (1 or 2)\n");
            scanf("%d",&val);
            printf(">>> Your choice : %d \n",val);
        if (val == 1) {
            printf("bmp8...\n");
            printf("Please assure your file is in the following path : c-imagery_main/t_bmp8/\n");
            printf("Please input your file name (don't forget .bmp) :\n");
            scanf("%20s",name); // [20 char is sufficient including .bmp]
            //sprintf(full_path, "t_bmp8/%s", name);
            //printf("Loading : %s\n", full_path);
            t_bmp8 * img8 = bmp8_loadImage(name);
            type = 8;
        }
        else{
            printf("bmp24...\n");
            printf("Please assure your file is in the following path : c-imagery_main/t_bmp24/\n");
            printf("Please input your file name (don't forget .bmp) :\n");
            scanf("%20s",name); // [20 char is sufficient including .bmp]
            //sprintf(full_path, "t_bmp24/%s", name);
            //printf("Loading : %s\n", full_path);
            t_bmp24 * img24 = bmp24_loadImage(full_path);
            type = 24;
        }
        break;
//////////////////////////////////////////// SAVE
        case 2:
            if (type == 8) {
                printf("Saving image");
                bmp8_saveImage(name, img8);
            }
            if (type == 24) {
                printf("Saving image");
                bmp24_saveImage(img24, name);
            }
        break;
//////////////////////////////////////////// APPLY FILTER
        case 3:
            if (type == 8) {
                printf("Choose from (0) Negative, (1) Brightness, (2) Threshold");
                int choice;
                scanf("Filter Choice : %d",&choice);
                switch (choice) {
                    case 0:
                        bmp8_negative(img8);
                    case 1:
                        int bri;
                    scanf("Brightness Limit : %d",&bri);
                        bmp8_brightness(img8, bri);
                    case 2:
                        int thress;
                        scanf("Threshold Limit : %d",&thress);
                        bmp8_threshold(img8,thress);
                }
            }
            if (type == 24) {
                printf("Choose from (0) Negative, (1) Grayscale  (2) Brightness");
                int choice;
                scanf("Filter Choice : %d",&choice);
                switch (choice) {
                    case 0:
                        bmp24_negative(img24);
                    case 2:
                        int bri;
                    scanf("Brightness Limit : %d",&bri);
                        bmp24_brightness(img24,bri);
                    case 1 :
                    bmp24_grayscale(img24);
                }

            }
        break;
//////////////////////////////////////////// DISPLAY INFO
        case 4:
            if (type == 8) {
                printf("Infos are : \n");
                bmp8_printInfo(img8);
            }
            if (type == 24) {
                bmp24_printInfo(img24);
            }
        break;
//////////////////////////////////////////// QUIT
        case 5:
            printf("You entered five, Bye bye !\n");
        break;
////////////////////////////////////////////
        default:
            printf("ERROR not in range. -_- \n");
        break;
    }




    return 0;
}