#include "syscall.h"

int main()
{
    int a[100];
    int n, choice, i, j, temp;

    PrintStr("**************************************\n");
    PrintStr("*     PROGRAM THAT SORTING ARRAY     *\n");
    PrintStr("**************************************\n");

    while (1)
    {
        PrintStr("Enter array's size: ");
        n = ReadNum();
        if (n < 0 || n > 100)
        {
            PrintStr("Error range, please enter again...\n");
            continue;
        }
        PrintStr("The number of elements is ");
        PrintNum(n);
        PrintChar('\n');
        break;
    }
    while (n < 0 || n > 100)
        ;

    for (i = 0; i < n; i++)
    {
        PrintStr("a[");
        PrintNum(i);
        PrintStr("]= ");
        a[i] = ReadNum();
    }

    do
    {
        do
        {
            PrintStr("\n=============MENU=============\n");
            PrintStr("\t1. Increasing.\n");
            PrintStr("\t2. Decreasing.\n");
            PrintStr("\t0. Exit.\n");
            PrintStr("==============================\n");
            PrintStr("Enter your choice: ");
            choice = ReadNum();
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n - 1; j++)
                {
                    if (choice == 1)
                    {
                        if (a[j] > a[j + 1])
                        {
                            temp = a[j];
                            a[j] = a[j + 1];
                            a[j + 1] = temp;
                        }
                    }
                    else if (choice == 2)
                    {
                        if (a[j] < a[j + 1])
                        {
                            temp = a[j];
                            a[j] = a[j + 1];
                            a[j + 1] = temp;
                        }
                    }
                    else if (choice == 0)
                        break;
                }
            }
            if (choice == 1)
            {
                PrintStr("Array was increasing sorted: ");
                for (i = 0; i < n; i++)
                {
                    PrintNum(a[i]);
                    PrintChar(' ');
                }
                PrintStr("\n");
            }
            else if (choice == 2)
            {
                PrintStr("Array was decreasing sorted: ");
                for (i = 0; i < n; i++)
                {
                    PrintNum(a[i]);
                    PrintChar(' ');
                }
                PrintStr("\n");
            }
            else if (choice == 0)
            {
                PrintStr("Thank you for doing!.\n");
            }
        } while (choice > 0 && choice <= 2);
        if (choice < 0 || choice > 2)
            PrintStr("Error range, xplease enter again...\n");
    } while (choice < 0 || choice > 2);

    Halt();
}