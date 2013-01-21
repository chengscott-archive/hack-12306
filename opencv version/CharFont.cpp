#include "CharFont.h"


int LoadCharSet(const char* fontFileName,const char* charArray,CharSet& charSet)
{
	if (!fontFileName || !charArray)
	{
		return 0;
	}

	IplImage* largeImage = cvLoadImage(fontFileName,CV_LOAD_IMAGE_GRAYSCALE);
	if (!largeImage)
	{
		return 0;
	}

	int charNum = strlen(charArray);
	int width = largeImage->width/charNum;
	int height = largeImage->height;

	charSet.clear();
	charSet.resize(charNum);

	for (int i = 0;i < charNum;i++)
	{
		IplImage* image = cvCreateImage(cvSize(width,height),8,1);
		cvSetImageROI(largeImage,cvRect(i*width,0,width,height));
		cvCopy(largeImage,image);
		CharFont font;
		font.code = charArray[i];
		font.image = image;
		charSet[i] = font;
	}

	cvReleaseImage(&largeImage);
	return charSet.size();
}

int FreeCharSet(CharSet& charSet)
{
	for (int i = 0;i < charSet.size();i++)
	{
		cvReleaseImage(&charSet[i].image);
	}

	return charSet.size();
}