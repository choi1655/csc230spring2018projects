/**
  * @file parse.c
  * @author John Choi mchoi
  * @author NCSU CSC230 TS
  *
  * The parse component parses the text of a regular expression
  * and turns it into a collection of Pattern objects that
  * represent it.
  */

#include "parse.h"
#include "pattern.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
  * Return true if the given character is ordinary, if it should just
  * match occurrences of itself.  This returns false for metacharacters
  * like '*' that control how patterns are matched.
  *
  * @param c Character that should be evaluated as ordinary or special.
  * @return True if c is not special.
  */
static bool ordinary(char c)
{
  // See if c is on our list of special characters.
  if ( strchr( ".^$*?+|()[{", c ) )
    return false;
  return true;
}

/**
  * Print the appropriate error message for an invalid pattern and exit
  * unsuccessfully.
  */
static void invalidPattern()
{
  fprintf( stderr, "Invalid pattern\n" );
  exit( EXIT_FAILURE );
}

/**
  * Parse regular expression syntax with the highest precedence,
  * individual, ordinary symbols, start and end anchors, character
  * classes and patterns surrounded by parentheses.
  *
  * @param str The string being parsed.
  * @param pos A pass-by-reference value for the location in str being parsed,
  * increased as characters from str are parsed.
  * @return a dynamically allocated representation of the pattern for the next
  * portion of str.
  */
static Pattern *parseAtomicPattern( char const *str, int *pos )
{
  if ( ordinary( str[ *pos ] ) ) {
    return makeSymbolPattern( str[ (*pos)++ ] );
  }

  // ...
  if (str[*pos] == '.' || str[*pos] == '^' || str[*pos] == '$') {
    return makeSymbolPattern(str[(*pos)++]);
  } else if (str[*pos] == '[') {
    char *stri = (char *)malloc(100 * sizeof(char));
    while (str[(*pos)++] != ']') {
      if (*pos == strlen(str)) {
        free(stri);
        invalidPattern();
      }
      //strcat(stri, &str[(*pos)]);
    }
    char string[strlen(str)];
    strcpy(string, stri);
    free(stri);
    return makeBracketPattern(string);
  }
  invalidPattern();
  return NULL; // Just to make the compiler happy.
}

/**
  * Parse regular expression syntax with the second-highest precedence,
  * a pattern, p, optionally followed by one or more repetition syntax like '*' or '+'.
  * If there's no repetition syntax, it just returns the pattern object for p.
  *
  * @param str The string being parsed.
  * @param pos A pass-by-reference value for the location in str being parsed,
  * increased as characters from str are parsed.
  * @return a dynamically allocated representation of the pattern for the next
  * portion of str.
  */
static Pattern *parseRepetition( char const *str, int *pos )
{
  Pattern *p = parseAtomicPattern( str, pos );
  if (str[*pos] == '*') {
    p = makeRepetitionPattern(p, '*');
    (*pos)++;
  } else if (str[*pos] == '+') {
    p = makeRepetitionPattern(p, '+');
    (*pos)++;
  } else if (str[*pos] == '?') {
    p = makeRepetitionPattern(p, '?');
    (*pos)++;
  }
  return p;
}

/**
  * Parse regular expression syntax with the third-highest precedence,
  * one pattern, p, (optionally) followed by additional patterns
  * (concatenation).  If there are no additional patterns, it just
  * returns the pattern object for p.
  *
  * @param str The string being parsed.
  * @param pos A pass-by-reference value for the location in str being parsed,
  * increased as characters from str are parsed.
  * @return a dynamically allocated representation of the pattern for the next
  * portion of str.
  */
static Pattern *parseConcatenation( char const *str, int *pos )
{
  // Parse the first pattern
  Pattern *p1 = parseRepetition( str, pos );
  // While there are additional patterns, parse them
  while ( str[ *pos ] && str[ *pos ] != '|' && str[ *pos ] != ')' ) {
    Pattern *p2 = parseRepetition( str, pos );
    // And build a concatenation pattern to match the sequence.
    p1 = makeConcatenationPattern( p1, p2 );
  }

  return p1;
}

/**
  * Parse regular expression syntax with the lowest precedence, one
  * pattern, p, (optionally) followed by additional patterns separated
  * by | (alternation).  If there are no additional patterns, it just
  * returns the pattern object for p.
  *
  * @param str The string being parsed.
  * @param pos A pass-by-reference value for the location in str being
  *            parsed, increased as characters from str are parsed.
  * @return a dynamically allocated representation of the pattern for
  * the next portion of str.
  */
static Pattern *parseAlternation( char const *str, int *pos )
{
  Pattern *p1 = parseConcatenation( str, pos );

  // ...
  // While there are additional patterns, parse them
  while ( str[ *pos ] && str[ *pos ] != ')' ) {
    if (str[*pos] == '|') {
      (*pos)++;
    }
    Pattern *p2 = parseConcatenation( str, pos );
    // And build a concatenation pattern to match the sequence.
    p1 = makeAlternationPattern( p1, p2 );
  }
  return p1;
}

// Documented in the header
Pattern *parsePattern( char const *str )
{
  // Parse the argument into a tree of pattern objects.
  int pos = 0;
  Pattern *pat = parseAlternation( str, &pos );

  // Complain if this didn't consume the whole pattern.
  if ( str[ pos ] ) {
    invalidPattern();
  }
  return pat;
}
