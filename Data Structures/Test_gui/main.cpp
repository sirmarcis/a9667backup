#include <windows.h>

#define Show(Window) RedrawWindow(Window,0,0,0); ShowWindow(Window,SH_SHOW);

#define AppName "BouncingBall1"
#define Caption "Bouncing Ball..."

char BCX_STR[1024*1024];

static int     BCX_GetDiaUnit;
static int     BCX_cxBaseUnit;
static int     BCX_cyBaseUnit;
static int     BCX_ScaleX;
static int     BCX_ScaleY;
static HANDLE  Form1;

double MIN (double,double);


void FormLoad(HANDLE);
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "CodeBlocksWindowsApp";

double MAX(double a, double b){
    if(a > b)
        return a;
    return b;
}

double MIN(double a, double b){
    if(a < b)
        return a;
    return b;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;
    FormLoad(hThisInstance);
    ShowWindow((HWND)Form1,nCmdShow);
    SetTimer((HWND)Form1,1,50,NULL);


    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


void FormLoad(HANDLE hInst){
    BCX_GetDiaUnit = GetDialogBaseUnits();
    BCX_cxBaseUnit = LOWORD(BCX_GetDiaUnit);
    BCX_cyBaseUnit = HIWORD(BCX_GetDiaUnit);
    BCX_ScaleX = BCX_cxBaseUnit/4;
    BCX_ScaleY = BCX_cyBaseUnit/8;

    Form1 = CreateWindow(AppName,
                         Caption,
                         DS_MODALFRAME|WS_POPUP|WS_CAPTION|WS_SYSMENU,
                         10*BCX_ScaleX,
                         20*BCX_ScaleY,
                         250*BCX_ScaleX,
                         175*BCX_ScaleY, NULL,
                         (HMENU)NULL, (HINSTANCE)hInst,NULL);
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
