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
  for (i = 0; i < 50 && !breakIt; i++) {
    for (int j = 0; j < 20; j++) {
      ch = fgetc(wordtextfile);
      if (!(ch >= 'a' && ch <= 'z') && ch != '\n' && ch != EOF) {
        fprintf(stderr, "Textfile contains invalid character\n");
        fclose(wordtextfile);
        exit(EXIT_FAILURE);
      }
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
    fprintf(stderr, "Invalid word file\n");
    fclose(wordtextfile);
    exit(EXIT_FAILURE);
  }
  char currentWord[20];
  bool nullExists = true;
  for (int i = 0; i < wordCount && nullExists; i++) {
    for (int j = 0; j < 20 && nullExists; j++) {
      currentWord[i] = words[i][j];
      for (int k = 0; k < 20; k++) {
        nullExists = false;
        if (currentWord[k] == '\0') {
          nullExists = true;
          break;
        }
      }
    }
  }
  if (!nullExists) {
    fprintf(stderr, "Invalid word file. Goes over 20.\n");
    fclose(wordtextfile);
    exit(EXIT_FAILURE);
  }
  //fclose(wordtextfile);
}

