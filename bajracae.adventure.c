#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

void findLatestFile(char * newestDirName);
struct room * createStruct();
void readRoomNames(char * newestDirName, struct room * rooms);
void readStructID(struct room * rooms);
void readRoomType(char * newestDirName, struct room * rooms);
void fillStruct(char * newestDirName, struct room * rooms);
void printStruct(struct room * rooms);

struct room {
	int id;
	char * name;
	char * roomType;
	int numConnection;
	// char * connections[7];
	char connections[7][256];
	
};

int main() {
	
	// init and lock mutex
	// CITE=======================================================================
	// https://docs.oracle.com/cd/E19455-01/806-5257/sync-112/index.html
	// pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;
	// pthread_mutex_lock(&mp);
	
	// spawn time_thread with time function (lock mutex and unlock in time function)
	
	// call gameFunction
	// unlock and destroy mutex
    
    char newestDirName[256];
    findLatestFile(newestDirName); // Name of the latest file
    
    // DIR * gameDir = opendir(newestDirName);
    struct room * rooms_struct_array = createStruct();
    // readRoomNames(newestDirName, rooms_struct_array);
    // readStructID(rooms_struct_array);
	// readRoomType(newestDirName, rooms_struct_array);
	// fillStruct(newestDirName, rooms_struct_array);
    printStruct(rooms_struct_array);
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
			// if ( (file = fopen(filename->d_name, "r")  ) == NULL) perror("Failed to open file:");
			while(fscanf(file, "ROOM NAME: %ms\n", &name)) {
				rooms[i].name = name;
			}
			while(fscanf(file, "CONNECTION %*d: %s\n", eachConnect)) {
				rooms[i].numConnection++;
				strcpy(rooms[i].connections[j], eachConnect);
				j++;
			}
			// fseek(file, rooms[i].numConnection+2, 1);
			getline(&temp2, &temp, file);
			// printf("%s\n", type);
			// while(fscanf(file, "ROOM TYPE: %s\n", type)) {
			// 	// printf("Hello\n");
			// 	rooms[i].roomType = type;
			// 	// printf("%s\n", rooms[i].roomType );
			// }
			if(sscanf(temp2, "ROOM TYPE: %ms\n", &type) > 0) {
				// printf("Here: %s\n", type);
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
		// printf("ID: %d\n", rooms[i].id);
		printf("Number of Connections: %d\n", rooms[i].numConnection);
		for(j = 0; j < rooms[i].numConnection; j++) {			
			printf("CONNECTION %d: %s\n", j+1, rooms[i].connections[j]);
		}
		printf("Room Type: %s\n", rooms[i].roomType);
	}
}

void gameFunction() {
	// while loop to run the game
	
	// if time requested {
	// unlock mutex 
	// wait for time thread to execute
	// lock mutex
	// spawn another time thread // the other time-thread has finished execution and doesn't exist anymore 
	// }
}


