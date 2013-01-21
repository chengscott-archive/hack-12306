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
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "Image.h"

int siSaveImage(const char* path,SimpleImage* src)
{
	FILE* file = NULL;
	if (!path || !src)
	{
		return 0;
	}
	file = fopen(path,"wb");
	if (!file)
	{
		return 0;
	}

	// write header
	fwrite(src,sizeof(SimpleImage),1,file);
	// write imagedata
	fwrite(src->imageData,src->imageSize,1,file);

	fclose(file);

	return 1;
}

SimpleImage* siCreateImage(int width,int height,int nChannels,char* data)
{
	SimpleImage* pImage = NULL;
	
	// header
	pImage = (SimpleImage*)malloc(sizeof(SimpleImage));
	if (!pImage)
	{
		return NULL;
	}
	pImage->width = width;
	pImage->height = height;
	pImage->nChannels = nChannels;
	pImage->widthStep = ((width*nChannels + 3)/4)*4;
	pImage->imageSize = pImage->widthStep*height;

	// imagedata
	char* imageData = (char*)malloc(pImage->imageSize);
	if (!imageData)
	{
		free(pImage);
		return NULL;
	}

	pImage->imageData = imageData;

	// copy data if specified
	if (data)
	{
		memcpy(pImage->imageData,data,pImage->imageSize);
	}

	return pImage;
}

SimpleImage* siLoadImageFromStream(const char* data)
{
	SimpleImage* pImage = NULL;
	
	// read header
	pImage = (SimpleImage*)malloc(sizeof(SimpleImage));
	if (!pImage)
	{
		return NULL;
	}

	memcpy(pImage,data,sizeof(SimpleImage));

	// read imagedata
	char* imageData = (char*)malloc(pImage->imageSize);
	if (!imageData)
	{
		free(pImage);
		return NULL;
	}

	memcpy(imageData,data+sizeof(SimpleImage),pImage->imageSize);
	pImage->imageData = imageData;

	return pImage;
}

SimpleImage* siLoadImage(const char* path)
{
	SimpleImage* pImage = NULL;
	FILE* file;
	file = fopen(path,"rb");
	if (!file)
	{
		return	NULL;
	}

	// read header
	pImage = (SimpleImage*)malloc(sizeof(SimpleImage));
	if (!pImage)
	{
		fclose(file);
		return NULL;
	}

	fread(pImage,sizeof(SimpleImage),1,file);
	// read imagedata
	char* imageData = (char*)malloc(pImage->imageSize);
	if (!imageData)
	{
		fclose(file);
		free(pImage);
		return NULL;
	}

	fread(imageData,pImage->imageSize,1,file);
	pImage->imageData = imageData;

	return pImage;
}

void siReleaseImage(SimpleImage** pImage)
{
	if (pImage && *pImage)
	{
		free((*pImage)->imageData);
		free(*pImage);
		*pImage = NULL;
	}
}

int siCopyImage(SimpleImage* src,SimpleImage* dst)
{
	if (!src || !dst || src->width != dst->width || src->height != dst->height)
	{
		return 0;
	}

	memcpy(dst->imageData,src->imageData,src->imageSize);
	return 1;
}

SimpleImage* siCloneImage(SimpleImage* src)
{
	SimpleImage* pImage = NULL;
	
	// read header
	pImage = (SimpleImage*)malloc(sizeof(SimpleImage));
	if (!pImage)
	{
		return NULL;
	}

	memcpy(pImage,src,sizeof(SimpleImage));
	// read imagedata
	char* imageData = (char*)malloc(pImage->imageSize);
	if (!imageData)
	{
		free(pImage);
		return NULL;
	}
	pImage->imageData = imageData;
	memcpy(pImage->imageData,src->imageData,src->imageSize);

	return pImage;
}

int siCopyImageROI(SimpleImage* src,int x,int y,int w,int h,SimpleImage* dst)
{
	if (!src || !dst || w != dst->width || h != dst->height)
	{
		return 0;
	}

	for (int i = 0;i < h;i++)
	{
		char* pLine = src->imageData + (y+i)*src->widthStep + x*src->nChannels;
		memcpy(dst->imageData+dst->widthStep*i,pLine,w*src->nChannels);
	}

	return 1;
}

void siRgbToGray(SimpleImage* src,SimpleImage* dst)
{
	for (int y = 0;y < src->height;y++)
	{
		uchar* pSrc = (uchar*)(src->imageData + src->widthStep*y);
		uchar* pDst = (uchar*)(dst->imageData + dst->widthStep*y);
		for (int x = 0;x < src->width;x++)
		{
			pDst[0] = (pSrc[0]*19595 + pSrc[1]*38469 + pSrc[2]*7472) >> 16;
			pSrc += src->nChannels;
			pDst += dst->nChannels;
		}
	}
}

void siThreshold(SimpleImage* src,SimpleImage* dst,int thresh,int maxValue)
{
	if (src->nChannels != 1 || dst->nChannels != 1 ||
		src->width != dst->width || src->height != dst->height)
	{
		return ;
	}

	uchar* pSrc = (uchar*)src->imageData;
	uchar* pDst = (uchar*)dst->imageData;

	for (int i = 0;i < src->imageSize;i++)
	{
		if (pSrc[i] < thresh)
		{
			pDst[i] = 0;
		}
		else
		{
			pDst[i] = maxValue;
		}
	}
}

#ifdef HAVE_OPENCV
void  siShowImage(char* windowName,SimpleImage* src)
{
	IplImage* image = cvCreateImage(cvSize(src->width,src->height),8,src->nChannels);
	memcpy(image->imageData,src->imageData,src->imageSize);
	cvShowImage(windowName,image);
	cvReleaseImage(&image);
}

#endif