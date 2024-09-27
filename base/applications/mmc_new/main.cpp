#include "precomp.h"
#include "resource.h"

#include "MainFrame.h"

int WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
    HRESULT hr;
    BOOL ret;

    // Initialize COM
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    if (hr != S_OK)
    {
        MessageBox(NULL, _T("Couldn't initialize COM"), NULL, MB_ICONERROR);
        return 1;
    }

    // Initialize Common Controls
    INITCOMMONCONTROLSEX icce;
    icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icce.dwICC = ICC_USEREX_CLASSES | ICC_WIN95_CLASSES;
    ret = InitCommonControlsEx(&icce);

    if (ret == FALSE)
    {
        MessageBox(NULL, TEXT("Couldn't initialize Common Controls!"), NULL, MB_ICONERROR);
        return 1;
    }

    // Create an instance of the main frame
    HMENU hMenuFrame = LoadMenu(hInstance, MAKEINTRESOURCE(IDM_CONSOLE_SMALL));
    CMainFrame wndMain;
    wndMain.Create(0, CWindow::rcDefault, _T("ReactOS Management Console"), 0, 0, hMenuFrame);
    if (!wndMain.m_hWnd)
        return 0;
    
    wndMain.ShowWindow(nCmdShow);
    wndMain.UpdateWindow();

    // Run the message loop
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        if(!wndMain.Translate(&msg, NULL))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    CoUninitialize();
    return msg.wParam;
}