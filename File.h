#pragma once
#include <stdio.h>
#include <PocketFrog/PocketFrog.h>
using namespace Frog;

// File Handler with built-in Error Logger
class File
{
	FILE *file;
	TCHAR u_userFilename[20];
	TCHAR path[MAX_PATH];
	char userFilename[20];
	FILE *errorlogger;
public:
	File(void);
	void Init();
	bool OpenForRead(TCHAR *filename);
	void OpenForWrite(TCHAR *filename);
	void Close();
	FILE* GetFile() { return file; }
	void Write(char *text);
	void Error(TCHAR *filename);
	void ErrorWrite(char *text);
	void End();
public:
	~File(void);
};
