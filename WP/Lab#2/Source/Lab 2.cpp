// Lab 2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab 2.h"

#include <iostream>
#include <cstdlib>
#include <io.h>
#include <cstdio>
#include <fcntl.h>
#include <ctime>

using namespace std;

HINSTANCE hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];				// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

ATOM			MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	SubClassListBoxProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void			CreateDeveloperConsole();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPTSTR    lpCmdLine,
			_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));

	srand(time(NULL));

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
	wcex.lpfnWndProc		= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP));
	wcex.hCursor		= LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName		= MAKEINTRESOURCE(IDC_LAB2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= 0;

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindow(szWindowClass, szTitle, 
	   		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VSCROLL | WS_HSCROLL,
			100, 100, 600, 500, 
			NULL, NULL, hInstance, NULL);

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
	HDC hdc;
	static HWND hWndListBox, hScroll, hWndEditBox, hWndAddButton, hWndRemoveButton, hWndClearButton;
	HFONT hFont;
	SCROLLINFO si;
	RECT rcClient;
	static int prevScrollY = 0, prevScrollX = 0;
	int iElemID, iTextLength;
	static int clRed, clGreen, clBlue;
	static COLORREF bgColor;
	static char szText[255];

	switch (message)
	{
	case WM_CREATE:
		{
			hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

			RegisterHotKey(hWnd, HK_MOVE_WINDOW, MOD_CONTROL, 0x4D); // Ctrl + M

			RegisterHotKey(hWnd, HK_ERASE_LIST_BOX, MOD_CONTROL, 0x45); // Ctrl + E

			//List Box
			hWndListBox = CreateWindowEx(WS_EX_CLIENTEDGE,
							L"LISTBOX", NULL,
							WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_NOTIFY, 
							230, 50, 150, 150,
							hWnd, (HMENU)IDC_LIST_BOX,
							hInst, NULL);

			hWndEditBox = CreateWindowEx((DWORD)NULL,
							TEXT("Edit"), TEXT(""),
							WS_CHILD | WS_VISIBLE | WS_BORDER,
							50, 50, 150, 50, 
							hWnd,
							(HMENU)IDC_EDIT_BOX,
							hInst,
							NULL);

			hWndAddButton = CreateWindowEx((DWORD)NULL,
							TEXT("Button"),	TEXT("Add"),
							WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
							85, 120, 80, 40, 
							hWnd,
							(HMENU)IDC_BUTTON_ADD,
							hInst,
							NULL);

			hWndRemoveButton = CreateWindowEx((DWORD)NULL,
							TEXT("Button"),	TEXT("Remove"),
							WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
							270, 230, 80, 30, 
							hWnd,
							(HMENU)IDC_BUTTON_REMOVE, 
							hInst,
							NULL);

			hWndClearButton = CreateWindowEx((DWORD)NULL,
							TEXT("Button"),	TEXT("Clear"),
							WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
							270, 270, 80, 30, 
							hWnd,
							(HMENU)IDC_BUTTON_CLEAR, 
							hInst,
							NULL);

			hFont = CreateFont(16, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Bookman Old Style");

			SendMessage(hWndEditBox, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

			SendMessage(hWndListBox, WM_SETFONT, (WPARAM)hFont, TRUE);

			SendMessage(hWndAddButton, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

			SendMessage(hWndRemoveButton, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

			SendMessage(hWndClearButton, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE,0));

			hScroll = CreateWindow(TEXT("SCROLLBAR"), NULL,
						WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_TOPALIGN,
						50, 200, 150, 20,
						hWnd, (HMENU)ID_SCROLLBAR_R,
						hInst, NULL);

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE|SIF_POS;
			si.nMin = 0;
			si.nMax = 255;
			si.nPos = 240;

			SetScrollInfo(hScroll, SB_CTL, &si, TRUE);

			hScroll = CreateWindow(TEXT("SCROLLBAR"), NULL,
						WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_TOPALIGN,
						50, 250, 150, 20,
						hWnd, (HMENU)ID_SCROLLBAR_G,
						hInst, NULL);

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE|SIF_POS;
			si.nMin = 0;
			si.nMax = 255;
			si.nPos = 240;

			SetScrollInfo(hScroll, SB_CTL, &si, TRUE);

			hScroll = CreateWindow(TEXT("SCROLLBAR"), NULL,
						WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_TOPALIGN,
						50, 300, 150, 20,
						hWnd, (HMENU)ID_SCROLLBAR_B,
						hInst, NULL);

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE|SIF_POS;
			si.nMin = 0;
			si.nMax = 255;
			si.nPos = 240;

			SetScrollInfo(hScroll, SB_CTL, &si, TRUE);

			//SetWindowSubclass(hListBox, SubClassListBoxProc, ID_LISTBOX, reinterpret_cast<DWORD_PTR>(1));
			GetClientRect(hWnd, &rcClient);
			

			// ScrollBars for managing the Width and the Height

			HWND hWndScrollWidth = CreateWindow(TEXT("SCROLLBAR"), NULL,
								WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_TOPALIGN,
								1, 1, 
								300, 
								20,
								hWnd, (HMENU)ID_SCROLLBAR_WIDTH,
								hInst, NULL);
			
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE|SIF_POS;
			si.nMin = 150;
			si.nMax = 405;
			si.nPos = 240; 

			SetScrollInfo(hWndScrollWidth, SB_CTL, &si, TRUE);

			HWND hWndScrollHeight = CreateWindow(TEXT("SCROLLBAR"), NULL,
								WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_TOPALIGN,
								1, GetSystemMetrics(SM_CXHSCROLL), 
								20, 
								250,
								hWnd, (HMENU)ID_SCROLLBAR_HEIGHT,
								hInst, NULL);
			
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE|SIF_POS;
			si.nMin = 30;
			si.nMax = 285;
			si.nPos = 240;

			SetScrollInfo(hWndScrollHeight, SB_CTL, &si, TRUE);


			//Default Scroll Bars

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE|SIF_DISABLENOSCROLL;
			si.nMin = 0;
			si.nMax = (rcClient.right - rcClient.left) - GetSystemMetrics(SM_CXVSCROLL) -1;
			 
			SetScrollInfo(hWnd, SB_HORZ, &si, FALSE);

			si.fMask = SIF_RANGE|SIF_DISABLENOSCROLL;
			si.nMin = 0;
			si.nMax = (rcClient.bottom - rcClient.top) - GetSystemMetrics(SM_CYHSCROLL) -1;

			SetScrollInfo(hWnd, SB_VERT, &si, FALSE);
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		switch (wmId)
		{
		case IDC_LIST_BOX:
			switch(wmEvent)
			{
			case LBN_DBLCLK:
				iElemID = SendMessage(hWndListBox, LB_GETCURSEL, 0, 0);
				if(iElemID != LB_ERR)
				{
					SendMessage(hWndListBox, LB_DELETESTRING, iElemID, 0);
				}
				break;
			}
			break;
			 
		case IDC_BUTTON_ADD: 
			iTextLength = SendMessage(hWndEditBox, WM_GETTEXTLENGTH, 0, 0);
			SendMessage(hWndEditBox, WM_GETTEXT, iTextLength+1, (LPARAM)szText);
			SendMessage(hWndListBox, LB_ADDSTRING, 0, (LPARAM)szText);
			SendMessage(hWndEditBox, WM_SETTEXT, 0, (LPARAM)"");
			break;

		case IDC_BUTTON_REMOVE:
			iElemID = SendMessage(hWndListBox, LB_GETCURSEL, 0, 0);
			if(iElemID != LB_ERR)
			{
				SendMessage(hWndListBox, LB_DELETESTRING, iElemID, 0);
			}
			break;

		case IDC_BUTTON_CLEAR:
			SendMessage(hWndListBox, LB_RESETCONTENT, 0, 0);
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_ADD:
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_ADD, 0), 0);
			break;

		case IDM_CLEAR:
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_CLEAR, 0), 0);
			break;

		case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_SIZE:
		{
			int clientWidth = LOWORD(lParam);
			int clientHeight = HIWORD(lParam);

			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_PAGE | SIF_DISABLENOSCROLL;

			si.nPage = clientWidth;
			SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

			si.nPage = clientHeight;
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);


			si.fMask = SIF_POS;

			GetScrollInfo(hWnd, SB_HORZ, &si);
			int currScrollX = si.nPos;

			GetScrollInfo(hWnd, SB_VERT, &si);
			int currScrollY = si.nPos;

			ScrollWindowEx(hWnd, prevScrollX - currScrollX, prevScrollY - currScrollY,
							NULL, NULL, NULL, NULL,
							SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);

			prevScrollX = currScrollX;
			prevScrollY = currScrollY;
		}
		break;

	case WM_HSCROLL:
		if(lParam)
		{
			int iScrollID = GetDlgCtrlID((HWND)lParam);

			switch (iScrollID)
			{
			case ID_SCROLLBAR_R:
				{
					SCROLLINFO si;
					si.cbSize = sizeof(SCROLLINFO);
					si.fMask = SIF_POS | SIF_RANGE;

					GetScrollInfo((HWND)lParam, SB_CTL, &si);

					switch (LOWORD(wParam))
					{
					case SB_LEFT:
						si.nPos = 0;
						break;

					case SB_RIGHT:
						si.nPos = 255;
						break;

					case SB_LINELEFT:                       
						si.nPos = max(si.nMin, si.nPos-5);
						break;

					case SB_PAGELEFT:
						si.nPos = max(si.nMin, si.nPos-50);
						break;

					case SB_LINERIGHT:
						si.nPos = min(si.nMax, si.nPos+5);
						break;                  

					case SB_PAGERIGHT:
						si.nPos = min(si.nMax, si.nPos+50);
						break;

					case SB_THUMBTRACK:     // fall through
					case SB_THUMBPOSITION:
						si.nPos = (int)HIWORD(wParam);
						break;

					default:
						break;
					}

					clRed = si.nPos;

					bgColor = RGB(clRed, clGreen, clBlue);
					InvalidateRect(hWnd, NULL, TRUE);

					si.fMask = SIF_POS;
					SetScrollInfo((HWND)lParam, SB_CTL, &si, TRUE);
				} 
				break;

			case ID_SCROLLBAR_G:
				{
					SCROLLINFO si;
					si.cbSize = sizeof(SCROLLINFO);
					si.fMask = SIF_POS | SIF_RANGE;

					GetScrollInfo((HWND)lParam, SB_CTL, &si);

					switch (LOWORD(wParam))
					{
					case SB_LEFT:
						si.nPos = 0;
						break;

					case SB_RIGHT:
						si.nPos = 255;
						break;

					case SB_LINELEFT:                       
						si.nPos = max(si.nMin, si.nPos-5);
						break;

					case SB_PAGELEFT:
						si.nPos = max(si.nMin, si.nPos-50);
						break;

					case SB_LINERIGHT:
						si.nPos = min(si.nMax, si.nPos+5);
						break;                  

					case SB_PAGERIGHT:
						si.nPos = min(si.nMax, si.nPos+50);
						break;

					case SB_THUMBTRACK:     // fall through
					case SB_THUMBPOSITION:
						si.nPos = (int)HIWORD(wParam);
						break;

					default:
						break;
					}

					clGreen = si.nPos;

					bgColor = RGB(clRed, clGreen, clBlue);
					InvalidateRect(hWnd, NULL, TRUE);

					si.fMask = SIF_POS;
					SetScrollInfo((HWND)lParam, SB_CTL, &si, TRUE);
				} 
				break;

			case ID_SCROLLBAR_B:
				{
					SCROLLINFO si;
					si.cbSize = sizeof(SCROLLINFO);
					si.fMask = SIF_POS | SIF_RANGE;

					GetScrollInfo((HWND)lParam, SB_CTL, &si);

					switch (LOWORD(wParam))
					{
					case SB_LEFT:
						si.nPos = 0;
						break;

					case SB_RIGHT:
						si.nPos = 255;
						break;

					case SB_LINELEFT:                       
						si.nPos = max(si.nMin, si.nPos-5);
						break;

					case SB_PAGELEFT:
						si.nPos = max(si.nMin, si.nPos-50);
						break;

					case SB_LINERIGHT:
						si.nPos = min(si.nMax, si.nPos+5);
						break;                  

					case SB_PAGERIGHT:
						si.nPos = min(si.nMax, si.nPos+50);
						break;

					case SB_THUMBTRACK:     // fall through
					case SB_THUMBPOSITION:
						si.nPos = (int)HIWORD(wParam);
						break;

					default:
						break;
					}

					clBlue = si.nPos;

					bgColor = RGB(clRed, clGreen, clBlue);
					InvalidateRect(hWnd, NULL, TRUE);

					si.fMask = SIF_POS;
					SetScrollInfo((HWND)lParam, SB_CTL, &si, TRUE);
				} 
				break;

			case ID_SCROLLBAR_WIDTH:
				{
					SCROLLINFO si;
					si.cbSize = sizeof(SCROLLINFO);
					si.fMask = SIF_POS|SIF_RANGE;

					GetScrollInfo((HWND)lParam, SB_CTL, &si);

					switch (LOWORD(wParam))
					{
					case SB_LEFT:
						si.nPos = si.nMin;
						break;

					case SB_RIGHT:
						si.nPos = si.nMax;
						break;

					case SB_LINELEFT:                       
						si.nPos = max(si.nMin, si.nPos-5);
						break;

					case SB_PAGELEFT:
						si.nPos = max(si.nMin, si.nPos-50);
						break;

					case SB_LINERIGHT:
						si.nPos = min(si.nMax, si.nPos+5);
						break;                  

					case SB_PAGERIGHT:
						si.nPos = min(si.nMax, si.nPos+50);
						break;

					case SB_THUMBTRACK:     // fall through
					case SB_THUMBPOSITION:
						si.nPos = (int)HIWORD(wParam);
						break;

					default:
						break;
					}

					si.fMask = SIF_POS;
					SetScrollInfo((HWND)lParam, SB_CTL, &si, TRUE);

					RECT rct;
					GetClientRect(hWnd, &rct);

					rct.left = 0;
					rct.top = 0;
					rct.right = SendMessage(GetDlgItem(hWnd, ID_SCROLLBAR_WIDTH), SBM_GETPOS, 0, 0);
					rct.bottom = SendMessage(GetDlgItem(hWnd, ID_SCROLLBAR_HEIGHT), SBM_GETPOS, 0, 0);

					AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);

					SetWindowPos(hWnd, HWND_TOP, 0, 0, 
						rct.right - rct.left, 
						rct.bottom - rct.top, 
						SWP_NOMOVE|SWP_NOREPOSITION);
				}        
				break;

			default:
				break;
			}
		}
		else
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;

			GetScrollInfo(hWnd, SB_HORZ, &si);

			int currScrollX = si.nPos;
			int maxLimit = si.nMax - si.nPage + 1;

			switch (LOWORD(wParam))  
			{
			case SB_LEFT:
				currScrollX = 0;
				break;

			case SB_RIGHT:
				currScrollX = maxLimit;
				break;

			case SB_LINELEFT:                       
				currScrollX = max(0, si.nPos-5);
				break;

			case SB_PAGELEFT:
				{
					RECT rct;
					GetClientRect(hWnd, &rct);
					currScrollX = max(0, si.nPos - (rct.right - rct.left));
				}               
				break;

			case SB_LINERIGHT:
				currScrollX = min(maxLimit, si.nPos+5);
				break;                  

			case SB_PAGERIGHT:
				{
					RECT rct;
					GetClientRect(hWnd, &rct);
					currScrollX = min(maxLimit, si.nPos + (rct.right -rct.left));
				}               
				break; 

			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				{                   
					currScrollX = HIWORD(wParam); 
				}
				break;

			default:
				break;
			}
			ScrollWindowEx(hWnd, prevScrollX - currScrollX, 0, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
			prevScrollX = currScrollX;
			si.nPos = prevScrollX;
			SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		}
		break;

	case WM_VSCROLL:
		if(lParam)
		{
			int iScrollID = GetDlgCtrlID((HWND)lParam);

			switch (iScrollID)
			{
			case ID_SCROLLBAR_HEIGHT:
				{
					SCROLLINFO si;
					si.cbSize = sizeof(SCROLLINFO);
					si.fMask = SIF_POS|SIF_RANGE;

					GetScrollInfo((HWND)lParam, SB_CTL, &si);

					switch (LOWORD(wParam))
					{
					case SB_TOP:
						si.nPos = si.nMin;
						break;

					case SB_BOTTOM:
						si.nPos = si.nMax;
						break;

					case SB_LINEUP:                       
						si.nPos = max(si.nMin, si.nPos-5);
						break;

					case SB_PAGEUP:
						si.nPos = max(si.nMin, si.nPos-50);
						break;

					case SB_LINEDOWN:
						si.nPos = min(si.nMax, si.nPos+5);
						break;                  

					case SB_PAGEDOWN:
						si.nPos = min(si.nMax, si.nPos+50);
						break;

					case SB_THUMBTRACK:     // fall through
					case SB_THUMBPOSITION:
						si.nPos = (int)HIWORD(wParam);
						break;

					default:
						break;
					}

					si.fMask = SIF_POS;
					SetScrollInfo((HWND)lParam, SB_CTL, &si, TRUE);

					RECT rct;
					rct.left = 0;
					rct.top = 0;
					rct.right = SendMessage(GetDlgItem(hWnd, ID_SCROLLBAR_WIDTH), SBM_GETPOS, 0, 0);
					rct.bottom = SendMessage(GetDlgItem(hWnd, ID_SCROLLBAR_HEIGHT), SBM_GETPOS, 0, 0);

					AdjustWindowRectEx(&rct, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_OVERLAPPEDWINDOW);

					SetWindowPos(hWnd, HWND_TOP, 0, 0, 
						rct.right - rct.left, 
						rct.bottom - rct.top + 50, 
						SWP_NOMOVE|SWP_NOREPOSITION);
				}       
				break;

			default:
				break;
			}
		}
		else
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_POS|SIF_RANGE|SIF_PAGE;

			GetScrollInfo(hWnd, SB_VERT, &si);

			int currScrollY = si.nPos;
			int maxLimit = si.nMax - si.nPage + 1;

			switch (LOWORD(wParam))
			{
			case SB_LEFT:
				currScrollY = 0;
				break;

			case SB_RIGHT:
				currScrollY = maxLimit;
				break;

			case SB_LINELEFT:                       
				currScrollY = max(0, si.nPos-5);
				break;

			case SB_PAGELEFT:
				{
					RECT rct;
					GetClientRect(hWnd, &rct);
					currScrollY = max(0, si.nPos - (rct.bottom -rct.top));
				}               
				break;

			case SB_LINERIGHT:
				currScrollY = min(maxLimit, si.nPos+5);
				break;                  

			case SB_PAGERIGHT:
				{
					RECT rct;
					GetClientRect(hWnd, &rct);
					currScrollY = min(maxLimit, si.nPos + (rct.bottom -rct.top));
				}               
				break;

			case SB_THUMBTRACK:
			case SB_THUMBPOSITION:
				{                   
					currScrollY = HIWORD(wParam);
				}
				break;

			default:
				break;
			}

			ScrollWindowEx(hWnd, 0, prevScrollY - currScrollY, NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
			prevScrollY = currScrollY;
			si.nPos = prevScrollY;
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		}
		break;

	case WM_HOTKEY:
		switch (wParam)
		{
		case HK_MOVE_WINDOW: // Ctrl + M
			{
				RECT rcWindow;

				int iScreenW = GetSystemMetrics(SM_CXSCREEN);
				int	iScreenH = GetSystemMetrics(SM_CYSCREEN);

				GetWindowRect(hWnd, &rcWindow);
				SetWindowPos(
						hWnd, 0,
						(iScreenW - rcWindow.right) / 10 * (rand() % 11),
						(iScreenH - rcWindow.bottom) / 10 * (rand() % 11),
						0, 0,
						SWP_NOZORDER|SWP_NOSIZE);
			}
			break;

		case HK_ERASE_LIST_BOX: //Ctrl + E
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_CLEAR, 0), 0);
			break;

		default:
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1)));
			return TRUE;
		}
		break;

	case WM_ERASEBKGND:
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, bgColor);
			HBRUSH hBrush = CreateSolidBrush(bgColor);

			HDC hDC = (HDC)wParam;

			HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

			RECT rct;

			GetClientRect(hWnd, &rct);

			Rectangle(hDC, rct.left, rct.top, rct.right, rct.bottom);

			SelectObject(hDC, hOldPen);
			SelectObject(hDC, hOldBrush);

			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


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
