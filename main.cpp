#include <windows.h>
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
    HWND hwndButton1 = CreateWindow( //Кнопка для запуска ворда
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
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            if(wParam == button_word)
            {
                ShellExecute(nullptr, "open","text.docx",  nullptr, nullptr, SW_RESTORE);
            }
        }
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
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
