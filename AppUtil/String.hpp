#pragma once

#ifndef APPUTIL_STRING_HPP
#define APPUTIL_STRING_HPP

#include "String.h"

#ifdef __cplusplus

#include <string>

namespace ProgramUtils
{
	inline std::wstring convert_string(std::string string)
	{
		int size = StringConvertToGBK(string.c_str(), nullptr, 0);
		wchar_t* temp_str = new wchar_t[size];
		StringConvertToGBK(string.c_str(), temp_str, size);
		std::wstring ret_str = temp_str;
		delete[] temp_str;
		return ret_str;
	}

	inline std::string convert_string(std::wstring string)
	{
		int size = StringConvertToUTF8(string.c_str(), nullptr, 0);
		char* temp_str = new char[size];
		StringConvertToUTF8(string.c_str(), temp_str, size);
		std::string ret_str = temp_str;
		delete[] temp_str;
		return ret_str;
	}
}

#endif // __cplusplus

#endif // !APPUTIL_STRING_HPP
