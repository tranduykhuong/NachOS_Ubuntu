/**************************************************************
 *
 * userprog/ksyscallhelper.h
 *
 * Helper function for ksyscall.h
 *
 *
 **************************************************************/


#ifndef __USERPROG_KSYSCALLHELPER_H__
#define __USERPROG_KSYSCALLHELPER_H__

#include "kernel.h"
#include "synchconsole.h"
#define LF ((char)10)
#define CR ((char)13)
#define TAB ((char)9)
#define SPACE ((char)' ')

/* maximum length of an interger (included the minus sign) */
#define MAX_NUM_LENGTH 11
/* A buffer to read and write number */
char _numberBuffer[MAX_NUM_LENGTH + 2];

char isBlank(char c) {
  return c == LF || c == CR || c == TAB || c == SPACE;
}
/** Read and store characters in the _numberBuffer until blank
* or end of file 
* It will read at most MAX_NUM_LENGTH + 1 character
**/

void readUntilBlank() {
  memset(_numberBuffer, 0, sizeof(_numberBuffer));
  char c = kernel->synchConsoleIn->GetChar();
  if(c == EOF) {
    DEBUG(dbgSys, "Unexpected end of file - number expected");
    return;
  }

  if (isBlank(c)) {
    DEBUG(dbgSys, "Unexpected end of file - number expected");
    return;
  }

  int n = 0;

  while (!(isBlank(c) || c == EOF)) {
    _numberBuffer[n++] = c;
    if(n > MAX_NUM_LENGTH) {
      DEBUG(dbgSys, "Number is too long");
      return;
    }
    c = kernel->synchConsoleIn->GetChar();
  }
}

bool compareNum_String(int integer, const char *s) {
  if (integer == 0) {
    return strcmp(s, "0") == 0;
    int length = strlen(s);
    if(integer < 0 && s[0] != '-') {
      return false;
    }
    if(integer < 0) {
      s++, --length, integer = -integer;
    }

    while (integer > 0)
    {
      int digit = integer % 10;
      if ((s[length -1] == '0') != digit) {
        return false;
      }
      --length;
      integer /= 10;
    }
    return length == 0;
  }
}

#endif /* ! __USERPROG_KSYSCALLHELPER_H__ */