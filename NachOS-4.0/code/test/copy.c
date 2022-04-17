#include "syscall.h"
int main()
{
    char fileName_1[50], fileName_2[50], dataFileCopy[255];
    int nameSizeFile_1, nameSizeFile_2, fileSize, openIDFile_1, openIDFile_2, read, write, setZero = 0, i;
    int choice;
    PrintStr("<==============>COPY TESTING<==============>\n");

    // Input file name size 1
    do
    {
        PrintStr("Enter size of file name 1: ");
        nameSizeFile_1 = ReadNum();
    } while (nameSizeFile_1 <= 0);

    // Input file name 1
    PrintStr("Enter file name 1: ");
    ReadStr(fileName_1, nameSizeFile_1);
    // PrintStr(fileName_1);

    // Open file 1, file to get data to copy
    openIDFile_1 = Open(fileName_1);
    if (openIDFile_1 == -1) // open file 1 fail because file name invalid
    {
        PrintStr("File name 1 is invalid. End Processing!!\n");
        PrintStr("<====================END====================>\n\n");

        Halt();
        return 0;
    }
    else
    {
        PrintStr("\n--------------------------------------------\n");
        // Input file name size 2
        PrintStr("Enter size of file name 2: ");
        do
        {
            nameSizeFile_2 = ReadNum();
        } while (nameSizeFile_2 <= 0);

        // Input file name 2
        PrintStr("Enter file name 2: ");
        ReadStr(fileName_2, nameSizeFile_2);

        // Open file 2, file receives data from file 1
        openIDFile_2 = Open(fileName_2);
        if (openIDFile_2 == -1) // open file 2 fail because file name invalid
        {
            PrintStr("File name 2 is invalid. End processing!!\n");
            PrintStr("<====================END====================>\n\n");

            Halt();
            return 0;
        }
        else // open 2 file successfully
        {
            PrintStr("\n--------------------------------------------\n");
            do
            {
                // i = ReadNum();
                ReadNum();
                PrintStr("Do you want to copy files? [ yes --> 1 || no --> 0 ]: ");
                choice = ReadNum();
                if (choice != 0 && choice != 1)
                    PrintStr("You entered wrong, please re-enter...\n");
            } while (choice != 0 && choice != 1);

            if (choice == 1)
            {
                fileSize = Seek(-1, openIDFile_1); // Get file size
                // PrintNum(fileSize);
                setZero = Seek(0, openIDFile_1); // Seek the pointer to file head to read

                // Read each character in file and print
                for (i = 0; i < fileSize; i++)
                {
                    read = Read(dataFileCopy, 1, openIDFile_1);
                    // PrintStr(character);
                    write = Write(dataFileCopy, 1, openIDFile_2);
                }

                PrintStr("Copied successfully!\n");
                PrintStr("<====================END====================>\n\n");
            }
            else
            {
                PrintStr("Thank you for doing it.\n");
                PrintStr("<====================END====================>\n\n");
                Halt();

                return 0;
            }
        }
    }

    Close(fileName_1);
    Close(fileName_2);

    Halt();
    return 0;
}