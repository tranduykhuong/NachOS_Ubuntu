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
#include "synchconsole.h"
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

void SyscallReadStr_Handler()
{
	int addStr;
	addStr = (int)kernel->machine->ReadRegister(4); // ?????c ?????a ch??? c???a chu???i t??? thanh ghi
	int length;
	length = (int)kernel->machine->ReadRegister(5); // ?????c ????? d??i chu???i m?? ng?????i d??ng cho tr?????c t??? thanh ghi

	// ki???m tra chi???u d??i c?? v?????t qu?? gi???i h???n c???a m???t chu???i string hay kh??ng?(t???i ??a 255)
	if (length > MAX_STRING)
	{
		DEBUG(dbgSys, "Chu???i v?????t qu?? " << MAX_STRING << " k?? t???\n");
		SysHalt();
	}

	// X??? l?? SysReadStr
	char *buff;
	buff = SysReadStr(length);
	System2User(buff, addStr); // chuy???n ?????i chu???i h??? th???ng th??nh chu???i ng?????i d??ng
	delete[] buff;			   // x??a v??ng nh??? ????? tr??nh b??? r?? r???

	/* Modify return point */
	NextPC();
}

void SyscallReadString_Handler()
{
	int addStr;
	addStr = (int)kernel->machine->ReadRegister(4); // ?????c ?????a ch??? c???a chu???i t??? thanh ghi
	
	// X??? l?? SysReadStr
	char *buff;
	buff = SysReadString();
	System2User(buff, addStr); // chuy???n ?????i chu???i h??? th???ng th??nh chu???i ng?????i d??ng
	delete[] buff;			   // x??a v??ng nh??? ????? tr??nh b??? r?? r???

	/* Modify return point */
	NextPC();
}

void SyscallPrintString_Handler()
{
	int addStr = (int)kernel->machine->ReadRegister(4); // ?????c ?????a ch??? c???a chu???i t??? thanh ghi

	char *buffer;
	buffer = User2System(addStr); // chuy???n ?????i chu???i ng?????i d??ng th??nh chu???i h??? th???ng

	// X??? l?? SysPrintStr
	SysPrintStr(buffer, strlen(buffer));
	delete[] buffer; // x??a v??ng nh??? ????? tr??nh b??? r?? r???

	/* Modify return point */
	NextPC();
}

/**********************************************/
void SyscallCreateFile_Handler()
{
	int virAddress;
	char *fileName;
	virAddress = kernel->machine->ReadRegister(4);
	fileName = User2System(virAddress);
	if (SysCreate(fileName))
		kernel->machine->WriteRegister(2, 0);
	else
		kernel->machine->WriteRegister(2, -1);
	delete[] fileName;
	NextPC();
}

void SyscallOpenFile_Handler()
{
	int virAddress;
	char *fileName;
	virAddress = kernel->machine->ReadRegister(4);
	int idFile;
	fileName = User2System(virAddress);
	idFile = (int)kernel->fileSystem->openFile(fileName);
	kernel->machine->WriteRegister(2, idFile);
	delete[] fileName;
	NextPC();
}

void SyscallCloseFile_Handler()
{
	int idFile, flag;
	idFile = kernel->machine->ReadRegister(4);
	flag = kernel->fileSystem->Close(idFile);
	kernel->machine->WriteRegister(2, flag);
	NextPC();
}

void SyscallReadFile_Handler()
{
	int address = kernel->machine->ReadRegister(4);
	int numCharCount = kernel->machine->ReadRegister(5);
	int fileId = kernel->machine->ReadRegister(6);
	char *buffer = new char[numCharCount];

	int size = SysReadFile(buffer, numCharCount, fileId);
	// cerr << size;
	kernel->machine->WriteRegister(2, size);
	buffer[numCharCount] = '\0';
	System2User(buffer, address);
	// cerr << buffer;
	delete[] buffer;
	return NextPC();
}

void SyscallWriteFile_Handler()
{
	int address = kernel->machine->ReadRegister(4);
	int numCharCount = kernel->machine->ReadRegister(5);
	int fileId = kernel->machine->ReadRegister(6);
	char *buffer = User2System(address);

	kernel->machine->WriteRegister(2, SysWriteFile(buffer, numCharCount, fileId));
	System2User(buffer, address, numCharCount);

	delete[] buffer;
	return NextPC();
}

void SyscallRemoveFile_Handler()
{
	char *fileName;
	int virAddress = kernel->machine->ReadRegister(4);
	fileName = User2System(virAddress);
	kernel->machine->WriteRegister(2, (int)kernel->fileSystem->Remove(fileName));
	delete[] fileName;
	NextPC();
}

void SyscallSeekFile_Handler()
{
	int position = kernel->machine->ReadRegister(4);
	int id = kernel->machine->ReadRegister(5);
	int flag;
	if (id <= 1) // if file's id is 0 or 1 (console IO), return -1
	{
		DEBUG(dbgSys, "\nCan't seek in console");
		flag = -1;
	}
	else
		flag = kernel->fileSystem->Seek(position, id);
	kernel->machine->WriteRegister(2, flag); // return the position that file pointer is standing
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
		DEBUG('a', "\nNo valid translation found.");
		printf("\n\nNo valid translation found.");
		SysHalt();
		ASSERTNOTREACHED();
		break;
	case ReadOnlyException:
		DEBUG('a', "\nWrite attempted to page marked \"read-only\".");
		printf("\n\nWrite attempted to page marked \"read-only\".");
		SysHalt();
		ASSERTNOTREACHED();
		break;
	case BusErrorException:
		DEBUG('a', "\nTranslation resulted in an invalid physical address.");
		printf("\n\nTranslation resulted in an invalid physical address.");
		SysHalt();
		ASSERTNOTREACHED();
		break;
	case AddressErrorException:
		DEBUG('a', "\nUnaligned reference or one that was beyond the end of the address space.");
		printf("\n\nUnaligned reference or one that was beyond the end of the address space.");
		SysHalt();
		ASSERTNOTREACHED();
		break;
	case OverflowException:
		DEBUG('a', "\nInteger overflow in add or sum.");
		printf("\n\nInteger overflow in add or sum.");
		SysHalt();
		ASSERTNOTREACHED();
		break;
	case IllegalInstrException:
		DEBUG('a', "\nUnimplemented or reserved instr.");
		printf("\n\nUnimplemented or reserved instr.");
		SysHalt();
		ASSERTNOTREACHED();
		break;
	case NumExceptionTypes:
		DEBUG('a', "\nNumber exception.");
		printf("\n\nNumber exception.");
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
		{
			int result3 = SysReadNum();
			kernel->machine->WriteRegister(2, (int)result3);
			NextPC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_PrintNum:
		{
			int character = kernel->machine->ReadRegister(4);
			SysPrintNum(character);
			NextPC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_ReadCh:
		{
			char temp;
			temp = SysReadChar();
			kernel->machine->WriteRegister(2, temp);
			NextPC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_PrintCh:
		{
			char temp;
			temp = kernel->machine->ReadRegister(4);
			SysPrintChar(temp);
			NextPC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_RandomNum:
		{
			int temp;
			temp = SysRandomNum();
			kernel->machine->WriteRegister(2, temp);
			NextPC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		case SC_ReadStr:
			SyscallReadStr_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_ReadString:
			SyscallReadString_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_PrintStr:
			SyscallPrintString_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_CreateFile:
			SyscallCreateFile_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_OpenFile:
			SyscallOpenFile_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_CloseFile:
			SyscallCloseFile_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_ReadFile:
			SyscallReadFile_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_WriteFile:
			SyscallWriteFile_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_SeekFile:
			SyscallSeekFile_Handler();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_RemoveFile:
			SyscallRemoveFile_Handler();
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
