#pragma once
#include "CharFont.h"

typedef struct MatchCharRet 
{
	char code;       // 匹配到的字符
	int  x,y;        // 位置

	int  bb;         // 模板黑 图像黑
	int  bw;         // 模板黑 图像白
	int  wb;         // 模板白 图像黑
	int  total;      // 匹配点数目
	int  match;      // 一致的点数
};

int DecodeImage(IplImage* image,CharSet& charSet,char codes[4],float *confidence = NULL);