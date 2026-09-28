#include <stdio.h>
#include <string.h>
#include "copy.h"

int main()
{
    char str[5][100];
    char temp[100];
    int i, j;

    for(i = 0; i < 5; i++)
        fgets(str[i], 100, stdin);

    for(i = 0; i < 5; i++)
        str[i][strcspn(str[i], "\n")] = '\0';

    for(i = 0; i < 4; i++)
    {
        for(j = i + 1; j < 5; j++)
        {
            if(strlen(str[i]) < strlen(str[j]))
            {
                copy(str[i], temp);
                copy(str[j], str[i]);
                copy(temp, str[j]);
            }
        }
    }

    for(i = 0; i < 5; i++)
        printf("%s\n", str[i]);

    return 0;
}
