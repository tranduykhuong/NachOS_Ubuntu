
/* readstr.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int main()
{
  char readstr[256];
  int n;
  PrintStr("Enter string's length: ");
  n = ReadNum();
  PrintStr("Enter string: ");
  ReadStr(readstr, n);
  PrintStr(readstr);
  PrintChar('\n');
  Halt();
  /* not reached */
}