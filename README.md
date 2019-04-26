# morse
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

## polling mode:
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

## STM32 interrupt Mode:
```C
static char * _morse_msg = NULL;
static uint8_t _morse_loopMsg = 0;

void setErrorMsg ( char * byte )
{
	_morse_msg = byte;
}

void TIM8_TRG_COM_TIM14_IRQHandler ( void )
{
	static uint32_t symbol = 0;
	static uint32_t timeout = X; // timeout is the min time allowed between two differents msg

	if ( timeout < X )
	{ // if timeout is smaller than threshold we quit
		timeout++;
		break;
	}

	if ( !_morse_msg )
	{ // if no msg available
		break;
	}

	if ( symbol == 0x01000000 )
	{ // if we reach this value that signify we just display a sapce
		// a space is deffined as 7 time unit with no signals
		// 0x80000000 >> 7 = 0x01000000
		// once space displayed get new char
		symbol = convertNextChar ( NULL );
	}
	else if ( !symbol )
	{ // if symbol is not set, we get the next byte convertion
		symbol = convertNextChar ( NULL );

		if ( symbol == 0xffffffff )
		{ // it's a space
			symbol = 0x80000000;
		}
		else if ( symbol )
		{ // if we get a new symbol (from the same string) and it's not a space
			// then add timming between lettes
			symbol <<= 3;
		}
		else
		{ // if symbol is null again, convert function is endded
			// reset timeout to 0 + 1 ( to take into account this loop )
			timeout = 1;
			if ( !_morse_loopMsg )
			{ // if loop mode not activated
				_morse_msg = NULL;
			}
			else
			{ // if loop mode activated
				symbol = convertNextChar ( _morse_msg );
				symbol <<= 1;
			}
		}
	}

	HAL_GPIO_WritePin ( BUZZER_GPIO_Port, BUZZER_Pin, (symbol & 0x01)?GPIO_PIN_SET:GPIO_PIN_RESET );
	symbol >>= 1;
}
```
