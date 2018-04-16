/**
  * @file parse.h
  * @author John Choi mchoi
  *
  * The parse component parses the text of a regular expression
  * and turns it into a collection of Pattern objects that
  * represent it.
  */

#ifndef PARSE_H
#define PARSE_H

#include "pattern.h"

/**
  * Parse the given string into Pattern object.
  *  
  * @param str string cntaining a pattern.
  * @return pointer to a representation of the pattern.
  */
Pattern *parsePattern( char const *str );

#endif
