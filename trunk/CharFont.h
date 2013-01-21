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
#ifndef __CHAR_FONT_H__
#define __CHAR_FONT_H__

#include "Image.h"

typedef struct CharFont
{
	char         code;
	SimpleImage* image;
}CharFont;

template <typename T>
class vector
{
public:
	T   *_data;
	int  _size;

	vector()
	{
		_size = 0;
		_data = 0;
	}

	vector(int size)
	{
		resize(size);
	}

	~vector()
	{
		if (_data)
		{
			delete [] _data;
		}
	}

	T& operator[](int i) 
	{
		return _data[i];
	}

	int size()
	{
		return _size;
	}

	void resize(int size)
	{
		if (size != _size && !_data)
		{
			delete [] _data;
			_data = 0;
		}

		_size = size;

		if (size > 0)
		{
			_data = new T[size];
		}
	}

	void clear()
	{
		resize(0);
	}

};

typedef std::vector<CharFont> CharSet;

int LoadCharSet(SimpleImage* largeImage,const char* charArray,CharSet& charSet);
int FreeCharSet(CharSet& charSet);

#endif /* __CHAR_FONT_H__ */