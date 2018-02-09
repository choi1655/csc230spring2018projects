/**
  * @file geometry.c
  * @author John Choi mchoi
  * 
  * This is the implementation file for the geometry component. It contains definitions
  * for functions that help decide whether a given point is inside the triangle.
  * Above comment taken from the instruction.
  */
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
  * This function helps to determine whether a pixel is inside the triangle. It
  * implements the left-turn test described in the Requirements. The first four
  * parameters give it the location of two consecutive vertices of the triangle (in
  * counter-clockwise order), and the last two parameters give the location of the
  * center of a pixel. This function is part of the geometry component.
  * Above comment taken from the instructions.
  * Calculates the vector.
  * Implemented on 02052018.
  *
  * @param xa location of triangle
  * @param ya location of triangle
  * @param xb location of triangle
  * @param yb location of triangle
  * @param x location of center of a pixel
  * @param y location of center of a pixel
  * @return true if the pixel is inside the triangle
  */
bool leftOf(double xa, double ya, double xb, double yb, double x, double y) 
{
  double xe = xb - xa;
  double ye = yb - ya;
  double xp = x - xa;
  double yp = y - ya;
  double val1 = xe * yp;
  double val2 = ye * xp;
  double val3 = val1 - val2;
  return val3 <= 0;
}

/**
  * This function uses leftOf() to determine whether a pixel (the last two parameters)
  * is inside the triangle. It's part of the geometry component.
  * Above comment taken from the instruction.
  * Implemented on 02052018.
  *
  * @param x1 location of triangle
  * @param y1 location of triangle
  * @param x2 location of triangle
  * @param y2 location of triangle
  * @param x3 location of triangle
  * @param y3 location of triangle
  * @param x location of center of a pixel
  * @param y location of center of a pixel
  * @return true if the pixel is inside the triangle
  */
bool inside(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y)
{
  bool val1, val2, val3;
  val1 = leftOf(x1, y1, x2, y2, x, y);
  val2 = leftOf(x2, y2, x3, y3, x, y);
  val3 = leftOf(x3, y3, x1, y1, x, y);
  return val1 && val2 && val3;
}

