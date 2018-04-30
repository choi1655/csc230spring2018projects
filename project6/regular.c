/**
  * @file regular.c
  * @author John Choi mchoi
  * @author NCSU CSC230 TS
  *
  * This component contains the main() function. It's responsible for handling
  * command-line arguments, using the parser to build a representation for the
  * regular expression, reading input lines, using the Pattern representation
  * to find places where the pattern matchs the an input line, and printing
  * out the lines with the matches highlighted in red. In the starter,
  * the main function contains some sample code to show how to parse and use
  * regular expressions. This sample code is written with some hard-coded
  * regular expressions and strings. As you complete your implementation,
  * you'll probably delete most of the code and replace it with your own code
  */

#include "parse.h"
#include "pattern.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// On the command line, which argument is the pattern.
#define PAT_ARG 1

// On the command line, which argument is the input file.
#define FILE_ARG 2

/**
  * Reports appropriately highlighted words.
  *
  * @param pat pattern to used
  * @param str single line
  * @return true if match found
  */
bool reportValidRegex(Pattern *pat, const char *str)
{
  bool match = false;
  int bookmark = 0;
  for (int begin = 0; begin < strlen(str); begin++) {
    for (int end = begin; end <= strlen(str); end++) {
      if (matches(pat, begin, end)) {
        match = true;
        if (begin != 0) {
          for (int i = bookmark; i < begin; i++) { //print before red
            printf("%c", str[i]);
          }
        }
        printf("%c[31m", 27); //change font color to red
        while (begin != end) {
          printf("%c", str[begin]);
          begin++;
        }
        printf("%c[0m", 27); //change color back to normal
        bookmark = end;
      }
    }
  }

  if (match) {
    for (int i = bookmark; i < strlen(str); i++) {
      printf("%c", str[i]);
    }
    printf("\n");
  }
  return match;
}

/**
  * Entry point for the program, parses command-line arguments, builds
  * the pattern and then tests it against lines of input.
  *
  * @param argc Number of command-line arguments.
  * @param argv List of command-line arguments.
  * @return exit status for the program.
  */
int main( int argc, char *argv[] )
{
  // Temporary code to show how the pattern matching mechanism is
  // supposed to work.  You'll need to replace this with code to do
  // what the program is actually supposed to do, get a pattern from
  // the command line, match it against lines from an input file, and
  // report matching lines with occurrences of the matches
  // highlighted.

  if (argc <= 1 || argc > 3) {
      fprintf(stderr, "usage: regular <pattern> [input-file.txt]\n");
      return EXIT_FAILURE;
  }


  char *patternString = argv[1];
  Pattern *pattern = parsePattern(patternString);
  FILE *input = argc == 2 ? stdin: fopen(argv[2], "r");
  if (!input) {
      pattern->destroy(pattern);
      fprintf(stderr, "Can't open input file: %s\n", argv[2]);
      return EXIT_FAILURE;
  }
  char *string = (char *)malloc(100 * sizeof(char));
  char last;
  while (fscanf(input, "%99[^\n]", string) != EOF) {
    fscanf(input, "%c", &last);
    if (last != '\n') {
      fprintf(stderr, "Input line too long\n");
      fclose(input);
      free(string);
      pattern->destroy(pattern);
      return EXIT_FAILURE;
    }
    pattern->locate(pattern, string);
    reportValidRegex(pattern, string);
  }

  // provided by the TS; to be deleted in the final submission
  // // Try matching a pattern containing just one letter.
  // {
  //   // Parse a simple pattern.
  //   char *pstr = "b";
  //   Pattern *pat = parsePattern( pstr );
  //
  //   char *str = "abc";
  //
  //   // Find matches for this pattern.
  //   pat->locate( pat, str );
  //
  //   // reportMatches( pat, pstr, str );
  //
  //   pat->destroy( pat );
  // }
  //
  // // Try a pattern with some concatenation and multiple matches.
  // {
  //   // Parse a simple pattern.
  //   char *pstr = "aba";
  //   Pattern *pat = parsePattern( pstr );
  //
  //   char *str = "ababababababab";
  //
  //   // Find matches for this pattern.
  //   pat->locate( pat, str );
  //
  //   // reportMatches( pat, pstr, str );
  //
  //   pat->destroy( pat );
  // }
  pattern->destroy(pattern);
  fclose(input);
  free(string);
  return EXIT_SUCCESS;
}
