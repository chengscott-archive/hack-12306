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
