/*
< 12306.cn ��֤��ʶ����� .>
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
#include <stdio.h>
#include "Match.h"
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
}MatchCharRet;

typedef struct SiRect
{
	int x;
	int y;
	int width;
	int height;
}SiRect;

int DecodeImage(SimpleImage* image,CharSet& charSet,char codes[4],float *confidence = 0);

#define DEBUG_STEP 0

MatchCharRet MatchFontImage(SimpleImage* image,int offX,int offY,
							SimpleImage* fontImage,int sx,int sy,int ex,int ey)
{
	MatchCharRet result = {0};
	for (int y = sy;y < ey;y++)
	{
		uchar* src = (uchar*)(image->imageData + (y - sy + offY)*image->widthStep + offX);
		uchar* dst = (uchar*)(fontImage->imageData + y*fontImage->widthStep + sx);
		for (int x = sx;x < ex;x++)
		{
			// ģ��=��ɫ
			if (*dst == 0)
			{
				if(*src == 0)
				{
					result.bb++;
					result.match++;
				}
				else
				{
					result.bw++;
					*src = 128;
				}
			}
			// ģ��=��ɫ
			else
			{
				if (*src == 0)
				{
					result.wb++;
					*src = 128;
				}
				else
				{
					result.match++;
				}
			}

			result.total++;

			src++;
			dst++;
		}
	}

	return result;
}

MatchCharRet UpdateBestMatchInChar(MatchCharRet best,MatchCharRet result)
{
	//if (result.bb - result.bw > best.bb - best.bw)
	if (result.bb > best.bb )
	//if (result.match*100/result.total > best.match*100/max(1,best.total))
	//if (result.bb > 16 && result.bb/(result.bb+result.bw) > best.bb/(max(1,best.bb+best.bw)))
	{
		best = result;
	}

	return best;
}

MatchCharRet UpdateBestMatchBetweenChar(MatchCharRet best,MatchCharRet result)
{
	//if (result.bb - result.bw > best.bb - best.bw)
	//if (result.bb > best.bb || 
	//	(result.bb == best.bb && result.bw < best.bw))
	//if (result.match*100/result.total > best.match*100/max(1,best.total))
	//if (result.bb > 16 && result.bb/(result.bb+result.bw) > best.bb/(max(1,best.bb+best.bw)))
	if(result.bb - result.bw > best.bb - best.bw)
	{
		best = result;
	}

	return best;
}


MatchCharRet MatchChar2(SimpleImage* image,SiRect roi,CharSet& charSet,int codeId)
{
	MatchCharRet bestMatch = {'?'};
	int XMargin = 4;
	int YMargin = 5;
	int XExtend = 0;
	const int SIZE = 20;

	SimpleImage* imgWork = siCloneImage(image);
	if (codeId == 3)
	{
		XExtend = 5;
	}

	// �������е�����ͼ��
	for (int i = 0;i < charSet.size();i++)
	{
		SimpleImage* fontImage = charSet[i].image;
		MatchCharRet localeBest = {0};

		// �������е�λ��
		for (int y = -YMargin;y < roi.height+YMargin-SIZE;y++)
		{
			for (int x = -XMargin; x < roi.width+XMargin+XExtend - SIZE;x++)
			{
				// �ڵ�ǰλ��ƥ�����е��ַ�
				int x0,y0,x1,y1; 

				// ��ģ�����ͼ������ϵ��
				x0 = x + roi.x;
				y0 = y + roi.y;
				x1 = x0+SIZE;
				y1 = y0+SIZE;

				// ����ģ����ͼ���ڵķ�Χ
				if (x0 < roi.x)
				{
					x0 = roi.x;
				}
				else if (x0 > roi.x + roi.width)
				{
					x0 = roi.x+roi.width;
				}

				if (y0 < roi.y)
				{
					y0 = roi.y;
				}
				else if (y0 > roi.y + roi.height)
				{
					y0 = roi.y+roi.height;
				}

				if (x1 > roi.x + roi.width)
				{
					x1 = roi.x+roi.width;
				}

				if (y1 > roi.y + roi.height)
				{
					y1 = roi.y+roi.height;
				}

				// ����ͼ�����Ч��Χ
				int sx = x0 - roi.x - x;
				int sy = y0 - roi.y - y;
				int ex = x1 - roi.x - x;
				int ey = y1 - roi.y - y;


				// ƥ������
				MatchCharRet result;
				siCopyImage(image,imgWork);

				result = MatchFontImage(imgWork,x0,y0,fontImage,sx,sy,ex,ey);
				result.code = charSet[i].code;
				result.x = x+roi.x;
				result.y = y+roi.y;  

				// �������ƥ����
				localeBest = UpdateBestMatchInChar(localeBest,result);
#if DEBUG_STEP == 1
				//if (codeId == 1 && (result.code == '7' || result.code == 'M'))
				{
					printf("result bb:%4d localbest bb:%4d\n",result.bb,localeBest.bb);
					siShowImage("font",fontImage);
					siShowImage("match",imgWork);
					cvWaitKey();
				}
#endif
			}
		}

		// ����ȫ������
        bestMatch = UpdateBestMatchBetweenChar(bestMatch,localeBest);
#if DEBUG_STEP == 1
		printf("\nlocalbest bb:%4d best bb:%4d ch:%c\n",localeBest.bb,bestMatch.bb,bestMatch.code);
#endif

	}

	siReleaseImage(&imgWork);
	return bestMatch;
}

int DecodeImage(SimpleImage* image,CharSet& charSet,char codes[4],float *confidence)
{
	SiRect roi[4] = 
	{
		{ 3,0,25,20},
		{15,0,27,20},
		{30,0,25,20},
		{45,0,14,20}
	};

	MatchCharRet result;
	if (confidence)
	{
		*confidence = 0.0;
	}

	for (int i = 0;i < 4;i++)
	{
		result = MatchChar2(image,roi[i],charSet,i);
		if (result.total == 0)
		{
			return 0;
		}

		codes[i] = result.code;
		if (confidence)
		{
			*confidence += ((float)result.match/(float)result.total);
		}
	}

	return 1;
}


MatchCharRet MatchChar(SimpleImage* image,SiRect roi,CharSet& charSet,int codeId)
{
	MatchCharRet bestMatch = {'?'};
	int XMargin = 4;
	int YMargin = 12;
	int XExtend = 0;
	const int SIZE = 20;

	SimpleImage* imgWork = siCloneImage(image);
	if (codeId == 3)
	{
		XExtend = 5;
	}

	// �������е�λ��
	for (int x = -XMargin; x < roi.width+XMargin+XExtend - SIZE;x++)
	{
		for (int y = -YMargin;y < roi.height+YMargin-SIZE;y++)
		{
			// �ڵ�ǰλ��ƥ�����е��ַ�
			int x0,y0,x1,y1; 

			// ��ģ�����ͼ������ϵ��
			x0 = x + roi.x;
			y0 = y + roi.y;
			x1 = x0+SIZE;
			y1 = y0+SIZE;

			// ����ģ����ͼ���ڵķ�Χ
			if (x0 < roi.x)
			{
				x0 = roi.x;
			}
			else if (x0 > roi.x + roi.width)
			{
				x0 = roi.x+roi.width;
			}

			if (y0 < roi.y)
			{
				y0 = roi.y;
			}
			else if (y0 > roi.y + roi.height)
			{
				y0 = roi.y+roi.height;
			}

			if (x1 > roi.x + roi.width)
			{
				x1 = roi.x+roi.width;
			}

			if (y1 > roi.y + roi.height)
			{
				y1 = roi.y+roi.height;
			}

			// ����ͼ�����Ч��Χ
			int sx = x0 - roi.x - x;
			int sy = y0 - roi.y - y;
			int ex = x1 - roi.x - x;
			int ey = y1 - roi.y - y;

			// �������е�����ͼ��
			for (int i = 0;i < charSet.size();i++)
			{
				SimpleImage* fontImage = charSet[i].image;
				// ƥ������
				MatchCharRet result;
				siCopyImage(image,imgWork);

				result = MatchFontImage(imgWork,x0,y0,fontImage,sx,sy,ex,ey);
				result.code = charSet[i].code;
				result.x = x+roi.x;
				result.y = y+roi.y;

				//cvShowImage("font",fontImage);
				//cvShowImage("match",imgWork);
				//cvWaitKey();

				// �������ƥ����
				bestMatch = UpdateBestMatchBetweenChar(bestMatch,result);
			}
		}
	}

	siReleaseImage(&imgWork);
	return bestMatch;
}

class Matcher
{
public:
	Matcher()
	{

	}

	~Matcher()
	{
		for (int i = 0;i < fontArray.size();i++)
		{
			FreeCharSet(fontArray[i]);
		}
		
	}

	int Init(const char* fontData,const char* charArray)
	{
		SimpleImage* largeImage = siLoadImageFromStream(fontData);
		if (!largeImage)
		{
			return 0;
		}

		CharSet charSet;
		int ret = LoadCharSet(largeImage,charArray,charSet);
		siReleaseImage(&largeImage);
		if (ret)
		{
			fontArray.push_back(charSet);
		}

		return ret;
	}

	int Init(SimpleImage* fontImage,const char* charArray)
	{
		if (!fontImage)
		{
			return 0;
		}

		CharSet charSet;
		int ret = LoadCharSet(fontImage,charArray,charSet);
		if (ret)
		{
			fontArray.push_back(charSet);
		}

		return ret;
	}

	int Decode(SimpleImage* image,char codes[4])
	{
		float maxConfidence = -1.0;
		char  maxCodes[4] = {0};

		// �����������壬�õ����Ž��
		for (int i = 0;i < fontArray.size();i++)
		{
			float confidence;
			char  code[4];

			DecodeImage(image,fontArray[i],code,&confidence);
			if (confidence > maxConfidence)
			{
				maxConfidence = confidence;
				memcpy(maxCodes,code,4);
			}
		}

		// ���ؽ��
		memcpy(codes,maxCodes,4);
		return maxConfidence > 2.0;
	}

protected:
	std::vector<CharSet> fontArray;
};

static Matcher thiz;

/************************************************************************/
/*                           C Wrappers                                 */
/************************************************************************/
int MatcherInit(const char* fontData,const char* charArray)
{
	return thiz.Init(fontData,charArray);
}

int MatcherInitImage(SimpleImage* image,const char* charArray)
{
	return thiz.Init(image,charArray);
}

int MatcherDecode(SimpleImage* image,char code[4])
{
	SimpleImage* gray = siCreateImage(image->width,image->height,1);
	if (image->nChannels != 1)
	{
		siRgbToGray(image,gray);
	}
	//siThreshold(gray,gray,150,255);
	siThreshold(gray,gray,115,255);

	int ret = thiz.Decode(gray,code);
	siReleaseImage(&gray);
	return ret;
}
