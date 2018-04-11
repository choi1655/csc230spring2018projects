/**
  * @file codes.c
  * @author John Choi mchoi
  *
  * This component will contain a data structure for representing the
  * contents of the code file, along with functions to convert between symbols
  * and the variable-length binary codes used to represent them.
  * Above comment taken from the instructions.
  */

#include "codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Data structure */
Data table[29];

/**
  * Populates the data structure.
  *
  * @param fp file to read data from
  */
void populateData(FILE *fp)
{
  //Data *table = (Data *)malloc(29 * sizeof(Data));
  char *letter = (char *)malloc(8 * sizeof(char));
  char *binary = (char *)malloc(100 * sizeof(char));
  for (int i = 0; i < 29; i++) {
    table[i].binary = (char *)malloc(100 * sizeof(char));
  }
  //table->binary = (char *)malloc(100 * sizeof(char));
  for (int i = 0; i < 29; i++) {
    if (fscanf(fp, "%s %s", letter, binary) == EOF) {
      //table[i].letter = -1;
    } else if (strcmp(letter, "space") == 0) {
      table[i].letter = 32;
    } else if (strcmp(letter, "newline") == 0) {
      table[i].letter = '\n';
    } else if (strcmp(letter, "eof") == 0) {
      table[i].letter = -1;
    } else {
      table[i].letter = letter[0];
    }
    strcpy(table[i].binary, binary);
  }
  free(letter);
  free(binary);
  /*for (int i = 0; i < 29; i++) {
    free(table[i].binary);
  }*/
}

const char *symToCode(int ch)
{
	for (int i = 0; i < 29; i++) {
	  if (table[i].letter == ch) {
	    return table[i].binary;
	  }
	}
	return NULL;
}

int codeToSym(const char *code)
{
	for (int i = 0; i < 29; i++) {
	  if (strcmp(code, table[i].binary) == 0) {
	    return table[i].letter;
	  }
	}
	return -2;
}
