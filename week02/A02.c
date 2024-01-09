#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n1 = atoi(argv[1]), n2 = atoi(argv[2]);
    // f is a flag used to control whether a child process should calculate the factorial. It's initially set to 1.
    // prev stores the result of the previous factorial calculation. Initialized to 0
    int f = 1, prev = 0;
    pid_t p;

    for (int i = n1; i <= n2; i++)
    {
        if (f == 0)
        {
            continue;
        }
        p = fork();
        if (p == 0)
        {
            f = 0;
            if (prev == 0)
            {
                int ans = 1, temp = i;
                while (temp)
                {
                    ans *= temp;
                    temp--;
                }
                printf("Factorial calculated at %dth child process: %d\n", i, ans);
                prev = ans;
            }
            else
            {
                int ans = prev * i;
                printf("Factorial caluclated at %dth child process: %d\n", i, ans);
                prev = ans;
            }
        }
    }
    return 0;
}
