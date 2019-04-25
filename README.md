# morce
This simple repo was created to convert string into morse code.

Each byte will be converted into uint32_t, where each bit represents a base time unit, so if a bit is set you should emit a signal if the bit is reset you shouldn't.

Don't forget to add timing between symbols and between words.

for exemple :
```
send ( "bonjour", 0 );
```

```
# ### # ###       ### # # #   ### ### ###   ### #   # ### ### ###   ### ### ###   # # ###   # ### #       # # # ### # ###
start             b           o             n       j               o             u         r             fin, sans reponse
```

```C
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/morse/morse.h"

#define set() putchar('#')
#define reset() putchar(' ')

const uint32_t time=150000;

void one ( uint32_t s )
{
	while ( s )
	{
		(s & 0x01)?set():reset();
		fflush ( NULL );
		s >>= 1;
		usleep( time );
	}
}

void send ( char * msg, uint8_t response )
{
	uint32_t symbol = 0;
	one ( startMsg ( ) );
	
	usleep ( time * 7 );
	printf ( "       " );
	
	one ( convertNextChar ( msg ) );

	while ( symbol = convertNextChar ( NULL ), symbol )
	{
		if ( symbol == 0xffffffff )
		{ // space between word is 7  blanc
			usleep ( time * 7 );
			printf ( "       " );
		}
		else
		{ // space between letters in a word is 3  blanc
			usleep ( time * 3 );
			printf ( "   " );
			one ( symbol );
		}
	}

	usleep ( time * 7 );
	printf ( "       " );

	if ( response )
	{
		one ( endMsgWithResponse ( ) );
	}
	else
	{
		one ( endMsgNoResponse ( ) );
	}
}

int main ( void )
{
	send ( "bonjour", 0 );

	return ( 0 );
}

```