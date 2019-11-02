///////////////////////////////////////////////////////////////////
// Title: Program 2 - Adventure
// Description: A program that runs the adventure game.
// Author: Aeijan Bajracharya
// Date: 1 November 2019
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

// GLOBAL VARIABLES
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;
pthread_t mp2;

// FUNCTION PROTOTYPES
void findFile(char * dirName);
struct room * createStructArr();
void fillStructArr(char * dirName, struct room * rooms);
void printStructArr(struct room * rooms);
bool isConnected(char * input, struct room eachRoom);
bool isTime(char * input);
void gameFunction(struct room * rooms);
void * writeCurrentTime(void * args);

struct room {
	int id;
	char * name;
	char * type;
	int numConnection;
	char connectionList[7][256];
};

int main() {
    char dirName[256]; // Stores the name of the most recently modified file
    findFile(dirName); // Sets dirName to the name of the most recently modified file
    struct room * structArr = createStructArr(); // New struct array is created and stored into the variable
	fillStructArr(dirName, structArr); // Reads information from files and stores them into the struct array
	
	///////////////////////////////////////////////////////////////////
	// Source: Piazza
	// Post #: @261 -> "Spawning a thread only when "time" is called, but not spawning a thread in main"
	// Author: Arash Shahbaz
	// Date: 2019
	// Code version: N/A
	// Availability: https://piazza.com/class/k0n1xcux3ah4r?cid=261
	///////////////////////////////////////////////////////////////////
	// init and lock mutex
	pthread_mutex_lock(&mp);
	// spawn time_thread with time function (lock mutex and unlock in time function)
	pthread_create(&mp2, NULL, writeCurrentTime, 0); 
	// call gameFunction
	gameFunction(structArr);
	// unlock and destroy mutex
	pthread_mutex_unlock(&mp);
	pthread_mutex_destroy(&mp);
	///////////////////////////////////////////////////////////////////
	
    return 0;
}

///////////////////////////////////////////////////////////////////
// Function: void findFile()
// Description: Finds the last modified directory with "bajracae.rooms"
//				on its name
// Parameters: char * dirName
// Pre-Conditions: Takes in the string directory
// Post-Conditions: Sets dirName to the name of the last modified directory
///////////////////////////////////////////////////////////////////
void findFile(char * dirName) {
	///////////////////////////////////////////////////////////////////
	// Source: 2.4 Manipulating Directories
	// Author: Benjamin Brewster
	// Date: 2017
	// Code version: N/A
	// Availability: https://oregonstate.instructure.com/courses/1738955/pages/2-dot-4-manipulating-directories
	///////////////////////////////////////////////////////////////////
    int newestDirTime = -1; // Modified timestamp of newest subdir examined
    char targetDirPrefix[32] = "bajracae.rooms."; // Prefix we're looking for
    memset(dirName, '\0', sizeof(dirName));

    DIR* dirToCheck; // Holds the directory we're starting in
    struct dirent *fileInDir; // Holds the current subdir of the starting dir
    struct stat dirAttributes; // Holds information we've gained about subdir

    dirToCheck = opendir("."); // Open up the directory this program was run in

    if (dirToCheck > 0) // Make sure the current directory could be opened
    {
      while ((fileInDir = readdir(dirToCheck)) != NULL) // Check each entry in dir
      {
        if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) // If entry has prefix
        {
          stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

          if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
          {
            newestDirTime = (int)dirAttributes.st_mtime;
            memset(dirName, '\0', sizeof(dirName));
            strcpy(dirName, fileInDir->d_name);
          }
        }
      }
    }
    closedir(dirToCheck); // Close the directory we opened
	///////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////
// Function: struct room * createStructArr()
// Description: Creates a struct room array that holds the  
// 				name, type, and array of string names of the connectionList
// Parameters: N/A
// Pre-Conditions: N/A
// Post-Conditions: Creates a struct room array
///////////////////////////////////////////////////////////////////
struct room * createStructArr() {
	struct room * rooms = (struct room *)malloc(sizeof(struct room) * 7);
	int i,j ;
	for(i = 0; i < 7; i++) {
		rooms[i].numConnection = 0;
		for(j = 0; j < 7; j++) {
			memset(rooms[i].connectionList[j], 0, 256);
		}
	}
	return rooms;
}

///////////////////////////////////////////////////////////////////
// Function: void fillStructArr()
// Description: Reads information from each of the files and fills
//				the struct array
// Parameters: char * dirName, struct room * rooms
// Pre-Conditions: Takes in the name of the last modified directory and the struct array
// Post-Conditions: Struct array is filled with read-in information
///////////////////////////////////////////////////////////////////
void fillStructArr(char * dirName, struct room * rooms) {
	struct dirent * filename;
	DIR * gameDir = opendir(dirName);
	chdir(dirName);
	FILE * file;
	char * name;
	size_t temp;
	char eachConnect[256];
	char * type;
	char * temp2;
	int i = 0; // increment for name
	int j = 0;
	while((filename = readdir(gameDir)) != NULL) {
		if((strcmp(".", filename->d_name) != 0) && (strcmp("..", filename->d_name) != 0)) {
			file = fopen(filename->d_name, "r");
			while(fscanf(file, "ROOM NAME: %ms\n", &name)) {
				rooms[i].name = name;
			}
			while(fscanf(file, "CONNECTION %*d: %s\n", eachConnect)) {
				rooms[i].numConnection++;
				strcpy(rooms[i].connectionList[j], eachConnect);
				j++;
			}
			getline(&temp2, &temp, file);
			if(sscanf(temp2, "ROOM TYPE: %ms\n", &type) > 0) {
				rooms[i].type = type;
			}
			i++;
			j=0;
			fclose(file);
		}
	}
	chdir("..");
	closedir(gameDir);
}

///////////////////////////////////////////////////////////////////
// Function: void printStructArr()
// Description: Prints the content of all of the variables in the
//				struct array
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Prints the information about each room to the
//				terminal
///////////////////////////////////////////////////////////////////
void printStructArr(struct room * rooms) {
	int i, j;
	for(i = 0; i < 7; i++){
		printf("Name: %s\n", rooms[i].name);
		printf("Number of Connections: %d\n", rooms[i].numConnection);
		for(j = 0; j < rooms[i].numConnection; j++) {			
			printf("CONNECTION %d: %s\n", j+1, rooms[i].connectionList[j]);
		}
		printf("Room Type: %s\n", rooms[i].type);
	}
}

///////////////////////////////////////////////////////////////////
// Function: isConnected()
// Description: Checks if the input matches any of the names in the struct's connectionList
// Parameters: char * input, struct room eachRoom
// Pre-Conditions: Takes in the user input and the current room's struct
// Post-Conditions: Returns true if they are the same, else return false
///////////////////////////////////////////////////////////////////
bool isConnected(char * input, struct room eachRoom) {
	int i;
	for(i = 0; i < eachRoom.numConnection; i++) {
		if(strcmp(input, eachRoom.connectionList[i]) == 0) {
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////
// Function: isTime()
// Description: Checks if user input is equal to "time"
// Parameters: char * input
// Pre-Conditions: Takes in the user input
// Post-Conditions: Returns true if the user input is "time", else false
///////////////////////////////////////////////////////////////////
bool isTime(char * input) {
	if(strcmp(input, "time") == 0) {
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////
// Function: void gameFunction()
// Description: Includes the while loop and functions that run the gam
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Allows for the game to be playable, game ends when
//				the user reaches the room with the room type "END_ROOM"
///////////////////////////////////////////////////////////////////
void gameFunction(struct room * rooms) {
	FILE * file;
	char * currentRoomType = NULL;
	char * currentTime = NULL;
	char * input = NULL;
	size_t len = 0;
	char * pathList[256];
	int countPath = 0;
	
	int index = 0;
	int start;
	for(start = 0; start < 7; start++) {
		if(strcmp(rooms[start].type, "START_ROOM") == 0) {
			index = start;
		}
	}
	 
	int i, j, k;
	while(currentRoomType != "END_ROOM") {
		printf("CURRENT LOCATION: %s\n", rooms[index].name);
		printf("POSSIBLE CONNECTIONS: ");
		for(j = 0; j < rooms[index].numConnection; j++) {
			if(j != rooms[index].numConnection - 1) {
				printf("%s, ", rooms[index].connectionList[j]);
			}
			else {
				printf("%s.", rooms[index].connectionList[j]);
			}
		} 
		printf("\n");
		printf("WHERE TO? >");
		getline(&input, &len, stdin);
		input[strlen(input) - 1] = 0;
		if((isConnected(input, rooms[index]) != true) && (isTime(input) != true)) {
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
		}
		else {
			if(isTime(input) == true) {
				
				// unlock mutex;
				pthread_mutex_unlock(&mp);
				
				// wait for time thread to execute
				pthread_join(mp2, NULL);
				
				// read from the currentTime
				printf("\n");
				file = fopen("currentTime.txt", "r");
				while(fscanf(file, "%ms", &currentTime) > 0) {
					printf("%s ", currentTime);
				}
				printf("\n");
				printf("\n");
				fclose(file);
				
				// lock mutex
				pthread_mutex_lock(&mp);
				
				// spawn another time thread
				pthread_create(&mp2, NULL, writeCurrentTime, 0); 
			}
			else {
				for(i = 0; i < 7; i++) {
					if(strcmp(input, rooms[i].name) == 0) {
						index = i;
						pathList[countPath] = rooms[index].name;
						countPath++;
					}
				}
				currentRoomType = rooms[index].type;
				if(strcmp(currentRoomType, "END_ROOM") == 0) {
					printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
					printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", countPath);
					for(k = 0; k < countPath; k++) {
						printf("%s\n", pathList[k]);
					}
					break;
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function: writeCurrentTime()
// Description: Writes the current date and time into a file called "currentTime.txt"
// Parameters: void * args
// Pre-Conditions: Takes in a void pointer
// Post-Conditions: Writes the current time into a file and returns null
///////////////////////////////////////////////////////////////////
void * writeCurrentTime(void * args) {
	///////////////////////////////////////////////////////////////////
	// Source: StackOverFlow -> Get the current time in C
	// Author: mingos
	// Date: 2/28/2019
	// Code version: N/A
	// Availability: https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
	///////////////////////////////////////////////////////////////////
	
	// unlock the mutex in that function and join it
	pthread_mutex_lock(&mp);
	pthread_join(mp2, NULL);
	
	time_t timeVar = time(NULL);
	char timeStr[100];
	size_t max = 100;
	struct tm * info;
	info = localtime(&timeVar);
	strftime(timeStr, max, "%l:%M%P, %A, %B %d, %Y", info);
	
	FILE * file;
	file = fopen("currentTime.txt", "w+");
	fprintf(file, timeStr);
	fclose(file);
	pthread_mutex_unlock(&mp);
	return NULL;
}



