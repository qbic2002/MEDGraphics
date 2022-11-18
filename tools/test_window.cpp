//
// Created by danil on 22.09.2022.
//
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND _hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//    hwnd = _hwnd;
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            switch (wParam) {
//                case VK_ESCAPE:
//                    togglePause();
//                    break;
                case VK_BACK:
                    PostQuitMessage(0);
                    break;
            }
            break;
        case WM_SIZE:
//            windowResize(LOWORD(lParam), HIWORD(lParam));
            break;
        default:
            return DefWindowProc(_hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hwnd;
    //HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex))
        return 0;

    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          768,
                          512,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    ShowWindow(hwnd, nCmdShow);

//    EnableOpenGL(&hRC);
//    glEnable(GL_DEPTH_TEST);

//    init();

    while (!bQuit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
//            update();
//            draw();
            Sleep(16);
        }
    }

//    DisableOpenGL(hRC);
    DestroyWindow(hwnd);
    return msg.wParam;
}