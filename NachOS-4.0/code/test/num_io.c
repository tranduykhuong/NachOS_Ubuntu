/* num_io.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a syscall that read an interger and print it
 *
 */

#include "syscall.h"

int main() { 
  PrintStr("Enter interger number: ");
  PrintNum(ReadNum());
  PrintChar('\n');
  Halt();
}