#pragma once
#include "CharFont.h"

typedef struct MatchCharRet 
{
	char code;       // ƥ�䵽���ַ�
	int  x,y;        // λ��

	int  bb;         // ģ��� ͼ���
	int  bw;         // ģ��� ͼ���
	int  wb;         // ģ��� ͼ���
	int  total;      // ƥ�����Ŀ
	int  match;      // һ�µĵ���
};

int DecodeImage(IplImage* image,CharSet& charSet,char codes[4],float *confidence = NULL);