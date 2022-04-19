#include "syscall.h"

int main() {
    char fileName[50];
    int id, pos, num;
    char buffer[255];
    int len=0;
    int read;

    PrintStr("===================================================\n");
    PrintStr("#                   CAT program                   #\n");
    PrintStr("===================================================\n");

    PrintStr("Enter file's name: ");
    ReadString(fileName);
    id = Open(fileName);
    if (id != -1)
    {
        num=Seek(-1,id);
        pos=Seek(0,id);
        
        read = Read(buffer,num,id);
        
        while (buffer[len] != '\0') ++len;
        PrintStr("Read ");
        PrintNum(len);
        PrintStr(" characters (from file abc):\n");
        if (len != 0) PrintStr(buffer);
        PrintStr("\n");
        Close(id);    
    }
    else
        PrintStr("Failed to open file\n");

    PrintStr("<=======================END=======================>\n\n");
    
    Halt();
} 