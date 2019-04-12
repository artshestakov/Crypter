#include "LCPixel.h"
//-----------------------------------------------------------------------------
LCPixel::LCPixel(const unsigned char &red, const unsigned char &green, const unsigned char &blue, const unsigned char &alpha) :
	Red(red),
	Green(green),
	Blue(blue),
	Alpha(alpha)
{

}
//-----------------------------------------------------------------------------
LCPixel::LCPixel() :
	Red(0),
	Green(0),
	Blue(0),
	Alpha(0)
{

}
//-----------------------------------------------------------------------------
LCPixel::~LCPixel()
{
	Red = 0;
	Green = 0;
	Blue = 0;
	Alpha = 0;
}
//-----------------------------------------------------------------------------
