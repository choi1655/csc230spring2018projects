/**
  * @file attractions.c
  * @author John Choi mchoi
  *
  * This component will define the main() function and any
  * other functions not provided by the other two components.
  * It will be responsible for reading commands from the user and
  * performing them (often by calling functions in the other components).
  * This component will also include some functions that it passes to
  * listPoints() (by address) to select which points to report (see below).
  * Above taken from the instruction.
  */
#include "point.h"
#include "pointlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/** Defines the current latitude */
#define CURRENT_LAT 35.772325
/** Defines the current longitude */
#define CURRENT_LON -78.673581
/** Defines the maximum length for description */
#define MAX_DESCLEN 1024
/** Defines the maximum name length */
#define MAX_NAMELENG 20
/** Defines the minimum latitude value */
#define MIN_LAT -90
/** Defines the maximum latitude value */
#define MAX_LAT 90
/** Defines the minimum longitude value */
#define MIN_LON -180
/** Defines the maximum longitude value */
#define MAX_LON 180
/** Defines the maximum command length */
#define MAX_COMMAND 7
/** Initializes current location */
Coords current = {CURRENT_LAT, CURRENT_LON};

/**
  * Converts string passed in to all lowercase.
  *
  * @param str to convert
  * @return str converted string
  */
char *convertToLower(char *str)
{
  unsigned char *mystr = (unsigned char *)str;
  while (*mystr) {
    *mystr = tolower(*mystr);
    mystr++;
  }
  return str;
}

/**
  * Matches the description, not name.
  *
  * @param pt pointlist
  * @param data word to find
  * @return true if found
  */
static bool testMatch(Point const *pt, void *data)
{
  char *word = data; //keyword to find
  char *ptDesc = (char *)malloc(MAX_DESCLEN * sizeof(char));
  strcpy(ptDesc, pt->description);
  char *lowercasePtDesc = convertToLower(ptDesc);
  char *token = strtok(lowercasePtDesc, "-,. ");
  while (token != NULL) {
    if (strcmp(token, word) == 0) {
      free(ptDesc);
      return true;
    }
    token = strtok(NULL, "-,. ");
  }
  free(ptDesc);
  return false;
}

/**
  * Test function for list command.
  *
  * @param pt point to pass in
  * @param data null for list command
  * @return true list command always returns truer
  */
static bool testList(Point const *pt, void *data)
{
  return true;
}

/**
  * Test function for nearby command.
  *
  * @param pt point to pass in
  * @param data distance within the passed in point
  * @return true if point is within the distance
  */
static bool testNearby(Point const *pt, void *data)
{
  double *distance = data;
  double globdistance = globalDistance(&pt->location, &current);
  if (globdistance < (*distance)) {
    return true;
  }
  return false;
}

/**
  * Executes the add command.
  *
  * @param ptlist pointlist to add to
  */
static void addCommand(Pointlist *ptlist)
{
  Point *pt = parsePoint();
  if (pt == NULL) {
    printf("Invalid command\n");
  } else {
    addPoint(ptlist, pt);
  }
}

/**
  * Executes the remove command.
  *
  * @param ptlist pointlist to remove from
  */
static void removeCommand(Pointlist *ptlist)
{
  char name[MAX_NAMELENG + 1];
  if (scanf("%s", name) != 1) {
    scanf("%*s"); //frees the buffer
    printf("Invalid command\n");
  }
  removePoint(ptlist, name);
}

/**
  * Executes the list command.
  *
  * @param ptlist pointlist to list points from
  */
static void listCommand(Pointlist *ptlist)
{
  listPoints(ptlist, &current, testList, NULL);
}

/**
  * Executes nearby command.
  *
  * @param ptlist pointlist to look for nearby points
  */
static void nearbyCommand(Pointlist *ptlist)
{
  double distance;
  if (scanf(" %lf", &distance) != 1) {
    scanf("%*s"); //frees the buffer
    printf("Invalid command\n");
  }
  listPoints(ptlist, &current, testNearby, &distance);
}

/**
  * Executes the match command.
  *
  * @param ptlist pointlist to match points from
  */
static void matchCommand(Pointlist *ptlist)
{
  char word[MAX_NAMELENG + 1];
  if (scanf("%s", word) != 1) {
    scanf("%*s");
    printf("Invalid command\n");
  }
  int wordLength = strlen(word);
  bool isValid = true;
  for (int i = 0; i < wordLength; i++) {
    if (word[i] < 'a' || word[i] > 'z') {
      scanf("%*s");
      printf("Invalid command\n");
      isValid = false;
      break;
    }
  }
  if (isValid) {
    listPoints(ptlist, &current, testMatch, word);
  }
}

/**
  * Executes help command.
  * Implementation complete on 032018.
  */
static void helpCommand()
{
  printf("add <name> <latitude> <longitude> <description>\n");
  printf("remove <name>\n");
  printf("move <latitude> <longitude>\n");
  printf("list\n");
  printf("nearby <distance>\n");
  printf("match <word>\n");
  printf("help\n");
  printf("quit\n");
}

/**
  * Executes the move command.
  * Moves the current location.
  */
static void moveCommand()
{
  double latitude, longitude;
  if (scanf("%lf%lf", &latitude, &longitude) != 2) {
    scanf("%*s");
    printf("Invalid command\n");
  } else if ((latitude < MIN_LAT || latitude > MAX_LAT) || (longitude < MIN_LON || longitude > MAX_LON)) {
    printf("Invalid command\n");
  } else {
    current.lat = latitude;
    current.lon = longitude;
  }
}

/**
  * Main function that executes this program.
  *
  * @return EXIT_SUCCESS if the program runs and terminates correctly
  */
int main(void)
{
  unsigned int lineCounter = 1;
  int scanValue;
  char command[MAX_COMMAND]; // main command
  Pointlist *ppt = createPointlist();
  while (1) {
    printf("%d> ", lineCounter);
    lineCounter++;
    scanValue = scanf("%s", command);
    if (scanValue == EOF) { //EOF input terminates program
      printf("\n");
      freePointlist(ppt);
      return EXIT_SUCCESS;
    }
    printf("\n");
    if (strlen(command) > MAX_NAMELENG) { // command length check
      printf("Invalid command\n");
      continue;
    }
    if (strcmp(command, "quit") == 0) {
      freePointlist(ppt);
      return EXIT_SUCCESS;
    } else if (strcmp(command, "add") == 0) {
      addCommand(ppt);
    } else if (strcmp(command, "remove") == 0) {
      removeCommand(ppt);
    } else if (strcmp(command, "nearby") == 0) {
      nearbyCommand(ppt);
    } else if (strcmp(command, "help") == 0) {
      helpCommand();
    } else if (strcmp(command, "list") == 0) {
      listCommand(ppt);
    } else if (strcmp(command, "match") == 0) {
      matchCommand(ppt);
    } else if (strcmp(command, "move") == 0) {
      moveCommand();
    } else {
      printf("Invalid command\n");
    }
  } //end of while loop
  freePointlist(ppt);
  return EXIT_SUCCESS;
}

