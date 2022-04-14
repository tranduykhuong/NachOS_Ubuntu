#include "syscall.h"

int main()
{
    char fileName[50];
    int length, id, pos, num;
    char *buffer;

    // Open file testing
    PrintStr("Enter the length of file name: ");
    length = ReadNum();
    PrintStr("Enter file's name: ");
    ReadStr(fileName, length);
    id = Open(fileName);
    PrintNum(id);
    if (id != -1)
    {
        PrintStr("File ");
        PrintStr(fileName);
        PrintStr(" was opened successfully!\n");
        PrintStr("File's ID: ");
        PrintNum(id);
        PrintStr("\n");
    }
    else
        PrintStr("Failed to open file\n");

    // seek
    ReadNum();
    do
    {
        PrintStr("Do you want to move file pointer to position? 1 - Yes, No - 0\n");
        num = ReadNum();
    } while (num != 1 && num != 0);

    if (num)
    {
        PrintStr("\nEnter the position you want to move file pointer: ");
        pos = ReadNum();
        if (Seek(pos, id) != -1)
        {
            PrintStr("The file pointer was moved successfully! ");
            PrintChar('(');
            PrintNum(Seek(pos, id));
            PrintChar(')');
        }
        else
            PrintStr("That position is invalid!");
    }

    // Close file testing
    do
    {
        PrintStr("\nDo you want to close this file? 1 - Yes, No - 0\n");
        num = ReadNum();
    } while (num != 1 && num != 0);

    if (num)
    {
        if (Close(id) != -1)
            PrintStr("This file was closed successfully!\n");
        else
            PrintStr("Can not close this file\n");
    }
    else
        PrintStr("\nBye Bye");

    Halt();
}