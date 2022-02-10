/* readstr.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

char readstr[256];
int main()
{
  ReadStr(readstr, 12);
  Halt();
  /* not reached */
}
