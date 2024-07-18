#pragma once

#ifndef APPUTIL_MD5_H
#define APPUTIL_MD5_H

#include "UtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	TYPEHANDLE(HMD5);

	APPUTIL_EXPORT HMD5 MD5Create();
	APPUTIL_EXPORT void MD5Release(HMD5 hMD5);
	APPUTIL_EXPORT ubool MD5Result(HMD5 hMD5);
	APPUTIL_EXPORT ubool MD5StrToLowerA(HMD5 hMD5, char* Buffer, size_t Length);
	APPUTIL_EXPORT ubool MD5StrToUpperA(HMD5 hMD5, char* Buffer, size_t Length);
	APPUTIL_EXPORT ubool MD5StrToLowerW(HMD5 hMD5, wchar_t* Buffer, size_t Length);
	APPUTIL_EXPORT ubool MD5StrToUpperW(HMD5 hMD5, wchar_t* Buffer, size_t Length);

	APPUTIL_EXPORT ubool MD5HashFileA(HMD5 hMD5, const char* File);
	APPUTIL_EXPORT ubool MD5HashFileW(HMD5 hMD5, const wchar_t* File);
	APPUTIL_EXPORT ubool MD5HashData(HMD5 hMD5, const void* Data, size_t Bytes);

	APPUTIL_EXPORT ubool MD5HashCheckStrA(HMD5 hMD5, const char* MD5HashStr);
	APPUTIL_EXPORT ubool MD5HashCheckStrW(HMD5 hMD5, const wchar_t* MD5HashStr);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !APPUTIL_MD5_H
