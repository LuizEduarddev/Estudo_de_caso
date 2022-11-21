#include<stdio.h>
#include<stdlib.h>
int main()
{
    char *dudu;
    
    dudu = (char *)(malloc(10 * sizeof(char)));

    for (int i=0 ; i < 10 ; i++)
    {
        scanf("%c", dudu + i);
    }
    for (int i=0 ; i < 10 ; i++)
    {
        if (dudu[i] == '\0')
        {
            printf("Oi");
        }
        if (dudu[i] == 'd')
            printf("oi");
    }



    printf("a string eh %s", dudu);

}