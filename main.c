// to compile gcc cJSON.c main.c -lm && a.exe
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <string.h>
char text[] = "";
void read_file(char fname[])
{
    FILE *fptr;
    char str;
    //char fname[40] = "test.json";
    char full[1024] = " ";
    fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        printf(" File does not exist or cannot be opened.\n");
        exit(0);
    }

    str = fgetc(fptr);
    while (str != EOF)
    {
        strncat(full, &str, 1);
        //printf("%c", str);
        str = fgetc(fptr);
    }
    printf("%s \n", full);
    fclose(fptr);

    //move full into text
    strcpy(text, full);
}
int main()
{
    printf("Here\n");
    read_file("test.json");
    printf("func: %s \n ", text);

    printf("Here2\n");
    read_file("test.json");
    printf("func2: %s \n", text);

    printf("\n\n");

    return 0;
}
