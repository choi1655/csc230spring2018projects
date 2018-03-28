/**
  * @file pointlist.h
  * @author John Choi mchoi
  *
  * This component will define the Pointlist struct, in the header,
  * and functions for working with a Pointlist, in the implementation
  * file. If you decide to create more functions to simplify your
  * implementation, here's where to put them if they're specific to Pointlist.
  * Above taken from the instruction.
  */
#include "point.h"
#include <stdbool.h>

typedef struct {
  int count;
  int cap;
  Point **list;
} Pointlist;

/**
  * This function will dynamically allocate an instance of Pointlist,
  * including the Pointlist object itself, its resizable array of pointers,
  * and all the Point instances in the list.
  * Above comment taken from the instruction.
  *
  * @return Pointlist dynamically allocated instance of pointlist
  */
Pointlist *createPointlist();

/**
  * This will free all the dynamically allocated memory used by a Pointlist,
  * including the Pointlist object itself, its resizable array of
  * pointers, and all the Point instances in the list.
  * Above taken from the instruction.
  *
  * @param ptlist memory to free
  */
void freePointlist(Pointlist *ptlist);

/**
  * This function is part of the pointlist component. It will add the given
  * Point to the given Pointlist list, resizing its internal array if
  * necessary. It will return true if successful and false otherwise
  * (say, if the given point has the same name as one that's already
  * on the list).
  * Above taken from the instruction.
  *
  * @param ptlist list to add pt
  * @param pt point to add to the list
  * @return true if successfully added
  */
bool addPoint(Pointlist *ptlist, Point *pt);

/**
  * It will remove the Point with the given name from the given Pointlist,
  * returning true if successful or false if there's no Point with that name.
  * Above taken from the instruction.
  *
  * @param ptlist list to remove point from
  * @param name of the point
  * @return true if successfully removed
  */
bool removePoint(Pointlist *ptlist, char const *name);

/**
  * It goes through the Points on the given Pointlist, printing
  * selected ones. Its first parameter is the Pointlist it's supposed
  * to print, and its second parameter is the user's current location.
  * The user's location lets it sort the points by distance and report
  * the distance along with each Point. As its third parameter, it takes
  * a pointer to a test function. This is used to decide which Points
  * to include in the output. By giving it different functions, we can
  * get it to report all the points, or ones within a given distance from
  * the user, or ones that include a given word in their description.
  * The last parameter is a pointer to additional data that the test function
  * can use to decide which points to print. The next section explains more
  * about how this is supposed to work.
  * Above taken from the instruction.
  *
  * @param ptlist given pointlist
  * @param ref coordinate of location
  * @param bool empty comment for now
  * @param data void
  */
void listPoints(Pointlist *ptlist, Coords const *ref, bool (*test)(Point const *pt, void *data), void *data);

