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
#define FILE_JSON "/appconfig/appconfig.json"
#define FILE_DEVICEID "/etc/barbara_id.json"
char device_ID[22];

/****************************************************************/
/*//////////////////////////////////////////////////////////////*/
/*==============================================================*/
/*|							UTILS							   |*/
/*==============================================================*/
/*//////////////////////////////////////////////////////////////*/
/****************************************************************/

int addLog(char *tag, char *log)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char date[100];
	char message[500];

	sprintf(&date[0],"%4d-%2d-%2d %2d:%2d:%2d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	sprintf(&message[0], "[%s] [%s] %s\n", &date[0], tag, log);
    printf(&message[0]);
    fflush(stdout);

	return 0;
}
int getConfig(char* param, char* box, char* file)
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


/****************************************************************/
/*//////////////////////////////////////////////////////////////*/
/*==============================================================*/
/*|							MAIN APP						   |*/
/*==============================================================*/
/*//////////////////////////////////////////////////////////////*/
/****************************************************************/

int main(void)
{

    char refresh[20];
    int frequency = 10;
	char tag[10];
    char version[10];
	char message[100];

	sprintf(&tag[0], "logApp");
    sprintf(&version[0], "1.0.0");

	addLog(&tag[0], "=================================");
    sprintf(&message[0], "App name: %s", &tag[0]);
	addLog(&tag[0], &message[0]);
    sprintf(&message[0], "App version: %s", &version[0]);
	addLog(&tag[0], &message[0]);
	addLog(&tag[0], "=================================");

    while (getConfig("id", &device_ID[0], FILE_DEVICEID) != 0)
    {
        sprintf(&message[0], "Device_ID not found");
		addLog(&tag[0], &message[0]);
        sleep(5);
    }
    
	while ( 1 )
	{
        while (getConfig("refresh", &refresh[0], FILE_JSON) != 0)
        {
            sprintf(&message[0], "Not found \"refresh\" in AppConfig");
            addLog(&tag[0], &message[0]);
            sleep(5);
        }

		sprintf(&message[0], "Hello, I am the device: %s", &device_ID[0]);
		addLog(&tag[0], &message[0]);
		sprintf(&message[0], "I am configured to show this message every %s seconds.", &refresh[0]);
		addLog(&tag[0], &message[0]);
        frequency = atoi(&refresh[0]);
        if (frequency > 4){
            sleep(frequency);
        }else{
            sleep(10);
        }
	}

    return 0; 
}