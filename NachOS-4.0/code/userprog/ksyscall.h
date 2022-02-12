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

#define LF ((char)10)
#define CR ((char)13)
#define TAB ((char)9)
#define SPACE ((char)' ')

/* maximum length of an interger (included the minus sign) */
#define MAX_NUM_LENGTH 11
/* A buffer to read and write number */
char _numberBuffer[MAX_NUM_LENGTH + 2];

char isBlank(char c)
{
  return c == LF || c == CR || c == TAB || c == SPACE;
}
/** Read and store characters in the _numberBuffer until blank
 * or end of file
 * It will read at most MAX_NUM_LENGTH + 1 character
 **/

// These macros must exactly match those in the Windows SDK's intsafe.h.
#define INT8_MIN (-127i8 - 1)
#define INT16_MIN (-32767i16 - 1)
#define INT32_MIN (-2147483647 - 1)
#define INT64_MIN (-9223372036854775807i64 - 1)
#define INT8_MAX 127i8
#define INT16_MAX 32767i16
#define INT32_MAX 2147483647i32
#define INT64_MAX 9223372036854775807i64
#define UINT8_MAX 0xffui8
#define UINT16_MAX 0xffffui16
#define UINT32_MAX 0xffffffffui32
#define UINT64_MAX 0xffffffffffffffffui64

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysMulti(int op1, int op2)
{
  return op1 * op2;
}

bool compareNum_String(int integer, const char *s)
{
  if (integer == 0)
    return strcmp(s, "0") == 0;

  int length = strlen(s);

  if (integer < 0 && s[0] != '-')
  {
    return false;
  }
  if (integer < 0)
  {
    s++, --length, integer = -integer;
  }

  while (integer > 0)
  {
    int digit = integer % 10;
    if ((s[length - 1] == '0') != digit)
    {
      return false;
    }
    --length;
    integer /= 10;
  }
  return length == 0;
}

void readUntilBlank()
{
  memset(_numberBuffer, 0, sizeof(_numberBuffer));
  char c = kernel->synchConsoleIn->GetChar();
  if (c == EOF)
  {
    DEBUG(dbgSys, "Unexpected end of file - number expected");
    return;
  }

  if (isBlank(c))
  {
    DEBUG(dbgSys, "Unexpected end of file - number expected");
    return;
  }

  int n = 0;

  while (!(isBlank(c) || c == EOF))
  {
    _numberBuffer[n++] = c;
    if (n > MAX_NUM_LENGTH)
    {
      DEBUG(dbgSys, "Number is too long");
      return;
    }
    c = kernel->synchConsoleIn->GetChar();
  }
}

int SysReadNum()
{
  readUntilBlank();

  int length = strlen(_numberBuffer);
  // Read nothing -> return 0
  if (length == 0)
  {
    return 0;
  }
  // Check comment below to understand this line of code
  if (strcmp(_numberBuffer, "-2147483648") == 0)
  {
    return INT32_MIN;
  }
  bool _negative = (_numberBuffer[0] == '-');
  int zero = 0;
  bool is_Leading = true;
  int number = 0;
  for (int i = _negative; i < length; ++i)
  {
    char c = _numberBuffer[i];
    if (c == '0' && is_Leading)
    {
      ++zero;
    }
    else
    {
      is_Leading = false;
    }
    if (c < '0' || c > '9')
    {
      DEBUG(dbgSys, "Ecpected number but " << _numberBuffer << " found");
      return 0;
    }
    number = number * 10 + (c - '0');
  }

  // 00            01 or -0
  if (zero > 1 || (zero && (number || _negative)))
  {
    DEBUG(dbgSys, "Expected number but " << _numberBuffer << " found");
    return 0;
  }

  if (_negative)
  {
    /*
     * This is why we need to handle -2147483648 individually:
     * 2147483648 is larger than the range of int32
     */
    number = -number;
  }
  // It's safe to return directly if the number is small
  if (length <= MAX_NUM_LENGTH - 2)
  {
    return number;
  }

  /* We need to make sure that number is equal to the number in the buffer. */
  if (compareNum_String(number, _numberBuffer))
  {
    return number;
  }
  else
  {
    DEBUG(dbgSys, "Expected number but " << _numberBuffer << " found");
  }
  return 0;
}


void SysPrintNum(int number)
{
  if (number == 0)
  {
    return kernel->synchConsoleOut->PutChar('0');
  }
  if (number == INT32_MIN)
  {
    kernel->synchConsoleOut->PutChar('-');
    for (int i = 0; i < 10; i++)
    {
      kernel->synchConsoleOut->PutChar("2147483648"[i]);
    }
    return;
  }

  if (number < 0)
  {
    kernel->synchConsoleOut->PutChar('-');
    number = -number;
  }

  int n = 0;
  while (number)
  {
    _numberBuffer[n++] = number % 10;
    number /= 10;
  }

  for (int i = n - 1; i >= 0; --i)
  {
    kernel->synchConsoleOut->PutChar(_numberBuffer[i] + '0');
  }
}


char SysReadChar()
{
  return kernel->synchConsoleIn->GetChar();
}
void SysPrintChar(char character)
{
  kernel->synchConsoleOut->PutChar(character);
}
int SysRandomNum()
{
  return random();
}

char *SysReadStr(int size)
{
  char *buffer = new char[size + 1];
  for (int i = 0; i < size; i++)
    buffer[i] = kernel->synchConsoleIn->GetChar();
  buffer[size] = '\0';

  return buffer;
}

void SysPrintStr(char *buffer, int length)
{
  for (int i = 0; i < length; i++)
    kernel->synchConsoleOut->PutChar(buffer[i]);
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
