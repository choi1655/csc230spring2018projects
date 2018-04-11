/**
  * @file encode.c
  * @author John Choi mchoi
  *
  * This component will implement the main function of the encode
  * program. It will be responsible for handling the command-line
  * arguments, reading characters from the input file, using
  * the codes component to convert them the codes, and using
  * the bits component to write them out to the output file.
  * Above comment taken from the instruction.
  */

#include "bits.h"
#include "codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Initializes Buffer struct */
BitBuffer buffer = {0, 0};
/** Data structure to check if the data is valid */
Data table[29];

/**
  * Function to check for duplicate letters or binaries that are too long in length.
  *
  * @return true if the data is valid
  */
bool secondValidityCheck()
{
  for (int i = 0; i < 29; i++) {
    for (int j = 0; j < i; j++) {
      if (table[j].letter == table[i].letter) {
        return false;
      }
    }
  }
  for (int i = 0; i < 29; i++) {
    for (int j = 0; j < i; j++) {
      if (strcmp(table[j].binary, table[i].binary) == 0) {
        return false;
      }
    }
  }
  for (int i = 0; i < 29; i++) {
    if (strlen(table[i].binary) > 12) {
      return false;
    }
  }
  return true;
}

/**
  * Checks to see if the code text file is valid.
  *
  * @param fp file to check
  * @return true if the file is valid
  */
bool codeFileValid(FILE *fp)
{
  int ch = fgetc(fp);
  char (*usedWords)[8] = (char (*)[8])malloc(29 * sizeof(*usedWords));//2d array to store strings
  int linecounter = 0; //counts the number of lines on the text file
  while (ch != EOF) {
    if (ch != '1' && ch != '0' && ch != ' ' && ch != '\n' && (ch < 97 || ch > 122)) {
      free(usedWords);
      return false;
    }
    if (ch == '\n') {
      linecounter++;
    }
    ch = fgetc(fp);
  }
  if (linecounter != 29) { //checks the number of lines
    free(usedWords);
    return false;
  }
  free(usedWords);
  return true;
}

/**
  * Checks to see if the text input file is valid.
  * Checks to make sure the file only contains lowercase letters, spaces, and line changers.
  *
  * @param fp file to scan
  * @return true if the file is valid, false if the file is invalid
  */
bool textFileValid(FILE *fp)
{
  int ch = fgetc(fp);
  while (ch != EOF) {
    if (ch != ' ' && ch != '\n' && (ch < 97 || ch > 122)) {
      return false;
    }
    ch = fgetc(fp);
  }
  return true;
}

/**
  * Main function that executes the program.
  *
  * @param argc number of arguments
  * @param argv argument tokens
  * @return EXIT_SUCCESS if program terminates successfully
  */
int main(int argc, char *argv[])
{
  if (argc != 4) {
    fprintf(stderr, "usage: encode <codes-file> <infile> <outfile>\n");
    return EXIT_FAILURE;
  }
  FILE *codesInput = fopen(argv[1], "r");
  if (!codesInput) { //cant open the codes input file
    perror(argv[1]);
    return EXIT_FAILURE;
  }
  FILE *textInput = fopen(argv[2], "r");
  if (!textInput) { //cant open the text input file
    perror(argv[2]);
    fclose(codesInput);
    return EXIT_FAILURE;
  }
  FILE *binaryOutput = fopen(argv[3], "wb");
  FILE *textInputDummy = fopen(argv[2], "r");
  FILE *codesInputDummy = fopen(argv[1], "r");

  if (!textFileValid(textInputDummy)) {
    fprintf(stderr, "Invalid input file\n");
    fclose(textInputDummy);
    fclose(textInput);
    fclose(codesInput);
    fclose(binaryOutput);
    fclose(codesInputDummy);
    return EXIT_FAILURE;
  }
  fclose(textInputDummy);
  if (!codeFileValid(codesInputDummy)) {
    fprintf(stderr, "Invalid code file\n");
    fclose(codesInputDummy);
    fclose(textInput);
    fclose(codesInput);
    fclose(binaryOutput);
    return EXIT_FAILURE;
  }
  fclose(codesInputDummy);

  populateData(codesInput); //reads the codes table from the codes*.txt
  if (!secondValidityCheck()) {
    fprintf(stderr, "Invalid code file\n");
    fclose(textInput);
    fclose(codesInput);
    fclose(binaryOutput);
    return EXIT_FAILURE;
  }

  int ch = fgetc(textInput);
  while (ch != EOF) {
    writeBits(symToCode(ch), &buffer, binaryOutput);
    ch = fgetc(textInput);
  }
  writeBits(symToCode(EOF), &buffer, binaryOutput);
  flushBits(&buffer, binaryOutput);

  for (int i = 0; i < 29; i++) {
    free(table[i].binary);
  }
  fclose(codesInput);
  fclose(textInput);
  fclose(binaryOutput);
  return EXIT_SUCCESS;
}
