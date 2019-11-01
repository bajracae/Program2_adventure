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
pthread_mutex_t mp;
pthread_t mp2;

void findLatestFile(char * newestDirName);
struct room * createStruct();
void readRoomNames(char * newestDirName, struct room * rooms);
void readStructID(struct room * rooms);
void readRoomType(char * newestDirName, struct room * rooms);
void fillStruct(char * newestDirName, struct room * rooms);
void printStruct(struct room * rooms);
void gameFunction(struct room * rooms);
char * getValidRoom(struct room eachRoom);
bool connectionExists(char * userInput, struct room eachRoom);
bool askTime(char * userInput);
void * timeIntoTxt(void);

struct room {
	int id;
	char * name;
	char * roomType;
	int numConnection;
	// char * connections[7];
	char connections[7][256];
	
};

int main() {

    char newestDirName[256];
    findLatestFile(newestDirName); // Name of the latest file
    struct room * rooms_struct_array = createStruct();
	fillStruct(newestDirName, rooms_struct_array);
	// CITE=======================================================================
	// https://docs.oracle.com/cd/E19455-01/806-5257/sync-112/index.html
	// init and lock mutex
	// spawn time_thread with time function (lock mutex and unlock in time function)
	// call gameFunction
	// unlock and destroy mutex

	// mp = PTHREAD_MUTEX_INITIALIZER;
	// pthread_mutex_lock(&mp);
	// gameFunction(rooms_struct_array);
	// pthread_mutex_unlock(&mp);
	// pthread_mutex_destroy(&mp);

	timeIntoTxt();
	// printStruct(rooms_struct_array);
    return 0;
}

void findLatestFile(char * newestDirName) {
    // -------------------------------------------------------------------------------------------
    // CITE this
    // -------------------------------------------------------------------------------------------
    int newestDirTime = -1; // Modified timestamp of newest subdir examined
    char targetDirPrefix[32] = "bajracae.rooms."; // Prefix we're looking for
    //char newestDirName[256]; // Holds the name of the newest dir that contains prefix
    memset(newestDirName, '\0', sizeof(newestDirName));

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
          // printf("Found the prefex: %s\n", fileInDir->d_name);
          stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

          if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
          {
            newestDirTime = (int)dirAttributes.st_mtime;
            memset(newestDirName, '\0', sizeof(newestDirName));
            strcpy(newestDirName, fileInDir->d_name);
            // printf("Newer subdir: %s, new time: %d\n", fileInDir->d_name, newestDirTime);
          }
        }
      }
    }

    closedir(dirToCheck); // Close the directory we opened
}

struct room * createStruct() {
	struct room * rooms = (struct room *)malloc(sizeof(struct room) * 7);
	int i,j ;
	for(i = 0; i < 7; i++) {
		rooms[i].numConnection = 0;
		for(j = 0; j < 7; j++) {
			// rooms[i].connections[j]= "";
			memset(rooms[i].connections[j], 0, 256);
		}
	}
	return rooms;
}

void fillStruct(char * newestDirName, struct room * rooms) {
	struct dirent * filename;
	DIR * gameDir = opendir(newestDirName);
	chdir(newestDirName);
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
				strcpy(rooms[i].connections[j], eachConnect);
				j++;
			}
			getline(&temp2, &temp, file);
			if(sscanf(temp2, "ROOM TYPE: %ms\n", &type) > 0) {
				rooms[i].roomType = type;
			}
			i++;
			j=0;
			fclose(file);
		}
	}
	chdir("..");
	closedir(gameDir);
}

void printStruct(struct room * rooms) {
	int i, j;
	for(i = 0; i < 7; i++){
		printf("Name: %s\n", rooms[i].name);
		printf("Number of Connections: %d\n", rooms[i].numConnection);
		for(j = 0; j < rooms[i].numConnection; j++) {			
			printf("CONNECTION %d: %s\n", j+1, rooms[i].connections[j]);
		}
		printf("Room Type: %s\n", rooms[i].roomType);
	}
}

void gameFunction(struct room * rooms) {
	char * currentType = NULL;
	
	// Find the index of the start room
	int start = 0;
	int index = 0;
	for(start = 0; start < 7; start++) {
		if(strcmp(rooms[start].roomType, "START_ROOM") == 0) {
			index = start;
		}
	}
	
	char * paths[256];
	char * userInput = NULL;
	size_t len = 0;
	int pathCounter = 0;
	 
	int i, j, k;
	while(currentType != "END_ROOM") {
		printf("CURRENT LOCATION: %s\n", rooms[index].name);
		printf("POSSIBLE CONNECTIONS: ");
		for(j = 0; j < rooms[index].numConnection; j++) {
			if(j != rooms[index].numConnection - 1) {
				printf("%s, ", rooms[index].connections[j]);
			}
			else {
				printf("%s.", rooms[index].connections[j]);
			}
		} 
		printf("\n");
		printf("WHERE TO? >");
		getline(&userInput, &len, stdin);
		userInput[strlen(userInput) - 1] = 0;
		if((connectionExists(userInput, rooms[index]) != true) && (askTime(userInput) != true)) {
			printf("HUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
		}
		else {
			if(askTime(userInput) == true) {
				pthread_mutex_unlock(&mp);
				
				pthread_mutex_lock(&mp);
			}
			else {
				for(i = 0; i < 7; i++) {
					if(strcmp(userInput, rooms[i].name) == 0) {
						index = i;
						paths[pathCounter] = rooms[index].name;
						pathCounter++;
					}
				}
				currentType = rooms[index].roomType;
				if(strcmp(currentType, "END_ROOM") == 0) {
					printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
					printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", pathCounter);
					for(k = 0; k < pathCounter; k++) {
						printf("%s\n", paths[k]);
					}
					break;
				}
			}
		}
	}
}



// CITE: https://stackoverflow.com/questions/5141960/get-the-current-time-in-c
void * timeIntoTxt(void) {
	time_t timer = time(NULL);
	char s[100];
	size_t max = 100;
	struct tm * info;

	info = localtime(&timer);
	strftime(s, max, "%l:%M%P, %A, %B %d, %Y", info);
	
	FILE * file;
	printf("Hello\n");
	fflush(stdout);
	file = fopen("currentTime.txt", "w+");
	fprintf(file, s);
	fclose(file);
}

// void * 

bool connectionExists(char * userInput, struct room eachRoom) {
	int i;
	for(i = 0; i < eachRoom.numConnection; i++) {
		// printf("Connection %d: %d\n", i, strcmp(userInput, eachRoom.connections[i]));
		if(strcmp(userInput, eachRoom.connections[i]) == 0) {
			return true;
		}
	}
	return false;
}

bool askTime(char * userInput) {
	if(strcmp(userInput, "time") == 0) {
		return true;
	}
	return false;
}

