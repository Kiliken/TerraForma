#include <windows.h>
#include <glad/gl.h>
#include <tchar.h>

// OpenGL
typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int *);
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

static void* Win32OpenGLLoader(const char* name)
{
    // 1. Core OpenGL 1.1 functions are exported directly by opengl32.dll
    static HMODULE hOpengl32 = LoadLibraryA("opengl32.dll");
    void* proc = (void*)GetProcAddress(hOpengl32, name);

    // 2. Modern OpenGL 1.2+ functions / extensions use wglGetProcAddress
    if (!proc) {
        proc = (void*)wglGetProcAddress(name);
    }
    
    return proc;
}

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
                   _T("Call to RegisterClassEx failed!"),
                   _T("Windows Desktop Guided Tour"),
                   NULL);

        return 1;
    }

    // Store instance
    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        720, 480,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hWnd)
    {
        MessageBox(NULL,
                   _T("Call to CreateWindowEx failed!"),
                   _T("Windows Desktop Guided Tour"),
                   NULL);

        return 1;
    }

    // Glad Initialization
    HDC hdc = GetDC(hWnd);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 24;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    // Create a temporary legacy context to fetch the wglCreateContextAttribsARB extension pointer
    HGLRC tempContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempContext);

    auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    // Clean up temporary context
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempContext);

    if (!wglCreateContextAttribsARB)
    {
        MessageBox(NULL, _T("Modern OpenGL (WGL) not supported!"), _T("Error"), MB_OK);
        return 1;
    }

    // Request a real OpenGL 4.6 Core Profile Context
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0};

    HGLRC glContext = wglCreateContextAttribsARB(hdc, 0, attribs);
    if (!glContext)
    {
        MessageBox(NULL, _T("Failed to create OpenGL 4.6 Core context!"), _T("Error"), MB_OK);
        return 1;
    }

    wglMakeCurrent(hdc, glContext);

    // Initialize GLAD using a custom proc address fetcher that checks both wglGetProcAddress and opengl32.dll
    if (!gladLoadGL((GLADloadfunc)Win32OpenGLLoader))
    {
        MessageBox(NULL, _T("Failed to initialize GLAD!"), _T("Error"), MB_OK);
        return 1;
    }

    // Output success confirmation to Visual Studio Output window
    OutputDebugString(_T("OpenGL 4.6 and GLAD Initialized Successfully!\n"));

    ShowWindow(hWnd,nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        TextOut(hdc,
                5, 5,
                greeting, static_cast<int>(_tcslen(greeting)));

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}