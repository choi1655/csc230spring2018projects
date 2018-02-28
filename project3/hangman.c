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

//TODO check for duplicates
/** Two dimensional array used to store the word list */
int words[50][20];
/** Word count used to count the numbers stored. */
int wordCount;

/**
  * @param arr takes in wordsToDisplay
  * @param len length of wordsToDisplay
  * @param a 
  */
void underscoreToLetter(char *selectedWord, char *wordToDisplay, int len, char input)
{
  for (int i = 0; i < len; i++) {
    if (selectedWord[i] == input) {
      *(wordToDisplay + i) = input;
    }
  }
}

/**
  * Takes in the word to compare and returns true if the character exists.
  *
  * @param selectedWord word to comapre
  * @param len length of the word
  * @param a character to check
  * @return true if character exists
  */
bool contains(char *selectedWord, int len, char a)
{
  for (int i = 0; i < len; i++) {
    if (*(selectedWord + i) == a) {
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
  for (int i = 0; i < selectedWordSize; i++) {
    *(pWordToDisplay + i) = '_'; // initializes word to underscores
  }
  *(pWordToDisplay + (selectedWordSize + 1)) = '\0';
  
  //actual game runs from here
  printf("Word chosen for debug: %s\n", selectedWord); //TODO delete this later
  displayWord(wordToDisplay);
  printChars(0);
  
  
  while (maxTimesIncorrect != 7) {
    printf("letter> ");
    if (fscanf(stdin, "%c", &input) != 1) { //if invalid input, re-run the loop
      printf("Invalid letter\n");
      continue;
    } else if (input == EOF) {
      exit(EXIT_SUCCESS); // exit the program if user inputs EOF
    }
    printf("\n");
    printf("Input for debug: %c\n", input); //TODO delete this later
    letterIsTrue = contains(selectedWord, selectedWordSize, input);
    
    
    if (!letterIsTrue) { //if input letter was wrong
      maxTimesIncorrect++;
      displayFigure(maxTimesIncorrect);//passes in maxTimesIncorrect to display stick figure
      displayWord(wordToDisplay);
      
      
      
    } else {  //if input letter was correct
      underscoreToLetter(selectedWord, wordToDisplay, selectedWordSize, input);
      numberTimesCorrect++;
      displayWord(wordToDisplay);
      if (numberTimesCorrect == selectedWordSize) { // if correct word reached terminate loop
        fprintf(stdout, "You win!\n\n");
        break;
      }
    }
    printChars(input);
  } //end of while loop
  
  
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
  fscanf(stdin, "%c", &response);
  printf("\n");
  if (response == EOF) {
    return EXIT_SUCCESS; //Exit the program successfully if user inputs EOF
  }
  while (response == 'y' && response == 'Y') {
    playGame();
    printf("Play again(y,n)> ");
    fscanf(stdin, "%c", &response);
    printf("\n");
    if (response == EOF) {
      return EXIT_SUCCESS; // exit the program successfully if user inputs EOF
    }
  }
  return EXIT_SUCCESS;
}

