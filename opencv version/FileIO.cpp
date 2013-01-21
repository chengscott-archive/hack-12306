#include "FileIO.h"

int GetFileBytes(const char* fileName)
{
	int ret;
#if defined _WINDOWS
	struct _stat st;
	ret = _stat(fileName,&st);
#else
	struct stat st;
	ret = stat(fileName,&st);
#endif

	if (!ret)
	{
		return (int)st.st_size;
	}

	return -1;
}

int RemoveDir(const char* dir)
{
	char cmd[512];
	sprintf(cmd,"rmdir /s /q \"%s\" ",dir);
	return system(cmd);
}

bool IsFileExist(const char* fileName)
{
	return ACCESS(fileName,0) != -1;
}

bool IsFileOrDirExist(const char* path)
{
	DWORD fileAttributes;
	fileAttributes = GetFileAttributes(path);
	if ( INVALID_FILE_ATTRIBUTES == fileAttributes)
	{
		return false;
	}

	return true;
}


int GetFileBytes(char* fileName)
{
	int ret;
#if defined _WIN32
	struct _stat st;
	ret = _stat(fileName,&st);
#else
	struct stat st;
	ret = stat(fileName,&st);
#endif

	if (!ret)
	{
		return (int)st.st_size;
	}

	return -1;
}

int MakeDir(const char* dir)
{
	char pszDir[512];
	int i = 0;
	int iRet;
	int iLen;

	strcpy(pszDir,dir);
	iLen = strlen(pszDir);

	//在末尾加/
	if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
	{
		pszDir[iLen] = '/';
		pszDir[iLen + 1] = '\0';
	}

	// 创建目录
	for (i = 0;i <= iLen;i ++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{ 
			pszDir[i] = '\0';

			//如果不存在,创建
			iRet = ACCESS(pszDir,0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return 0;
				} 
			}
			//支持linux,将所有\换成/
			pszDir[i] = '/';
		} 
	}

	return 1;
}

char* GetFileExt(const char* path,char ext[256])
{
	char fileName[256] = {0};
	int len;
	int i;
	GetFileName(path,fileName);
	len = strlen(fileName);

	for(i = len - 1;i > 0;i--)
	{
		if (fileName[i] == '.')
		{
			strcpy(ext,fileName + i + 1);
			return ext;
		}
	}

	return NULL;
}

char* GetFileNameNoExt(const char* path,char title[256])
{
	char fileName[256] = {0};
	int len;
	int i;
	GetFileName(path,fileName);
	len = strlen(fileName);

	for( i = len - 1;i > 0;i--)
	{
		if (fileName[i] == '.')
		{
			memcpy(title,fileName,i);
			title[i] = 0;
			return title;
		}
	}

	strcpy(title,path);
	return title;
}

char* GetFileName(const char* path,char fileName[256])
{
	int i;
	int len;

	if (!path)
	{
		return NULL;
	}

	len = strlen(path);

	for (i = len - 1;i >= 0;i--)
	{
		if (path[i] == '\\' || path[i] == '/' )
		{
			break;
		}
	}

	strcpy(fileName,path + i + 1);

	return fileName;
}

char* GetFileDir(const char* path,char fileName[256])
{
	int i;
	int len;

	int dirPos = 0;
	if (!path)
	{
		return NULL;
	}

	len = strlen(path);

	for (i = len - 1;i >= 0;i--)
	{
		if (path[i] == '\\' || path[i] == '/' )
		{
			dirPos = i;
			break;
		}
	}

	fileName[dirPos] = 0;
	for (i = 0;i < dirPos;i++)
	{
		fileName[i] = path[i];
	}

	return fileName;
}

