#include <MATH.H>
#include "typedef.h"       
#include "global.h"
#include "utili.h"
#include "key.h"
extern uchar key;         
extern u8 lcd_buffer;    
/**************************������********************************/
void Key_Init()
{
	key = KEY_INVALID;
}

