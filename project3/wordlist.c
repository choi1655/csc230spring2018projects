/**
  * @file wordlist.c
  * @author John Choi mchoi
  *
  * This component defines functions for working with the word list.
  * Above taken from the instruction.
  */

#include "wordlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//TODO check to see if length of word goes over 20

/**
  * It will read the words in the file and store them in the words array.
  *
  * @param filename to read
  */
void readWords(char const * filename)
{
  FILE *wordtextfile = fopen(filename, "r");
  int ch;
  int i;
  bool breakIt = false; //to break out of nested for loops
  bool triggerLimit = false;
  for (i = 0; i < 50 && !breakIt; i++) {
    for (int j = 0; j < 20; j++) {
      ch = fgetc(wordtextfile);
      if (ch == '\n') {
        break;
      }
      if (ch == EOF) { //if EOF reached, save the current i value to the wordCount
        wordCount = i;
        breakIt = true;
        break;
      }
      words[i][j] = ch;
    }
  }
  if (wordCount > 50) { // exits the program with failure code if the word count is > 50
    fprintf(stderr, "Invalid word file");
    fclose(wordtextfile);
    exit(EXIT_FAILURE);
  }
  fclose(wordtextfile);
}

