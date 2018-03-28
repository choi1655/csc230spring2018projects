/**
  * @file pointlist.c
  * @author John Choi mchoi
  *
  * This component will define the Pointlist struct, in the header,
  * and functions for working with a Pointlist, in the implementation
  * file. If you decide to create more functions to simplify your
  * implementation, here's where to put them if they're specific to Pointlist.
  * Above taken from the instruction.
  */
#include "pointlist.h"
#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Defines the initial capacity */
#define INIT_CAP 10

Pointlist *createPointlist()
{
  Pointlist *pt = (Pointlist *)malloc(INIT_CAP * sizeof(Pointlist));
  pt->count = 0;
  pt->cap = INIT_CAP;
  pt->list = (Point **)malloc(INIT_CAP * sizeof(Point *));
  return pt;
}

void freePointlist(Pointlist *ptlist)
{
  for (int i = 0; i < ptlist->count; i++) {
    if (ptlist->list[i] != NULL) {
      freePoint(ptlist->list[i]);
    }
  }
  free(ptlist->list);
  free(ptlist);
}

bool addPoint(Pointlist *ptlist, Point *pt)
{
  if (ptlist->count != 0) {
    for (int i = 0; i < ptlist->count; i++) { //checks for duplicate
      if (ptlist->list[i] == NULL) {
        continue; //handlse the gap
      }
      if (strcmp(ptlist->list[i]->name, pt->name) == 0) {
        printf("Invalid command\n");
        freePoint(pt);
        return false;
      }
    } //end of for loop that checks for duplicate
  }
  if (ptlist->count >= ptlist->cap) { //resizes if over capacity
    ptlist->cap *= 2;
    Point *newList = (Point *)malloc(ptlist->cap * sizeof(Point));
    memcpy(newList, ptlist->list, ptlist->count * sizeof(Point));
    free(ptlist->list);
    *ptlist->list = newList;
  }
  ptlist->list[ptlist->count] = pt;
  ptlist->count += 1;
  return true;
}

bool removePoint(Pointlist *ptlist, char const *name)
{
  int index = -1;
  int i;
  for (i = 0; i < ptlist->count; i++) {
    if (ptlist->list[i] == NULL) {
      continue; // handles the gap in the list
    }
    if (strcmp(ptlist->list[i]->name, name) == 0) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    printf("Invalid command\n");
    return false;
  } else {
    freePoint(ptlist->list[i]);
    ptlist->list[i] = NULL;
    return true;
  }
}

/**
  * Compare function required for qsort() function.
  *
  * @param a first comparing distance
  * @param b second comparing distance
  * @return int zero if equal, 1 if first distance is larger than the second, -1 if opposite
  */
int compare(const void *a, const void *b)
{
  if (*(Point **)a == NULL) {
    return 0;
  } else if (*(Point **)b == NULL) {
    return 0;
  }
  Coords *pCurrent = &current;
  double d1 = globalDistance(pCurrent, &(*(Point **)a)->location);
  double d2 = globalDistance(pCurrent, &(*(Point **)b)->location);
  double cmp = d1 - d2;
  if (cmp > 0) {
    return 1;
  } else if (cmp < 0) {
    return -1;
  }
  return 0;
}

void listPoints(Pointlist *ptlist, Coords const *ref, bool (*test)(Point const *pt, void *data), void *data)
{
  double distance;
  qsort(ptlist->list, ptlist->count, sizeof(ptlist->list[0]), compare);
  for (int i = 0; i < ptlist->count; i++) {
    if (ptlist->list[i] == NULL) {
      continue;
    }
    distance = globalDistance(ref, &ptlist->list[i]->location);
    if (test(ptlist->list[i], data)) {
      printf("%s (%.1f miles)\n", ptlist->list[i]->name, distance);
      printf("  %s\n", ptlist->list[i]->description);
    }
  }
}

