// to compile:
// g++ cJSON.c main.cpp -lm && a.exe
#include <dirent.h>
#include <cstdio>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cJSON.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <regex>
using namespace std;
//#include <regex.h>
double total_incubation = 0;
long amnt = 1;
/*
char *str_slice(char str[], int slice_from, int slice_to)
{

    char *buffer;
    size_t str_len, buffer_len;

    if (slice_from >= 0 && slice_to > slice_from)
    {
        str_len = strlen(str);

        // if "slice_from" goes beyond permissible limits
        if (slice_from > str_len - 1)
            return NULL;

        buffer_len = slice_to - slice_from;
        str += slicoe_from;
    }

    buffer = calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}
*/
int is_regular_file(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

void parse_array(cJSON *array)
{

	//printf("Parse time: \n");
	cJSON *item = array ? array->child : 0;

	while (item)
	{
		cJSON *text = cJSON_GetObjectItem(item, "text");

		char *string_text = cJSON_Print(text);
		std::string string_text_str = string_text;
		//printf("Entry: %s \n", string_text);

		// split into sentences

		int length = strlen(string_text);
		//char sentences[];

		int prev_scentence_index = 0;

		//do not check for sentence end if last char (there wont be a char in front)
		vector<string> total_sentences;
		for (int i = 0; i < length - 1; i++)
		{

			// for every char, check if it marks a new sentence
			if (string_text[i] == '.' && string_text[i + 1] == ' ')
			{

				string sentence = string_text_str.substr(prev_scentence_index, i);
				//cout << "Sent: " << sentence << endl;

				//printf("Sent len %d\n", sent_len);
				//cout << "Sent: " << sentence << endl;
				total_sentences.push_back(sentence);
				prev_scentence_index = i;
				//sentence_amount += 1;
			}
		}

		for (int i = 0; i < total_sentences.size(); i++)
		{
			string sentence = total_sentences[i];
			string word = "incubation ";
			string time = "day ";

			if (sentence.find(word) != string::npos)
			{
				if (sentence.find(time) != string::npos)
				{
					int sent_len = sentence.size();
					if (sent_len < 400)
					{
						cout << "Low Inc Sent: " << sentence << endl;

						const regex r("(?=.[\\d.]+)\\s+(\\S+)(\\b(?=.\\w*day\\w*)\\b)");
						smatch sm;
						/*
                        if (regex_search(sentence, sm, r))
                        {
                            printf("Regex match\n");
                            for (int j = 0; j < sm.size(); j++)
                            {
                                string phrase = sm.str(j);
                                cout << "test" << endl;

                                // iterate through str
                                
                            }
                        }
                        */
						/*
                        while (regex_search(sentence, sm, r))
                        {
                            for (auto x : sm)
                            {
                                std::cout << "look" << x << " ";
                            }

                            std::cout << std::endl;
                            sentence = sm.suffix().str();
                        }
                        */
						std::sregex_iterator next(sentence.begin(), sentence.end(), r);
						std::sregex_iterator end;
						while (next != end)
						{
							std::smatch match = *next;
							std::cout << match.str() << " LOOK \n";
							string mainmatch = match.str();
							int n = mainmatch.length();

							// declaring character array
							char mainmatcharr[n + 1];

							// copying the contents of the
							// string to char array
							strcpy(mainmatcharr, mainmatch.c_str());
							try
							{
								float inc_val = 0;

								sscanf(mainmatcharr, "%f", &inc_val);

								printf("Float: %9.6f \n", inc_val);
								if (inc_val < 100.0)
								{
									total_incubation = total_incubation + inc_val;
									amnt++;
								}
								else
								{
									cout << "Bad inc val: ";
									cout << inc_val << endl;
								}
							}
							catch (...)
							{
								printf("Couldnt be converted\n");
							}
							next++;
						}
					}
				}
			}
		}
		//printf("-----------------\n");

		item = item->next;
	}

	//cJSON_Delete(array);
	//free(array);
}

long filenum = 0;
int main()
{

	char dircontainer[] = "C:\\Users\\leome\\Downloads\\CORD-19-research-challenge\\";

	struct dirent *direntp = NULL;
	DIR *dirp = NULL;
	dirp = opendir(dircontainer);

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
	dirp2 = opendir(dircontainer);

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

		char name[100];
		strcpy(name, names[i]);
		printf("Folder Name: %s\n", name);
		char dirname[25];

		strcpy(dirname, name);
		char path_to_dir[90] = "";

		//IN DATASET DIR, there is only 1 folder with the same name that has files

		strcat(path_to_dir, dircontainer);
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
				//printf("Starting to read\n");
				char filename_from_data_dir[90];
				strcpy(filename_from_data_dir, dir->d_name);

				//check if its not a special file
				if (filename_from_data_dir[0] != '.')
				{
					//printf("File: %s\n", filename_from_data_dir);

					//parse file
					/* declare a file pointer */
					FILE *infile;
					char *buffer;
					long numbytes;

					/* open an existing file for reading */

					//create full path
					char path_to_file[90] = "";
					strcat(path_to_file, path_to_dir);
					strcat(path_to_file, "/");

					strcat(path_to_file, filename_from_data_dir);
					//printf("Opening %s \n", path_to_file);
					infile = fopen(path_to_file, "r");
					printf("Filenum %d\n", filenum);

					/* quit if the file does not exist */
					if (infile == NULL)
					{
						printf("File with path %s doesn't exist. ", path_to_file);
						return 1;
					}

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
					{
						printf("Memory error \n");
						return 1;
					}

					/* copy all the text into the buffer */
					fread(buffer, sizeof(char), numbytes, infile);
					fclose(infile);

					/* confirm we have read the file by
                    outputing it to the console */

					//VALUE IS IN BUFFER

					//printf("\nThe file called test.dat contains this text\n\n");

					cJSON *json = cJSON_Parse(buffer);
					/* free the memory we used for the buffer */
					free(buffer);
					printf("Deallocated\n");
					/*
                    char *string = cJSON_Print(json);
                    if (string == NULL)
                    {
                        fprintf(stderr, "Failed to print monitor.\n");
                    }

                    printf("CJSONP: %s \n", string);
                    */

					cJSON *texts = cJSON_GetObjectItemCaseSensitive(json, "body_text");
					//parse_array(cJSON_GetObjectItem(json, "body_text"));
					parse_array(texts);
					//scJSON_Delete(texts);
					// remember to deallocate
					//cJSON_Delete(texts);
					cJSON_Delete(json);

					// FOR DEVELOPMENT in 1 file
					//break;
					filenum++;
				}
			}
			closedir(d);
		}
	}

	//calculate avg
	double avg = total_incubation / amnt;
	printf("Avg: %9.6f \n", avg);
}
