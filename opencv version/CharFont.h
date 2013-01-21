#pragma once
#include <tzk/opencv.h>
#include <vector>

using namespace std;

typedef struct CharFont
{
	char      code;
	IplImage* image;
}CharFont;

typedef std::vector<CharFont> CharSet;

int LoadCharSet(const char* fontFileName,const char* charArray,CharSet& charSet);
int FreeCharSet(CharSet& charSet);
