#ifndef _CONFIG_INI_H_
#define _CONFIG_INI_H_

#define DEF_INI_ITEM(name, type, value) \
    type name() { return GetValue(szApp, TEXT(#name), value); } \
    BOOL name(type v) { return SetValue(szApp, TEXT(#name), v); }

#define BEGIN_INI_SECTION(name) class name { \
private: \
    LPCTSTR szApp; \
public: \
    name(LPCTSTR app = TEXT(#name)): szApp(app) {}
#define END_INI_SECTION() };

class CConfig
{
public:
    BEGIN_INI_SECTION(Main)
        DEF_INI_ITEM(StayOnTop, int, 0)
    END_INI_SECTION()

    BEGIN_INI_SECTION(Widget)
        DEF_INI_ITEM(Left, int, 0)
        DEF_INI_ITEM(Top, int, 0)
        DEF_INI_ITEM(Show, int, 0)
        DEF_INI_ITEM(Alpha, int, 0xFF)
    END_INI_SECTION()
    /**
     * 初始化配置类
     */
     static HRESULT Init(CAtlString& szName)
    {
        HRESULT hr = S_OK;
        DWORD uSize = MAX_PATH;
        // 从获配置文件保存路径
        BOOL_CHECK(::SHGetSpecialFolderPath(HWND_DESKTOP, szPath, CSIDL_APPDATA, TRUE));
        BOOL_CHECK(::PathCombine(szPath, szPath, TEXT("Dango.ini")));
        // 获取图像节点
        while (::GetPrivateProfileSectionNames(szName.GetBuffer(uSize), uSize, szPath) == uSize - 2)
        {
            uSize *= 2;
        }
    exit:
        return hr;
    }
private:
    static int GetValue(LPCTSTR szApp, LPCTSTR szName, int nDefault)
    {
        return ::GetPrivateProfileInt(szApp, szName, nDefault, szPath);
    }
    static BOOL SetValue(LPCTSTR szApp, LPCTSTR szName, int nValue)
    {
        TCHAR szValue[MAXCHAR] = { 0 };
        _stprintf_s(szValue, TEXT("%d"), nValue);
        return ::WritePrivateProfileString(szApp, szName, szValue, szPath);
    }
private:
    static TCHAR szPath[MAX_PATH];
};

__declspec(selectany) TCHAR CConfig::szPath[MAX_PATH] = { 0 };

#endif // _CONFIG_INI_H_