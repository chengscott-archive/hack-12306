#include "Match.h"

#define DEBUG_STEP 0

MatchCharRet MatchFontImage(IplImage* image,int offX,int offY,
							IplImage* fontImage,int sx,int sy,int ex,int ey)
{
	MatchCharRet result = {0};
	for (int y = sy;y < ey;y++)
	{
		uchar* src = (uchar*)(image->imageData + (y - sy + offY)*image->widthStep + offX);
		uchar* dst = (uchar*)(fontImage->imageData + y*fontImage->widthStep + sx);
		for (int x = sx;x < ex;x++)
		{
			// 模板=黑色
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
			// 模板=白色
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


MatchCharRet MatchChar2(IplImage* image,CvRect roi,CharSet& charSet,int codeId)
{
	MatchCharRet bestMatch = {'?'};
	int XMargin = 4;
	int YMargin = 5;
	int XExtend = 0;
	const int SIZE = 20;

	IplImage* imgWork = cvCloneImage(image);
	if (codeId == 3)
	{
		XExtend = 5;
	}

	// 遍历所有的字体图像
	for (int i = 0;i < charSet.size();i++)
	{
		IplImage* fontImage = charSet[i].image;
		MatchCharRet localeBest = {0};

		// 测试所有的位置
		for (int y = -YMargin;y < roi.height+YMargin-SIZE;y++)
		{
			for (int x = -XMargin; x < roi.width+XMargin+XExtend - SIZE;x++)
			{
				// 在当前位置匹配所有的字符
				int x0,y0,x1,y1; 

				// 把模板放入图像坐标系中
				x0 = x + roi.x;
				y0 = y + roi.y;
				x1 = x0+SIZE;
				y1 = y0+SIZE;

				// 计算模板在图像内的范围
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

				// 字体图像的有效范围
				int sx = x0 - roi.x - x;
				int sy = y0 - roi.y - y;
				int ex = x1 - roi.x - x;
				int ey = y1 - roi.y - y;


				// 匹配字体
				MatchCharRet result;
				cvCopy(image,imgWork);

				result = MatchFontImage(imgWork,x0,y0,fontImage,sx,sy,ex,ey);
				result.code = charSet[i].code;
				result.x = x+roi.x;
				result.y = y+roi.y;  

				// 更新最佳匹配结果
				localeBest = UpdateBestMatchInChar(localeBest,result);
#if DEBUG_STEP == 1
				if (codeId == 1 && (result.code == '7' || result.code == 'M'))
				{
					printf("result bb:%4d localbest bb:%4d\n",result.bb,localeBest.bb);
					cvShowImage("font",fontImage);
					cvShowImage("match",imgWork);
					cvWaitKey();
				}
#endif
			}
		}

		// 更新全局最优
        bestMatch = UpdateBestMatchBetweenChar(bestMatch,localeBest);
#if DEBUG_STEP == 1
		printf("\nlocalbest bb:%4d best bb:%4d ch:%c\n",localeBest.bb,bestMatch.bb,bestMatch.code);
#endif

	}

	cvReleaseImage(&imgWork);
	return bestMatch;
}

int DecodeImage(IplImage* image,CharSet& charSet,char codes[4],float *confidence)
{
	CvRect roi[4] = 
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
		codes[i] = result.code;
		if (confidence)
		{
			*confidence += (result.match/result.total/4);
		}
	}

	return 1;
}


MatchCharRet MatchChar(IplImage* image,CvRect roi,CharSet& charSet,int codeId)
{
	MatchCharRet bestMatch = {'?'};
	int XMargin = 4;
	int YMargin = 12;
	int XExtend = 0;
	const int SIZE = 20;

	IplImage* imgWork = cvCloneImage(image);
	if (codeId == 3)
	{
		XExtend = 5;
	}

	// 测试所有的位置
	for (int x = -XMargin; x < roi.width+XMargin+XExtend - SIZE;x++)
	{
		for (int y = -YMargin;y < roi.height+YMargin-SIZE;y++)
		{
			// 在当前位置匹配所有的字符
			int x0,y0,x1,y1; 

			// 把模板放入图像坐标系中
			x0 = x + roi.x;
			y0 = y + roi.y;
			x1 = x0+SIZE;
			y1 = y0+SIZE;

			// 计算模板在图像内的范围
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

			// 字体图像的有效范围
			int sx = x0 - roi.x - x;
			int sy = y0 - roi.y - y;
			int ex = x1 - roi.x - x;
			int ey = y1 - roi.y - y;

			// 遍历所有的字体图像
			for (int i = 0;i < charSet.size();i++)
			{
				IplImage* fontImage = charSet[i].image;
				// 匹配字体
				MatchCharRet result;
				cvCopy(image,imgWork);

				result = MatchFontImage(imgWork,x0,y0,fontImage,sx,sy,ex,ey);
				result.code = charSet[i].code;
				result.x = x+roi.x;
				result.y = y+roi.y;

				//cvShowImage("font",fontImage);
				//cvShowImage("match",imgWork);
				//cvWaitKey();

				// 更新最佳匹配结果
				bestMatch = UpdateBestMatchBetweenChar(bestMatch,result);
			}
		}
	}

	cvReleaseImage(&imgWork);
	return bestMatch;
}
