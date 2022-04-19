#include "syscall.h"

#define MaxFileLength 32

// Concatenate
int main()
{
    char fileName_1[50], fileName_2[50];
    int openIDFile_1, openIDFile_2;
    char character1[255], character2[255];
    int fileSize_1, fileSize_2, i, choice, read, write, setZero1, setZero2;

    PrintStr("===================================================\n");
    PrintStr("#               CONCATENATE program               #\n");
    PrintStr("===================================================\n");

    // Input file name 1
    PrintStr("Enter file name 1: ");
    ReadString(fileName_1);

    // Open file 1
    openIDFile_1 = Open(fileName_1);
    if (openIDFile_1 == -1) // open file 1 fail because file name invalid
    {
        PrintStr("File name 1 is invalid. End Processing!!\n");
    }
    else
    {

        fileSize_1 = Seek(-1, openIDFile_1); // Get file size 1 and seek pointer to the end file

        // Input file name 2
        PrintStr("Enter file name 2: ");
        ReadString(fileName_2);
        // Open file 2
        openIDFile_2 = Open(fileName_2);
        if (openIDFile_2 == -1) // open file 2 fail because file name invalid
        {
            PrintStr("File name 2 is invalid. End Processing!!\n");
        }
        else
        {
            PrintStr("\n---------------------------------------------------\n");
            do
            {
                PrintStr("Do you want to concatenate files? [ yes --> 1 || no --> 0 ]: ");
                choice = ReadNum();
                if (choice != 0 && choice != 1)
                    PrintStr("You entered wrong, please re-enter...\n");
            } while (choice != 0 && choice != 1);

            if (choice == 1)
            {
                fileSize_2 = Seek(-1, openIDFile_2); // Get file size 2
                setZero2 = Seek(0, openIDFile_2);    // Seek the pointer to file head to read

                read = Read(character2, fileSize_2, openIDFile_2);
                write = Write(character2, fileSize_2, openIDFile_1);
                if (write >= 0)
                {
                    PrintStr("Concatenated successfully!\n");
                }
                else
                {
                    PrintStr("Concatenated fail!!\n");
                }
            }
            else
            {
                PrintStr("Thank you for doing it!\n");
            }
        }
    }

    Close(fileName_1);
    Close(fileName_2);

    PrintStr("<=======================END=======================>\n\n");

    Halt();
    return 0;
} 