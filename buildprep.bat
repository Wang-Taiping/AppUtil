@echo off

if not exist AppUtil.sln (
	echo Folder error.
	exit 1
)

rmdir /s /q libcurl\curl >nul 2>nul
git clone --branch curl-8_6_0 --single-branch --depth=1 https://github.com/curl/curl.git libcurl\curl

rmdir /s /q openssl >nul 2>nul
git clone --branch openssl-3_3_1 --single-branch --depth=1 https://github.com/Wang-Taiping/Library.git openssl

cmd /c libcurl\curl\buildconf.bat

exit 0