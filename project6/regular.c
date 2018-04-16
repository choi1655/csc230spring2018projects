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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pattern.h"
#include "parse.h"

// On the command line, which argument is the pattern.
#define PAT_ARG 1

// On the command line, which argument is the input file.
#define FILE_ARG 2

// You won't need this function in the final version of your program. //TODO
// It prints out the input string and all matches of the pattern inside
// it.
void reportMatches( Pattern *pat, char const *pstr, char const *str )
{
  // Report the original string and copies of all the matches.
  printf( "Pattern: %s\n", pstr );
  printf( "String:  %s\n", str );

  int len = strlen( str );
  bool mflag = false;
  for ( int begin = 0; begin <= len; begin++ )
    for ( int end = begin; end <= len; end++ )
      if ( matches( pat, begin, end ) ) {
        // Report the matching substring.
        
        // Skip over to the start of the match.
        printf( "Match:   %*s", begin, "" );
        
        // Print the matchng string.
        for ( int k = begin; k < end; k++ )
          printf( "%c", str[ k ] );
        printf( "\n" );

        // Remember that we found a match.
        mflag = true;
      }

  if ( !mflag )
    printf( "No matches\n" );
  
  printf( "\n" );
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

  
  // Try matching a pattern containing just one letter.
  {
    // Parse a simple pattern.
    char *pstr = "b";
    Pattern *pat = parsePattern( pstr );

    char *str = "abc";

    // Find matches for this pattern.
    pat->locate( pat, str );

    reportMatches( pat, pstr, str );

    pat->destroy( pat );
  }

  // Try a pattern with some concatenation and multiple matches.
  {
    // Parse a simple pattern.
    char *pstr = "aba";
    Pattern *pat = parsePattern( pstr );

    char *str = "ababababababab";

    // Find matches for this pattern.
    pat->locate( pat, str );

    reportMatches( pat, pstr, str );

    pat->destroy( pat );
  }

  return EXIT_SUCCESS;
}
