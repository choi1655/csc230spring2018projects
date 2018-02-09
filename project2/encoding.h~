/**
  * @file encoding.h
  * @author NCSU CSC230 TS
  * @author John Choi mchoi
  *
  * Provides prototypes for text.c and binary.c
  */

/** Maximum color component value. */
#define CMAX 255

/** 
  * This function is part of the encoding component. It's implemented in the text.c to
  * print a header for a text encoding and in the binary.c file, to print a header for a
  * binary encoding. The parameters give the size of the image (which is required for the
  * header).
  * Above comment taken from the instruction.
  * Used by the main function to print the header.
  * 
  * @param width of the image
  * @param height of the image
  */
void printHeader( int width, int height );

/**
  * This function is part of the encoding component. It prints one of the RGB components
  * (given via the parameter). It's implemented in the text.c to print a value in a text
  * encoding and in the binary.c file, to print a value for a binary encoding.
  * Above comment taken from the instructions. 
  * Used by the main function to print.
  * Called 3 times = one pixel component.
  *
  * @param c one of the RGB components to print
  */
void printValue( unsigned char c );
