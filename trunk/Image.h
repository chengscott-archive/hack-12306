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
#ifndef __SIMPLE_IMAGE_H__
#define __SIMPLE_IMAGE_H__

#define HAVE_OPENCV
#ifdef HAVE_OPENCV
#include <tzk/opencv.h>
#endif

#ifndef uchar
typedef unsigned char uchar;
#endif

typedef struct _SimpleImage
{
	int  nSize;             /* sizeof(IplImage) */
	int  nChannels;         /* Most of OpenCV functions support 1,2,3 or 4 channels */

	int  depth;             /* Pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16S,
							IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F are supported.  */
	int  width;             /* Image width in pixels.                           */
	int  height;            /* Image height in pixels.                          */
	int  imageSize;         /* Image data size in bytes
							(==image->height*image->widthStep
							in case of interleaved data)*/
	char *imageData;        /* Pointer to aligned image data.         */
	int  widthStep;         /* Size of aligned image row in bytes.    */
}SimpleImage;

int          siSaveImage(const char* path,SimpleImage* src);
SimpleImage* siLoadImage(const char* path);
SimpleImage* siLoadImageFromStream(const char* data);
SimpleImage* siCreateImage(int width,int height,int nChannels,char* data = 0);
void         siReleaseImage(SimpleImage** pImage);
int          siCopyImage(SimpleImage* src,SimpleImage* dst);
int          siCopyImageROI(SimpleImage* src,int x,int y,int w,int h,SimpleImage* dst);
SimpleImage* siCloneImage(SimpleImage* src);

/************************************************************************/
/*                             Algorithms                               */
/************************************************************************/
void         siRgbToGray(SimpleImage* src,SimpleImage* dst);
void         siThreshold(SimpleImage* src,SimpleImage* dst,int thresh,int maxValue);

#ifdef HAVE_OPENCV
void         siShowImage(char* windowName,SimpleImage* src);
#endif

#endif /* __SIMPLE_IMAGE_H__ */