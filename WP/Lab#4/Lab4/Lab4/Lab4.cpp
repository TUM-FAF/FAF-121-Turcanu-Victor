

#include "stdafx.h"
#include "Lab4.h"

#define MAX_LOADSTRING	100
#define ID_TIMER		111

#define Rows	7
#define Columns 7

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void	DrawSquare(HDC hdc, RECT rcClient, int ** field_array);
void	DrawCell(HDC hdc, int value, int x, int y);
void	NextMove(int ** field_array);
void	CreateDeveloperConsole();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;


	CreateDeveloperConsole();

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB4));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB4));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAB4);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc1;
	HDC hdc2;

	int iSpeed = 500;
	static int ** field_array;

	RECT rcClientRect;

	rcClientRect.top = 20;
	rcClientRect.left = 200;
	rcClientRect.bottom = rcClientRect.top + 25 * Rows;
	rcClientRect.right = rcClientRect.left + 25 * Columns;

	switch (message)
	{

	case WM_CREATE:
		{
			field_array = new int*[Rows];
			for (int i = 0; i < Rows; i++)
			{
				field_array[i] = new int[Columns];
				for (int j = 0; j < Columns; j++)
				{
					field_array[i][j] = 0;
				}
			}

			field_array[0][0] = 1;

			SetTimer(hWnd, ID_TIMER, iSpeed, NULL);
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		switch (wmId)
		{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;

			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;

			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_TIMER:

		NextMove(field_array);

		InvalidateRect(hWnd, &rcClientRect, true);

		break;

	case WM_PAINT:
		//hdc2 = GetDC(hWnd);

		hdc1 = BeginPaint(hWnd, &ps);
		DrawSquare(hdc1, rcClientRect, field_array);
		EndPaint(hWnd, &ps);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void NextMove(int ** field_array)
{
	int iC = 0;
	int iR = 0;

	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			if (field_array[i][j] == 1)
			{
				iR = i;
				iC = j;
			}
		}
	}

	std::cout << "Current row and column " << iR << " " << iC << std::endl;

	field_array[iR][iC] = 0;

	if (iC + 1 >= Columns) 
	{
		iC = 0;
		if (iR + 1 >= Rows)
		{
			iR = 0;
		} else
		iR++;
	}
	else 
	{
		iC++;
	}

	field_array[iR][iC] = 1;

	return;
}

void DrawSquare(HDC hdc, RECT rcClient, int ** field_array)
{
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			DrawCell(hdc, field_array[i][j], rcClient.top + i * 25, rcClient.left + j * 25);
		}
	}
	return;
}


void DrawCell(HDC hdc, int value, int iy, int jx)
{
	int xLeft = jx;
	int yTop = iy;

	COLORREF color;

	switch (value)
	{
		case 0:
			color = RGB(10, 25, 75);
			break;

		case 1:
			color = RGB(50, 75, 110);
			std::cout << "Draw row and column " << ((iy - 20)/25) << " " << ((jx - 200)/25) << std::endl;
			break;

		case 2:
			color = RGB(250, 250, 10);
			break;

		case 3:
			color = RGB(255, 50, 10);
			break;

		default:
			color = RGB(255, 250, 250);
			break;
	}

	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN strokePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	SelectObject(hdc, strokePen);
    SelectObject(hdc, hBrush);
	Rectangle(hdc, xLeft, yTop, xLeft + 26, yTop + 26);
    DeleteObject(hBrush);
}



// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void CreateDeveloperConsole()
{
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    int consoleHandleR, consoleHandleW ;
    long stdioHandle;
    FILE *fptr;

    AllocConsole();
    std::wstring strW = L"Dev Console";
    SetConsoleTitle( strW.c_str() );

    EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE , MF_GRAYED);
    DrawMenuBar(GetConsoleWindow());

    GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo );

    stdioHandle = (long)GetStdHandle( STD_INPUT_HANDLE );
    consoleHandleR = _open_osfhandle( stdioHandle, _O_TEXT );
    fptr = _fdopen( consoleHandleR, "r" );
    *stdin = *fptr;
    setvbuf( stdin, NULL, _IONBF, 0 );

    stdioHandle = (long) GetStdHandle( STD_OUTPUT_HANDLE );
    consoleHandleW = _open_osfhandle( stdioHandle, _O_TEXT );
    fptr = _fdopen( consoleHandleW, "w" );
    *stdout = *fptr;
    setvbuf( stdout, NULL, _IONBF, 0 );

    stdioHandle = (long)GetStdHandle( STD_ERROR_HANDLE );
    *stderr = *fptr;
    setvbuf( stderr, NULL, _IONBF, 0 );
}