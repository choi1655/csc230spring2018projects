/**
  * @file codes.h
  * @author John Choi mchoi
  *
  * This component will contain a data structure for representing the contents
  * of the code file, along with functions to convert between symbols
  * and the variable-length binary codes used to represent them.
  * Above comment taken from the instructions.
  */

#include <stdio.h>

typedef struct {
  int letter; // in ASCII ' ' or '\n' or a to z space
  char *binary; // binary in string
} Data;

extern Data table[29];

/**
  * Used to populate letters and binaries array with characters.
  * Assumes the file is valid.
  *
  * @param fp to read letters from
  */
void populateData(FILE *fp);

/**
  * Given the ASCII code for a character or EOF (-1), this
  * function returns a string containing the code used to
  * represent it. For example, if you are using the codes-1.txt
  * code file, symToCode(     'a' ) should return "0000" and
  * symToCode(EOF) should return "11010000111". The string returned
  * by this function can't be changed by the caller, and it doesn't
  * need to be freed by the caller.
  * If there's no code to represent the given character, this
  * function returns NULL.
  * Above comment taken from the instruction.
  *
  * @param ch ASCII code to convert
  * @return char encoded string
  */
const char *symToCode(int ch);

/**
  * Given a string containing a code, this function returns
  * the ASCII character it represents or EOF (-1). For example,
  * if you are using codes-1.txt, codeToSym( "0000" ) should
  * return 'a' and codeToSym( "11010000111" ) should return EOF.
  * If the code does not represent a character, this function
  * returns -2.
  * Above comment taken from the instruction.
  *
  * @param code string to convert to ASCII character
  * @return int decoded string
  */
int codeToSym(const char *code);
