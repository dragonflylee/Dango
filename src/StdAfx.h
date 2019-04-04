#ifndef _STDAFX_H_
#define _STDAFX_H_

#define STRICT					// 型チェックを厳密に行なう
#define WIN32_LEAN_AND_MEAN		// ヘッダーからあまり使われない関数を省く
#define WINVER        0x0600	// Windows Vista以降対応アプリを指定
#define _WIN32_WINNT  0x0600	// 同上

// ATL 头文件:
#include <atlbase.h>
#include <atlstr.h>

extern CComModule _Module;

#include <atlwin.h>
#include <atlcoll.h>

// Windows 头文件:
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>
#include <wincodec.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <Live2D.h>
#include <Live2DModelD3D11.h>

#define HR_CHECK(_hr_) hr = _hr_; if (FAILED(hr)) { ATLTRACE(TEXT("0x%.8x\n"), hr); goto exit; }
#define BOOL_CHECK(_hr_) if (!(_hr_)) { hr = HRESULT_FROM_WIN32(::GetLastError()); ATLTRACE(TEXT("0x%.8x\n"), hr); goto exit; }

#include "Resource.h"
#include "Config.h"

#endif // _STDAFX_H_