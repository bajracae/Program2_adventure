# Program2_adventure
Goal: write a game akin to old text adventure games like Adventure:
http://en.wikipedia.org/wiki/Colossal_Cave_Adventure

Lesson: introduction to C in UNIX and getting familiar with reading and writing files

## Overview
Two C programs
- First program will be in a "bajracae.buildrooms.c" file, runs and creates a series of files that hold descriptions of the in-game rooms and how the rooms are connected
- Second program will be in a "bajracae.adventure.c" file, runs provides an interface for playing the game using the most recently generated rooms

Player will begin in the "starting room" and win the game entering the "ending room" which causes the game to exit, displaying the path taken by the player

Player can enter a command that returns the current time, utilizes mutexes and multithreading

## Specifications
- Rooms Program
 - create a directory called "bajracae.rooms.<PROCESS IF OF ROOMS PROGRAM>." It must generate 7 different room files, which will contain one room per file
 - Room Name
   - can't be assigned to more than one room
   - each name is max 8 characters long with letters only!
   - must hard code a list of ten different room names into your rooms program and have your rooms program randomly assign one of these to each room generated. 7 of the 1- names will be used each time the rooms program is run
 - Room Type
   - possible room type: START_ROOM, END_ROOM, MID_ROOM
   - assignment of which room gets which type should be randomly generated each time the rooms program is run
   - Only one START_ROOM and END_ROOM, rest will be MID_ROOM type
 - Outbound connections
   - at least 3 outbound connections, at most 6 outbound connections from this room to other rooms
   - connections from one room to other rooms should be assigned randomly each time the rooms program is run
   - connections must have matching connections coming back
   - room can't have an connection that points to itself
   - room can't have more than one connect to the same room
 
 ## Program 2 part 2
 - Overview
   - Provides an interface for playing the game using the most recently generated rooms
 - The Game
   - Present interface to the player
   - Use the most recently created files:
     - perform a stat() function call on rooms directory in the same directory as the game
     - open the one with the most recent st_mtime component of the returned stat struct
