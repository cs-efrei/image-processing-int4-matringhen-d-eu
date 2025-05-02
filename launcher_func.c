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
  return val;
}