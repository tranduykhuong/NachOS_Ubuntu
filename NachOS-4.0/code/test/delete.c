#include "syscall.h"

int main() {
    char fileName[50];
    int id, temp;
    int len=0;

    PrintStr("===================================================\n");
    PrintStr("#                  DELETE program                 #\n");
    PrintStr("===================================================\n");

    PrintStr("Enter file's name: ");
    ReadString(fileName);
    id = Open(fileName);
    if (id != -1)
    {
       Close(id);
       temp=Remove(fileName);
       PrintStr("File was delete successfully!\n");
    }
    else
        PrintStr("Failed to delete file!\n");

    PrintStr("<=======================END=======================>\n\n");
    
    Halt();
} 