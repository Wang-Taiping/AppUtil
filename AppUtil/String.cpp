#include "String.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

Endian MachineEndian()
{
	union
	{
		uint8_t LowByte;
		uint16_t Data;
	} Checker{};
	Checker.Data = 0xff00;
	if (Checker.LowByte == 0xff) return BigEndian;
	return LittleEndian;
}

void* reverse_memcpy(void* Dest, const void* Src, size_t Bytes)
{
	if (!Dest || !Src) return Dest;
	char* _Dst = (char*)Dest;
	const char* _Src = (const char*)Src;
	const size_t Idx = Bytes - 1;
	for (size_t i = 0; i < Bytes; i++) {
		_Dst[i] = _Src[Idx - i];
	}
	return Dest;
}

int StringConvertToGBK(const char* String, wchar_t* Buffer, int Elems)
{
	return MultiByteToWideChar(936, 0, String, -1, Buffer, Elems);
}

int StringConvertToUTF8(const wchar_t* String, char* Buffer, int Elems)
{
	return WideCharToMultiByte(65001, 0, String, -1, Buffer, Elems, nullptr, 0);
}
