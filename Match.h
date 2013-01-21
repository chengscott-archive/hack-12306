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
#ifndef __PWNTCHA_MATCHER_H__
#define __PWNTCHA_MATCHER_H__

#include "Image.h"

int MatcherInit(const char* fontFileName,const char* charArray);
int MatcherInitImage(SimpleImage* image,const char* charArray);
int MatcherDecode(SimpleImage* image,char code[4]);

#endif /* __PWNTCHA_MATCHER_H__ */