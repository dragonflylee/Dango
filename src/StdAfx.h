#ifndef _STDAFX_H_
#define _STDAFX_H_

// Windows Í·ÎÄ¼þ
#include <windows.h>
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

#define SAFE_RELEASE(p) if (NULL != (p)) { p->Release(); p = NULL; }
#define HR_CHECK(_hr_) hr = _hr_; if (FAILED(hr)) { goto exit; }
#define BOOL_CHECK(_hr_) if (!(_hr_)) { hr = HRESULT_FROM_WIN32(::GetLastError()); goto exit; }

#include "Resource.h"

#endif // _STDAFX_H_