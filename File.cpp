#include "File.h"

File::File(void)
{

}

File::~File(void)
{
}

void File::Close() {
	if (file)
		fclose(file);
}

void File::Init() {
	#ifdef _UNICODE
		Frog::PocketPC::GetFullPathName(L"error.log",path);
		errorlogger = _wfopen(path,TEXT("w"));
	#else
		errorlogger = fopen("error.log", "w");
	#endif
}
void File::End() {
	if (errorlogger)
		fclose(errorlogger);
}
bool File::OpenForRead(TCHAR *filename) {
	#ifdef _UNICODE
		_stprintf(u_userFilename,filename);
		Frog::PocketPC::GetFullPathName(u_userFilename,path);
		file = _wfopen(path,TEXT("rb"));
	#else
		sprintf(userFilename,filename);
		file = fopen(userFilename, "rb");
	#endif

	if (file == NULL) 
		return false;
	return true;
}

void File::OpenForWrite(TCHAR *filename) {
	#ifdef _UNICODE
		_stprintf(u_userFilename,filename);
		Frog::PocketPC::GetFullPathName(u_userFilename,path);
		file = _wfopen(path,TEXT("wb"));
	#else
		sprintf(userFilename,filename);
		file = fopen(userFilename, "wb");
	#endif

	if (file == NULL) 
		Error(filename);
}

void File::Error(TCHAR *filename) {
	#ifdef _UNICODE
		TCHAR errorpath[MAX_PATH];
		TCHAR filename2[20];
		_stprintf(filename2,TEXT("fileError.log"));
		Frog::PocketPC::GetFullPathName(filename2,errorpath);
		FILE *error = _wfopen(errorpath, TEXT("w"));
		fprintf(error,"error opening %s", filename);
		fclose(error);
	#else
		FILE *error = fopen("fileError.log","w");
		fprintf(error,"error opening %s", filename);
		fclose(error);
	#endif
}

void File::Write(char *text) {
	fprintf(file, "%s", text);
}

void File::ErrorWrite(char *text) {
	fprintf(errorlogger,"%s\n" ,text);
}