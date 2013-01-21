#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <Windows.h>
#elif _LINUX
#include <stdarg.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#define RMDIR(a) _rmdir((a))
#elif LINUX
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#define RMDIR(a) rmdir((a))
#endif


//////////////////////////////////////////////////////////////////////////
//                           File/Folder Helper

/** MakeDir Make a directory. */
int MakeDir(const char* dir);
/** RemoveDir Remove a directory. */
int RemoveDir(const char* dir);

/** Get file data size in bytes. */
int GetFileBytes(const char* fileName);
/** Test if file existed. */
bool IsFileExist(const char* fileName);
bool IsFileOrDirExist(const char* path);


/** Get filename string */
char* GetFileName(const char* path,char fileName[256]);
char* GetFileExt(const char* path,char ext[256]);
char* GetFileNameNoExt(const char* path,char title[256]);

/** Get file directory string */
char* GetFileDir(const char* path,char dir[256]);

#endif /* __FILE_IO_H__ */