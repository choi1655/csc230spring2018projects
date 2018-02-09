#include <stdio.h>
#include <stdlib.h>

#define SEVENTY 72

void printWord( int x )
{
    for ( int i = 0; i < x; i++ )
    {
            // Print a random lower-case letter.
            printf( "%c", 97 + rand() % 26 );
    }
}

int printLine() {
	int count = 0, pos = 0, space = 0;
	int len = 1 + rand() % 10;
	// Print a line of words up to a limited length.
	while ( pos + len + space < SEVENTY ) {
		if ( space > 0 ) {
			printf( " " );
                }
		printWord( len );
		pos += len + space;
		len = 1 + rand() % 10;
		space = 1;
		count += 1;
	}
	printf( "\n" );
	return count;
}

int printParagraph( int n )
{
int total = 0;
for ( int i = 0; i < n; i++ )
total += printLine();
return total;
}

int main(){int w=printParagraph(10);printf("Words: %d\n",w);return 0;}
