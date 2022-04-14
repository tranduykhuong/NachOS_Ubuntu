#include "syscall.h"

int main()
{
    char fileName[50];
    int length, num, id;

    PrintStr("Enter the length of file name: ");
    length = ReadNum();

    PrintStr("Enter file's name: ");
    ReadStr(fileName, length);

    id = Open(fileName);
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
    ReadNum();
    do
    {
        PrintStr("Do you want to remove this file? 1 - Yes, No - 0\n");
        num = ReadNum();
    } while (num != 1 && num != 0);
    
    if (num)
    {
        if (Remove(fileName))
            PrintStr("This file was removed successfully!");
        else
            PrintStr("Can not remove this file\n");
    }
    else
        PrintStr("\nBye Bye");
    Halt();
    /* not reached */
}
