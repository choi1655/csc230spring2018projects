/**
  * @file wordlist.h
  * @author John Choi mchoi
  *
  * Provides prototypes for these global variables.
  */
/** Two dimensional array used to store the word list */
extern char words[50][20];

/** Word count used to count the numbers stored. */
extern int wordCount;

/** 
  * This is part of the wordlist component. 
  * It will read the words in the file and store them in the words array.
  */
void readWords(char const * filename);

