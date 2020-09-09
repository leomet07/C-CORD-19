// to compile:
// g++ cJSON.c main.cpp -lm && a.exe
#include <dirent.h>
#include <cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cJSON.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <regex>
#include <fstream>

using namespace std;

double total_incubation = 0;
long amnt = 1;
bool DISPLAY_STDOUT = false;
bool WRITE_LOG = false;
ofstream fout;
ifstream fin;

void print(vector<string> vect)
{

	string all = "";
	for (string x : vect)
	{
		all.append(x);
		all.append(" ");
	}

	if (fin.is_open() && WRITE_LOG)
		fout << all << endl; // Writing data to file
	if (DISPLAY_STDOUT)
	{
		cout << all << endl;
	}
}

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

		int prev_scentence_index = 0;

		string word = "incubation ";
		string time = "day ";
		//do not check for sentence end if last char (there wont be a char in front)

		for (int i = 0; i < length - 1; i++)
		{

			// for every char, check if it marks a new sentence
			if (string_text[i] == '.' && string_text[i + 1] == ' ')
			{

				string sentence = string_text_str.substr(prev_scentence_index, i);

				if (sentence.find(word) != string::npos)
				{
					if (sentence.find(time) != string::npos)
					{
						int sent_len = sentence.size();
						if (sent_len < 400)
						{
							print({"Sentence with time mentioned:  ", sentence});

							const regex r("(?=.[\\d.]+)\\s+(\\S+)(\\b(?=.\\w*day\\w*)\\b)");
							smatch sm;

							std::sregex_iterator next(sentence.begin(), sentence.end(), r);
							std::sregex_iterator end;
							while (next != end)
							{
								std::smatch match = *next;
								print({"LOOK", match.str()});
								string mainmatch = match.str();
								int n = mainmatch.length();

								// declaring character array
								char mainmatcharr[n + 1];

								// copying the contents of the
								// string to char array
								strcpy(mainmatcharr, mainmatch.c_str());
								try
								{
									float current_incubation_val = 0;

									sscanf(mainmatcharr, "%f", &current_incubation_val);

									if (current_incubation_val < 100.0)
									{
										total_incubation = total_incubation + current_incubation_val;
										amnt++;
									}
								}
								catch (...)
								{
									print({"Couldnt be converted\n"});
								}
								next++;
							}
						}
					}
				}

				prev_scentence_index = i;
			}
		}

		free(string_text);

		item = item->next;
	}

	cJSON_Delete(item);
}

long filenum = 0;

string logfile = "output.log";

int main()
{
	if (WRITE_LOG)
	{
		// Open log file
		fin.open(logfile);

		fout.open(logfile, ios::out); // Write mode
	}

	char dircontainer[] = "C:\\Users\\leome\\Downloads\\CORD-19-research-challenge\\";

	vector<string> names;

	struct dirent *direntp2 = NULL;
	DIR *dirp2 = NULL;
	dirp2 = opendir(dircontainer);

	int index = 0;
	while ((direntp2 = readdir(dirp2)) != NULL)
	{

		/* Print only if it is really directory. */
		if (!is_regular_file(direntp2->d_name))
		{
			if (!(direntp2->d_name[0] == '.'))
			{

				names.push_back(direntp2->d_name);
				index++;
			}
		}
	}

	int directory_amount = names.size();

	printf("Total dirs: %d\n", directory_amount);

	/* Finalize resources. */
	(void)closedir(dirp2);

	for (int i = 0; i < directory_amount; i++)
	{

		string dirname = names[i];
		print({"Folder Name: ", names[i]});

		string path_to_dir = "";

		//IN DATASET DIR, there is only 1 folder with the same name that has files
		path_to_dir.append(dircontainer);
		path_to_dir.append(dirname);
		path_to_dir.append("/");
		path_to_dir.append(dirname);

		print({"PATH TO DIR:", path_to_dir});

		DIR *d;
		struct dirent *dir;
		d = opendir(path_to_dir.c_str());

		if (d)
		{
			while ((dir = readdir(d)) != NULL)
			{
				//printf("Starting to read\n");
				string filename_from_data_dir = dir->d_name;

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
					string path_to_file = "";
					path_to_file.append(path_to_dir);
					path_to_file.append("/");

					path_to_file.append(filename_from_data_dir);
					print({"Opening:", path_to_file});
					infile = fopen(path_to_file.c_str(), "r");
					// print({"Filenum", filenum});

					/* quit if the file does not exist */
					if (infile == NULL)
					{
						printf("File with path %s doesn't exist. ", path_to_file.c_str());
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

					/* copy all the text from the file into the buffer */
					fread(buffer, sizeof(char), numbytes, infile);
					fclose(infile);

					// VALUE IS IN BUFFER

					cJSON *json = cJSON_Parse(buffer);

					/* free the memory we used for the buffer */
					free(buffer);

					print({"Deallocated"});

					cJSON *texts = cJSON_GetObjectItemCaseSensitive(json, "body_text");
					//parse_array(cJSON_GetObjectItem(json, "body_text"));
					parse_array(texts);

					cJSON_Delete(json);

					filenum++;
				}
			}
			closedir(d);
		}
	}

	//calculate avg
	double avg = total_incubation / amnt;
	printf("Avg: %9.6f \n", avg);

	fin.close();
	fout.close(); // Closing the file
}
