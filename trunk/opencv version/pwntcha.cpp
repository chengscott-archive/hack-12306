// pwntcha.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <tzk/opencv.h>
#include <io.h>
#include <vector>
#include "match.h"

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

int main(int argc, char* argv[])
{
	CharSet charSet;
	LoadCharSet("ABCDEFGHJKMNOPQRSTUVWXYZ23456789.png","ABCDEFGHJKMNOPQRSTUVWXYZ23456789",charSet);

	std::vector<std::string> filePathArray;
	GetFolderFileList("images/bw","*.png",filePathArray);
	for (int i = 0;i < filePathArray.size();i++)
	{
		IplImage* image = cvLoadImage(filePathArray[i].c_str(),CV_LOAD_IMAGE_GRAYSCALE);
		char codes[10] = {0};

		DecodeImage(image,charSet,codes);
		//cvDestroyAllWindows();
		//cvShowImage(codes,image);
		char fileName[256];
		sprintf(fileName,"images/rec/%s.png",codes);
		cvSaveImage(fileName,image);

		cvReleaseImage(&image);
	}

	FreeCharSet(charSet);
	return 0;
}

