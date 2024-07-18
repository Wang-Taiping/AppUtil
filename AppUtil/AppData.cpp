#pragma warning(disable: 4996)

#include "AppData.h"
#include "Path.hpp"
#include <filesystem>
#include <regex>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define APPDATA_DIR_MAGIC_NUMBER	0x0c414444
#define APPDATA_FILE_MAGIC_NUMBER	0x0c414446

typedef struct NativeAppDataDir {
	volatile uint32_t MagicNumber;
	std::filesystem::path DirPath;
} *PNativeAppDataDir;

typedef struct NativeAppDataFile {
	volatile uint32_t MagicNumber;
	std::filesystem::path FilePath;
	HANDLE hFile;
	int OpenMode;
} *PNativeAppDataFile;

std::regex regFileName(R"([\\/:*?\"<>|])");
std::wregex wregFileName(LR"([\\/:*?\"<>|])");

static bool AppDataInit = false;
static NativeAppDataDir;

HAPPDIR AppDataRootDirectory()
{
	std::error_code ec;
	std::filesystem::path RootDir = ProgramUtils::module_dir_absolute_path(nullptr) / "AppData";
	if (!std::filesystem::exists(RootDir, ec) &&
		!std::filesystem::create_directories(RootDir, ec)) return nullptr;
	PNativeAppDataDir hDir = new NativeAppDataDir;
	hDir->MagicNumber = APPDATA_DIR_MAGIC_NUMBER;
	hDir->DirPath = RootDir;
	return HAPPDIR(hDir);
}

HAPPDIR AppDataOpenDirectoryA(HAPPDIR pDir, const char* DirName)
{
	std::error_code ec;
	if (!pDir) return nullptr;
	if (std::regex_match(DirName, regFileName)) return nullptr;
	PNativeAppDataDir npDir = PNativeAppDataDir(pDir);
	if (npDir->MagicNumber != APPDATA_DIR_MAGIC_NUMBER) return nullptr;
	if (!std::filesystem::exists(npDir->DirPath / DirName, ec) &&
		!std::filesystem::create_directories(npDir->DirPath / DirName, ec)) return nullptr;
	PNativeAppDataDir hDir = new NativeAppDataDir;
	hDir->MagicNumber = APPDATA_DIR_MAGIC_NUMBER;
	hDir->DirPath = npDir->DirPath / DirName;
	return HAPPDIR(hDir);
}

HAPPDIR AppDataOpenDirectoryW(HAPPDIR pDir, const wchar_t* DirName)
{
	std::error_code ec;
	if (!pDir) return nullptr;
	if (std::regex_match(DirName, wregFileName)) return nullptr;
	PNativeAppDataDir npDir = PNativeAppDataDir(pDir);
	if (npDir->MagicNumber != APPDATA_DIR_MAGIC_NUMBER) return nullptr;
	if (!std::filesystem::create_directories(npDir->DirPath / DirName, ec)) return nullptr;
	PNativeAppDataDir hDir = new NativeAppDataDir;
	hDir->MagicNumber = APPDATA_DIR_MAGIC_NUMBER;
	hDir->DirPath = npDir->DirPath / DirName;
	return HAPPDIR(hDir);
}

ubool AppDataClearDirectory(HAPPDIR hDir)
{
	std::error_code ec;
	if (!hDir) return 0;
	PNativeAppDataDir nhDir = PNativeAppDataDir(hDir);
	if (nhDir->MagicNumber != APPDATA_DIR_MAGIC_NUMBER) return 0;
	std::filesystem::remove_all(nhDir->DirPath, ec);
	return std::filesystem::create_directories(nhDir->DirPath, ec);
}

HAPPFILE AppDataOpenFileA(HAPPDIR pDir, const char* FileName, int OpenMode)
{
	if (!pDir) return nullptr;
	if (std::regex_match(FileName, regFileName)) return nullptr;
	PNativeAppDataDir npDir = PNativeAppDataDir(pDir);
	if (npDir->MagicNumber != APPDATA_DIR_MAGIC_NUMBER) return nullptr;
	DWORD Access = 0, Flag = 0;
	if (OpenMode | APPDATA_READ) { Access |= GENERIC_READ; Flag = OPEN_EXISTING; }
	if (OpenMode | APPDATA_WRITE) { Access |= GENERIC_WRITE; Flag = OPEN_ALWAYS; }
	if ((OpenMode | APPDATA_TRUNCATE) && !(OpenMode | APPDATA_WRITE)) return nullptr;
	if (OpenMode | APPDATA_TRUNCATE) Flag = CREATE_ALWAYS;
	std::filesystem::path FilePath = npDir->DirPath / FileName;
	HANDLE hFile = CreateFileA(
		FilePath.string().c_str(), Access, FILE_SHARE_READ,
		nullptr, Flag, FILE_ATTRIBUTE_NORMAL, nullptr
	);
	if (hFile == INVALID_HANDLE_VALUE) return nullptr;
	if (OpenMode | APPDATA_APPEND) SetFilePointerEx(hFile, { 0 }, nullptr, FILE_END);
	PNativeAppDataFile nhFile = new NativeAppDataFile;
	nhFile->MagicNumber = APPDATA_FILE_MAGIC_NUMBER;
	nhFile->FilePath = FilePath;
	nhFile->hFile = hFile;
	nhFile->OpenMode = OpenMode;
	return HAPPFILE(nhFile);
}

HAPPFILE AppDataOpenFileW(HAPPDIR pDir, const wchar_t* FileName, int OpenMode)
{
	if (!pDir) return nullptr;
	if (std::regex_match(FileName, wregFileName)) return nullptr;
	PNativeAppDataDir npDir = PNativeAppDataDir(pDir);
	if (npDir->MagicNumber != APPDATA_DIR_MAGIC_NUMBER) return nullptr;
	DWORD Access = 0, Flag = 0;
	if (OpenMode | APPDATA_READ) { Access |= GENERIC_READ; Flag = OPEN_EXISTING; }
	if (OpenMode | APPDATA_WRITE) { Access |= GENERIC_WRITE; Flag = OPEN_ALWAYS; }
	if ((OpenMode | APPDATA_TRUNCATE) && !(OpenMode | APPDATA_WRITE)) return nullptr;
	if (OpenMode | APPDATA_TRUNCATE) Flag = CREATE_ALWAYS;
	std::filesystem::path FilePath = npDir->DirPath / FileName;
	HANDLE hFile = CreateFileW(
		FilePath.wstring().c_str(), Access, FILE_SHARE_READ,
		nullptr, Flag, FILE_ATTRIBUTE_NORMAL, nullptr
	);
	if (hFile == INVALID_HANDLE_VALUE) return nullptr;
	if (OpenMode | APPDATA_APPEND) SetFilePointerEx(hFile, { 0 }, nullptr, FILE_END);
	PNativeAppDataFile nhFile = new NativeAppDataFile;
	nhFile->MagicNumber = APPDATA_FILE_MAGIC_NUMBER;
	nhFile->FilePath = FilePath;
	nhFile->hFile = hFile;
	nhFile->OpenMode = OpenMode;
	return HAPPFILE(nhFile);
}

size_t AppDataReadFile(HAPPFILE hFile, void* Buffer, size_t Bytes)
{
	if (!hFile || !Buffer || Bytes == 0) return 0;
	PNativeAppDataFile nhFile = PNativeAppDataFile(hFile);
	if (nhFile->MagicNumber != APPDATA_FILE_MAGIC_NUMBER) return 0;
	DWORD Read = 0;
	if (!ReadFile(nhFile->hFile, Buffer, Bytes, &Read, nullptr)) return 0;
	return size_t(Read);
}

size_t AppDataWriteFile(HAPPFILE hFile, const void* Data, size_t Bytes)
{
	if (!hFile || !Data || Bytes == 0) return 0;
	PNativeAppDataFile nhFile = PNativeAppDataFile(hFile);
	if (nhFile->MagicNumber != APPDATA_FILE_MAGIC_NUMBER) return 0;
	if (nhFile->OpenMode | APPDATA_APPEND_ALWAYS) SetFilePointerEx(nhFile->hFile, { 0 }, nullptr, FILE_END);
	DWORD Written = 0;
	if (!WriteFile(nhFile->hFile, Data, Bytes, &Written, nullptr)) return 0;
	return size_t(Written);
}

size_t AppDataMoveFilePointer(HAPPFILE hFile, size_t MoveBytes, int Beginning)
{
	if (!hFile) return 0;
	PNativeAppDataFile nhFile = PNativeAppDataFile(hFile);
	if (nhFile->MagicNumber != APPDATA_FILE_MAGIC_NUMBER) return 0;
	LARGE_INTEGER Move{}, Result{};
	Move.QuadPart = MoveBytes;
	if (!SetFilePointerEx(nhFile->hFile, Move, &Result, Beginning)) return size_t(-1);
	return size_t(Result.QuadPart);
}

size_t AppDataGetFilePointer(HAPPFILE hFile)
{
	if (!hFile) return 0;
	PNativeAppDataFile nhFile = PNativeAppDataFile(hFile);
	if (nhFile->MagicNumber != APPDATA_FILE_MAGIC_NUMBER) return 0;
	LARGE_INTEGER Move{}, Result{};
	Move.QuadPart = 0;
	if (!SetFilePointerEx(nhFile->hFile, Move, &Result, FILE_CURRENT)) return size_t(-1);
	return size_t(Result.QuadPart);
}

ubool AppDataCloseHandle(HAPPDATA hAppData)
{
	if (!hAppData) return 0;
	uint32_t* MagicNumber = (uint32_t*)hAppData;
	if (*MagicNumber == APPDATA_DIR_MAGIC_NUMBER) {
		PNativeAppDataDir nhDir = PNativeAppDataDir(hAppData);
		delete nhDir;
	}
	else if (*MagicNumber == APPDATA_FILE_MAGIC_NUMBER) {
		PNativeAppDataFile nhFile = PNativeAppDataFile(hAppData);
		CloseHandle(nhFile->hFile);
		delete nhFile;
	}
	else return 0;
	return 1;
}

ubool AppDataGeneratePathA(HAPPDIR pDir, const char* FileName, char* Buffer, size_t Buflen)
{
	if (!pDir || !FileName || !Buffer || Buflen == 0) return 0;
	if (std::regex_match(FileName, regFileName)) return 0;
	PNativeAppDataDir npDir = PNativeAppDataDir(pDir);
	if (npDir->MagicNumber != APPDATA_DIR_MAGIC_NUMBER) return 0;
	std::filesystem::path Path = npDir->DirPath / FileName;
	if (Buflen <= Path.string().size()) return 0;
	strcpy(Buffer, Path.string().c_str());
	Buffer[Path.string().size()] = 0;
	return 1;
}

ubool AppDataGeneratePathW(HAPPDIR pDir, const wchar_t* FileName, wchar_t* Buffer, size_t Buflen)
{
	if (!pDir || !FileName || !Buffer || Buflen == 0) return 0;
	if (std::regex_match(FileName, wregFileName)) return 0;
	PNativeAppDataDir npDir = PNativeAppDataDir(pDir);
	if (npDir->MagicNumber != APPDATA_DIR_MAGIC_NUMBER) return 0;
	std::filesystem::path Path = npDir->DirPath / FileName;
	if (Buflen <= Path.wstring().size()) return 0;
	wcscpy(Buffer, Path.wstring().c_str());
	Buffer[Path.wstring().size()] = 0;
	return 1;
}
