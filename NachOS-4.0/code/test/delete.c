#include "syscall.h"

int main() {
    char fileName[50];
    int length, id, temp;
    int len=0;
   
    PrintStr("Enter the length of file name: ");
    length = ReadNum();
    PrintStr("Enter file's name: ");
    ReadStr(fileName, length);
    id = Open(fileName);
    if (id != -1)
    {
       Close(id);
       temp=Remove(fileName);
       PrintStr("File delete successful\n");
    }
    else
        PrintStr("Failed to delete file\n");
    Halt();
}