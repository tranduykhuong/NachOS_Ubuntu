#include "syscall.h"

int
main()
{
  int result;
  
  result = Multi(42, 23);

  Halt();
  /* not reached */
}