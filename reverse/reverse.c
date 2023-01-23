#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num = 0;
char arr[100][100];
char in[100];

int main()
{
    printf("This program will read your input until you say STOP.");
    for (;;)
    {
        printf("\nEnter a string.\n");
        scanf("%s", in);
        if (strcmp(in, "STOP\0") == 0) break;
        strcpy(arr[num], in);
        num ++;
    }
    printf("\nYour inputs will now be displayed in reverse.\n");
    for (int i = num; i >= 0; i--) printf("%s\n", arr[i]);
    memset(arr, 0, 100);
    return 0;
}
