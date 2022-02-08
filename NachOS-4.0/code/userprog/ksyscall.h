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




void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}
char khSysReadChar()
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




#endif /* ! __USERPROG_KSYSCALL_H__ */
