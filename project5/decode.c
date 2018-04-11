/**
  * @file decode.c
  * @author John Choi mchoi
  *
  * This component will implement the main function of the decode program.
  * It will be responsible for handling the command-line arguments,
  * using the bits component to read codes from the input file,
  * using the codes component to convert them back to characters and
  * writing those to the output file.
  * Above comment taken from the instructions.
  */

#include "codes.h"
#include "bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** Initializes buffer */
BitBuffer buffer = {0, 0};

/**
  * Checks to see if the code text file is valid.
  *
  * @param fp file to check
  * @return true if the file is valid
  */
bool codeFileValid(FILE *fp)
{
  int ch = fgetc(fp);
  char (*usedWords)[8] = (char (*)[8])malloc(29 * sizeof(*usedWords)); //2d array to store strings
  int linecounter = 0; //counts the number of lines on the text file
  while (ch != EOF) {
    if (ch != '1' && ch != '0' && ch != ' ' && ch != '\n' && (ch < 97 || ch > 122)) {
      return false;
    }
    if (ch == '\n') {
      linecounter++;
    }
    ch = fgetc(fp);
  }
  if (linecounter > 29) { //checks the number of lines
    return false;
  }
  free(*usedWords);
  return true;
}

/**
  * Main function that executes the program.
  *
  * @param argc number of arguments
  * @param argv each argument tokens
  * @return EXIT_SUCCESS if the program terminates correctly
  */
int main(int argc, char *argv[])
{
  if (argc != 4) {
    fprintf(stderr, "usage: decode <codes-file> <infile> <outfile>\n");
    return EXIT_FAILURE;
  }
  FILE *codesInput = fopen(argv[1], "r");
  if (!codesInput) {
    perror(argv[1]);
    return EXIT_FAILURE;
  }
  FILE *binaryInput = fopen(argv[2], "rb");
  if (!binaryInput) {
    fclose(codesInput);
    perror(argv[2]);
    return EXIT_FAILURE;
  }
  FILE *output = fopen(argv[3], "w");
  FILE *codesInputDummy = fopen(argv[1], "r");

  if (!codeFileValid(codesInputDummy)) {
    fclose(codesInputDummy);
    fclose(codesInput);
    fclose(binaryInput);
    fclose(output);
    fprintf(stderr, "Invalid input file\n");
    return EXIT_FAILURE;
  }
  fclose(codesInputDummy);
  populateData(codesInput);
  
  int counter = 0;
  char *temp = (char *)malloc(13 * sizeof(char));
  strcpy(temp, "");
  buffer.bcount = 8;
  buffer.bits = fgetc(binaryInput);
  int bit = readBit(&buffer, binaryInput);
  int character;
  while (bit != -1) {
    if (bit == 1) {
      counter++;
    }
    strcat(temp, (bit == 0 ? "0" : "1"));
    character = codeToSym(temp);
    if (character != -2) {
      strcpy(temp, "");
      counter = 0;
      if (character == -1) {
        break;
      }
      fputc(character, output);
    }
    bit = readBit(&buffer, binaryInput);
  }
  //fprintf(output, "\n");
  if (counter != 0) {
    fprintf(stderr, "Invalid input file\n");
    free(temp);
    for (int i = 0; i < 29; i++) {
      free(table[i].binary);
    }
    fclose(codesInput);
    fclose(binaryInput);
    fclose(output);
    return EXIT_FAILURE;
  }
  free(temp);
  
  for (int i = 0; i < 29; i++) {
    free(table[i].binary);
  }
  fclose(codesInput);
  fclose(binaryInput);
  fclose(output);
  return EXIT_SUCCESS;
}
