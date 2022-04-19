#include "syscall.h"

int main() {
    char fileName[255];

    PrintStr("===================================================\n");
    PrintStr("#               CREATE FILE program               #\n");
    PrintStr("===================================================\n");

    PrintStr("Enter file's name: ");
    ReadString(fileName);

    if (Create(fileName) == 0) {
        PrintStr("File "); PrintStr(fileName);
        PrintStr(" was created successfully!\n");
    } else
        PrintStr("Error create file.\n");

    PrintStr("<=======================END=======================>\n\n");
    
    Halt();
}