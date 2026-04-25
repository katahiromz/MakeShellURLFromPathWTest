#include <windows.h>
#include <stdio.h>

typedef BOOL (WINAPI *FN_MakeShellURLFromPathW)(LPCWSTR lpPath, LPWSTR lpUrl, INT cchMax);

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
    WCHAR szURL[MAX_PATH];
    BOOL ret = fnMakeShellURLFromPathW(wargv[1], szURL, MAX_PATH);
    CHAR szText[MAX_PATH];
    WideCharToMultiByte(CP_ACP, 0, szURL, -1, szText, MAX_PATH, NULL, NULL);
    printf("%s: %d\n", szText, ret);
    FreeLibrary(hShell32);
    puts("OK");
    return 0;
}
