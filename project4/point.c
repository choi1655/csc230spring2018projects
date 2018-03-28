/**
  * @file point.c
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
#include "point.h"
#include "pointlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //for trig functions

/** Multiplier for converting degrees to radians */
#define DEG_TO_RAD (M_PI / 180)

/** Radius of the earth in miles. */
#define EARTH_RADIUS 3959.0
/** Defines the maximum description length */
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
/**
  * Calculates the distance between current location and coordinate.
  * This function was given by the teaching staff in gps.c.
  *
  * @param lat1 current location
  * @param lon1 current location
  * @param lat2 target location
  * @param lon2 target location
  * @return double distance
  */
double calculateDistance( double lat1, double lon1, double lat2, double lon2 )
{
  double v1[] = { cos( lon1 * DEG_TO_RAD ) * cos( lat1 * DEG_TO_RAD ),
                  sin( lon1 * DEG_TO_RAD ) * cos( lat1 * DEG_TO_RAD ),
                  sin( lat1 * DEG_TO_RAD ) };

  double v2[] = { cos( lon2 * DEG_TO_RAD ) * cos( lat2 * DEG_TO_RAD ),
                  sin( lon2 * DEG_TO_RAD ) * cos( lat2 * DEG_TO_RAD ),
                  sin( lat2 * DEG_TO_RAD ) };

  // Dot product these two vectors.
  double dp = 0.0;
  for ( int i = 0; i < sizeof( v1 ) / sizeof( v1[ 0 ] ); i++ )
    dp += v1[ i ] * v2[ i ];
  
  // Compute the angle between the vectors based on the dot product.
  double angle = acos( dp );

  // Return distance based on the radius of the earth.
  return EARTH_RADIUS * angle;
}

Point *parsePoint()
{
  char nameInput[MAX_NAMELENG];
  char descriptionInput[MAX_DESCLEN + 1];
  double latitude, longitude;
  if (scanf("%s%lf%lf %[^\n]", nameInput, &latitude, &longitude, descriptionInput) != 4) {
    scanf("%*s"); //frees the buffer
    return NULL;
  }
  if (strlen(nameInput) > MAX_NAMELENG) {
    return NULL;
  }
  if (strlen(descriptionInput) > MAX_DESCLEN) { //description length check
    return NULL;
  }
  if ((latitude < MIN_LAT || latitude > MAX_LAT) || (longitude < MIN_LON || longitude > MAX_LON)) {
    return NULL;
  }
  Point *pt = (Point *)malloc(sizeof(Point));
  char *description = (char *)malloc((strlen(descriptionInput) + 1) * sizeof(char));
  strcpy(description, descriptionInput);
  description[strlen(descriptionInput)] = '\0';
  strcpy(pt->name, nameInput);
  pt->location.lat = latitude;
  pt->location.lon = longitude;
  pt->description = description;
  return pt;
}

void freePoint(Point *pt)
{
  free(pt->description);
  free(pt);
}

void reportPoint(Point const *pt, Coords const *ref)
{
  double distance = globalDistance(&pt->location, ref);
  printf("%s (%.1f miles)\n", pt->name, distance);
  printf("  %s\n", pt->description);
}

double globalDistance(Coords const *c1, Coords const *c2)
{
  double lat1 = c1->lat;
  double lon1 = c1->lon;
  double lat2 = c2->lat;
  double lon2 = c2->lon;
  return calculateDistance(lat1, lon1, lat2, lon2);
}

