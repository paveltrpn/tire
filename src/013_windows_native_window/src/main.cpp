
#include <iostream>

#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <winuser.h>

#include <fmt/format.h>
#include "main.h"

LRESULT kWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_KEYDOWN:
            OnKeyDown(wParam, lParam);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;

            BeginPaint(m_hWnd, &ps);
            OnDraw(ps.hdc);
            EndPaint(m_hWnd,&ps);
        }
        return 0;

        case WM_DESTROY:
            std::cout << "Destroy window \n";
            PostQuitMessage(0);
            return 0;
    };

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK kWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    kWindow *pWindow;

    if (uMsg == WM_NCCREATE) {
        assert (!IsBadReadPtr((void*) lParam, sizeof(CREATESTRUCT)));
        MDICREATESTRUCT *pMDIC = (MDICREATESTRUCT *)((LPCREATESTRUCT) lParam)->lpCreateParams;

        pWindow = (kWindow *)(pMDIC->lParam);

        assert(!IsBadReadPtr(pWindow, sizeof(kWindow)));
        // В оригинальной программе макрос GWL_USERDATA, но он не определён в заголовках,
        // использовал вместо него GWLP_USERDATA
        SetWindowLong(hWnd, GWLP_USERDATA, (LONG)(size_t)pWindow);
    } else {
        // Приведение типов с (size_t) в этом месте снимает warning
        pWindow = (kWindow *)(size_t)GetWindowLong(hWnd, GWLP_USERDATA); 

        if (pWindow)
            return pWindow->WndProc(hWnd, uMsg, wParam, lParam);
        else
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    // Вот тут пришлось побороться, в примере небыло этой строчки, clang++ выдавал
    // предупреждение: "warning: non-void function does not return a value in all control paths"
    // добавил и всё заработало
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool kWindow::RegisterClass(LPCTSTR lpszClass, HINSTANCE hInst) {
    WNDCLASSEX wc;

    if (!GetClassInfoEx(hInst, lpszClass, &wc)) {
        GetWndClassEx(wc);

        wc.hInstance        = hInst;
        wc.lpszClassName    = lpszClass;

        if (!RegisterClassEx(&wc)) {
            return false;
        }
    }
    return true;
}

bool kWindow::CreateEx(DWORD dwExStyle, LPCTSTR lpszClass, LPCTSTR lpszName, DWORD dwStyle,
                       int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, HINSTANCE hInst) {

    if (!RegisterClass(lpszClass, hInst)) {
        std::cout << fmt::format("kWindow::CreateEx(): cannot register class!\n");
        return false;
    }
    
    MDICREATESTRUCT mdic;
    memset(&mdic, 0, sizeof(MDICREATESTRUCT));
    mdic.lParam = (LPARAM)this;

    std::cout << fmt::format("prepare to CreateWindowEx()!\n");
    m_hWnd = CreateWindowEx(dwExStyle, lpszClass, lpszName, dwStyle, x, y, nWidth, nHeight, hParent, hMenu, hInst, &mdic);
    
    if( !m_hWnd ) {
        std::cout << fmt::format("CreateWindowEx() error: {}", GetLastError());
    }

    return m_hWnd;
}

void kWindow::GetWndClassEx(WNDCLASSEX &wc) {
    memset(&wc, 0, sizeof(wc));
    
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = 0;
    wc.lpfnWndProc      = (WNDPROC)WindowProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = nullptr;
    wc.hIcon            = nullptr;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = NULL;
    wc.hIcon            = nullptr;
}

WPARAM kWindow::MessagLoop() {
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

class kHelloWindow : public kWindow {
    private:

        void OnKeyDown(WPARAM wParam, LPARAM lParam) {
            if (wParam == VK_ESCAPE) {
                PostMessage(m_hWnd, WM_CLOSE, 0, 0);
            }
        };

        void OnDraw(HDC hDC) {
        };

    public:
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nShow) {
    kHelloWindow win;
    const TCHAR app_name[] = _T("Native window");

    std::cout << fmt::format("Native window app start!\n");

    win.CreateEx(0, app_name, app_name, WS_POPUP, 
                 GetSystemMetrics(SM_CXSCREEN)/4, 
                 GetSystemMetrics(SM_CXSCREEN)/4,
                 640,
                 480,
                 NULL, NULL, hInst);

    win.ShowWindow(nShow);

    win.UpdateWindow();

    return win.MessagLoop();
}