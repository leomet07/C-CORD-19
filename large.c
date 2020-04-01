// to compile gcc cJSON.c large.c -lm && a.exe
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void parse_array(cJSON *array)
{
    printf("Parse time: \n");
    cJSON *item = array ? array->child : 0;
    while (item)
    {
        cJSON *text = cJSON_GetObjectItem(item, "text");

        /*cJSON *index = cJSON_GetObjectItem(item, "index");
        cJSON *optional = cJSON_GetObjectItem(item, "optional");
        
        */
        char *string = cJSON_Print(text);
        //printf("Entry: %s \n\n", string);

        item = item->next;
    }
}
/* Print all the dirs starting from <path> [maybe recursive]. */
int get_dirs()
{

    return 0;
}

int main()
{

    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    dirp = opendir(".");

    int dir_amnt = 0;

    while ((direntp = readdir(dirp)) != NULL)
    {
        struct stat fstat;

        /* Print only if it is really directory. */
        if (!is_regular_file(direntp->d_name))
        {
            if (!(direntp->d_name[0] == '.'))
            {

                dir_amnt++;
            }
        }
    }

    const int size = dir_amnt;
    char names[size][90];
    printf("Total dirs: %d\n", dir_amnt);

    struct dirent *direntp2 = NULL;
    DIR *dirp2 = NULL;
    dirp2 = opendir(".");

    int index = 0;
    while ((direntp2 = readdir(dirp2)) != NULL)
    {
        struct stat fstat;

        /* Print only if it is really directory. */
        if (!is_regular_file(direntp2->d_name))
        {
            if (!(direntp2->d_name[0] == '.'))
            {

                //printf("dir: %s\n", direntp2->d_name);
                strcpy(names[index], direntp2->d_name);
                index++;
            }
        }
    }

    /* Finalize resources. */
    (void)closedir(dirp);

    //printf("size of nums %s\n", (int)sizeof(names));
    for (int i = 0; i < size; i++)
    {

        printf("Name: %s\n", names[i]);
    }

    char dirname[25] = "biorxiv_medrxiv";
    char path_to_dir[90] = "";

    //IN DATASET DIR, there is only 1 folder with the same name that has files
    strcat(path_to_dir, dirname);
    strcat(path_to_dir, "/");
    strcat(path_to_dir, dirname);

    printf("PATH TO DIR: %s\n", path_to_dir);
    DIR *d;
    struct dirent *dir;
    d = opendir(path_to_dir);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char filename[90];
            //printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

    /* declare a file pointer */
    FILE *infile;
    char *buffer;
    long numbytes;

    /* open an existing file for reading */
    infile = fopen("large.json", "r");

    /* quit if the file does not exist */
    if (infile == NULL)
        return 1;

    /* Get the number of bytes */
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);

    /* reset the file position indicator to 
the beginning of the file */
    fseek(infile, 0L, SEEK_SET);

    /* grab sufficient memory for the 
buffer to hold the text */
    buffer = (char *)calloc(numbytes, sizeof(char));

    /* memory error */
    if (buffer == NULL)
        return 1;

    /* copy all the text into the buffer */
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);

    /* confirm we have read the file by
    outputing it to the console */

    //VALUE IS IN BUFFER

    printf("\nThe file called test.dat contains this text\n\n");

    cJSON *json = cJSON_Parse(buffer);
    /* free the memory we used for the buffer */
    free(buffer);

    char *string = cJSON_Print(json);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print monitor.\n");
    }
    //printf("CJSONP: %s \n", string);

    parse_array(cJSON_GetObjectItem(json, "body_text"));

    // remember to deallocate
    cJSON_Delete(json);
}
