#include <stdio.h>
#include <time.h>
#include<stdlib.h>

void delay(int wait_time)
{
    time_t t0, current;
    t0 = time(NULL);
    while (true)
    {
        time(&current);
        if (!(difftime(current, t0) < wait_time))
        {
            break;
        }
    }
}

int main()
{
    printf("24-780 Engineering Computation Problem Set 2-1 - Fast Number Typing Game\n");
    for (int i = 0; i < 4; i++)
    {
        delay(1);
        if (i < 3)
        {

            printf("%d\n", 3 - i);
        }
        else
        {
            printf("Go!\n");
        }

    }

    int B, count;
    count = 0;
    B = 0;
    const time_t start_time = time(NULL);
    srand(start_time);
    int A = rand();
    A = A % 1000;
    printf("Type %d:", A);
    scanf("%d", &B);

    while (true)
    {
        if (B == A)
        {
            count++;
            if (count < 10)
            {
                A = rand();
                A = A % 1000;
                printf("Type %d:", A);
                scanf("%d", &B);
                continue;
            }
            else
            {
                break;
            }

        }
        else
        {
            printf("Incorrect!Try again!\n");
            printf("Type %d:", A);
            scanf("%d", &B);
        }
    }
    time_t current;
    time(&current);
    printf("Elapsed %lld seconds.\n", (current - start_time));
    return 0;
}
