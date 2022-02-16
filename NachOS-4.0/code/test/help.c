/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int
main()
{
  
  PrintStr(" _______________________________________\n");
  PrintStr("|           NHOM CHUA BIET              |\n");          
  PrintStr("|       20127539 TRAN DUY KHUONG        |\n");    
  PrintStr("|       20127454 TRAN HUU CHINH         |\n");
  PrintStr("|       20127540 TRAN MANH KHUONG       |\n");
  PrintStr("|_______________________________________|\n");

  PrintStr(" ________________________________________________________________________________________________\n");
  PrintStr("|                                             MENU                                               |\n");
  PrintStr("|   [1]  SORT: Chuong trinh dung de sap xep mang tang dan hoac giam dan gom N phan tu (N<100)    |\n");
  PrintStr("|                                                                                                |\n");
  PrintStr("|   [2]  ASCII: IN ra tat ca cac ki tu co the doc duoc trong ma ASCII                            |\n");
  PrintStr("|________________________________________________________________________________________________|\n");

  Halt();
  /* not reached */
}