/* printstr.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int main()
{
  PrintStr("Chinh dep trai\n");
  Halt();
  /* not reached */
}
