#include "syscall.h"

int main() {

    char fileName[50];
    int length, id;
   
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
        PrintStr("Failed to open file\n");


    if (Close(id) != -1)
    PrintStr("This file was closed successfully!");
    else PrintStr("Cant close this file");
    PrintStr("\n");
    Halt();
}