#include <windows.h>
#include <stdio.h>

typedef BOOL (WINAPI *FN_MakeShellURLFromPathW)(LPCWSTR lpPath, LPWSTR lpUrl, INT cchMax);

FN_MakeShellURLFromPathW g_fnMakeShellURLFromPathW;

BOOL DoTestEntry(BOOL expect_ret, LPCWSTR lpPath, LPCWSTR lpUrl, INT lineno)
{
    WCHAR szURL[MAX_PATH];
    szURL[0] = '@';
    szURL[1] = 0;
    BOOL ret = g_fnMakeShellURLFromPathW(lpPath, szURL, MAX_PATH);
    if (expect_ret != ret || (ret && lstrcmpiW(szURL, lpUrl) != 0))
    {
        CHAR szText[MAX_PATH];
        WideCharToMultiByte(CP_ACP, 0, szURL, -1, szText, MAX_PATH, NULL, NULL);
        printf("Line %d: %s, %d\n", lineno, szText, ret);
        return FALSE;
    }
    return TRUE;
}

void DoTest()
{
    DoTestEntry(FALSE, L"C:\\Win", L"", __LINE__);
    DoTestEntry(TRUE, L"C:\\Windows", L"shell:Windows", __LINE__);
    DoTestEntry(TRUE, L"C:\\Documents and Settings\\Administrator\\My Documents", L"shell:Personal", __LINE__);
    DoTestEntry(TRUE, L"C:\\Windows\\Fonts", L"shell:Windows\\Fonts", __LINE__);
}

extern "C"
int wmain(int argc, wchar_t **wargv)
{
    HINSTANCE hShell32 = LoadLibraryA("shell32");
    FARPROC fn = GetProcAddress(hShell32, MAKEINTRESOURCEA(721));
    if (!fn)
    {
        puts("NULL");
        return 1;
    }
    FN_MakeShellURLFromPathW fnMakeShellURLFromPathW;
    CopyMemory(&fnMakeShellURLFromPathW, &fn, sizeof(fn));
    g_fnMakeShellURLFromPathW = fnMakeShellURLFromPathW;
    DoTest();
    FreeLibrary(hShell32);
    puts("OK");
    return 0;
}
