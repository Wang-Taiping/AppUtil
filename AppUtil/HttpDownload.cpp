#pragma warning(disable: 4996)

#include "HttpDownload.h"
#include <curl/curl.h>
#include <string>
#include <winhttp.h>
#include <io.h>

typedef struct http_meta
{
	CURL* curl;
	std::string url;
	uint64_t total;
	uint64_t speed;
	FILE* file;
	char* buffer;
	uint64_t buflen;
	uint64_t cached;
	uint64_t storaged;
	uint64_t info_last_downloaded;
	HTTPDownloadETA ETA;
	time_t last;
	time_t begin;
	uint32_t timeout;
	bool exit;
	bool retry;
	bool display;
	HttpDownloadCallback callback;
	void* user_data;
} http_meta;

static CURLSH* share_dns = nullptr;
static bool tag_init = false;

static std::string NativeProxy();
static size_t NativeHttpDownload(http_meta* Metadata, bool KeepDisplay);
static int NativeProgress(http_meta* Metadata, curl_off_t Total, curl_off_t Now, curl_off_t, curl_off_t);
static size_t NativeWrite(char* Buffer, size_t Size, size_t nItems, http_meta* Metadata);

void HttpInitialize()
{
	if (tag_init) return;
	curl_global_init(CURL_GLOBAL_ALL);
	share_dns = curl_share_init();
	curl_share_setopt(share_dns, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
	tag_init = true;
}

void HttpUninitialize()
{
	if (!tag_init) return;
	curl_share_cleanup(share_dns);
	curl_global_cleanup();
	tag_init = false;
}

size_t HttpDownload(const char* Url, const char* Path, HttpDownloadCallback Callback, void* UserData, ubool KeepDisplay)
{
	if (!tag_init) return 0;
	http_meta Metadata = { 0 };
	Metadata.url = Url;
	Metadata.file = fopen(Path, "wb");
	if (Metadata.file == nullptr) return 0;
	Metadata.callback = Callback;
	Metadata.user_data = UserData;
	size_t DownloadSize = NativeHttpDownload(&Metadata, KeepDisplay);
	fclose(Metadata.file);
	return DownloadSize;
}

size_t HttpDownloadMemory(const char* Url, void* Buffer, size_t BufBytes, ubool KeepDisplay)
{
	if (!tag_init) return 0;
	http_meta Metadata = { 0 };
	Metadata.url = Url;
	Metadata.buffer = (char*)Buffer;
	Metadata.buflen = BufBytes;
	size_t DownloadSize = NativeHttpDownload(&Metadata, KeepDisplay);
	if (BufBytes > Metadata.cached) Metadata.buffer[Metadata.cached] = 0;
	return DownloadSize;
}

std::string NativeProxy()
{
	WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ProxyConfig = { 0 };
	WinHttpGetIEProxyConfigForCurrentUser(&ProxyConfig);
	char Buffer[512] = { 0 };
	int Retval = 0;
	if (ProxyConfig.lpszProxy) Retval = WideCharToMultiByte(CP_UTF8, 0L, ProxyConfig.lpszProxy, -1, Buffer, 512, nullptr, nullptr);
	return Retval ? Buffer : std::string();
}

size_t NativeHttpDownload(http_meta* Metadata, bool KeepDisplay)
{
	Metadata->display = KeepDisplay;
	Metadata->curl = curl_easy_init();
	curl_easy_setopt(Metadata->curl, CURLOPT_URL, Metadata->url.c_str());
	curl_easy_setopt(Metadata->curl, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(Metadata->curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(Metadata->curl, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(Metadata->curl, CURLOPT_AUTOREFERER, 1L);
	curl_easy_setopt(Metadata->curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(Metadata->curl, CURLOPT_MAXREDIRS, 1L);
	curl_easy_setopt(Metadata->curl, CURLOPT_LOW_SPEED_LIMIT, 102400);
	curl_easy_setopt(Metadata->curl, CURLOPT_LOW_SPEED_TIME, 5);
	curl_easy_setopt(Metadata->curl, CURLOPT_CONNECTTIMEOUT, 3L);
	curl_easy_setopt(Metadata->curl, CURLOPT_XFERINFODATA, Metadata);
	curl_easy_setopt(Metadata->curl, CURLOPT_XFERINFOFUNCTION, NativeProgress);
	curl_easy_setopt(Metadata->curl, CURLOPT_WRITEDATA, Metadata);
	curl_easy_setopt(Metadata->curl, CURLOPT_WRITEFUNCTION, NativeWrite);
	std::string Proxy = NativeProxy();
	if (!Proxy.empty()) {
		curl_easy_setopt(Metadata->curl, CURLOPT_PROXY, Proxy.c_str());
		curl_easy_setopt(Metadata->curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
	}
	CURLcode code = CURLE_OK;
	int retry_times = 0;
	uint64_t last_download = 0;
	char range[32] = { 0 };
retry:
	last_download = Metadata->cached + Metadata->storaged;
	Metadata->cached = 0;
	if (Metadata->storaged != 0) {
		memset(range, 0, 32);
		snprintf(range, 32, "%llu-", Metadata->storaged);
		curl_easy_setopt(Metadata->curl, CURLOPT_RANGE, range);
	}
	Metadata->timeout = 30u;
	time(&Metadata->begin);
	curl_easy_setopt(Metadata->curl, CURLOPT_TIMEOUT, Metadata->timeout);
	code = curl_easy_perform(Metadata->curl);
	if (code != CURLE_OK && Metadata->retry && retry_times < 5) {
		Metadata->retry = false;
		retry_times++;
		goto retry;
	}
	else if (code == CURLE_OPERATION_TIMEDOUT) goto retry;
	return code == CURLE_OK ? Metadata->cached + Metadata->storaged : 0L;
}

int NativeProgress(http_meta* Metadata, curl_off_t Total, curl_off_t Now, curl_off_t, curl_off_t)
{
	time_t now_t = 0;
	time(&now_t);
	if (now_t - Metadata->begin > static_cast<time_t>(Metadata->timeout) - 5) {
		Metadata->timeout += 15;
		curl_easy_setopt(Metadata->curl, CURLOPT_TIMEOUT, Metadata->timeout);
		SetThreadExecutionState((Metadata->display ? ES_DISPLAY_REQUIRED : 0u) | ES_SYSTEM_REQUIRED);
	}
	if (!Metadata->callback) return 0;
	if (now_t - Metadata->last >= 1) {
		Metadata->last = now_t;
		Metadata->speed = Now - Metadata->info_last_downloaded;
		Metadata->info_last_downloaded = Now;
		uint64_t rest = Total - Now;
		Metadata->ETA.Day = rest / 86400u;
		Metadata->ETA.Hour = rest % 86400u / 3600u;
		Metadata->ETA.Min = rest % 86400u % 3600u / 60u;
		Metadata->ETA.Sec = rest % 86400u % 3600u % 60u;
	}
	return Metadata->callback(Total, Now, Total ? (float(Now) / float(Total)) * 100 : 0, Metadata->speed, Metadata->ETA, Metadata->user_data) ? 0 : -1;
}

size_t NativeWrite(char* Buffer, size_t Size, size_t nItems, http_meta* Metadata)
{
	if (Metadata->exit) return 0;
	if (Metadata->file) {
		Metadata->storaged += (Size * nItems);
		return fwrite(Buffer, Size, nItems, Metadata->file);
	}
	else if (Metadata->buffer == nullptr || Metadata->buflen == 0 || Size * nItems > Metadata->buflen - Metadata->cached) {
		Metadata->exit = true;
		return 0;
	}
	memcpy(&Metadata->buffer[Metadata->cached], Buffer, Size * nItems);
	Metadata->cached += (Size * nItems);
	return Size * nItems;
}
