#include "syscall.h"

#define console_in 0
#define console_out 1

int main()
{
    char buffer[255];
    char buffer_io[255];
    int file_id, read, write;
    int num = 0, len = 0;

    // Read file
    PrintStr("---READ FILE---\n");
    file_id = Open("file.txt");
    if (file_id == -1)
        PrintStr("File ID is invalid.\n");
    else
    {
        PrintStr("Input the number of characters for reading: ");
        num = ReadNum();
        read = Read(buffer, num, file_id);

        while (buffer[len] != '\0')
            ++len;

        PrintStr("Read ");
        PrintNum(len);
        PrintStr(" characters (from file abc): ");
        if (len != 0)
            PrintStr(buffer);
        PrintStr("\n");

        Close(file_id);
    }

    // Write file
    PrintStr("\n---WRITE FILE---\n");
    file_id = Open("createfile.txt");
    if (file_id == -1)
        PrintStr("File ID is invalid.\n");
    else
    {
        write = Write(buffer, len, file_id);

        PrintStr("Write ");
        PrintNum(write);
        PrintStr(" characters (into file createfile.txt): ");
        if (len != 0)
            PrintStr(buffer);
        PrintStr("\n");

        Close(file_id);
    }

    // CONSOLE IO
    // Console Input
    PrintStr("Enter the number of characters for Console Input: ");
    num = ReadNum();

    PrintStr("Console input: ");
    read = Read(buffer_io, num, console_in);
    len = 0;

    while (buffer_io[len] != '\0')
        ++len;
        
    PrintStr("(Read ");
    PrintNum(len);
    PrintStr(" characters into buffer)\n");

    // Console Output
    PrintStr("\nConsole output: ");
    write = Write(buffer_io, len, console_out);
    PrintStr("\n(Wrote ");
    PrintNum(write);
    PrintStr(" characters from buffer)");

    Halt();
}