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

#include "stdafx.h"
#include <io.h>
#include <vector>
#include "match.h"
#include "Samples.h"

//{ opencv stuff
#include "cv.h"
#include "highgui.h"
#include "cvaux.h"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/contrib/contrib.hpp"

#ifdef _DEBUG
#pragma comment(lib,"opencv_core231d.lib")
#pragma comment(lib,"opencv_highgui231d.lib")
#pragma comment(lib,"opencv_video231d.lib")
#pragma comment(lib,"opencv_ml231d.lib")
#pragma comment(lib,"opencv_legacy231d.lib")
#pragma comment(lib,"opencv_imgproc231d.lib")
#pragma comment(lib,"opencv_features2d231d.lib")
#pragma comment(lib,"opencv_flann231d.lib")
#pragma comment(lib,"opencv_objdetect231d.lib")
#pragma comment(lib,"opencv_calib3d231d.lib")
#pragma comment(lib,"opencv_contrib231d.lib")
#else
#pragma comment(lib,"opencv_core231.lib")
#pragma comment(lib,"opencv_highgui231.lib")
#pragma comment(lib,"opencv_video231.lib")
#pragma comment(lib,"opencv_ml231.lib")
#pragma comment(lib,"opencv_legacy231.lib")
#pragma comment(lib,"opencv_imgproc231.lib")
#pragma comment(lib,"opencv_features2d231.lib")
#pragma comment(lib,"opencv_flann231.lib")
#pragma comment(lib,"opencv_objdetect231.lib")
#pragma comment(lib,"opencv_calib3d231.lib")
#pragma comment(lib,"opencv_contrib231.lib")
#endif

using namespace cv;

//}

SimpleImage* IplimageToSimpleImage(IplImage* ipl)
{
	SimpleImage* dst = siCreateImage(ipl->width,ipl->height,ipl->nChannels);
	if (!dst)
	{
		return NULL;
	}

	memcpy(dst->imageData,ipl->imageData,ipl->imageSize);
	return dst;
}

int GetFolderFileList(const char* folder,const char* filter,std::vector<std::string>& filePathArray)
{
	struct _finddata_t files;
	int fileHandle;
	char fspec[256];
	sprintf(fspec,"%s\\%s",folder,filter);

	fileHandle = _findfirst(fspec,&files);

	if (-1 == fileHandle)
	{
		return 0;
	}

	filePathArray.clear();

	do
	{
		char path[260];
		sprintf(path,"%s\\%s",folder,files.name);

		filePathArray.push_back(std::string(path));

	}while( 0 ==_findnext(fileHandle,&files));

	_findclose(fileHandle);
	return filePathArray.size();
}

void DecodeTest(const char* colorDir,char* ext,const char* recDir)
{
	std::vector<std::string> filePathArray;
	GetFolderFileList(colorDir,ext,filePathArray);
	for (int i = 0;i < filePathArray.size();i++)
	{
		IplImage* image = cvLoadImage(filePathArray[i].c_str(),1);
		char codes[10] = {0};

		SimpleImage* si = IplimageToSimpleImage(image);
		//DecodeImage(si,charSet,codes);
		MatcherDecode(si,codes);
		//cvDestroyAllWindows();
		//cvShowImage(codes,image);
		char fileName[256];
		sprintf(fileName,"%s/%s.png",recDir,codes);
		cvSaveImage(fileName,image);

		cvReleaseImage(&image);
	}
}

void MakeFontData(const char* imageName,const char* dataName)
{
	IplImage* image = cvLoadImage(imageName,0);
	SimpleImage* siImage;
	siImage = IplimageToSimpleImage(image);	
	siSaveImage(dataName,siImage);

	cvReleaseImage(&image);
	siReleaseImage(&siImage);
}

int main(int argc, char* argv[])
{
	// MakeFontData("ABCDEFGHJKMNPQRSTUVWXY23456789.png","ABCDEFGHJKMNPQRSTUVWXY23456789.dat");
	// CollectNewSamples();
	// return 1;

	//CharSet charSet;
	//LoadCharSet("ABCDEFGHJKMNOPQRSTUVWXYZ23456789.png","ABCDEFGHJKMNOPQRSTUVWXYZ23456789",charSet);
	//LoadCharSet("font.dat","ABCDEFGHJKMNOPQRSTUVWXYZ23456789",charSet);
	SimpleImage* font = siLoadImage("ABCDEFGHJKMNPQRSTUVWXY23456789.dat");
	MatcherInitImage(font,"ABCDEFGHJKMNPQRSTUVWXY23456789");

	SimpleImage* font1 = siLoadImage("font.dat");
	MatcherInitImage(font1,"ABCDEFGHJKMNOPQRSTUVWXYZ23456789");

	DecodeTest("v3/color","*.png","v3/result");

	return 0;
}

