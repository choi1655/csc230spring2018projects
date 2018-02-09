/** 
  * @file geometry.h
  * @author NCSU CSC230 TS
  * @author John Choi mchoi
  * 
  * Provides prototypes for the geometry.c
  */

#include <stdbool.h>

/** For the extra credit, the number of horizontal and vertical super-samples
    per pixel.  This is a preprocessor macro with conditional compilation
    code around it.  That lets us replace this value on the gcc line, if
    we want to.  This constant definition should probably be in triangle.c
    rather than in this header, but putting it here lets me give it to you
    with the starter. */
#ifndef SSAMP
#define SSAMP 1
#endif

/**
  * This function helps to determine whether a pixel is inside the triangle. It
  * implements the left-turn test described in the Requirements. The first four
  * parameters give it the location of two consecutive vertices of the triangle (in
  * counter-clockwise order), and the last two parameters give the location of the
  * center of a pixel. This function is part of the geometry component.
  * Above comment taken from the instructions.
  *
  * @param xa location of triangle
  * @param ya location of triangle
  * @param xb location of triangle
  * @param yb location of triangle
  * @param x location of center of a pixel
  * @param y location of center of a pixel
  * @return true if the pixel is inside the triangle
  */
bool leftOf( double xa, double ya, double xb, double yb,
             double x, double y );

/**
  * This function uses leftOf() to determine whether a pixel (the last two parameters)
  * is inside the triangle. It's part of the geometry component.
  * Above comment taken from the instruction.
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
bool inside( double x1, double y1, double x2, double y2,
             double x3, double y3, double x, double y );
