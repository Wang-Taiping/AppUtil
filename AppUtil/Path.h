#pragma once

#ifndef APPUTIL_PATH_H
#define APPUTIL_PATH_H

#include "UtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	APPUTIL_EXPORT size_t ModuleFilePathA(void* hModule, char* Buffer, uint32_t Elems);
	APPUTIL_EXPORT size_t ModuleFilePathW(void* hModule, wchar_t* Buffer, uint32_t Elems);
	APPUTIL_EXPORT size_t ModuleDirPathA(void* hModule, char* Buffer, uint32_t Elems);
	APPUTIL_EXPORT size_t ModuleDirPathW(void* hModule, wchar_t* Buffer, uint32_t Elems);
	APPUTIL_EXPORT size_t TemporaryPathA(char* Buffer, uint32_t Elems);
	APPUTIL_EXPORT size_t TemporaryPathW(wchar_t* Buffer, uint32_t Elems);
	APPUTIL_EXPORT int RenamePathA(const char* OldPath, const char* NewPath, ConflictBehavior Behavior);
	APPUTIL_EXPORT int RenamePathW(const wchar_t* OldPath, const wchar_t* NewPath, ConflictBehavior Behavior);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !APPUTIL_PATH_H
