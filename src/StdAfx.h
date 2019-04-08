#ifndef _STDAFX_H_
#define _STDAFX_H_

#define WIN32_LEAN_AND_MEAN
// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <tchar.h>

#include <Shlwapi.h>
#include <Windowsx.h>
#include <shellapi.h>
#include <shlobj.h>
#include <Commdlg.h>

#include <wincodec.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Shlwapi.lib")

#define HR_CHECK(_hr_) hr = _hr_; if (FAILED(hr)) { goto exit; }
#define BOOL_CHECK(_hr_) if (!(_hr_)) { hr = HRESULT_FROM_WIN32(::GetLastError()); goto exit; }

template <typename T> inline void SafeRelease(T *&pI)
{
    if (nullptr != pI)
    {
        pI->Release();
        pI = nullptr;
    }
}

#include "Resource.h"
#include "Config.h"

#endif // _STDAFX_H_