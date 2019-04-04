/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

class ModelLoader
{
public:
    static unsigned char *loadFile(const char *filepath, int *ret_bufsize)
    {
        HINTERNET hInst = ::InternetOpenA("Mozilla/5.0 Dango", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, WININET_API_FLAG_SYNC);
        HINTERNET hReq = ::InternetOpenUrlA(hInst, filepath, nullptr, 0, INTERNET_REQFLAG_FROM_CACHE, 0);
        DWORD dwRead = 0, dwSize = sizeof(DWORD);
        if (!::HttpQueryInfoA(hReq, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwRead, &dwSize, nullptr))
            dwRead = 4096;

        PBYTE pData = (PBYTE)malloc(dwRead);
        DWORD dwRecived = 0;

        while (::InternetReadFile(hReq, pData + dwRecived, dwRead - dwRecived, &dwSize) && dwSize > 0)
        {
            dwRecived += dwSize;
        }
        *ret_bufsize = dwRecived;

        ::InternetCloseHandle(hReq);
        ::InternetCloseHandle(hInst);
        return pData;
    }

    static void releaseBuffer(void* ptr)
    {
        free(ptr);
    }

};
