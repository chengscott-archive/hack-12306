/*
< 12306.cn 验证码识别程序 .>
Copyright (C) <2013>  <zekang.tian@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CharFont.h"
#include <string.h>

int LoadCharSet(SimpleImage* largeImage,const char* charArray,CharSet& charSet)
{
	if (!largeImage || !charArray)
	{
		return 0;
	}

	int charNum = strlen(charArray);
	int width = largeImage->width/charNum;
	int height = largeImage->height;

	charSet.clear();
	charSet.resize(charNum);

	for (int i = 0;i < charNum;i++)
	{
		SimpleImage* image = siCreateImage(width,height,1);
		siCopyImageROI(largeImage,i*width,0,width,height,image);
		CharFont font;
		font.code = charArray[i];
		font.image = image;
		charSet[i] = font;
	}

	return charSet.size();
}

int FreeCharSet(CharSet& charSet)
{
	for (int i = 0;i < charSet.size();i++)
	{
		siReleaseImage(&charSet[i].image);
	}

	return charSet.size();
}