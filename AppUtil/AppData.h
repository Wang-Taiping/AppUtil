#pragma once

#ifndef APPDATA_H
#define APPDATA_H

#include "UtilBase.h"

#define APPDATA_READ			0x01
#define APPDATA_WRITE			0x02
#define APPDATE_READWRITE		0x03 // 3 = 1 | 2
#define APPDATA_TRUNCATE		0x04 // 'TRUNCATE' must be used in conjunction with 'WRITE' permissions.
#define APPDATA_APPEND			0x08 // When you open a file, the file pointer is positioned at the end of the file. 'APPEND' must be used in conjunction with 'WRITE' permissions.
#define APPDATA_APPEND_ALWAYS	0x10 // Each time a piece is written, the file pointer is positioned at the end of the file. 'APPEND_ALWAYS' must be used in conjunction with 'WRITE' permissions.

#define APPDATA_BEGIN		0
#define APPDATA_CURRENT		1
#define APPDATA_END			2

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef void* HAPPDATA;
	APPUTIL_TYPENAME(HAPPDIR);
	APPUTIL_TYPENAME(HAPPFILE);

	APPUTIL_EXPORT HAPPDIR APPUTIL_API AppDataRootDirectory();
	APPUTIL_EXPORT HAPPDIR APPUTIL_API AppDataOpenDirectoryA(HAPPDIR pDir, const char* DirName);
	APPUTIL_EXPORT HAPPDIR APPUTIL_API AppDataOpenDirectoryW(HAPPDIR pDir, const wchar_t* DirName);
	APPUTIL_EXPORT ubool APPUTIL_API AppDataClearDirectory(HAPPDIR hDir);
	APPUTIL_EXPORT HAPPFILE APPUTIL_API AppDataOpenFileA(HAPPDIR pDir, const char* FileName, int OpenMode);
	APPUTIL_EXPORT HAPPFILE APPUTIL_API AppDataOpenFileW(HAPPDIR pDir, const wchar_t* FileName, int OpenMode);
	APPUTIL_EXPORT size_t APPUTIL_API AppDataReadFile(HAPPFILE hFile, void* Buffer, size_t Bytes);
	APPUTIL_EXPORT size_t APPUTIL_API AppDataWriteFile(HAPPFILE hFile, const void* Data, size_t Bytes);
	APPUTIL_EXPORT size_t APPUTIL_API AppDataMoveFilePointer(HAPPFILE hFile, size_t MoveBytes, int Beginning);
	APPUTIL_EXPORT size_t APPUTIL_API AppDataGetFilePointer(HAPPFILE hFile);
	APPUTIL_EXPORT ubool APPUTIL_API AppDataCloseHandle(HAPPDATA hAppData);

	APPUTIL_EXPORT ubool APPUTIL_API AppDataGeneratePathA(HAPPDIR pDir, const char* FileName, char* Buffer, size_t Buflen);
	APPUTIL_EXPORT ubool APPUTIL_API AppDataGeneratePathW(HAPPDIR pDir, const wchar_t* FileName, wchar_t* Buffer, size_t Buflen);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !APPDATA_H
