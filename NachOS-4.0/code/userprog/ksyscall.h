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

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysSub(int op1, int op2)
{
  return op1 - op2;
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
