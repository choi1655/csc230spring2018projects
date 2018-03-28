/**
  * @file point.h
  * @author John Choi mchoi
  *
  * This component will define the Coords and Point structs,
  * in the header, and functions for working with coordinates
  * and individual points of interest in the implementation file.
  * You don't have to try to enforce encapsulation. Other source
  * files can directly examine the fields of a Point or Coords struct.
  * You should just try to put the functions that work with Coords
  * and Point in this component.
  * Above comment taken from the instruction.
  */
#ifndef POINT_H
#define POINT_H

/** Representation for a location, in latitude and longitude. */
typedef struct {
  /** Latitude value for a global location. */
  double lat;
  
  /** Longitude value for a global location. */
  double lon;
} Coords;

/** Representation of a point */
typedef struct {
  /** name of the location */
  char name[21];
  /** Coordinates of the location */
  Coords location;
  char *description;
} Point;

/** Current location */
extern Coords current;

/**
  * This function will create a new Point based on text read from
  * standard input. It will be responsible for parsing all the arguments
  * of the add command, everything after the word add itself. So, when it
  * starts reading from standard input, it should expect to see a name,
  * followed by latitude and longitude, then a text description that continues
  * up to the end of the line. If successful, it returns a pointer to the
  * new dynamically allocated Point. If the input doesn't contain valid
  * arguments for the add command, it should return NULL.
  * Above comment taken from the instruction.
  *
  * @return Point new point based on input
  */
Point *parsePoint();

/**
  * This function will free the memory for the given Point,
  * including the Point struct itself and the memory for its description.
  *
  * @param pt pointer memory to free
  */
void freePoint(Point *pt);

/**
  * This function should print out a description of the given point
  * of interest, in the format required by the list, nearby and match
  * commands. The Coords parameter gives the user's current location.
  * The function can use this to report how far away the Point is.
  * Above comment taken from the instruction.
  *
  * @param pt description of point to print
  * @param ref current location
  */
void reportPoint(Point const *pt, Coords const *ref);

/**
  * Given the addresses of two Coords, this function will return the distance
  * between them in miles. This function is a lot like the one from
  * exercise 13, except it uses two structs as parameters rather than four
  * real values. You can adapt the version from the exercise to implement
  * this function, or, if you want a little more fun, you can write it
  * yourself. As in the exercise, you can assume the earth is a perfect
  * sphere, with a radius of 3959.0 miles.
  *
  * @param c1 first coordinate for distance
  * @param c2 second coordinate for distance
  * @return double distance between the two coordinates
  */
double globalDistance(Coords const *c1, Coords const *c2);
#endif

