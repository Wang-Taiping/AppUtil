#pragma warning(disable: 4996)

#include "MD5.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <algorithm>
#include <openssl/evp.h>
#include <openssl/md5.h>

#define APPUTIL_MD5_MAGIC_NUMBER 0x0c4d4435
#define APPUTIL_MD5_BUFFER_SIZE  0x2000000

typedef struct NativeMD5
{
	volatile uint32_t MagicNumber;
	bool md5Init;
	bool md5Data;
	EVP_MD_CTX* evpCtx;
	char md5Str[32];
} *PNativeMD5;

HMD5 MD5Create()
{
	PNativeMD5 md5 = new NativeMD5;
	memset(md5, 0, sizeof(NativeMD5));
	md5->MagicNumber = APPUTIL_MD5_MAGIC_NUMBER;
	md5->evpCtx = EVP_MD_CTX_new();
	return HMD5(md5);
}

void MD5Release(HMD5 hMD5)
{
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return;
	md5->MagicNumber = 0;
	if (md5->md5Init) {
		unsigned int len = 0;
		unsigned char result[MD5_DIGEST_LENGTH] = { 0 };
		EVP_DigestFinal_ex(md5->evpCtx, result, &len);
	}
	EVP_MD_CTX_free(md5->evpCtx);
	delete md5;
}

ubool MD5Result(HMD5 hMD5)
{
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	if (md5->md5Init) {
		unsigned int len = 0;
		unsigned char result[MD5_DIGEST_LENGTH] = { 0 };
		EVP_DigestFinal_ex(md5->evpCtx, result, &len);
		for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
			sprintf(md5->md5Str + i * 2, "%02x", result[i]);
		}
		md5->md5Init = false;
		md5->md5Data = true;
		return 1;
	}
	return 0;
}

ubool MD5StrToLowerA(HMD5 hMD5, char* Buffer, size_t Length)
{
	if (Length <= 32) return 0;
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	MD5Result(hMD5);
	if (md5->md5Data) {
		for (size_t i = 0; i < 32; i++) Buffer[i] = md5->md5Str[i];
		Buffer[32] = 0;
		return 1;
	}
	return 0;
}

ubool MD5StrToUpperA(HMD5 hMD5, char* Buffer, size_t Length)
{
	if (Length <= 32) return 0;
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	MD5Result(hMD5);
	if (md5->md5Data) {
		std::string md5str;
		md5str.resize(32, '0');
		for (size_t i = 0; i < 32; i++) md5str[i] = md5->md5Str[i];
		std::transform(md5str.begin(), md5str.end(), md5str.begin(), std::toupper);
		for (size_t i = 0; i < 32; i++) Buffer[i] = md5str[i];
		Buffer[32] = 0;
		return 1;
	}
	return 0;
}

ubool MD5StrToLowerW(HMD5 hMD5, wchar_t* Buffer, size_t Length)
{
	if (Length <= 32) return 0;
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	MD5Result(hMD5);
	if (md5->md5Data) {
		for (size_t i = 0; i < 32; i++) Buffer[i] = md5->md5Str[i];
		Buffer[32] = 0;
		return 1;
	}
	return 0;
}

ubool MD5StrToUpperW(HMD5 hMD5, wchar_t* Buffer, size_t Length)
{
	if (Length <= 32) return 0;
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	MD5Result(hMD5);
	if (md5->md5Data) {
		std::wstring md5str;
		md5str.resize(32, '0');
		for (size_t i = 0; i < 32; i++) md5str[i] = md5->md5Str[i];
		std::transform(md5str.begin(), md5str.end(), md5str.begin(), std::toupper);
		for (size_t i = 0; i < 32; i++) Buffer[i] = md5str[i];
		Buffer[32] = 0;
		return 1;
	}
	return 0;
}

ubool MD5HashFileA(HMD5 hMD5, const char* File)
{
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	HANDLE hFile = CreateFileA(File, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	if (!md5->md5Init) {
		EVP_DigestInit_ex(md5->evpCtx, EVP_md5(), nullptr);
		md5->md5Init = true;
		md5->md5Data = false;
	}
	char* Buffer = new char[APPUTIL_MD5_BUFFER_SIZE];
	DWORD Readed = 0;
	do {
		if (!ReadFile(hFile, Buffer, APPUTIL_MD5_BUFFER_SIZE, &Readed, nullptr)) break;
		if (Readed != 0) EVP_DigestUpdate(md5->evpCtx, Buffer, Readed);
	} while (Readed != 0);
	CloseHandle(hFile);
	delete[] Buffer;
	return 1;
}

ubool MD5HashFileW(HMD5 hMD5, const wchar_t* File)
{
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	HANDLE hFile = CreateFileW(File, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	if (!md5->md5Init) {
		EVP_DigestInit_ex(md5->evpCtx, EVP_md5(), nullptr);
		md5->md5Init = true;
		md5->md5Data = false;
	}
	char* Buffer = new char[APPUTIL_MD5_BUFFER_SIZE];
	DWORD Readed = 0;
	do {
		if (!ReadFile(hFile, Buffer, APPUTIL_MD5_BUFFER_SIZE, &Readed, nullptr)) break;
		if (Readed != 0) EVP_DigestUpdate(md5->evpCtx, Buffer, Readed);
	} while (Readed != 0);
	CloseHandle(hFile);
	delete[] Buffer;
	return 1;
}

ubool MD5HashData(HMD5 hMD5, const void* Data, size_t Bytes)
{
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	if (Bytes == 0) return 0;
	if (!md5->md5Init) {
		EVP_DigestInit_ex(md5->evpCtx, EVP_md5(), nullptr);
		md5->md5Init = true;
		md5->md5Data = false;
	}
	EVP_DigestUpdate(md5->evpCtx, Data, Bytes);
	return 1;
}

ubool MD5HashCheckStrA(HMD5 hMD5, const char* MD5HashStr)
{
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	if (!MD5HashStr || !md5->md5Data) return 0;
	std::string md5str1, md5str2 = MD5HashStr;
	md5str1.resize(32, '0');
	for (size_t i = 0; i < 32; i++) md5str1[i] = md5->md5Str[i];
	std::transform(md5str2.begin(), md5str2.end(), md5str2.begin(), std::tolower);
	return md5str1 == md5str2;
}

ubool MD5HashCheckStrW(HMD5 hMD5, const wchar_t* MD5HashStr)
{
	PNativeMD5 md5 = PNativeMD5(hMD5);
	if (md5->MagicNumber != APPUTIL_MD5_MAGIC_NUMBER) return 0;
	if (!MD5HashStr || !md5->md5Data) return 0;
	std::wstring md5str1, md5str2 = MD5HashStr;
	md5str1.resize(32, '0');
	for (size_t i = 0; i < 32; i++) md5str1[i] = md5->md5Str[i];
	std::transform(md5str2.begin(), md5str2.end(), md5str2.begin(), std::tolower);
	return md5str1 == md5str2;
}
