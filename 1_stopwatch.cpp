#include <stdio.h>
#include <time.h>

int main()
{
    char a, b;
    time_t t0, tf;
    printf("24-780 Engineering Computation Problem Set 1-3 - Stopwatch\n");
    printf("Press the ENTER key to start timer >");
    a = getchar();
    //printf("%d", a);
    if (a == '\n')
    {
        t0 = time(NULL);
        printf("Press the ENTER key to stop timer >");
        while (true)
        {

            b = getchar();
            if (b != '\n')
            {
                printf("Wrong Key! Programme Terminated");
                break;
            }
            else
            {
                tf = time(NULL);
                printf("Time elapsed: %.0f seconds.\n", difftime(tf, t0));
                break;
            }
        }


    }
    else
    {
        printf("Wrong Key! Please try again");
    }

    return 0;
}
