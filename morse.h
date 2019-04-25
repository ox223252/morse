#ifndef __MORSE_H__
#define __MORSE_H__

// time unit 1u = 1/4s env
// each bits set to one need a HIGH level transmission
// each bit unset is a sleep time
// time between two letters = 3u
// time between tow words = 7u

#include <stdint.h>

uint32_t helpMsg ( void );
uint32_t requestContact ( void );
uint32_t startMsg ( void );
uint32_t errorMsg ( void );
uint32_t unreadableMsg ( void );
uint32_t lightUp ( void );
uint32_t lightDown ( void );
uint32_t slowDown ( void );
uint32_t newLineMsg ( void );
uint32_t endMsgWithResponse ( void );
uint32_t repeatMsg ( void );
uint32_t understoodMsg ( void );
uint32_t waitMsg ( void );
uint32_t endMsgNoResponse ( void );
uint32_t convertNextChar ( char * string );

#endif