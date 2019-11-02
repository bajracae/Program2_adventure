///////////////////////////////////////////////////////////////////
// Author: Aeijan Bajracharya
// Title: Program 2 - Adventure
// Description: This program randomly generates seven rooms (files) in a directory
// for the adventure game.
// Date: 11/1/19
///////////////////////////////////////////////////////////////////

// LIBRARIES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

// FUNCTION PROTOTYPES
void createRoomDir();
struct room * createStructArr();
char ** getTenRooms();
void fillNames(struct room * rooms, char ** tenNames);
void fillID(struct room * rooms);
void fillRoomType(struct room * rooms);
void fillNumConnection(struct room * rooms);
void fillConnections(struct room * rooms);
void printStruct(struct room * rooms);
void createFiles(struct room * rooms, char ** tenNames);
bool IsGraphFull(struct room * rooms);
void AddRandomConnection(struct room * rooms);
struct room * GetRandomRoom(struct room * rooms);
bool CanAddConnectionFrom(struct room * x);
bool IsSameRoom(struct room * x, struct room * y);
bool ConnectAlreadyExists(struct room * x, struct room * y);
void ConnectRoom(struct room * x, struct room * y);

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
	char ** tenrooms = getTenRooms();
	struct room * rooms_struct_array = createStructArr();
	fillNames(rooms_struct_array, tenrooms);
	fillRoomType(rooms_struct_array);
	fillID(rooms_struct_array);
	fillNumConnection(rooms_struct_array);
	fillConnections(rooms_struct_array);
	while (IsGraphFull(rooms_struct_array) == false) {
		AddRandomConnection(rooms_struct_array);
	}
	createFiles(rooms_struct_array, tenrooms);
	return 0;
}

///////////////////////////////////////////////////////////////////
// Function: createRoomDir()
// Description: Makes a directory named "bajracae.rooms.(unique PID)"
// Parameters: N/A
// Pre-Conditions: N/A
// Post-Conditions: Makes a new directory
///////////////////////////////////////////////////////////////////
void createRoomDir() {
	char buf[50];
	int pid = getpid(); // Gets a random PID value
	sprintf(buf, "bajracae.rooms.%d", pid); // Name of the generated directory 
	mkdir(buf, 0777);
	chdir(buf);
}

///////////////////////////////////////////////////////////////////
// Function: createStructArr()
// Description: Creates a struct room array that holds the room id, 
// name, type, number of connections, and char array that indicates
// the connections for each room
// Parameters: N/A
// Pre-Conditions: N/A
// Post-Conditions: Creates a struct room array
///////////////////////////////////////////////////////////////////
struct room * createStructArr() {
	struct room * rooms = (struct room *)malloc(sizeof(struct room) * 7); // Allocating memory for the struct array
	int i,j ;
	for(i = 0; i < 7; i++) {
		rooms[i].numConnection = 0; // Sets all of the numConnections to 0
		for(j = 0; j < 7; j++) {
			rooms[i].connections[j] = ' '; // Sets the values of the char connections for each room to spaces
		}
	}
	return rooms;
}

///////////////////////////////////////////////////////////////////
// Function: getTenRooms()
// Description: Creates an array of ten room names and randomizes its
// order
// Parameters: N/A
// Pre-Conditions: N/A
// Post-Conditions: Returns an array of ten room names
///////////////////////////////////////////////////////////////////
char ** getTenRooms() {
	char ** tenNames = malloc(sizeof(char *) * 10); // Allocating memory for the ten rooms
	// Hard coded names into the tenNames array
	tenNames[0] = "Dearborn";
	tenNames[1] = "Kelley";
	tenNames[2] = "LINC";
	tenNames[3] = "Bed";
	tenNames[4] = "Valley";
	tenNames[5] = "Furman";
	tenNames[6] = "Covell";
	tenNames[7] = "Owen";
	tenNames[8] = "Kearney";
	tenNames[9] = "Withy";

	int i;
	for(i = 0; i < 10; i++) {
		int j = rand() % 10; // Generates a random number
		
		// Randomizes the order of the ten rooms
		char * temp = tenNames[j];
		tenNames[j] = tenNames[i];
		tenNames[i] = temp;
	}
	return tenNames;
}

///////////////////////////////////////////////////////////////////
// Function: fillNames()
// Description: Fills the struct array with the first seven names 
// from the randomized array of ten names
// Parameters: struct room * rooms, char ** tenNames
// Pre-Conditions: Takes in the struct array and array of ten random
// room names
// Post-Conditions: Fills the struct array with seven names from the 
// array of ten names
///////////////////////////////////////////////////////////////////
void fillNames(struct room * rooms, char ** tenNames) {
	int i;
	for(i = 0; i < 7; i++) {
		rooms[i].name = tenNames[i]; // Takes the first seven names from tenNames arrary and inserts it into the struct array
	}
}

///////////////////////////////////////////////////////////////////
// Function: fillID()
// Description: Assigns each room in the struct with an id number, 
// id values range from 0 to 6
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Assigns each room with a unique id number
///////////////////////////////////////////////////////////////////
void fillID(struct room * rooms) {
	int counter = 0;
	int i;
	for(i = 0; i < 7; i++) {
		rooms[i].id = counter; // Sets each room with an id number
		counter = counter + 1;
	}
}

///////////////////////////////////////////////////////////////////
// Function: fillRoomType()
// Description: Assigns each room in the struct with room types, two rooms are
// the START and END rooms while the rest are MID rooms, these are assigned randomly
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Assigns each room with a room type
///////////////////////////////////////////////////////////////////
void fillRoomType(struct room * rooms) {
	int j = 0;
	int k = 0;
	while(j == k) { // Keep generating random numbers until the two numbers are not the same
		j = rand() % 7;
		k = rand() % 7;
	}
	rooms[j].roomType = "START_ROOM"; // Sets a random room at index j to START_ROOM
	rooms[k].roomType = "END_ROOM"; // Sets a random room at index k to END_ROOM
	
	int i;
	for(i = 0; i < 7; i++) {
		if(i != j && i != k) { // Sets all the rooms that are not at index j or k to MID_ROOM
			rooms[i].roomType = "MID_ROOM";
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function: fillNumConnection()
// Description: Sets all of the struct rooms' numConnections to 0
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Sets all of the numConnections to 0
///////////////////////////////////////////////////////////////////
void fillNumConnection(struct room * rooms) {
	int i;
	for(i = 0; i < 7; i++) {
		rooms[i].numConnection = 0; // Sets all the numConnection to 0
	}
}

///////////////////////////////////////////////////////////////////
// Function: fillConnections()
// Description: Sets all of the char connections for each room to "F"
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Sets all of the char connections for each room to "F"
///////////////////////////////////////////////////////////////////
void fillConnections(struct room * rooms) {
	int j, i;
	for(j = 0; j < 7; j++) {
		for(i = 0; i < 7; i++){
			rooms[j].connections[i] = 'F'; // Sets all of the connections elements of each room to "F"
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function: createFiles()
// Description: Creates a file for each room, names each file based
// on the room name, and writes information for each room from the struct 
// array into the files
// Parameters: struct room * rooms, char ** tenNames
// Pre-Conditions: Takes in the struct array and the array of ten names
// Post-Conditions: Creates the files and writes in the information
// about each room into them
///////////////////////////////////////////////////////////////////
void createFiles(struct room * rooms, char ** tenNames) {
	FILE * files[7]; // Array of seven files
	int i;
	int index = 1; // counter for %d in "CONNECTION %d: %s"
	for(i = 0; i < 7; i++) {
		char filename[50]; // Stores name of file
		sprintf(filename, "%s", tenNames[i]); // File name is set to the name of the rooms
		files[i] = fopen(filename, "w"); // Opens the file and gives writting permissions for each file
		fprintf(files[i], "ROOM NAME: %s\n", rooms[i].name); // Writes the room name into the file
		int j;
		for(j = 0; j < 7; j++) {
			if(rooms[i].connections[j] == 'T') { // If the char in rooms[i].connections[j] is "T", then the room at index j is a connection to rooms[i]
				fprintf(files[i],"CONNECTION %d: %s\n", index, rooms[j].name); // Writes the connections of each room, based on the index of T's in connections, into the file 
				index++;
			}
		}
		index = 1;
		fprintf(files[i], "ROOM TYPE: %s\n", rooms[i].roomType); // Writes the room type into the file
		fclose(files[i]); // Closes the File
	}	
}

///////////////////////////////////////////////////////////////////
// Function: printStruct()
// Description: Prints the content of all of the variables in the
// struct array
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Prints the information about each room to the
// terminal
///////////////////////////////////////////////////////////////////
void printStruct(struct room * rooms) {
	int i, j;
	for(i = 0; i < 7; i++){
		printf("Name: %s\n", rooms[i].name);
		printf("ID: %d\n", rooms[i].id);
		printf("Room Type: %s\n", rooms[i].roomType);
		printf("Number of Connections: %d\n", rooms[i].numConnection);
		for(j = 0; j < 7; j++) {			
			if(rooms[i].connections[j] == 'T') {
				printf("Connection with: %s\n", rooms[j].name);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function: IsGraphFull()
// Description: Checks if all the rooms has at least three connections 
// but less than or equal to six connections
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Checks if all the rooms have the required number
// of connections
///////////////////////////////////////////////////////////////////
bool IsGraphFull(struct room * rooms) {
	int counter = 0;
	int i;
	for(i = 0; i < 7; i++) {
		if(rooms[i].numConnection >= 3 && rooms[i].numConnection <= 6) { // If the number of connections is greater than 2 and less than 7
			counter++;
		}
	}
	if(counter == 7) { // Counter should be 7 when all of the rooms have more than 2 and less than 7 connections
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////
// Function: AddRandomConnection()
// Description: Connects two random rooms together only if they can 
// be connected, are not the same room, and if they are not connected already
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Connects two random rooms together
///////////////////////////////////////////////////////////////////
void AddRandomConnection(struct room * rooms) {
	struct room * A;
	struct room * B;
	while(true) {
		A = GetRandomRoom(rooms); // A is a random struct value from the array
		if(CanAddConnectionFrom(A) == true) { // If connections can be added
			break;
		}
	}
	do {
		B = GetRandomRoom(rooms); // B is another random struct value from the array
	}
	while(CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectAlreadyExists(A, B) == true);
	ConnectRoom(A, B); // Connect A and B together
}

///////////////////////////////////////////////////////////////////
// Function: GetRandomRoom()
// Description: Generates a random number and returns the struct room 
// associated with that index from the struct
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array and the array of ten names
// Post-Conditions: Creates the files and writes in the information
// about each room into them
///////////////////////////////////////////////////////////////////
struct room * GetRandomRoom(struct room * rooms) {
	int i = rand() % 7;
	return rooms + i; // returns random room from the struct array
}

///////////////////////////////////////////////////////////////////
// Function: CanAddConnectionFrom()
// Description: Checks if the struct room has less than 6 connections,
// meaning that it can be connected to another room if it does
// Parameters: struct room * rooms
// Pre-Conditions: Takes in the struct array
// Post-Conditions: Returns true if connections can be made, else returns false
///////////////////////////////////////////////////////////////////
bool CanAddConnectionFrom(struct room * x) {
	if(x->numConnection < 6) { // If the number of connections is less than 6
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////
// Function: IsSameRoom()
// Description: Checks if the two randomly generated rooms are the 
// same or not, if their ids' are the same then the two rooms are the same
// Parameters: struct room * rooms
// Pre-Conditions: Takes in two room structs to compare
// Post-Conditions: Returns true if connections can be made, else returns false
///////////////////////////////////////////////////////////////////
bool IsSameRoom(struct room * x, struct room * y) {
	if(x->id == y->id) { // If the id's are the same, then they are the same room
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////
// Function: ConnectAlreadyExists()
// Description: Checks if two rooms are already connected
// Parameters: struct room * rooms
// Pre-Conditions: Takes in two room structs to compare
// Post-Conditions: Returns true if the rooms are already connected,
// else returns false
///////////////////////////////////////////////////////////////////
bool ConnectAlreadyExists(struct room * x, struct room * y) {
	if((x->connections[y->id] == 'T') && (y->connections[x->id] == 'T')) { // if connection of x at the id of y and vice versa are "T"
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////
// Function: ConnectRoom()
// Description: Sets the first rooms' connections element at the index
// id of the second room to "T" and vice versa to indicate a connection
// Parameters: struct room * rooms
// Pre-Conditions: Takes in two room structs to compare
// Post-Conditions: Sets the "F" to "T" in both of the char connections
// array based on the index id of the rooms
///////////////////////////////////////////////////////////////////
void ConnectRoom(struct room * x, struct room * y) {
	x->connections[y->id] = 'T'; // Set the connection of x at the id of y to "T" (x is connected to y)
	y->connections[x->id] = 'T'; // Set the connection of y at the id of x to "T" (y is connected to x)
	x->numConnection++; // Increment x's number of connections
	y->numConnection++; // Increment y's number of connections
}