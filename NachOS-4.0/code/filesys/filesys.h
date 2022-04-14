// filesys.h
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system.
//	The "STUB" version just re-defines the Nachos file system
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of
//	a disk simulator.  The disk is simulated using the native UNIX
//	file system (in a file named "DISK").
//
//	In the "real" implementation, there are two key data structures used
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "sysdep.h"
#include "openfile.h"
#include <stdlib.h>

#define MAX_FILE 10

#ifdef FILESYS_STUB // Temporarily implement file system calls as
// calls to UNIX, until the real file system
// implementation is available
class FileSystem
{
private:
	struct FileFeature
	{
		OpenFile *open_file;
		char *name;
	};

public:
	FileFeature *openFileList;
	int size;

	FileSystem()
	{
		openFileList = new FileFeature[MAX_FILE];
		size = 0;
		for (int i = 0; i < MAX_FILE; i++)
		{
			openFileList[i].open_file = NULL;
			openFileList[i].name = NULL;
		}
	}
	~FileSystem()
	{
		for (int i = 0; i < MAX_FILE; i++)
		{
			if (openFileList[i].open_file != NULL)
			{
				delete openFileList[i].open_file;
				openFileList[i].open_file = NULL;
			}
			if (openFileList[i].name != NULL)
			{
				delete openFileList[i].name;
				openFileList[i].name = NULL;
			}
		}
		delete[] openFileList;
	}

	// Function that create file
	// return 1 if create file successfully
	// else return 0
	bool Create(char *name)
	{
		int fileDescriptor;
		fileDescriptor = OpenForReadWrite(name, FALSE);
		if (fileDescriptor != -1) 	// File was already exist
			return FALSE; 

		fileDescriptor = OpenForWrite(name);
		if (fileDescriptor == -1)	// Can not create this file
			return FALSE;

		Close(fileDescriptor);
		return TRUE;
	}

	OpenFile *Open(char *name)
	{
		int fileDescriptor = OpenForReadWrite(name, FALSE);

		if (fileDescriptor == -1)
			return NULL;
		size++;
		return new OpenFile(fileDescriptor);
	}

	// Function that open file
	// return -1 when error open file
	// return file's ID when file was opened successfully 
	int openFile(char *fileName)
	{
		int freeSpace = -1, fileDescriptor;
		for (int i = 2; i < MAX_FILE; i++)
		{
			if (openFileList[i].open_file == NULL)
			{
				openFileList[i].name = new char[strlen(fileName) + 1];
				for (int j = 0; j < strlen(fileName); j++)
				{
					openFileList[i].name[j] = fileName[j];
				}
				openFileList[i].name[strlen(fileName)] = '\0';
				freeSpace = i;
				break;
			}
		}
		if (freeSpace == -1)		// if file table is full, return -1
			return -1;										
		fileDescriptor = OpenForReadWrite(fileName, FALSE); // create sector
		if (fileDescriptor == -1)	// can not create file, return -1
			return -1; 
		size++;
		openFileList[freeSpace].open_file = new OpenFile(fileDescriptor);
		return freeSpace;
	}

	// Function that close file
	// return 0 when file was closen successfully
	// return -1 else that
	int Close(int idFile)
	{
		if (idFile < 2 || idFile >= MAX_FILE)
			return -1;
		if (openFileList[idFile].open_file)
		{
			delete openFileList[idFile].open_file;
			openFileList[idFile].open_file = NULL;
			delete openFileList[idFile].name;
			openFileList[idFile].name = NULL;
			size--;
			return 0;
		}
		return -1;
	}

	// Function that Read file
	// return -1 when can't read file
	// else that, return the total of bytes was read
	int Read(char *buffer, int numBytes, int idFile)
	{
		if (idFile >= MAX_FILE)
			return -1;
		if (openFileList[idFile].open_file == NULL)
			return -1;
		int res = openFileList[idFile].open_file->Read(buffer, numBytes);

		return res;
	}

	// Function that write file
	// return -1 when can't write file
	// else that, return the total of bytes was writen
	int Write(char *buffer, int numBytes, int idFile)
	{
		if (idFile >= MAX_FILE || openFileList[idFile].open_file == NULL)
			return -1;

		return openFileList[idFile].open_file->Write(buffer, numBytes);
	}

	// Function that seek the pointer to index in a file
	// return -1 if seeking was failed
	// else that, return index was seeked to.
	int Seek(int position, int idFile)
	{
		if (idFile >= MAX_FILE || idFile < 0 || openFileList[idFile].open_file == NULL)
			return -1;
		if (position == -1)		// if position == -1, seek to end of file
			position = openFileList[idFile].open_file->Length();
		else if (position < 0 || position > openFileList[idFile].open_file->Length())
			return -1;
		return openFileList[idFile].open_file->Seek(position);
	}

	// Function that remove file
	// return 1 if file was removed successfully
	// else that return 0
	bool Remove(char *fileName)
	{
		for (int i = 2; i < MAX_FILE; i++)
		{
			// if file is open, can not remove it
			if (openFileList[i].name != NULL && strcmp(openFileList[i].name, fileName) == 0) 
			{
				return false;
			}
		}
		return Unlink(fileName) == 0;
	}
};

#else // FILESYS
class FileSystem
{
public:
	FileSystem(bool format); // Initialize the file system.
							 // Must be called *after* "synchDisk"
							 // has been initialized.
							 // If "format", there is nothing on
							 // the disk, so initialize the directory
							 // and the bitmap of free blocks.

	bool Create(char *name, int initialSize);
	// Create a file (UNIX creat)

	OpenFile *Open(char *name); // Open a file (UNIX open)

	bool Remove(char *name); // Delete a file (UNIX unlink)

	void List(); // List all the files in the file system

	void Print(); // List all the files and their contents

private:
	OpenFile *freeMapFile;	 // Bit map of free disk blocks,
							 // represented as a file
	OpenFile *directoryFile; // "Root" directory -- list of
							 // file names, represented as a file
};

#endif // FILESYS

#endif // FS_H
