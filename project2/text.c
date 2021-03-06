/**
  * @file text.c
  * @author John Choi mchoi
  * 
  * This file is a little unusual. It's one of two implementation files that goes with
  * the encoding.h header. Normally, an implementation and its header would share the
  * same base name, but here we're using the encoding.h header to describe the interface
  * for the encoding functions, with either text.c or binary.c being linked in to supply
  * the definitions of these functions. This file contains implementations of the
  * encoding functions to write out the image in the text format.
  * Above comment taken from the instructions.
  */
#include "encoding.h"
#include <stdio.h>
#include <stdlib.h>

/** Defines the line character limit for each lines to 70 */
#define C_LIMIT 71

static int counter = 0;
/** 
  * This function is part of the encoding component. It's implemented in the text.c to
  * print a header for a text encoding and in the binary.c file, to print a header for a
  * binary encoding. The parameters give the size of the image (which is required for the
  * header).
  * Above comment taken from the instruction.
  * Used by the main function to print the header.
  * Implemented on 02052018.
  * 
  * @param width of the image
  * @param height of the image
  */
void printHeader(int width, int height) 
{
  printf("P3\n");
  printf("%d %d", width, height);
  printf("\n%d\n", CMAX);
}

/**
  * This function is part of the encoding component. It prints one of the RGB components
  * (given via the parameter). It's implemented in the text.c to print a value in a text
  * encoding and in the binary.c file, to print a value for a binary encoding.
  * Above comment taken from the instructions. 
  * Used by the main function to print.
  * Called 3 times = one pixel component.
  * Implemented on 02062018.
  *
  * @param c one of the RGB components to print
  */
void printValue(unsigned char c)
{
  if (counter == 0) { //only runs once
    printf("%d", c);
    if (c >= 100 && c <= 255) {
      counter += 3;
    } else if (c >= 10 && c < 100) {
      counter += 2;
    } else if (c >= 0 && c < 10) {
      counter++;
    }
  } else {
    if (c >= 100 && c <= 255 && counter + 4 < C_LIMIT) {
      printf(" %d", c);
      counter += 4;
    } else if (c >= 10 && c < 100 && counter + 3 < C_LIMIT) {
      counter += 3;
      printf(" %d", c);
    } else if (c >= 0 && c < 10 && counter + 2 < C_LIMIT) {
      counter += 2;
      printf(" %d", c);
    } else if (c >= 100 && c <= 255 && counter + 4 >= C_LIMIT) {
      printf("\n%d", c);
      counter = 3;
    } else if (c >= 10 && c < 100 && counter + 3 >= C_LIMIT) {
      printf("\n%d", c);
      counter = 2;
    } else if (c >= 0 && c < 10 && counter + 2 >= C_LIMIT) {
      printf("\n%d", c);
      counter = 1;
    }
  }
}

