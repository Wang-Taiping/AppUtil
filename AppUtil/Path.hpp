#pragma once

#ifndef APPUTIL_PATH_HPP
#define APPUTIL_PATH_HPP

#include "Path.h"

#if _HAS_CXX17

#include <filesystem>
#include <string>

namespace ProgramUtils
{
	inline std::filesystem::path module_file_absolute_path(void* hModule)
	{
		std::wstring path;
		path.reserve(2048);
		ModuleFilePathW(hModule, &path[0], 2048);
		return &path[0];
	}

	inline std::filesystem::path module_dir_absolute_path(void* hModule)
	{
		std::wstring path;
		path.reserve(2048);
		ModuleDirPathW(hModule, &path[0], 2048);
		return &path[0];
	}

	inline std::filesystem::path temporary_absolute_path()
	{
		std::wstring path;
		path.reserve(2048);
		TemporaryPathW(&path[0], 2048);
		return &path[0];
	}

	inline bool rename(std::filesystem::path Old_p, std::filesystem::path New_p, ConflictBehavior Behavior = ConflictError)
	{
		return RenamePathW(Old_p.wstring().c_str(), New_p.wstring().c_str(), Behavior);
	}
}

#endif // _HAS_CXX17

#endif // !APPUTIL_PATH_HPP
