/**
  * @file hangman.c
  * @author John Choi mchoi
  * 
  * This component will contain the main function, and any other functions
  * that don't belong in one of the other components (i.e., any functions
  * that don't pertain to reading the wordlist or displaying the word or
  * stick figure).
  * Above comment taken from the instruction.
  */

#include "display.h"
#include "wordlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/** Two dimensional array used to store the word list */
int words[50][20];
/** Word count used to count the numbers stored. */
int wordCount;

/**
  * @param arr takes in wordsToDisplay
  * @param len length of wordsToDisplay
  * @param a 
  */
char * underscoreToLetter(char *arr, int len, char a)
{
  for (int i = 0; i < len; i++) {
    if (*(arr + i) == a) {
      *arr == a;
    }
  }
  return arr;
}

/**
  * Takes in the word to compare and returns true if the character exists.
  *
  * @param arr word to comapre
  * @param len length of the word
  * @param a character to check
  * @return true if character exists
  */
bool contains(char *arr, int len, char a)
{
  for (int i = 0; i < len; i++) {
    if (*(arr + i) == a) {
      return true;
    }
  }
  return false;
}

/**
  * Called after checking everything else required to run the program.
  */
void playGame()
{
  initializeChars();
  bool letterIsTrue = false;
  int maxTimesIncorrect = 0;
  int numberTimesCorrect = 0;
  int index = rand() % wordCount; // randomly chosen word for user to guess
  char selectedWord[20];
  for (int i = 0; i < 20; i++) {
    selectedWord[i] = words[index][i];
  }
  int selectedWordSize = strlen(selectedWord); //from string.h returns words size
  char input;
  char wordToDisplay[selectedWordSize + 1];
  char *pWordToDisplay = wordToDisplay;
  for (int i = 0; i < selectedWordSize + 1; i++) {
    *(pWordToDisplay + i) = '_'; // initializes word to underscores
  }
  
  //actual game runs from here
  displayWord(wordToDisplay);
  printChars(0);
  while (maxTimesIncorrect != 7) {
    printf("letter> ");
    if (scanf("%c", &input) != 1) { //if invalid input, re-run the loop
      printf("Invalid letter\n");
      continue;
    } else if (input == EOF) {
      exit(EXIT_SUCCESS); // exit the program if user inputs EOF
    }
    //TODO calls display components repeatedly
    letterIsTrue = contains(selectedWord, selectedWordSize, input);
    if (!letterIsTrue) { //if input letter was wrong
      maxTimesIncorrect++;
      displayFigure(maxTimesIncorrect);//passes in maxTimesIncorrect to display stick figure
      displayWord(wordToDisplay);
    } else {  //if input letter was correct
      //TODO need something replaces underscore characters in string
      numberTimesCorrect++;
      if (numberTimesCorrect == selectedWordSize) { // if correct word reached terminate loop
        fprintf(stdout, "You win!\n\n");
        break;
      }
    }
    printChars(input);
  }
  
  
  if (maxTimesIncorrect == 7) {
    fprintf(stdout, "You lose!\nWord was %s\n\n", selectedWord);
  }
}

/**
  * Main function that executes the program.
  *
  * @param argc counts the number of arguments
  * @param argv stores each arguments in this array
  * @return EXIT_SUCCESS if the program compiles and exits correctly
  */
int main(int argc, char *argv[]) 
{
  int seedNum;
  FILE *stream = fopen(argv[1], "r"); // opens the file to read
  if (!stream) { // checks to see if the file can be opened
    fprintf(stderr, "Can't open word file\n");
    return EXIT_FAILURE;
  }
  if (argc > 3) { //checks the number of arguments passed in
    fprintf(stderr, "Too many arguments.\n");
    fclose(stream);
    return EXIT_FAILURE;
  }
 /* 
  int current = fgetc(stream);
  /**
    * Check to see if there's any capital letters in the text file
    */
  /*while (current != EOF) {
    if (!(current >= 'a' && current <= 'z')) {
      fprintf(stderr, "Invalid word file");
      fclose(stream);
      return EXIT_FAILURE; // exit with code 1 if a character is capitalized
    }
    current = getchar();
  } */
  if (argc > 2) {
    seedNum = atoi(argv[2]); //optional seed number
    if (seedNum < 0) {
      fprintf(stderr, "Seed cannot be negative.\n");
      fclose(stream);
      return EXIT_FAILURE;
    }
    srand(seedNum);
  } else {
    srand(time(NULL));
  }
  fclose(stream); //closes the file to prevent file leak
  readWords(argv[1]); // calls display components; populates word list
  
  playGame();
  
  char response;
  printf("Play again(y,n)> ");
  scanf("%c", &response);
  if (response == EOF) {
    return EXIT_SUCCESS; //Exit the program successfully if user inputs EOF
  }
  while (response == 'y' && response == 'Y') {
    playGame();
    printf("Play again(y,n)> ");
    scanf("%c", &response);
    if (response == EOF) {
      return EXIT_SUCCESS; // exit the program successfully if user inputs EOF
    }
  }
  return EXIT_SUCCESS;
}

