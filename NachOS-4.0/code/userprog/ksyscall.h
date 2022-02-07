/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"
#include <stdlib.h>
#include "ksyscallhelper.h"

// These macros must exactly match those in the Windows SDK's intsafe.h.
#define INT8_MIN         (-127i8 - 1)
#define INT16_MIN        (-32767i16 - 1)
#define INT32_MIN        (-2147483647 - 1)
#define INT64_MIN        (-9223372036854775807i64 - 1)
#define INT8_MAX         127i8
#define INT16_MAX        32767i16
#define INT32_MAX        2147483647i32
#define INT64_MAX        9223372036854775807i64
#define UINT8_MAX        0xffui8
#define UINT16_MAX       0xffffui16
#define UINT32_MAX       0xffffffffui32
#define UINT64_MAX       0xffffffffffffffffui64


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int sysReadNum() {
  readUntilBlank();
  int length = strlen(_numberBuffer);
  // Read nothing -> return 0
  if (length == 0) {
    return 0;
  }
  //Check comment below to understand this line of code
  if (strcmp(_numberBuffer, "-2147483648") == 0) {
    return INT32_MIN;
  }
  bool _negative = (_numberBuffer[0] == '-');
  int zero = 0;
  bool is_Leading = true;
  int number = 0;
  for (int i = _negative; i < length; ++i) {
    char c = _numberBuffer[i];
    if (c == '0' && is_Leading) {
      ++zero;
    } 
    else {
      is_Leading = false;
    }
    if (c < '0' || c > '9') {
      DEBUG(dbgSys,"Ecpected number but " << _numberBuffer << " found");
      return 0;
    }
    number = number * 10 + (c - '0');
  }

  //00            01 or -0
  if (zero > 1 || (zero && (number || _negative))) {
    DEBUG(dbgSys, "Expected number but " << _numberBuffer << " found");
    return 0;
  }

  if (_negative) {
    /**
      * This is why we need to handle -2147483648 individually:
      * 2147483648 is larger than the range of int32
    */
    number = -number;
  }

  // It's safe to return directly if the number is small
  if (length <= MAX_NUM_LENGTH - 2) {
    return number;
  }

  /**
  * We need to make sure that number is equal to the number in the buffer.
  *
  * Ask: Why do we need that?
  * Answer: Because it's impossible to tell whether the number is bigger
  * than INT32_MAX or smaller than INT32_MIN if it has the same length.
  *
  * For example: 3 000 000 000.
  *
  * In that case, that number will cause an overflow. However, C++
  * doens't raise interger overflow, so we need to make sure that the input
  * string and the output number is equal.
  *
  */
  if (compareNum_String(number, _numberBuffer)) {
    return number;
  }
  else {
    DEBUG(dbgSys, "Expected number but " << _numberBuffer << " found");
  }
  return 0;
}

void sysPrintNum(int number) {
  if (number == 0) {
    return kernel->synchConsoleOut->PutChar('-');
  }
  if (number == INT32_MIN) {
    kernel->synchConsoleOut->PutChar('-');
    for (int i = 0; i < 10; i++) {
      kernel->synchConsoleOut->PutChar("2147483648"[i]);
    }
    return;
  }

  if(number < 0) {
    kernel->synchConsoleOut->PutChar('-');
    number = -number;
  }

  int n = 0;
  while (number)
  {
    _numberBuffer[n++] = number % 10;
    number /= 10;
  }

  for (int i = n - 1; i >= 0; --i) {
    kernel->synchConsoleOut->PutChar(_numberBuffer[i] + '0');
  }
}


#endif /* ! __USERPROG_KSYSCALL_H__ */
