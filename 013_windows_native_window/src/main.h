
#ifndef __main_h_
#define __main_h_

#include <windows.h>

class kWindow
    {
    private:
        virtual void OnDraw(HDC hDC) {
        }

        virtual void OnKeyDown(WPARAM wParam, LPARAM lParam) {
        }

        virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void GetWndClassEx(WNDCLASSEX &wc);

    public:
        HWND m_hWnd;

        kWindow() {
            m_hWnd = nullptr;
        };

        virtual ~kWindow() {
        };

        virtual bool CreateEx(DWORD dwExStyle, LPCTSTR lpszClass, LPCTSTR lpszName, DWORD dwStyle,
                              int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, HINSTANCE hInst);

        bool RegisterClass(LPCTSTR lpszClass, HINSTANCE hInst);
        virtual WPARAM MessagLoop();

        BOOL ShowWindow(int nCmdShow) const {
            return ::ShowWindow(m_hWnd, nCmdShow);
        };

        BOOL UpdateWindow() const {
            return ::UpdateWindow(m_hWnd);
        };
}; 

#endif