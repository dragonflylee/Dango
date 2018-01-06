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
        DEF_INI_ITEM(Left, int, 0)
        DEF_INI_ITEM(Top, int, 0)
        DEF_INI_ITEM(StayOnTop, int, 0)
    END_INI_SECTION()

    BEGIN_INI_SECTION(Widget)
        DEF_INI_ITEM(Left, int, 0)
        DEF_INI_ITEM(Top, int, 0)
        DEF_INI_ITEM(Show, int, 0)
        DEF_INI_ITEM(Alpha, int, 0xFF)
    END_INI_SECTION()
    /**
     * ��ʼ��������
     */
     static HRESULT Init(CAtlString& szName)
    {
        HRESULT hr = S_OK;
        int uSize = MAX_PATH;
        
        // �ӻ������ļ�����·��
        BOOL_CHECK(::SHGetSpecialFolderPath(HWND_DESKTOP, m_szPath, CSIDL_APPDATA, TRUE));
        BOOL_CHECK(::PathCombine(m_szPath, m_szPath, TEXT("Dango.ini")));
        // ��ȡͼ��ڵ�
        do
        {
            uSize *= 2;
            LPTSTR szBuffer = szName.GetBuffer(uSize);
            uSize = ::GetPrivateProfileSectionNames(szBuffer, szName.GetAllocLength(), m_szPath);
        } while (uSize + 2 >= szName.GetAllocLength());
    exit:
        return hr;
    }
private:
    static int GetValue(LPCTSTR szApp, LPCTSTR szName, int nDefault)
    {
        return ::GetPrivateProfileInt(szApp, szName, nDefault, m_szPath);
    }
    static BOOL SetValue(LPCTSTR szApp, LPCTSTR szName, int nValue)
    {
        TCHAR szValue[MAXCHAR] = { 0 };
        _stprintf_s(szValue, TEXT("%d"), nValue);
        return ::WritePrivateProfileString(szApp, szName, szValue, m_szPath);
    }
private:
    static TCHAR m_szPath[MAX_PATH];
};

__declspec(selectany) TCHAR CConfig::m_szPath[MAX_PATH] = { 0 };

#endif // _CONFIG_INI_H_