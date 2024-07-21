#pragma once

#ifndef APPUTIL_HTTPDOWNLOAD_H
#define APPUTIL_HTTPDOWNLOAD_H

#include "UtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	typedef struct HTTPDownloadETA
	{
		uint16_t Day;
		uint16_t Hour;
		uint16_t Min;
		uint16_t Sec;
	} DownloadETA;
	typedef int(*HttpDownloadCallback)(uint64_t Total, uint64_t Now, float Percentage, uint64_t Speed, HTTPDownloadETA ETA, void* UserData);

	APPUTIL_EXPORT void APPUTIL_API HttpInitialize();
	APPUTIL_EXPORT void APPUTIL_API HttpUninitialize();
	APPUTIL_EXPORT size_t APPUTIL_API HttpDownload(const char* Url, const char* Path, HttpDownloadCallback Callback, void* UserData, ubool KeepDisplay);
	APPUTIL_EXPORT size_t APPUTIL_API HttpDownloadMemory(const char* Url, void* Buffer, size_t BufBytes, ubool KeepDisplay);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !APPUTIL_HTTPDOWNLOAD_H
