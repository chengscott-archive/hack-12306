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
#include "Samples.h"
#include <stdio.h>
#include <tzk/opencv.h>

void CollectSamples()
{
	char fileName[256];
	IplImage* gray;
	IplImage* bw;
	int i;
	int bwCount = 1;

	for (i = 1;i < 340;i++)
	{	
		sprintf(fileName,"images/color/%d.bmp",i);
		gray = cvLoadImage(fileName,CV_LOAD_IMAGE_GRAYSCALE);
		if (!gray)
		{
			continue;
		}

		bw = cvCloneImage(gray);

		cvThreshold(gray,bw,150,255,THRESH_BINARY);

		cvShowImage("gray",gray);
		cvShowImage("bw",bw);
		sprintf(fileName,"images/bw/%d.png",bwCount);
		cvSaveImage(fileName,bw);
		bwCount++;

		//WaitKeyboard(10);

		cvReleaseImage(&gray);
		cvReleaseImage(&bw);
	}

	for (i = 1;i <= 124;i++)
	{	
		sprintf(fileName,"images/old/%d.bmp",i);
		gray = cvLoadImage(fileName,CV_LOAD_IMAGE_GRAYSCALE);
		if (!gray)
		{
			continue;
		}

		bw = cvCloneImage(gray);

		cvThreshold(gray,bw,150,255,THRESH_BINARY);

		cvShowImage("gray",gray);
		cvShowImage("bw",bw);
		sprintf(fileName,"images/bw/%d.png",bwCount);
		cvSaveImage(fileName,bw);
		bwCount++;

		//WaitKeyboard(10);

		cvReleaseImage(&gray);
		cvReleaseImage(&bw);
	}
}


void CollectNewSamples()
{
	char fileName[256];
	IplImage* gray;
	IplImage* bw;
	int i;
	int bwCount = 1;

	for (i = 1;i <= 200;i++)
	{	
		sprintf(fileName,"newSample/new/%d.png",i);
		gray = cvLoadImage(fileName,CV_LOAD_IMAGE_GRAYSCALE);
		if (!gray)
		{
			continue;
		}

		bw = cvCloneImage(gray);

		cvThreshold(gray,bw,115,255,THRESH_BINARY);

		cvShowImage("gray",gray);
		cvShowImage("bw",bw);
		sprintf(fileName,"newSample/bw/%d.png",bwCount);
		cvSaveImage(fileName,bw);
		bwCount++;

		//WaitKeyboard(10);

		cvReleaseImage(&gray);
		cvReleaseImage(&bw);
	}
}