#include "syscall.h"

int main() {
    char fileName[50];
    int length, id, pos, num;
    char *buffer;
    int len=0;
    int read;
   
    PrintStr("Enter the length of file name: ");
    length = ReadNum();
    PrintStr("Enter file's name: ");
    ReadStr(fileName, length);
    id = Open(fileName);
    if (id != -1)
    {
        num=Seek(-1,id);
        pos=Seek(0,id);
        read = Read(buffer,num,id);
        while (buffer[len] != '\0') ++len;
        PrintStr("Read ");
        PrintNum(len);
        PrintStr(" characters (from file abc): ");
        if (len != 0) PrintStr(buffer);
        PrintStr("\n");
        Close(id);    
    }
    else
        PrintStr("Failed to open file\n");
    Halt();
}