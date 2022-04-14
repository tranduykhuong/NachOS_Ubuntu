#include "syscall.h"

#define console_in 0
#define console_out 1

int main() {
    int i;
    int file_id;
    char buffer[255];
    char buffer_io[255];
    int read,write;
    int len = 0;
    int num = 0; 

    // Read testing
    PrintStr("---READ FILE TESTING---\n");
    file_id = Open("a.txt"); 
    if (file_id == -1) PrintStr("File ID is invalid. PLease try again!\n");
    else
    {
        PrintStr("Enter the number of characters for reading: "); num = ReadNum();
        read = Read(buffer,num,file_id);
        while (buffer[len] != '\0') ++len;
        PrintStr("Read ");
        PrintNum(len);
        PrintStr(" characters (from file abc): ");
        if (len != 0) PrintStr(buffer);
        PrintStr("\n");
        Close(file_id);
    }
    
    //Write testing
    PrintStr("\n---WRITE FILE TESTING---\n");
    file_id = Open("b.txt");
    PrintNum(file_id);
    if (file_id == -1) PrintStr("File ID is invalid. Please try again!\n");
    else 
    {
        write = Write(buffer, len, file_id);
        PrintStr("Write ");
        PrintNum(write);
        PrintStr(" characters (into file createfile.txt): ");
        if (len != 0) PrintStr(buffer);
        PrintStr("\n");
        Close(file_id);
    }

    // CONSOLE IO
    //Console Input testing
    PrintStr("Enter the number of characters for Console Input: "); num = ReadNum();
    PrintStr("Console input: ");
    read = Read(buffer_io,num,console_in);
    len=0;  
    while (buffer_io[len] != '\0') ++len;
    PrintStr("(Read ");
    PrintNum(len);
    PrintStr(" characters into buffer)\n");

    // Console Output testing
    PrintStr("\nConsole output: ");
    write = Write(buffer_io, len, console_out);
    PrintStr("\n(Wrote ");
    PrintNum(write);
    PrintStr(" characters from buffer)");

    Halt();
 }