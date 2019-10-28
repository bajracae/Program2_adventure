#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

void createRoomDir();
char ** randomTenRooms();
struct room * createStruct();
void fillStructNames(struct room * rooms, char ** tenNames);
void fillStructRoomType(struct room * rooms);
void fillStructID(struct room * rooms);
void fillConnections(struct room * rooms);
void fillnumConnection(struct room * rooms);
void printStruct(struct room * rooms);
void createFiles(struct room * rooms, char ** tenNames);

bool IsGraphFull(struct room * rooms);
void AddRandomConnection(struct room * rooms);
struct room * GetRandomRoom(struct room * rooms);
bool CanAddConnectionFrom(struct room * x);
bool ConnectAlreadyExists(struct room * x, struct room * y);
void ConnectRoom(struct room * x, struct room * y);
bool IsSameRoom(struct room * x, struct room * y);

struct room {
	int id;
	char * name;
	char * roomType;
	int numConnection;
	char connections[7];
};

int main() {
	srand(time(NULL));
	createRoomDir();
	char ** tenrooms = randomTenRooms();
	struct room * rooms_struct_array = createStruct();
	fillStructNames(rooms_struct_array, tenrooms);
	fillStructRoomType(rooms_struct_array);
	fillStructID(rooms_struct_array);
	fillnumConnection(rooms_struct_array);
	fillConnections(rooms_struct_array);
	while (IsGraphFull(rooms_struct_array) == false) {
		AddRandomConnection(rooms_struct_array);
	}
	createFiles(rooms_struct_array, tenrooms);
	return 0;
}

bool IsGraphFull(struct room * rooms) {
	int counter = 0;
	int i;
	for(i = 0; i < 7; i++) {
		if(rooms[i].numConnection >= 3 && rooms[i].numConnection <= 6) {
			// printf("Inside the counter condition\n");
			counter++;
		}
	}
	// printf("Counter in graph: %d\n", counter);
	if(counter == 7) {
		return true;
	}
		return false;
}

void AddRandomConnection(struct room * rooms) {
	struct room * A;
	struct room * B;
	while(true) {
		A = GetRandomRoom(rooms);
		// // printf("A connection: %c\n",A->connections[1]);
		// A->connections[1] = 'T';
		// // printf("A connection: %c\n",A->connections[1]);
		// printf("Name: %s\n", A->name);
		// printf("ID: %d\n", A->id);
		// printf("Room Type: %s\n", A->roomType);
		// int i;
		// for(i = 0; i < 6; i++){
		// 	printf("Connection: %c\n", A->connections[i]);
		// }
		// printf("\n");
		// return;
		if(CanAddConnectionFrom(A) == true) {
			// printf("Connection can be added\n");
			break;
		}
	}
	// printf("While loop break\n");

	do {
		B = GetRandomRoom(rooms);
	}
	while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectAlreadyExists(A, B) == true);
	ConnectRoom(A, B);
	// printf("A numConnection: %d\n", A->numConnection);
	// printf("B numConnection: %d\n", B->numConnection);
	// ConnectRoom(B, A);
}

struct room * GetRandomRoom(struct room * rooms) {
	int i = rand() % 7;
	return rooms + i;
}

bool CanAddConnectionFrom(struct room * x) {
	if(x->numConnection < 6) {
		return true;
	}
	else {
		return false;
	}
}

bool ConnectAlreadyExists(struct room * x, struct room * y) {
	if((x->connections[y->id] == 'T') && (y->connections[x->id] == 'T')) {
		// printf("Connect exists already\n"); // NEVER REACHES HERE
		return true;
	}
	// printf("Connection doesn't exist\n");
	return false;
}

void ConnectRoom(struct room * x, struct room * y) {
	// printf("XName: %s\n", x->name);
	// printf("XID: %d\n", x->id);
	// printf("XRoom Type: %s\n", x->roomType);
	// printf("XNumber of Connections: %d\n", x->numConnection);
	// int i;
	// for(i = 0; i < 6; i++){
	// 	printf("XConnection: %c\n", x->connections[i]);
	// }
	// 
	// printf("YName: %s\n", y->name);
	// printf("YID: %d\n", y->id);
	// printf("YRoom Type: %s\n", y->roomType);
	// printf("Number of Connections: %d\n", y->numConnection);
	// int j;
	// for(j = 0; j < 6; j++){
	// 	printf("YConnection: %c\n", y->connections[j]);
	// }
	// printf("\n");
	x->connections[y->id] = 'T';
	y->connections[x->id] = 'T';
	x->numConnection++;
	// printf("x numConnections: %d\n", x->numConnection);
	y->numConnection++;
	// printf("y numConnections: %d\n\n", y->numConnection);
	// printf("Room is connected\n");
	
	// printf("XName: %s\n", x->name);
	// printf("XID: %d\n", x->id);
	// printf("XRoom Type: %s\n", x->roomType);
	// printf("XNumber of Connections: %d\n", x->numConnection);
	// // int i;
	// for(i = 0; i < 6; i++){
	// 	printf("XConnection: %c\n", x->connections[i]);
	// }
	// 
	// printf("YName: %s\n", y->name);
	// printf("YID: %d\n", y->id);
	// printf("YRoom Type: %s\n", y->roomType);
	// printf("Number of Connections: %d\n", y->numConnection);
	// // int j;
	// for(j = 0; j < 6; j++){
	// 	printf("YConnection: %c\n", y->connections[j]);
	// }
	// printf("-------------------------------------\n");
}

bool IsSameRoom(struct room * x, struct room * y) {
	if(x->id == y->id) {
		// printf("Same Room\n");
		return true;
	}
	// printf("Not the same room\n");
	return false;
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
	tenNames[9] = "Withycombe";

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
	int i,j ;
	for(i = 0; i < 7; i++) {
		rooms[i].numConnection = 0;
		for(j = 0; j < 7; j++) {
			rooms[i].connections[j] = ' ';
		}
	}
	return rooms;
}

void fillStructNames(struct room * rooms, char ** tenNames) {
	int i;
	for(i = 0; i < 7; i++) {
		rooms[i].name = tenNames[i];
	}
}

void fillStructID(struct room * rooms) {
	int counter = 0;
	int i;
	for(i = 0; i < 7; i++) {
		rooms[i].id = counter;
		counter = counter + 1;
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

void fillnumConnection(struct room * rooms) {
	int i;
	for(i = 0; i < 7; i++) {
		rooms[i].numConnection = 0;
	}
}

void fillConnections(struct room * rooms) {
	int j, i;
	for(j = 0; j < 7; j++) {
		for(i = 0; i < 7; i++){
			rooms[j].connections[i] = 'F';
		}
	}
}

void printStruct(struct room * rooms) {
	int i, j;
	for(i = 0; i < 7; i++){
		printf("Name: %s\n", rooms[i].name);
		printf("ID: %d\n", rooms[i].id);
		printf("Room Type: %s\n", rooms[i].roomType);
		printf("Number of Connections: %d\n", rooms[i].numConnection);
		for(j = 0; j < 7; j++) {
			// printf("%c\n", rooms[i].connections[j]);
			
			if(rooms[i].connections[j] == 'T') {
				printf("Connection with: %s\n", rooms[j].name);
			}
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
		fprintf(files[i], "ROOM NAME: %s\n", rooms[i].name);
		int j;
		for(j = 0; j < rooms[i].numConnection; j++) {
			fprintf(files[i],"CONNECTION %d: %s\n", j+1, rooms[j].name);
		}
		fprintf(files[i], "ROOM TYPE: %s\n", rooms[i].roomType);
		fclose(files[i]);
	}	
}



