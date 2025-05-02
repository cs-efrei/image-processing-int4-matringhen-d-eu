#include <stdio.h>
#include "launcher_func.h"

int main(){
    int val = load_menu();
switch (val) {
        case 1:
            printf("We will need you to say if it is a bmp8 or bmp24. (1 or 2)\n");
            scanf("%d",&val);
            printf(">>> Your choice : %d \n,val");
        if (val == 1) {
            print("bmp8...\n");
            print("Please assure your file is in the following path : c-imagery_main/t_bmp8/\n");
            print("Please input your file name (don't forget .bmp) :\n");
            scanf("%20s",name); // [20 char is sufficient including .bmp
            t_bmp8 *img = bmp8_loadImage(name);
        }
        else{
            print("bmp24...\n");
            print("Please assure your file is in the following path : c-imagery_main/t_bmp24/\n");
            print("Please input your file name (don't forget .bmp) :\n");
            scanf("%20s",name); // [20 char is sufficient including .bmp
            t_bmp24 *image = bmp24_loadImage(name);
        }
        break;
////////////////////////////////////////////
        case 2:
            printf("\n");
        break;
////////////////////////////////////////////
        case 3:
            printf("\n");
        break;
////////////////////////////////////////////
        case 4:
            printf("\n");
        break;
////////////////////////////////////////////
        case 5:
            printf("You entered five, Bye bye !\n");
        return 0;
        break;
////////////////////////////////////////////
        default:
            printf("ERROR not in range. -_- \n");
        load_menu();
        return 0;
        break;
    }




    return 0;
}