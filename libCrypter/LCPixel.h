#pragma once
//-----------------------------------------------------------------------------
//Структура описывающая графический пиксель.
struct LCPixel
{
	LCPixel(const unsigned char &red, const unsigned char &green, const unsigned char &blue, const unsigned char &alpha);
	LCPixel();
	~LCPixel();

	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
	unsigned char Alpha;
};
//-----------------------------------------------------------------------------
