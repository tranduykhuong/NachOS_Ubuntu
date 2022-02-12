// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

#define MAX_STRING 255

/**
 * Convert user string to system string
 *
 * param addr: addess of user string
 * param _length: set max length of string to convert,
				  if default param to convert all characters of user string
 * return char*
 **/
char *User2System(int addr, int _length = -2)
{
	_length++;
	int length = 0, size = 0;
	char *kernelStr = NULL;

	// check _length invalid
	if (_length < -1)
		return kernelStr;

	// count length of string
	while (1)
	{
		int oneChar;
		kernel->machine->ReadMem(addr + length, 1, &oneChar);
		length++;

		// End string '\0'
		if (oneChar == '\0')
			break;
	};

	// Get min length to convert
	if (_length == -1)
		size = length;
	else
		size = length < _length ? length : _length;

	kernelStr = new char[size];
	int i;
	for (i = 0; i < size; i++)
	{
		int oneChar;
		kernel->machine->ReadMem(addr + i, 1, &oneChar);
		kernelStr[i] = (unsigned char)oneChar;

		if (oneChar == '\0')
			break;
	}
	i = (i == size) ? i - 1 : i;
	kernelStr[i] = (unsigned char)'\0';
	return kernelStr;
}

/**
 * Convert system string to user string
 *
 * param str: string to convert
 * param addr: addess of user string
 * param _length: set max length of string to convert,
				  if default param to convert all characters of system string
 */
void System2User(char *str, int addr, int _length = -1)
{
	int length = strlen(str);
	if (_length < -1)
		return;

	int size = (_length == -1 || _length > length ? length : _length);
	for (int i = 0; i < size; i++)
	{
		kernel->machine->WriteMem(addr + i, 1, str[i]);
	}
	kernel->machine->WriteMem(addr + size, 1, '\0');
}

// Increasing PC
void NextPC()
{
	/* registers[PrevPCReg] = registers[PCReg]; */
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* registers[PCReg] = registers[NextPCReg]; */
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* registers[NextPCReg] = pcAfter; */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

void SyscallReadNum_Handler()
{
	int result = SysReadNum();
	kernel->machine->WriteRegister(2, (int)result);
	return NextPC();
}

void SyscallPrintNum_Handler()
{
	int character = kernel->machine->ReadRegister(4);
	SysPrintNum(character);
	return NextPC();
}

void SyscallReadChar_Handler()
{
	char temp;
	temp = SysReadChar();
	kernel->machine->WriteRegister(2, temp);
	NextPC();
}

void SyscallPrintChar_Handler()
{
	char temp;
	temp = kernel->machine->ReadRegister(4);
	SysPrintChar(temp);
	NextPC();
}

void SyscallRandomNum_Handler()
{
	int temp;
	temp = SysRandomNum();
	kernel->machine->WriteRegister(2, temp);
	NextPC();
}

void SyscallReadString_Handler()
{
	int addStr;
	addStr = (int)kernel->machine->ReadRegister(4); // Đọc địa chỉ của chuỗi từ thanh ghi
	int length;
	length = (int)kernel->machine->ReadRegister(5); // Đọc độ dài chuỗi mà người dùng cho trước từ thanh ghi

	// kiểm tra chiểu dài có vượt quá giới hạn của một chuỗi string hay không?(tối đa 255)
	if (length > MAX_STRING)
	{
		DEBUG(dbgSys, "Chuỗi vượt quá " << MAX_STRING << " kí tự\n");
		SysHalt();
	}

	// Xử lí SysReadStr
	char *buff;
	buff = SysReadStr(length);
	System2User(buff, addStr); // chuyển đổi chuỗi hệ thống thành chuỗi người dùng
	delete[] buff;			   // xóa vùng nhớ để tránh bị rò rỉ

	/* Modify return point */
	NextPC();
}

void SyscallPrintString_Handler()
{
	int addStr = (int)kernel->machine->ReadRegister(4); // Đọc địa chỉ của chuỗi từ thanh ghi

	char *buffer;
	buffer = User2System(addStr); // chuyển đổi chuỗi người dùng thành chuỗi hệ thống

	// Xử lí SysPrintStr
	SysPrintStr(buffer, strlen(buffer));
	delete[] buffer; // xóa vùng nhớ để tránh bị rò rỉ

	/* Modify return point */
	NextPC();
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case NoException: // return control to kernel
		DEBUG(dbgSys, "Switch to system mode\n");
		kernel->interrupt->setStatus(SystemMode);
		break;

	case PageFaultException:
	case ReadOnlyException:
	case BusErrorException:
	case AddressErrorException:
	case OverflowException:
	case IllegalInstrException:
	case NumExceptionTypes:
		DEBUG(dbgAddr, "\nRuntime error\n");
		cerr << "Error " << which << " occured\n";
		SysHalt();
		ASSERTNOTREACHED();
		break;

	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			NextPC();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_Multi:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result2;
			result2 = SysMulti((int)kernel->machine->ReadRegister(4),
							   (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result2 << "\n");
			kernel->machine->WriteRegister(2, (int)result2);
			NextPC();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_ReadNum:
			SyscallReadNum_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_PrintNum:
			SyscallPrintNum_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_ReadCh:
			SyscallReadChar_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_PrintCh:
			SyscallPrintChar_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_RandomNum:
			SyscallRandomNum_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_ReadStr:
			SyscallReadString_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_PrintStr:
			SyscallPrintString_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
