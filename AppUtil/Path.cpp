#pragma warning(disable: 4996)

#include "Path.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <filesystem>

size_t ModuleFilePathA(void* hModule, char* Buffer, uint32_t Elems)
{
	char* path_str = new char[2048];
	GetModuleFileNameA(HMODULE(hModule), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	std::filesystem::path path = std::filesystem::absolute(path_str);
	delete[] path_str;
	if (Elems <= path.string().size()) return path.string().size() + 1;
	strcpy(Buffer, path.string().c_str());
	return path.string().size() + 1;
}

size_t ModuleFilePathW(void* hModule, wchar_t* Buffer, uint32_t Elems)
{
	wchar_t* path_str = new wchar_t[2048];
	GetModuleFileNameW(HMODULE(hModule), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	std::filesystem::path path = std::filesystem::absolute(path_str);
	delete[] path_str;
	if (Elems <= path.wstring().size()) return path.wstring().size() + 1;
	wcscpy(Buffer, path.wstring().c_str());
	return path.string().size() + 1;
}

size_t ModuleDirPathA(void* hModule, char* Buffer, uint32_t Elems)
{
	char* path_str = new char[2048];
	GetModuleFileNameA(HMODULE(hModule), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	std::filesystem::path path = std::filesystem::absolute(path_str).parent_path();
	delete[] path_str;
	if (Elems <= path.string().size()) return path.string().size() + 1;
	strcpy(Buffer, path.string().c_str());
	return path.string().size() + 1;
}

size_t ModuleDirPathW(void* hModule, wchar_t* Buffer, uint32_t Elems)
{
	wchar_t* path_str = new wchar_t[2048];
	GetModuleFileNameW(HMODULE(hModule), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	std::filesystem::path path = std::filesystem::absolute(path_str).parent_path();
	delete[] path_str;
	if (Elems <= path.wstring().size()) return path.wstring().size() + 1;
	wcscpy(Buffer, path.wstring().c_str());
	return path.string().size() + 1;
}

size_t TemporaryPathA(char* Buffer, uint32_t Elems)
{
	char* path_str = new char[2048];
	int need_size = GetEnvironmentVariableA("TEMP", path_str, 2048);
	if (GetLastError() == ERROR_ENVVAR_NOT_FOUND || need_size > 2048) return 0;
	std::filesystem::path path = std::filesystem::absolute(path_str);
	delete[] path_str;
	if (Elems <= path.string().size()) return path.string().size() + 1;
	strcpy(Buffer, path.string().c_str());
	return path.string().size() + 1;
}

size_t TemporaryPathW(wchar_t* Buffer, uint32_t Elems)
{
	wchar_t* path_str = new wchar_t[2048];
	int need_size = GetEnvironmentVariableW(L"TEMP", path_str, 2048);
	if (GetLastError() == ERROR_ENVVAR_NOT_FOUND || need_size > 2048) return 0;
	std::filesystem::path path = std::filesystem::absolute(path_str);
	delete[] path_str;
	if (Elems <= path.wstring().size()) return path.wstring().size() + 1;
	wcscpy(Buffer, path.wstring().c_str());
	return path.string().size() + 1;
}

static int NativeRenamePath(std::filesystem::path OldPath, std::filesystem::path NewPath, ConflictBehavior Behavior)
{
	std::error_code ec;
	OldPath = std::filesystem::absolute(OldPath, ec);
	NewPath = std::filesystem::absolute(NewPath, ec);
	if (OldPath == NewPath) return 0;
	if (!std::filesystem::exists(OldPath, ec)) return 0;
	if (!std::filesystem::exists(NewPath, ec)) {
		if (!std::filesystem::exists(NewPath.parent_path(), ec) && !std::filesystem::create_directories(NewPath.parent_path(), ec)) return 0;
		std::filesystem::rename(OldPath, NewPath, ec);
		return 1;
	}
	else {
		if (std::filesystem::is_regular_file(OldPath, ec) || !std::filesystem::is_directory(NewPath, ec)) { // Conflict
			if (Behavior == ConflictForce) {
				std::filesystem::remove_all(NewPath, ec);
				std::filesystem::rename(OldPath, NewPath, ec);
				return 1;
			}
			else return 0;
		}
		else if (!std::filesystem::is_empty(NewPath, ec)) { // Conflict, Recursive
			int Ret = 0;
			for (auto& i : std::filesystem::directory_iterator(OldPath, ec)) {
				Ret = NativeRenamePath(OldPath / i.path().filename(), NewPath / i.path().filename(), Behavior);
				if (Behavior != ConflictSkip && !Ret) return 0;
			}
			std::filesystem::remove_all(OldPath, ec);
			return 1;
		}
		else {
			std::filesystem::remove_all(NewPath, ec);
			std::filesystem::rename(OldPath, NewPath, ec);
			return 1;
		}
	}
	return 0;
}

int RenamePathA(const char* OldPath, const char* NewPath, ConflictBehavior Behavior)
{
	return NativeRenamePath(OldPath, NewPath, Behavior);
}

int RenamePathW(const wchar_t* OldPath, const wchar_t* NewPath, ConflictBehavior Behavior)
{
	return NativeRenamePath(OldPath, NewPath, Behavior);
}
