/**
  * @file pattern.c
  * @author John Choi mchoi
  * @author NCSU CSC230 TS
  *
  * The pattern component implements the inheritance hierarchy used
  * for regular expressions. It defines the abstract superclass for a Pattern
  * as well as different concrete subclasses for represent various parts of
  * regular expression syntax.
  * The starter includes a partial implementaton for the pattern component.
  * It defines the   pattern superclass, along with a few functions that serve
  * as methods for the class. It also includes concrete implementations of pattern
  * for matching individual symbols (SymbolPattern) and for matching concatenated
  * patterns (BinaryPattern). As you add support for different regular expression
  * syntax, you'll create new struct and function definitions in the pattern
  * in the header.
  */

#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
  * Free the table inside a pattern, if there is one.
  *
  * @param this The pattern we're supposed to operate on.
  */
static void freeTable( Pattern *this )
{
  if ( this->table ) {
    for ( int r = 0; r <= this->len; r++ )
    free( this->table[ r ] );
    free( this->table );
  }
}

/**
  * Make and initialize a new match table inside the given pattern,
  *  large enough to store matches for the given string.
  *
  * @param this The pattern we're supposed to operate on.
  * @param str The string we're going to store mageches for
  */
static void initTable( Pattern *this, char const *str )
{
  // If we already had a table, free it.
  freeTable( this );

  // Make a table big enough for str.
  this->len = strlen( str );
  this->table = (bool **) malloc( ( this->len + 1 ) * sizeof( bool * ) );
  for ( int r = 0; r <= this->len; r++ )
  this->table[ r ] = (bool *) calloc( ( this->len + 1 ), sizeof( bool ) );
}

// Documented in the header.
bool matches( Pattern *pat, int begin, int end )
{
  return pat->table[ begin ][ end ];
}

/**
  * A simple function that can be used to free the memory for any
  * pattern that doesn't allocate any additional memory other than the
  * struct used to represent it (e.g., if it doesn't contain any
  * sub-patterns).  It's like a default implementation of the destroy
  * method.
  *
  * @param pat The pattern to free memory for.
  */
static void destroySimplePattern( Pattern *pat )
{
  freeTable( pat );
  // If we don't need fields that are specific to the sub-type, we can just
  // free the block of memory where the object is stored.
  free( pat );
}

/**
  * Type of pattern used to represent a single, ordinary symbol,
  * like 'a' or '5'.
  */
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );

  /** Symbol this pattern is supposed to match. */
  char sym;
} SymbolPattern;

/**
  * Overridden locate() method for a SymbolPattern.
  * Locates the pattern using SymbolPattern and regex passed in.
  *
  * @param pat Pattern to use
  * @param str regex to match
  */
static void locateSymbolPattern( Pattern *pat, char const *str )
{
  // Cast down to the struct type pat really points to.
  SymbolPattern *this = (SymbolPattern *) pat;

  // Make a fresh table for this input string.
  initTable( pat, str );
  // Find all occurreces of the symbol we're supposed to match, and
  // mark them in the match table as matching, 1-character substrings.
  for ( int begin = 0; str[ begin ]; begin++ ) {
    if ( str[ begin ] == this->sym || this->sym == '.') {
      this->table[ begin ][ begin + 1 ] = true;
    } else if (this->sym == '^' && begin == 0) {
      this->table[begin][begin] = true;
    } else if (this->sym == '$' && begin == (strlen(str) - 1)) {
      this->table[this->len][this->len] = true;
    }
  }
}

// Documented in the header.
Pattern *makeSymbolPattern( char sym )
{
  // Make an instance of SymbolPattern, and fill in its state.
  SymbolPattern *this = (SymbolPattern *) malloc( sizeof( SymbolPattern ) );
  this->table = NULL;

  this->locate = locateSymbolPattern;
  this->destroy = destroySimplePattern;
  this->sym = sym;
  //SymbolPattern *thisCopy;
  //memcpy(thisCopy, this, sizeof(this));

  //free(this);

  return (Pattern *) this;
}

/**
  * Representation for a type of pattern that contains two sub-patterns
  * (e.g., concatenation).  This representation could be used by more
  * than one type of pattern, as long as it uses a pointer to a
  * different locate() function.
  */
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );

  // Pointers to the two sub-patterns.
  Pattern *p1, *p2;
} BinaryPattern;

/**
  * Type of repetition pattern.
  */
typedef struct {
  int len;
  bool **table;
  void (*locate)(Pattern *pat, char const *str);
  void (*destroy)(Pattern *pat);

  Pattern *p;
  char pattern;
} RepetitionPattern;

/**
  * Type of Bracket pattern.
  */
typedef struct {
  int len;
  bool **table;
  void (*locate)(Pattern *pat, char const *str);
  void (*destroy)(Pattern *pat);

  char *strBetween;
} BracketPattern;

// destroy function used for BinaryPattern
/**
  * Destroy function used for BinaryPattern.
  *
  * @param pat pattern to destroy
  */
static void destroyBinaryPattern( Pattern *pat )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Free our table.
  freeTable( pat );
  // Free our two sub-patterns.
  this->p1->destroy( this->p1 );
  this->p2->destroy( this->p2 );
  // Free the struct representing this object.
  free( this );
}

// locate function for a BinaryPattern used to handle concatenation.
/**
  * Locate function for a BinaryPattern used to handle concatenation.
  *
  * @param pat pattern to locate
  * @param str regex to match
  */
static void locateConcatenationPattern( Pattern *pat, const char *str )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  initTable( pat, str );

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->locate( this->p1, str );
  this->p2->locate( this->p2, str );

  // Then, based on their matches, look for all places where their
  // concatenaton matches.  Check all substrings of the input string.
  for ( int begin = 0; begin <= this->len; begin++ ) {
    for ( int end = begin; end <= this->len; end++ ) {

      // For the [ begin, end ) range, check all places where it could
      // be split into two substrings, the first matching p1 and the second
      // matching p2.
      for ( int k = begin; k <= end; k++ ) {
        if ( matches( this->p1, begin, k ) && matches( this->p2, k, end ) ) {
          this->table[ begin ][ end ] = true;
        }
      }
    }
  }
}

// Documented in header.
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 )
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc( sizeof( BinaryPattern ) );
  this->table = NULL;
  this->p1 = p1;
  this->p2 = p2;

  this->locate = locateConcatenationPattern;
  this->destroy = destroyBinaryPattern;

  return (Pattern *) this;
}

/**
  * Locates AlternatePattern using pattern passed in and regex.
  *
  * @param pat pattern to use
  * @param str regex to match
  */
static void locateAlternatePattern(Pattern *pat, const char *str)
{
  BinaryPattern *this = (BinaryPattern *) pat;

  initTable( pat, str );

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->locate( this->p1, str );
  this->p2->locate( this->p2, str );

  // Then, based on their matches, look for all places where their
  // concatenaton matches.  Check all substrings of the input string.
  for ( int begin = 0; begin <= this->len; begin++ ) {
    for ( int end = begin; end <= this->len; end++ ) {

      // For the [ begin, end ) range, check all places where it could
      // be split into two substrings, the first matching p1 and the second
      // matching p2.
      if ( matches( this->p1, begin, end ) || matches( this->p2, begin, end ) )
      this->table[ begin ][ end ] = true;
    }
  }
}

// Documented in header
Pattern *makeAlternationPattern(Pattern *p1, Pattern *p2)
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc( sizeof( BinaryPattern ) );
  this->table = NULL;
  this->p1 = p1;
  this->p2 = p2;

  this->locate = locateAlternatePattern;
  this->destroy = destroyBinaryPattern;

  return (Pattern *) this;
}

/**
  * Locates RepetitionPattern using pattern and regex passed in.
  *
  * @param pat pattern to use
  * @param str regex to match
  */
static void locateRepetitionPattern(Pattern *pat, const char *str)
{
  RepetitionPattern *this = (RepetitionPattern *) pat;
  initTable(pat, str); //initializes struct (len, etc)
  this->p->locate(this->p, str);
  char pattern = this->pattern; //* ? +
  int counter = 0;
  for (int begin = 0; begin <= this->len; begin++) {
    for (int end = begin; end <= this->len; end++) {
      if (matches(this->p, begin, end)) {
        if (pattern == '*') {
          for (int a = 0; a < strlen(str); a++) { //sets the diagonal to true
            this->table[a][a] = true;
          }
          for (int k = counter; k >= 0 && begin > 0; k--) {
            this->table[begin - k][end] = true;
          }
          counter++;
        } else if (pattern == '+') {
          for (int a = 0; a < strlen(str); a++) { //sets the diagonal to true
            this->table[a][a] = true;
          }
          for (int k = counter; k >= 0 && begin > 0; k--) {
            this->table[begin - k][end] = true;
          }
          counter++;
        } else { // pattern = ?
          for (int a = 0; a < strlen(str); a++) { //sets the diagonal to true
            this->table[a][a] = true;
          }
          if (begin + 1 <= this->len && end + 1 <= this->len) {
            if (!this->p->table[begin + 1][end + 1]) {
              this->table[begin][end] = true;
            }
          }
        }
      }
      if (pattern == '*' || pattern == '?') {
        for (int a = 0; a < strlen(str); a++) { //sets the diagonal to true
          this->table[a][a] = true;
        }
      }
    }
  }

  //  printf("\n");
  // // printf("symbol %c\n", this->str);  // printf("PLUS\n"); for + pattern function, or similar for other functions.
  // int len = strlen( str );
  // for( int i = 0; i <= len; i++ ) {
  //   for( int j = 0; j <= len; j++ )
  //     printf( "%d", this->table[i][j] );
  //   printf("\n");
  // }
}

/**
  * Destroys the existing RepetitionPattern.
  *
  * @param pat pattern to find
  */
static void destroyRepetitionPattern(Pattern *pat)
{
  RepetitionPattern *this = (RepetitionPattern *) pat;
  freeTable(pat);
  this->p->destroy(this->p);
  free(this);
}

/**
  * Destroys the existing BracketPattern.
  *
  * @param pat pattern to find
  */
static void destroyBracketPattern(Pattern *pat)
{
  BracketPattern *this = (BracketPattern *) pat;
  freeTable(pat);
  free(this->strBetween);
  free(this);
}

// Documented in the header
Pattern *makeRepetitionPattern(Pattern *p, char pattern)
{
  RepetitionPattern *this = (RepetitionPattern *)malloc(sizeof(RepetitionPattern));
  this->table = NULL;
  this->p = p;
  this->pattern = pattern;

  this->locate = locateRepetitionPattern;
  this->destroy = destroyRepetitionPattern;
  return (Pattern *) this;
}

/**
  * Locates BracketPattern using pattern and regex passed in.
  *
  * @param p pattern to use
  * @param str regex to match
  */
static void locateBracketPattern(Pattern *p, const char *str)
{
  BracketPattern *this = (BracketPattern *) p;
  initTable(p, str);
  // this->p->locate(this->p, str);
  for (int begin = 0; begin < strlen(str); begin++) {
    for (int end = 0; end < strlen(this->strBetween); end++) {
      if (str[begin] == this->strBetween[end]) {
        this->table[begin][begin + 1] = true;
      }
    }
  }
}

// Documented in the header
Pattern *makeBracketPattern(char *str)
{
  BracketPattern *this = (BracketPattern *)malloc(sizeof(BracketPattern));
  this->table = NULL;
  this->strBetween = str;

  this->locate = locateBracketPattern;
  this->destroy = destroyBracketPattern;
  return (Pattern *) this;
}
