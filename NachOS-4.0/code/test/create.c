#include "syscall.h"

int main() {
    char fileName[255];
    int size;

    PrintStr("Enter file name's size: ");
    size = ReadNum();
    PrintStr("Enter file's name: ");
    ReadStr(fileName, size);

    if (Create(fileName) == 0) {
        PrintStr("File "); PrintStr(fileName);
        PrintStr(" was created successfully!\n");
    } else
        PrintStr("Error create file.\n");
    Halt();
}