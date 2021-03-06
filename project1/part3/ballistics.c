/**
  * @file ballistics.c
  * @author John Choi mchoi
  * 
  * Prints a table with velocity, angle, time, and distance of the projectile.
  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
  * Function that returns the sine value in degrees instead of radian.
  * 
  * @param theta the theta value in radian
  * @return double sin in degrees
  */
double sinDegree(double theta) 
{
  return sin(theta * (M_PI / 180));
}

/**
  * Function that calculates horizontal distance of the projectile.
  * 
  * @param angle of the projectile
  * @param v0 initial velocity of the projectile
  * @return double horizontal distance
  */
double calculateDistance(int angle, double v0) 
{
  double a = pow(v0, 2);
  double b = sinDegree(2 * angle);
  double grav = 9.81;
  return (a * b) / grav;
}

/**
  * Given the angle in degrees that a projectile leaves the ground and its initial
  * velocity, this function should return the time the projectile is in the air.
  * Above quoted from the instructions.
  * 
  * @param angle of the projectile
  * @param v0 initial velocity
  * @return double time the projectile is in the air
  */
double flightTime(int angle, double v0) 
{
  double time = 0;
  double upper = 2 * v0 * sinDegree(angle);
  double lower = 9.81;
  time = upper / lower;
  return time;
}

/**
  * This function prints out a row of the table, given the angle and initial velocity 
  * (like the previous function) and the flight time (the output from the previous
  * function), it should print a row of the table reporting these three values, along
  * with the distance the projectile travels.
  * Above quoted from the instructions.
  * 
  * @param angle of the projectile
  * @param v0 initial velocity
  * @param t flight time
  */
void tableRow(int angle, double v0, double t) 
{
  double distance_horizontal = calculateDistance(angle, v0);
  printf("%10d | %10.3f | %10.3f | %10.3f\n", angle, v0, t, distance_horizontal);
}

/**
  * This is the starting point for running your program. It will read input from 
  * the user, print the table header, then use the other two functions to print the rest
  * of the table.
  * Above quoted from the instructions.
  *
  * @return EXIT_SUCCESS if the program exited successfully
  */
int main() 
{
  double v0 = 0;
  double projectile_time = 0;
  printf("V0: ");
  scanf("%lf", &v0);
  printf("\n%10s |%11s |%11s |%11s\n", "angle", "v0", "time", "distance");
  printf("-----------+------------+------------+-----------");
  printf("\n");
  for (int i = 0; i <= 90; i += 5) {
    projectile_time = flightTime(i, v0);
    tableRow(i, v0, projectile_time);
  }
  return EXIT_SUCCESS;
}

