#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

void createRoomDir();
char ** randomTenRooms();
struct room * createStruct();
void fillStructNames(struct room * rooms, char ** tenNames);
void fillStructRoomType(struct room * rooms);
void createFiles(struct room * rooms, char ** tenNames);

struct room {
	int id;
	char * name;
	char * roomType;
	int connectCount;
	struct ROOM* connections[6];
};

int main() {
	srand(time(NULL));
	// createRoomDir();
	char ** tenrooms = randomTenRooms();
	struct room * rooms_struct_array = createStruct();
	fillStructNames(rooms_struct_array, tenrooms);
	fillStructRoomType(rooms_struct_array);
	// createFiles(rooms);
	
	
	
	return 0;
}

// Creates a new folder
void createRoomDir() {
	char buf[50];
	int pid = getpid();
	sprintf(buf, "bajracae.rooms.%d", pid);
	mkdir(buf, 0777);
	chdir(buf);
}

char ** randomTenRooms() {
	char ** tenNames = malloc(sizeof(char *) * 10);
	tenNames[0] = "Dearborn";
	tenNames[1] = "Kelley";
	tenNames[2] = "LINC";
	tenNames[3] = "Bed";
	tenNames[4] = "Valley";
	tenNames[5] = "Furman";
	tenNames[6] = "Covell";
	tenNames[7] = "Owen";
	tenNames[8] = "Kearney";
	tenNames[9] = "Kell";

	int i;
	for(i = 0; i < 10; i++) {
		int j = rand() % 10;
		
		char * temp = tenNames[j];
		tenNames[j] = tenNames[i];
		tenNames[i] = temp;
	}
	return tenNames;
}

struct room * createStruct() {
	struct room * rooms = (struct room *)malloc(sizeof(struct room) * 7);
	return rooms;
}

void fillStructNames(struct room * rooms, char ** tenNames) {
	int i;
	for(i = 0; i < 7; i++) {
		rooms[i].name = tenNames[i];
	}
}

void fillStructRoomType(struct room * rooms) {
	int j = 0;
	int k = 0;
	while(j == k) {
		j = rand() % 7;
		k = rand() % 7;
	}
	rooms[j].roomType = "START_ROOM";
	rooms[k].roomType = "END_ROOM";
	
	int i;
	for(i = 0; i < 7; i++) {
		if(i != j && i != k) {
			rooms[i].roomType = "MID_ROOM";
		}
	}
}



void createFiles(struct room * rooms, char ** tenNames) {
	FILE * files[7];
	int i;
	for(i = 0; i < 7; i++) {
		char filename[50];
		sprintf(filename, "%s_room", tenNames[i]);
		files[i] = fopen(filename, "w");
		// fprintf(files[i], "ROOM NAME: %s\n", rooms.name);
		// int j;
		// for(j = 0; j < rooms.connectCount; j++) {
		// 	fprintf(files[i],"CONNECTION %d: %s\n", j, rooms.connections[j].name);
		// }
		// fprintf(files[i], "ROOM TYPE: %s\n", rooms.roomType);
		// fclose(filename);
	}	
}



