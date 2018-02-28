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

int wordCount = 0;
/**
  * It will read the words in the file and store them in the words array.
  *
  * @param filename to read
  */
void readWords(char const * filename)
{
  FILE *wordtextfile = fopen(filename, "r");
  while(1) {
    if (fscanf(wordtextfile, "%s", words[wordCount]) == EOF) {
      break;
    }
    for (int i = 0; i < strlen(words[wordCount]); i++) {
      if (!(words[wordCount][i] >= 'a' && words[wordCount][i] <= 'z')) {
        fprintf(stderr, "Invalid word file\n");
        exit(EXIT_FAILURE);
      }
    }
    if (strlen(words[wordCount]) > 20) {
      fprintf(stderr, "Invalid word file\n");
      exit(EXIT_FAILURE);
    }
    wordCount++;
    if (wordCount > 50) {
      fprintf(stderr, "Invalid word file\n");
      exit(EXIT_FAILURE);
    }
  }
  
  
  fclose(wordtextfile);
}

