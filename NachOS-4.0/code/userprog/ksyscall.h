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
#define INT32_MIN (-2147483647 - 1)
#define INT32_MAX (2147483647)

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

int SysReadNum()
{
  // listen and read until blank
  memset(_numberBuffer, 0, sizeof(_numberBuffer));
  char c = kernel->synchConsoleIn->GetChar();
  if (c == EOF || isBlank(c))
  {
    DEBUG(dbgSys, "Unexpected end of file - number expected");
    return 0;
  }

  int n = 0;

  while (!(isBlank(c) || c == EOF))
  {
    _numberBuffer[n++] = c;
    if (n > MAX_NUM_LENGTH)
    {
      DEBUG(dbgSys, "Number is too long");
      return 0;
    }
    c = kernel->synchConsoleIn->GetChar();
  }
  _numberBuffer[n] = '\0';

  int length = n;
  // Read nothing -> return 0
  if (length == 0)
    return 0;

  // Check number is MIN_INT
  if (strcmp(_numberBuffer, "-2147483648") == 0)
    return INT32_MIN;

  bool _negative = (_numberBuffer[0] == '-');
  int zero = 0; // check number zero at the begining of the string
  bool is_Leading = true;
  int number = 0;
  for (int i = _negative; i < length; ++i)
  {
    char c = _numberBuffer[i];

    if (c == '0' && is_Leading)
      ++zero;
    else
      is_Leading = false;

    if (number > 214748364 || (number == 214748364 && c > '7'))
      return 0;

    if (c < '0' || c > '9')
    {
      DEBUG(dbgSys, "Ecpected number but " << _numberBuffer << " found");
      return 0;
    }
    number = number * 10 + (c - '0');
  }

  if (_negative)
    number = -number;

  // 00            01 or -0 or -01
  if (zero > 1 || (zero && (number || _negative)))
  {
    DEBUG(dbgSys, "Expected number but " << _numberBuffer << " found");
    return number;
  }

  return number;
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
  srand(time(0));
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

int Create(char*FileName)
{
    int FileLenght=strlen(FileName);
    if(FileLenght==0)
    {
      DEBUG(dbgSys,"\nFile name can not be empty"); // Ten file khong duoc bo trong
      return 0;
    }
    else
    {
      DEBUG(dbgSys,"\nFinish reading filename");
      if(!kernel->fileSystem->Create(FileName))
      {
        DEBUG(dbgSys,"\nError create file");
        return 0;
      }
      else
        return 1;
    }

}

#endif /* ! __USERPROG_KSYSCALL_H__ */
