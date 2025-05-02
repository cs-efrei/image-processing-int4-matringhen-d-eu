#include "launcher_func.h"

int load_menu(){
  int val = 5;
  printf("Welcome to our menu :\n");
  printf("  1. Open an Image\n");
  printf("  2. Save an Image\n");
  printf("  3. Apply a filter\n");
  printf("  4. Display image information\n");
  printf("  5. Quit\n");

  scanf("%d",&val);
  printf(">>> Your choice : %d \n,val");

  switch (val) {
    case 1:
      printf("We will need you to input the file path.\n");
    break;
    case 2:
      printf("\n");
    break;
    case 3:
      printf("\n");
    break;
    case 4:
      printf("\n");
    break;
    case 5:
      printf("You entered five, Bye bye !\n");
    break;
    default:
      printf("ERROR not in range. -_- \n");

    break;
  }

  return 0;
}


  return val;
}