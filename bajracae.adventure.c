#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
    
    char newestDirName[256];
    findLatestFile(newestDirName); // Name of the latest file
    
    // DIR * gameDir = opendir(newestDirName);
    struct room * rooms_struct_array = createStruct();
    // readRoomNames(newestDirName, rooms_struct_array);
    // readStructID(rooms_struct_array);
	// readRoomType(newestDirName, rooms_struct_array);
	fillStruct(newestDirName, rooms_struct_array);
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

// void readRoomNames(char * newestDirName, struct room * rooms) {
//     struct dirent * filename;
// 	DIR * gameDir = opendir(newestDirName);
//     int i = 0;
//     while((filename = readdir(gameDir)) != NULL) {
//         if((strcmp(".", filename->d_name) != 0) && (strcmp("..", filename->d_name) != 0)) {
//             rooms[i].name = filename->d_name;
//             i++;
//         }
//     }
// 	closedir(gameDir);
// }

// void readStructID(struct room * rooms) {
// 	int counter = 0;
// 	int i;
// 	for(i = 0; i < 7; i++) {
// 		rooms[i].id = counter;
// 		counter = counter + 1;
// 	}
// }

// void readConnection(DIR * gameDir, struct room * rooms) {
//     struct dirent * filename;
//     int i = 0;
//     while((filename = readdir(gameDir)) != NULL) {
// 
//     }
// }

// void readRoomType(char * newestDirName, struct room * rooms) {
//     struct dirent * filename;
// 	DIR * gameDir = opendir(newestDirName);
// 	char buffer[256];
// 	FILE * file;
// 	int i = 0;
// 	printf("a\n");
// 	while((filename = readdir(gameDir)) != NULL) {
// 		file = fopen(rooms[i].name, "r");
// 		printf("b\n");
// 		while(fscanf(file, "ROOM TYPE: %s\n", buffer)) {
// 			printf("bugger: %s\n", buffer);
// 			if((strcmp(buffer, "START_ROOM") == 0) || (strcmp(buffer, "MID_ROOM") == 0) || (strcmp(buffer, "END_ROOM") == 0)) {
// 				rooms[i].roomType = buffer;
// 			}
// 		}
// 		i++;
// 	}
// 	closedir(gameDir);
// }

void fillStruct(char * newestDirName, struct room * rooms) {
	struct dirent * filename;
	DIR * gameDir = opendir(newestDirName);
	chdir(newestDirName);
	FILE * file;
	char * name;
	char eachConnect[256];
	char type[256];
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
				// rooms[i].connections[j] = eachConnect;// array of strings
				strcpy(rooms[i].connections[j], eachConnect);
				// printf("Connection: %s\n", rooms[i].connections[j]);
				j++;
			}
			while(fscanf(file, "ROOM TYPE: %s", type)) {
				rooms[i].roomType = type;
			}
			i++;
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
		// printf("Room Type: %s\n", rooms[i].roomType);
		printf("Number of Connections: %d\n", rooms[i].numConnection);
		for(j = 0; j < rooms[i].numConnection; j++) {			
			printf("CONNECTION %d: %s\n", j+1, rooms[i].connections[j]);
		}
	}
}


