#include "syscall.h"

int main() {
    char fileName[50];
    int length;

    PrintStr("Enter the length of file name: ");
    length = ReadNum();
    PrintStr("Enter file's name: ");
    ReadStr(fileName, length);

    if (Create(fileName) == 0) {
        PrintStr("File ");
        PrintStr(fileName);
        PrintStr(" was created successfully!\n");
    } else
        PrintStr("Error create file.\n");
    Halt();
}