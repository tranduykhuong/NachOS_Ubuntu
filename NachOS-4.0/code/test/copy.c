#include "syscall.h"
int main()
{
    char fileName_1[50], fileName_2[50], dataFileCopy[255];
    int fileSize, openIDFile_1, openIDFile_2, read, write, setZero = 0, i;
    int choice;
    
    PrintStr("==============================================\n");
    PrintStr("#                 COPY program               #\n");
    PrintStr("==============================================\n");

    // Input file name 1
    PrintStr("Enter Source file's name: ");
    ReadString(fileName_1);

    // Open file 1, file to get data to copy
    openIDFile_1 = Open(fileName_1);
    if (openIDFile_1 == -1) // open file 1 fail because file name invalid
    {
        PrintStr("File name 1 is invalid. End Processing!!\n");
    }
    else
    {
        // Input file name 2
        PrintStr("Enter Destination file's name: ");
        ReadString(fileName_2);

        // Open file 2, file receives data from file 1
        openIDFile_2 = Open(fileName_2);
        if (openIDFile_2 == -1) // open file 2 fail because file name invalid
        {
            PrintStr("File name 2 is invalid. End processing!!\n");
        }
        else // open 2 file successfully
        {
            PrintStr("\n--------------------------------------------\n");
            do
            {
                PrintStr("Do you want to copy from ");
                PrintStr(fileName_1);
                PrintStr(" file to ");
                PrintStr(fileName_2);
                PrintStr(" file? [ yes --> 1 || no --> 0 ]: ");
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
            }
            else
            {
                PrintStr("Thank you for doing it.\n");
            }
        }
    }

    Close(fileName_1);
    Close(fileName_2);

    PrintStr("<====================END====================>\n\n");

    Halt();
    return 0;
} 