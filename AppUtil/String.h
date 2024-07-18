#pragma once

#ifndef APPUTIL_STRING_H
#define APPUTIL_STRING_H

#include "UtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	enum Endian
	{
		BigEndian,
		LittleEndian,
		NetworkEndian = BigEndian
	};

	APPUTIL_EXPORT void* reverse_memcpy(void* Dest, const void* Src, size_t Bytes);
	APPUTIL_EXPORT Endian MachineEndian();
	APPUTIL_EXPORT int StringConvertToGBK(const char* String, wchar_t* Buffer, int Elems);
	APPUTIL_EXPORT int StringConvertToUTF8(const wchar_t* String, char* Buffer, int Elems);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !APPUTIL_STRING_H
