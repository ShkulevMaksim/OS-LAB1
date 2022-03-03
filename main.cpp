#include <windows.h>
#include <iostream>
#define button_word 1
#define button_run 2
#define button_end 3
#define button_close 4
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

    auto CLASS_NAME  = "Window Class";
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window.
    int windowWidth = 800;
    int windowHeight = 600;

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "OS-LAB1",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
            nullptr,
            nullptr,
            hInstance,
            nullptr
    );
    if (hwnd == nullptr)
    {
        return 0;
    }
    int buttonWidth = 100;
    int buttonHeight = 50;
    HWND hwndButton1 = CreateWindow(
            "BUTTON",
            "Run Word",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            windowWidth/2-buttonWidth,
            windowHeight/2-buttonHeight,
            buttonWidth,
            buttonHeight,
            hwnd,
            (HMENU) button_word,
            nullptr,
            nullptr);

    HWND hwndButton2 = CreateWindow(
            "BUTTON",
            "Run Process",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            windowWidth/2-buttonWidth*3,
            windowHeight/2+buttonHeight,
            buttonWidth,
            buttonHeight,
            hwnd,
            (HMENU) button_run,
            nullptr,
            nullptr);
    HWND hwndButton3 = CreateWindow(
            "BUTTON",
            "End Process",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            windowWidth/2-buttonWidth,
            windowHeight/2+buttonHeight,
            buttonWidth,
            buttonHeight,
            hwnd,
            (HMENU) button_end,
            nullptr,
            nullptr);

    HWND hwndButton4 = CreateWindow(
            "BUTTON",
            "Exit",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            windowWidth/2+buttonWidth,
            windowHeight/2+buttonHeight,
            buttonWidth,
            buttonHeight,
            hwnd,
            (HMENU) button_close,
            nullptr,
            nullptr);



    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );;
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            if(wParam == button_word)
            {
                ShellExecute(nullptr, "open","text.docx",  nullptr, nullptr, SW_RESTORE);
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
            else{
                if(wParam == button_run)
                {
                    if(CreateProcess(nullptr,(LPSTR)"subprocess.exe", nullptr, nullptr, false, DETACHED_PROCESS ,
                                     nullptr, nullptr, &si, &pi)){
                        CloseHandle(pi.hThread);
                        return DefWindowProc(hwnd, uMsg, wParam, lParam);
                    } else{
                        printf( "CreateProcess failed (%lu).\n", GetLastError() );
                        return DefWindowProc(hwnd, uMsg, wParam, lParam);
                    }


            }
                else{
                    if(wParam == button_close){
                        PostQuitMessage(0);
                        return 0;
                    }
                    else {
                        if(wParam == button_end){
                            TerminateProcess(pi.hProcess,0);
                            CloseHandle(pi.hThread);
                            return DefWindowProc(hwnd, uMsg, wParam, lParam);
                        }
                    }
                }


            }


        }
        case WM_CLOSE:
            if (MessageBox(hwnd, "Really quit?", "My application", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }

            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;


        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
        }
        default:{
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }



}
