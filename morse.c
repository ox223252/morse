#include <stdlib.h>
#include <stdio.h>

#include "morse.h"

// https://fr.wikipedia.org/wiki/Code_Morse_international
static char *_morse_code[] = {
	".-", // A
	"-...", // B
	"-.-.", // C
	"-..", // D
	".", // E
	"..-.", // F
	"--.", // G
	"....", // H
	"..", // I
	".---", // J
	"-.-", // K
	".-..", // L
	"--", // M
	"-.", // N
	"---", // O
	".--.", // P
	"--.-", // Q
	".-.", // R
	"...", // S
	"-", // T
	"..-", // U
	"...-", // V
	".--", // W
	"-..-", // X
	"-.--", // Y
	"--..", // Z
	"-----", // 0
	".----", // 1
	"..---", // 2
	"...--", // 3
	"....-", // 4
	".....", // 5
	"-....", // 6
	"--...", // 7
	"---..", // 8
	"----.", // 9
	NULL
};
static uint32_t _morse_baseChar[ 26 + 10 + 1 ];

static void initMorseTable ( void )
{
	for ( uint8_t i = 0; _morse_code[ i ]; i++ )
	{
		uint8_t bitId = 0;
		for ( uint8_t j = 0; _morse_code[ i ][ j ]; j++ )
		{
			if ( _morse_code[ i ][ j ] == '-' )
			{
				_morse_baseChar[ i ] |= ( 0x7 << bitId );
				bitId += 4;
			}
			else
			{
				_morse_baseChar[ i ] |= ( 0x01 << bitId );
				bitId += 2;
			}
		}
	}
}

static char *_morse_prt = NULL;

uint32_t helpMsg ( void )
{ // ...---...
	return ( 0x55ddd5 );
}

uint32_t requestContact ( void )
{ // -.-
	return ( 0x1d7 );
}

uint32_t startMsg ( void )
{ // .-.-
	return ( 0x75d );
}

uint32_t errorMsg ( void )
{ // ........
	return ( 0x5555 );
}

uint32_t unreadableMsg ( void )
{ // .-...-.
	return ( 0x1755d );
}

uint32_t lightUp ( void )
{ // .-...-..
	return ( 0x5755d );
}

uint32_t lightDown ( void )
{ // .--..--.
	return ( 0x5dd5dd );
}

uint32_t slowDown ( void )
{ // ....-..
	return ( 0x5755 );
}

uint32_t newLineMsg ( void )
{ // -...-
	return ( 0x1d57 );
}

uint32_t endMsgWithResponse ( void )
{ // .-.-.
	return ( 0x175d );
}

uint32_t repeatMsg ( void )
{ // ..--..
	return ( 0x5775 );
}

uint32_t understoodMsg ( void )
{ // ...-.
	return ( 0x5d5 );
}

uint32_t waitMsg ( void )
{ // .-...
	return ( 0x55d );
}

uint32_t endMsgNoResponse ( void )
{ // ...-.-
	return ( 0x75d5 );
}


uint32_t convertNextChar ( char * string )
{
	if ( string )
	{
		_morse_prt = string;
	}

	if ( !_morse_prt ||
		!*_morse_prt )
	{
		// return stop : -...-
		return ( 0 );
	}

	if ( ( *_morse_prt >= '0' ) &&
		( *_morse_prt <= '9' ) )
	{
		uint32_t byte = _morse_baseChar[ 26 + (*_morse_prt - '0') ];
		_morse_prt++;
		return ( byte );
	}

	if ( ( *_morse_prt >= 'a' ) &&
		( *_morse_prt <= 'z' ) )
	{
		uint32_t byte = _morse_baseChar[ *_morse_prt - 'a' ];
		_morse_prt++;
		return ( byte );
	}

	if ( ( *_morse_prt >= 'A' ) &&
		( *_morse_prt <= 'Z' ) )
	{
		uint32_t byte = _morse_baseChar[ *_morse_prt - 'A' ];
		_morse_prt++;
		return ( byte );
	}

	switch ( *_morse_prt++ )
	{
		case '\n':
		{
			return ( newLineMsg ( ) );
		}
		case ' ':
		{
			return ( 0xffffffff );
		}
		case '.': // .-.-.-
		{
			return ( 0x1d75d );
		}
		case ',': // --..--
		{
			return ( 0x77577 );
		}
		default:
		case '?': // ..--..
		{
			return ( 0x5775 );
		}
		case '\'': // .----.
		{
			return ( 0x5dddd );
		}
		case '!': // -.-.--
		{
			return ( 0x775d7 );
		}
		case '/': // -..-.
		{
			return ( 0x1757 );
		}
		case '(': // -.--.
		{
			return ( 0x5dd7 );
		}
		case ')': // -.--.-
		{
			return ( 0x75dd7 );
		}
		case '&': // .-...
		{
			return ( 0x55d );
		}
		case ':': // ---...
		{
			return ( 0x15777 );
		}
		case ';': // -.-.-.
		{
			return ( 0x175d7 );
		}
		case '=': // -...-
		{
			return ( 0x1d57 );
		}
		case '+': // .-.-.
		{
			return ( 0x175d );
		}
		case '-': // -....-
		{
			return ( 0x7557 );
		}
		case '_': // ..--.-
		{
			return ( 0x1d775 );
		}
		case '"': // .-..-.
		{
			return ( 0x5d5d );
		}
		case '$': // ...-..-
		{
			return ( 0x1d5d5 );
		}
		case '@': // .--.-.
		{
			return ( 0x175dd );
		}
	}
}

#if 0

#include <unistd.h>

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

#endif