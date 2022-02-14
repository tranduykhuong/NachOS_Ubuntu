#include "syscall.h"

int main()
{
    int a[100];
    int n, choice, i, j, temp;
    do
    {
        PrintStr("Nhập vào số phần tử của mảng: ");
        n = ReadNum();
        if (n < 0 || n > 100)
            PrintStr("Bạn đã nhập sai, xin vui lòng nhập lại...\n");
    } while (n < 0 || n > 100);

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
            PrintStr("===========MENU===========\n");
            PrintStr("\t1. Increasing.\n");
            PrintStr("\t1. Decreasing.\n");
            PrintStr("\t0. Exit.\n");
            PrintStr("==========================\n");
            PrintStr("Nhập vào lựa chọn của bạn: ");
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
                PrintStr("Mảng tăng dần là: ");
                for (i = 0; i < n; i++)
                {
                    PrintNum(a[i]);
                    PrintChar(' ');
                }
                PrintStr("\n");
            }
            else if (choice == 2)
            {
                PrintStr("Mảng giảm dần là: ");
                for (i = 0; i < n; i++)
                {
                    PrintNum(a[i]);
                    PrintChar(' ');
                }
                PrintStr("\n");
            }
            else if (choice == 0)
            {
                PrintStr("Cảm ơn bạn đã thực hiện.\n");
            }
        } while (choice>0 && choice <=2);
        if(choice < 0 || choice > 2)
            PrintStr("Bạn đã nhập sai, xin vui lòng nhập lại...\n");
    } while (choice < 0 || choice > 2);

    Halt();
}