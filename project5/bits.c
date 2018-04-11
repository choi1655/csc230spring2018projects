/**
  * @file bits.c
  * @author John Choi mchoi
  *
  * This component will define functions for writing out bit
  * sequences to and from a file. With the help of these functions,
  * it will be easy for the main program
  * to read and write the bit codes needed for these functions,
  * it will be easy for the main program to read and write the bit codes
  * needed for encoding/decoding. Some of the header for this component
  * has already been written for you in the starter. You can add more functions
  * to this component if you want to.
  * Above comment taken from the instructions.
  */

#include "bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Initialize BitBuffer struct */
BitBuffer bufferspace = {0, 0};

void writeBits(const char *code, BitBuffer *buffer, FILE *fp)
{
  for (int i = 0; i < strlen(code); i++) { //converts "01010101" to 01010101
    if (buffer->bcount > 7) {
      fputc(buffer->bits, fp);
      buffer->bcount = 0;
      buffer->bits = 0; //resets bits
    }
    buffer->bits = (buffer->bits << 1) | (code[i] == '1' ? 1 : 0);
    buffer->bcount++;
  }
}

void flushBits(BitBuffer *buffer, FILE *fp)
{
  if (buffer->bcount > 0) {
    buffer->bits = buffer->bits << (8 - buffer->bcount);
    buffer->bcount = 0;
    fputc(buffer->bits, fp);
    buffer->bits = 0;
  }
  /*for (int i = buffer->bcount; i >= 0; i--) {
    fputc(0, fp);
  }*/
}

int readBit(BitBuffer *buffer, FILE *fp)
{
  if (buffer->bcount == 0) { //if the buffer is empty
    int ch = fgetc(fp);
    if (ch == EOF) {
      return -1;
    }
    buffer->bcount = 8;
    buffer->bits = ch;
    int digit = buffer->bits >> 7;
    buffer->bits = buffer->bits << 1;
    buffer->bcount--;
    return digit;
  } else { // if the buffer is not empty
    int digit = buffer->bits >> 7;
    buffer->bits = buffer->bits << 1;
    buffer->bcount--;
    return digit;
  }
}
