#include "syscall.h"

#define MaxFileLength 32

// Concatenate
int main()
{
    char fileName_1[50], fileName_2[50];
    int openIDFile_1, openIDFile_2;
    char character1[255], character2[255];
    int sizeNameFile_1, sizeNameFile_2, fileSize_1, fileSize_2, i, choice, read, write, setZero1, setZero2;

    PrintStr("<==============>CONCATENATE TESTING<==============>\n");

    // Input file name size 1
    do
    {
        PrintStr("Enter size of file name 1: ");
        sizeNameFile_1 = ReadNum();
    } while (sizeNameFile_1 <= 0);

    // Input file name 1
    PrintStr("Enter file name 1: ");
    ReadStr(fileName_1, sizeNameFile_1);

    // Open file 1
    openIDFile_1 = Open(fileName_1);
    if (openIDFile_1 == -1) // open file 1 fail because file name invalid
    {
        PrintStr("File name 1 is invalid. End Processing!!\n");
        PrintStr("<=======================END=======================>\n\n");

        Halt();

        return 0;
    }
    else
    {

        fileSize_1 = Seek(-1, openIDFile_1); // Get file size 1 and seek pointer to the end file

        PrintStr("\n---------------------------------------------------\n");
        // Input file name size 2
        PrintStr("Enter size of file name 2: ");
        do
        {
            sizeNameFile_2 = ReadNum();
        } while (sizeNameFile_2 <= 0);

        // Input file name 2
        PrintStr("Enter file name 2: ");
        ReadStr(fileName_2, sizeNameFile_2);
        // Open file 2
        openIDFile_2 = Open(fileName_2);
        if (openIDFile_2 == -1) // open file 2 fail because file name invalid
        {
            PrintStr("File name 2 is invalid. End Processing!!\n");
            PrintStr("<=======================END=======================>\n\n");

            Halt();

            return 0;
        }
        else
        {
            PrintStr("\n---------------------------------------------------\n");
            do
            {
                ReadNum();
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
                    PrintStr("<=======================END=======================>\n\n");
                }
                else
                {
                    PrintStr("Concatenated fail!!\n");
                    PrintStr("<=======================END=======================>\n\n");
                }
            }
            else
            {
                PrintStr("Thank you for doing it.\n");
                PrintStr("<=======================END=======================>\n\n");

                Halt();

                return 0;
            }
        }
    }
    
    Close(fileName_1);
    Close(fileName_2);

    Halt();
}