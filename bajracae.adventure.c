///////////////////////////////////////////////////////////////////
// Author: Aeijan Bajracharya
// Title: Program 2 - Adventure
// Description: This program takes the seven rooms (files) and 
// runs an adventure game.
// Date: 11/1/19
///////////////////////////////////////////////////////////////////

// LIBRARIES
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
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER; // Initialize mutex
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
	char * name;
	char * type;
	int numConnection;
	char connectionList[7][256];
};

int main() {
    char dirName[256]; // Stores name of the most recent directory
    findFile(dirName); // Sets dirName to name of the most recent directory
    struct room * structArr = createStructArr(); // Create new struct array and store it into a variable
	fillStructArr(dirName, structArr); // Read information from files and store them into the struct array
	
	///////////////////////////////////////////////////////////////////
	// Source: Piazza
	// Post #: @261 -> "Spawning a thread only when "time" is called, but not spawning a thread in main"
	// Author: Arash Shahbaz
	// Date: 2019
	// Code version: N/A
	// Availability: https://piazza.com/class/k0n1xcux3ah4r?cid=261
	///////////////////////////////////////////////////////////////////
	pthread_mutex_lock(&mp); // Lock mutex
	pthread_create(&mp2, NULL, writeCurrentTime, 0); // Create a thread with the time function
	gameFunction(structArr); // Game function
	pthread_mutex_unlock(&mp); // Unlock the mutex
	pthread_mutex_destroy(&mp); // Destroy the mutex
	///////////////////////////////////////////////////////////////////
	
    return 0;
}

///////////////////////////////////////////////////////////////////
// Function: void findFile()
// Description: Finds the last modified directory with "bajracae.rooms"
// in its name
// Parameters: char * dirName
// Pre-Conditions: Takes in the char * (string)
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
    char targetDirPrefix[32] = "bajracae.rooms."; // Part of the directory name that we are looking for
    memset(dirName, '\0', sizeof(dirName));

    DIR* dirToCheck; // Directory pointer to hold the current directory
    struct dirent *fileInDir; // Holds the current subdir of the starting dir
    struct stat dirAttributes; // Holds information we've gained about subdir

    dirToCheck = opendir("."); // Open up the directory where the game is run in

    if (dirToCheck > 0) // Check if the current directory can be/is open
    {
      while ((fileInDir = readdir(dirToCheck)) != NULL) // Loop through each file or directory in the current directory
      {
        if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) // If the file or directory has "bajracae.rooms."
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
// name, type, and array of string names of the connectionList
// Parameters: N/A
// Pre-Conditions: N/A
// Post-Conditions: Creates a struct room array
///////////////////////////////////////////////////////////////////
struct room * createStructArr() {
	struct room * rooms = (struct room *)malloc(sizeof(struct room) * 7); // Allocate memory for the struct array
	int i,j ;
	for(i = 0; i < 7; i++) {
		rooms[i].numConnection = 0; // Initialize each numConnection as 0
		for(j = 0; j < 7; j++) {
			memset(rooms[i].connectionList[j], 0, 256); // Set each connection in each room with "0"
		}
	}
	return rooms;
}

///////////////////////////////////////////////////////////////////
// Function: void fillStructArr()
// Description: Reads information from each of the files and fills
// the struct array
// Parameters: char * dirName, struct room * rooms
// Pre-Conditions: Takes in the name of the last modified directory
// and the struct array
// Post-Conditions: Struct array is filled with read-in information
///////////////////////////////////////////////////////////////////
void fillStructArr(char * dirName, struct room * rooms) {
	struct dirent * filename;
	DIR * roomDir = opendir(dirName); // Open room directory
	chdir(dirName);
	FILE * file;
	char * name; // Holds the room name
	char eachConnect[256]; // Holds the name of each connected rooms
	char * type; // Holds the room type
	char * roomType;
	size_t size;
	int i = 0; // Index to iterate through each room struct
	int j = 0; // Index to iterate through each connection of each room struct
	while((filename = readdir(roomDir)) != NULL) { // Loop through each file in the room directory
		if((strcmp(".", filename->d_name) != 0) && (strcmp("..", filename->d_name) != 0)) { // If filename is not "." or ".."
			file = fopen(filename->d_name, "r"); 
			while(fscanf(file, "ROOM NAME: %ms\n", &name)) { // While "ROOM NAME" is found, store the name into the variable
				rooms[i].name = name; // Store name into the struct array
			}
			while(fscanf(file, "CONNECTION %*d: %s\n", eachConnect)) { // While "CONNECTION" is found, store the connection name into the variable
				rooms[i].numConnection++;
				strcpy(rooms[i].connectionList[j], eachConnect); // Store each connected room of the room into the struct array
				j++;
			}
			getline(&roomType, &size, file);
			if(sscanf(roomType, "ROOM TYPE: %ms\n", &type) > 0) { // While "ROOM TYPE" is found, store room type into the variable
				rooms[i].type = type;
			}
			i++;
			j = 0;
			fclose(file);
		}
	}
	chdir("..");
	closedir(roomDir);
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
		if(strcmp(input, eachRoom.connectionList[i]) == 0) { // If the input is the same as one of the connected rooms
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
	if(strcmp(input, "time") == 0) { // If the input is "time"
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
	
	int flag = 0; // Differentiate print-out for input "time" and room name
	
	int index = 0;
	int start;
	for(start = 0; start < 7; start++) { // Loop through all of the rooms
		if(strcmp(rooms[start].type, "START_ROOM") == 0) { // If the room at index "start" is the "START_ROOM"
			index = start; // Set "start" to "index"
		}
	}
	 
	int i, j, k;
	while(currentRoomType != "END_ROOM") { // While the player has not reached the end room
		if(flag == 0){
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
		}
		printf("WHERE TO? >");
		getline(&input, &len, stdin); // Get input from the user
		input[strlen(input) - 1] = 0; // Remove the newline at the end of each input
		if(flag == 1){
		}
		if((isConnected(input, rooms[index]) != true) && (isTime(input) != true)) { // If the input is not a connected room and not "time"
			printf("\n");
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n"); // Print error
			printf("\n");
		}
		else {
			if(isTime(input) == true) { // If the input is "time"
				///////////////////////////////////////////////////////////////////
				// Source: Piazza
				// Post #: @261 -> "Spawning a thread only when "time" is called, but not spawning a thread in main"
				// Author: Arash Shahbaz
				// Date: 2019
				// Code version: N/A
				// Availability: https://piazza.com/class/k0n1xcux3ah4r?cid=261
				///////////////////////////////////////////////////////////////////`
				pthread_mutex_unlock(&mp); // Unlock mutex
				pthread_join(mp2, NULL); // Wait for time thread to execute
			
				printf("\n");
				file = fopen("currentTime.txt", "r");
				while(fscanf(file, "%ms", &currentTime) > 0) { // Read the current time from "currentTime.txt"
					printf("%s ", currentTime);
				}
				printf("\n");
				printf("\n");
				fclose(file);
				
				pthread_mutex_lock(&mp); //lock mutex
				pthread_create(&mp2, NULL, writeCurrentTime, 0);// Spawn another time thread
				flag = 1;
			}
			else {
				printf("\n");
				for(i = 0; i < 7; i++) {
					if(strcmp(input, rooms[i].name) == 0) { // If the input is the same as one of the rooms
						index = i; // Set index "i" to "index"
						pathList[countPath] = rooms[index].name; // Put the input room name into the list of paths that the user took
						countPath++;
					}
				}
				flag = 0;
				currentRoomType = rooms[index].type; // Set the room type to the new room's type
				if(strcmp(currentRoomType, "END_ROOM") == 0) { // If the current room type is END_ROOM
					// Print the game done message
					printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n"); // Print the game done message
					printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", countPath);
					for(k = 0; k < countPath; k++) {
						printf("%s\n", pathList[k]); // Print the paths that the user took
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
	pthread_mutex_lock(&mp); // Lock the mutex
	pthread_join(mp2, NULL); // Join the thread
	
	///////////////////////////////////////////////////////////////////
	// Source: StackOverFlow -> Get the current time in C
	// Author: mingos
	// Date: 2/28/2019
	// Code version: N/A
	// Availability: https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
	///////////////////////////////////////////////////////////////////
	time_t timeVar = time(NULL);
	char timeStr[100];
	size_t max = 100;
	struct tm * info;
	info = localtime(&timeVar); // Get the local time
	strftime(timeStr, max, "%l:%M%P, %A, %B %d, %Y", info); // Store the time string into timeStr
	
	FILE * file;
	file = fopen("currentTime.txt", "w+");
	fprintf(file, timeStr); // Write the time into the file
	fclose(file);
	pthread_mutex_unlock(&mp); // Unlock the mutex
	return NULL;
}



