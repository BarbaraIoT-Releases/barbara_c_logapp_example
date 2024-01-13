/****************************************************************/
/*//////////////////////////////////////////////////////////////*/
/*==============================================================*/
/*|							DEFINITIONS 					   |*/
/*==============================================================*/
/*//////////////////////////////////////////////////////////////*/
/****************************************************************/


#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#define FILE_ID "/etc/barbara_id.json"
#define FILE_JSON "/appconfig/appconfig.json"
char device_ID;

/****************************************************************/
/*//////////////////////////////////////////////////////////////*/
/*==============================================================*/
/*|							UTILS							   |*/
/*==============================================================*/
/*//////////////////////////////////////////////////////////////*/
/****************************************************************/

int addLog(char *tag, char *log)
{
	char message[500];
	time_t current_time = time(NULL);
    char date_string[40];
    strftime(date_string, 40, "%Y-%m-%d %H:%M:%S", localtime(&current_time));

	sprintf(&message[0], "[%s] [%s] %s\n", date_string, tag, log);
    printf("%s", &message[0]);
    fflush(stdout);

	return 0;
}

int getConfigInt(const char* param, int* box, const char* file)
{
    char *data;
    unsigned long fileLen;
    int match = 0;

    FILE* my_file;
    my_file = fopen(file, "r");
    if (my_file == NULL) {
        *box = 10;
        return 1;
    }

    fseek(my_file, 0, SEEK_END);
    fileLen = ftell(my_file);
    fseek(my_file, 0, SEEK_SET);

    data = (char*)malloc(fileLen + 1);
    if (fread(data, fileLen, 1, my_file) == 0) {
        printf("Utils: Error Reading\n");
        fflush(stderr);
        free(data);
        fclose(my_file);
        return 1;
    }
    fclose(my_file);

    json_t *root;
    json_error_t error;

    root = json_loads(data, 0, &error);
    free(data);

    if (root == NULL) {
        *box = 10;
        return 1;
    }

    const char *key;
    json_t *value;

    json_object_foreach(root, key, value) {
        if (strcmp(key, param) == 0) {
            if (json_is_integer(value)) {
                *box = (int)json_integer_value(value);
                match = 1;
            } else {
                *box = 10;
                return 1;
            }
        }
    }

    json_decref(root);

    if (match == 0) {
        *box = 10;
        return 1;
    }

    return 0;
}

int getConfigString(char* param, char* box, char* file)
{
	char *data;
	unsigned long fileLen;
	int match = 0;
	const char *key;
	json_t *value;
	json_error_t error;

	FILE* my_file;
	my_file = fopen(file, "r");
	if (my_file == NULL){
		box = NULL;
		return 1;
	}
	fseek(my_file, 0, SEEK_END);
	fileLen=ftell(my_file);
	fseek(my_file, 0, SEEK_SET);

	data = (char*)malloc(fileLen+1);
	if( fread(data, fileLen, 1, my_file) == 0 ){
		printf("Utils: Error Reading\n");
		fflush(stderr);
		free(data);

		return 1;
	}
	fclose(my_file);
	fflush(my_file);

	json_t *root = json_loads(data, 0, &error);
	json_object_foreach(root, key, value) {
		if ( strcmp(key, param) == 0 ){
			strcpy(box, json_string_value(value));
			match = 1;
		}
	}

	if(match == 0){
		box = NULL;
		free(data);

		return 1;
	}

	free(data);

	return 0;
}

/*****************************************************************/
/*///////////////////////////////////////////////////////////////*/
/*===============================================================*/
/*|							MAIN APP							|*/
/*===============================================================*/
/*///////////////////////////////////////////////////////////////*/
/*****************************************************************/

int main(void)
{
	int refresh = 10;
	char tag[10];
	char version[10];
	char message[100];

	sprintf(&tag[0], "logApp");
	sprintf(&version[0], "1.0.1");

	while (getConfigString("id", &device_ID, FILE_ID) != 0)
	{
		sprintf(&message[0], "Not found, or not string \"id\" value in barbara_id.json");
		addLog(&tag[0], &message[0]);
		sleep(5);
	}

	addLog(&tag[0], "=================================");
	sprintf(&message[0], "App name: %s", &tag[0]);
	addLog(&tag[0], &message[0]);
	sprintf(&message[0], "App version: %s", &version[0]);
	addLog(&tag[0], &message[0]);
	sprintf(&message[0], "Device ID: %s", &device_ID);
	addLog(&tag[0], &message[0]);
	addLog(&tag[0], "=================================");

	while ( 1 )
	{
		while (getConfigInt("refresh", &refresh, FILE_JSON) != 0)
		{
			sprintf(&message[0], "Not found, or not integer \"refresh\" value found in AppConfig");
			addLog(&tag[0], &message[0]);
			sleep(5);
		}

		if (refresh > 4){
			sprintf(&message[0], "I am configured to show this message every %d seconds.", refresh);
			addLog(&tag[0], &message[0]);
			sleep(refresh);
		} else {
			sprintf(&message[0], "Detected refresh value below minimal refresh value (5). Sleep 5 seconds.");
			addLog(&tag[0], &message[0]);
			sleep(5);
		}
	}

	return 0;
}