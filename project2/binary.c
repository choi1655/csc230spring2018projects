/** 
  * @file binary.c
  * @author John Choi mchoi
  * 
  * This file is like text.c, but it provides implementations of the encoding functions
  * that write out an image in our binary format.
  * Above comment taken from the instruction.
  */
#include "encoding.h"
#include <stdio.h>
#include <stdlib.h>

/** 
  * This function is part of the encoding component. It's implemented in the text.c to
  * print a header for a text encoding and in the binary.c file, to print a header for a
  * binary encoding. The parameters give the size of the image (which is required for the
  * header).
  * Above comment taken from the instruction.
  * Implemented on 02052018.
  * 
  * @param width of the image
  * @param height of the image
  */
void printHeader(int width, int height) 
{
  printf("P6\n");
  printf("%d %d", width, height);
  printf("\n%d\n", CMAX);
}

/**
  * This function is part of the encoding component. It prints one of the RGB components
  * (given via the parameter). It's implemented in the text.c to print a value in a text
  * encoding and in the binary.c file, to print a value for a binary encoding.
  * Above comment taken from the instructions.
  * Implemented on 02022018.
  *
  * @param c one of the RGB components to print
  */
void printValue(unsigned char c) 
{
  putchar(c); //prints char c in ASCII format
}

