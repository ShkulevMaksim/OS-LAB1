#include <windows.h>
#include <vector>
#define button_word 1
#define button_run 2
#define button_end 3
#define button_close 4
std::vector<PROCESS_INFORMATION> processes;
std::vector<STARTUPINFO> startupInfo;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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
                    STARTUPINFO si={0};
                    PROCESS_INFORMATION pi={nullptr};
                    si.cb = sizeof(si);
                    if(CreateProcess(nullptr,(LPSTR)"subprocess.exe", nullptr, nullptr, false, DETACHED_PROCESS ,
                                     nullptr, nullptr, &si, &pi)){
                        processes.push_back(pi);
                        startupInfo.push_back(si);
                        CloseHandle(pi.hThread);
                        return DefWindowProc(hwnd, uMsg, wParam, lParam);
                    } else{
                        MessageBox(nullptr,"CreateProcess failed","Error",MB_OK);
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
                            if(!processes.empty()){
                                TerminateProcess(processes.back().hProcess,-1);
                                CloseHandle(processes.back().hProcess);
                                processes.pop_back();
                                startupInfo.pop_back();
                                return DefWindowProc(hwnd, uMsg, wParam, lParam);
                            }

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

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR lpCmdLine, INT nCmdShow)
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
    HWND hwndButtonWord = CreateWindow(
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

    HWND hwndButtonStartProcess = CreateWindow(
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
    HWND hwndButtonEndProcess = CreateWindow(
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

    HWND hwndButtonExit = CreateWindow(
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


