#include <windows.h>
#include <windowsx.h>

#define IDW_DRAWING_AREA    100

#define IDB_PEN_TOOL        101
#define IDB_LINE_TOOL       102
#define IDB_RECTANGLE_TOOL  103
#define IDB_ELLIPSE_TOOL    104
#define IDB_BEZIER_TOOL     105
#define IDB_ERASER_TOOL     106

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void UpdateColorPreview(HDC, COLORREF, int, int);
bool IsInArea(POINT, RECT);
int GetIntValue(HWND);
POINT AdjustDrawLimits(POINT, RECT, int);
HRGN GetLastEllipticRegion(RECT, int, BOOL*);
HRGN GetLastRectRegion(RECT, int, BOOL*);

char szClassName[ ] = "ClassLab3";
HINSTANCE hInstance;

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = 0;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if(!RegisterClassEx(&wincl)) return 0;

    hwnd = CreateWindowEx (
        0,
        szClassName,
        "Lab 3 Project",
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX),
        CW_USEDEFAULT, CW_USEDEFAULT,
        780, 450,
        HWND_DESKTOP,
        NULL,
        hThisInstance,
        NULL);

    ShowWindow (hwnd, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0))
    {
      TranslateMessage(&messages);
      DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndEraserWeight;
    static HWND hWndStrokeWeight;
    static HWND hWndPenTool;
    static HWND hWndLineTool;
    static HWND hWndRectanTool;
    static HWND hWndEllipseTool;
    static HWND hWndBezierTool;
    static HWND hWndEraserTool;
    static HWND hWndFillCheck;

    // Drawing area rectangle
    static RECT rcDrawingArea = {10, 15, 610, 415};

    // Drawing tools
    static POINT ptStartPen;

    static BOOL bDrawingLine;
    static POINT newLine;

    static BOOL bDrawingRectangle;
    static RECT rcNewRect;

    static BOOL bDrawingEllipse;
    static RECT rcNewEllipse;

    static BOOL bDrawingBezierFirst;
    static BOOL bDrawingBezierSecond;
    static BOOL bDrawingBezierThird;
    static POINT ptBezierPoints[4];

    static HRGN hRgLastRegion;
    static BOOL bCanDeleteRegion;

    POINT ptMouse;

    UINT fillRED      = 255;
    UINT fillGREEN    = 255;
    UINT fillBLUE     = 255;
    UINT strokeRED    = 0;
    UINT strokeGREEN  = 0;
    UINT strokeBLUE   = 0;

    // Rectangles for color preview
    HBRUSH hBrush;
    int xFillPreview   = 635;
    int yFillPreview   = 47;
    int xStrokePreview = 698;
    int yStrokePreview = 47;

    // The bars for the color picker rectangles
    RECT rectRED     = {640, 112, 665, 172};
    RECT rectGREEN   = {680, 112, 705, 172};
    RECT rectBLUE    = {720, 112, 745, 172};
    RECT rectTemp;

    HDC hdc = GetDC(hwnd);
    COLORREF strokeRGB;
    int stroke_weight;
    COLORREF fillRGB;
    HBRUSH fillBrush;
    PAINTSTRUCT ps;
    HPEN strokePen;
    POINT point;
    RECT rect;

    // Displaying the bitmap image
    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldBitmap;
    HBITMAP hbmpImage = NULL;
    hbmpImage = (HBITMAP)LoadImage(hInstance, "Senju_Clan_Symbol.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmpImage, sizeof(bitmap), &bitmap);

    switch(message)
    {
        case WM_CREATE:
            CreateWindowEx(0, "Button", "Color",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                615, 10, 145, 170,
                hwnd, 0, hInstance, NULL);

            CreateWindowEx(0, "Static", "Fill",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                640, 30, 30, 17,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, "Static", "Stroke",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                700, 30, 40, 17,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, "Static", "R",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                644, 95, 10, 15,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, "Static", "G",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                684, 95, 10, 15,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, "Static", "B",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                724, 95, 10, 15,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, "Button", "",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                615, 210, 145, 50,
                hwnd, 0, hInstance, NULL);

            hWndFillCheck = CreateWindowEx(0, "Button", "Fill",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                620, 230, 50, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, "Static", "Margin",
                WS_VISIBLE | WS_CHILD,
                680, 230, 45, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            hWndStrokeWeight = CreateWindowEx(0, "Edit", "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                730, 232, 20, 19,
                hwnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, "Button", "Tools",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                615, 260, 145, 150,
                hwnd, 0, hInstance, NULL);

            hWndPenTool = CreateWindowEx(0, "Button", "Pen",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                650, 280, 90, 20,
                hwnd, (HMENU)IDB_PEN_TOOL, hInstance, NULL);
            Button_SetCheck(hWndPenTool, BST_CHECKED);

            hWndLineTool = CreateWindowEx(0, "Button", "Line",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                650, 300, 90, 20,
                hwnd, (HMENU)IDB_LINE_TOOL, hInstance, NULL);

            hWndRectanTool = CreateWindowEx(0, "Button", "Rectangle",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                650, 320, 90, 20,
                hwnd, (HMENU)IDB_RECTANGLE_TOOL, hInstance, NULL);

            hWndEllipseTool = CreateWindowEx(0, "Button", "Ellipse",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                650, 340, 90, 20,
                hwnd, (HMENU)IDB_ELLIPSE_TOOL, hInstance, NULL);

            hWndBezierTool = CreateWindowEx(0, "Button", "Bezier",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                650, 360, 90, 20,
                hwnd, (HMENU)IDB_BEZIER_TOOL, hInstance, NULL);

            hWndEraserTool = CreateWindowEx(0, "Button", "Eraser",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE,
                650, 380, 90, 20,
                hwnd, (HMENU)IDB_ERASER_TOOL, hInstance, NULL);

            hWndEraserWeight = CreateWindowEx(0, "Edit", "5",
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                620, 382, 20, 18,
                hwnd, (HMENU)0, hInstance, NULL);

            return 0;

        case WM_LBUTTONDOWN:
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);

            strokeRGB = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);

            // Get the red component of the stroke from the y coord of the point inside the Red Gradient
            if (IsInArea(ptMouse, rectRED))
            {
                strokeRED   = (ptMouse.y - rectRED.top) * 255 / (rectRED.bottom - rectRED.top);
                strokeGREEN = GetGValue(strokeRGB);
                strokeBLUE  = GetBValue(strokeRGB);
                UpdateColorPreview(hdc, RGB(strokeRED, strokeGREEN, strokeBLUE), xStrokePreview, yStrokePreview);
                return 0;
            }

            // Get the green component of the stroke from the y coord of the point inside the Green Gradient 
            if (IsInArea(ptMouse, rectGREEN))
            {
                strokeRED   = GetRValue(strokeRGB);
                strokeGREEN = (ptMouse.y - rectGREEN.top) * 255 / (rectGREEN.bottom - rectGREEN.top);
                strokeBLUE  = GetBValue(strokeRGB);
                UpdateColorPreview(hdc, RGB(strokeRED, strokeGREEN, strokeBLUE), xStrokePreview, yStrokePreview);
                return 0;
            }

            // Get the blue component of the stroke from the y coord of the point inside the Blue Gradient
            if (IsInArea(ptMouse, rectBLUE))
            {
                strokeRED   = GetRValue(strokeRGB);
                strokeGREEN = GetGValue(strokeRGB);
                strokeBLUE  = (ptMouse.y - rectBLUE.top) * 255 / (rectBLUE.bottom - rectBLUE.top);
                UpdateColorPreview(hdc, RGB(strokeRED, strokeGREEN, strokeBLUE), xStrokePreview, yStrokePreview);
                return 0;
            }

            if (IsInArea(ptMouse, rcDrawingArea))
            {
                stroke_weight = GetIntValue(hWndStrokeWeight);
                ptMouse = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hWndPenTool) == BST_CHECKED))
                {
                    ptStartPen = ptMouse;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hWndLineTool) == BST_CHECKED))
                {
                    newLine = ptMouse;
                    bDrawingLine = true;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hWndRectanTool) == BST_CHECKED)) {
                    rcNewRect.left = ptMouse.x;
                    rcNewRect.top = ptMouse.y;
                    bDrawingRectangle = true;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hWndEllipseTool) == BST_CHECKED)) {
                    rcNewEllipse.left = ptMouse.x;
                    rcNewEllipse.top = ptMouse.y;
                    bDrawingEllipse = true;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hWndBezierTool) == BST_CHECKED)) {
                    ptBezierPoints[0] = ptMouse;
                    bDrawingBezierFirst  = true;
                    bDrawingBezierSecond = false;
                    bDrawingBezierThird  = false;
                }
            }
             return 0;

        case WM_LBUTTONUP:
            ptMouse.x       = GET_X_LPARAM(lParam);
            ptMouse.y       = GET_Y_LPARAM(lParam);
            strokeRGB       = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);
            fillRGB         = GetPixel(hdc, xFillPreview + 5, yFillPreview + 5);
            stroke_weight   = GetIntValue(hWndStrokeWeight);

            if(bDrawingLine)
            {
                ptMouse     = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);

                strokePen   = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                SelectObject(hdc, strokePen);
                MoveToEx(hdc, ptMouse.x, ptMouse.y, NULL);
                LineTo(hdc, newLine.x, newLine.y);
                DeleteObject(strokePen);

                bDrawingLine = false;
            }

            if(bDrawingRectangle)
            {
                point               = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);
                rcNewRect.right    = point.x;
                rcNewRect.bottom   = point.y;

                strokePen = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                fillBrush = (Button_GetCheck(hWndFillCheck) == BST_CHECKED)? CreateSolidBrush(fillRGB) : (HBRUSH)GetStockObject(NULL_BRUSH);
                SelectObject(hdc, strokePen);
                SelectObject(hdc, fillBrush);
                Rectangle(hdc, rcNewRect.left, rcNewRect.top, rcNewRect.right, rcNewRect.bottom);
                DeleteObject(strokePen);
                DeleteObject(fillBrush);

                hRgLastRegion = GetLastRectRegion(rcNewRect, stroke_weight, &bCanDeleteRegion);

                bDrawingRectangle = false;
            }

            if(bDrawingEllipse)
            {
                point = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);
                rcNewEllipse.right = point.x;
                rcNewEllipse.bottom = point.y;

                strokePen = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                fillBrush = (Button_GetCheck(hWndFillCheck) == BST_CHECKED)? CreateSolidBrush(fillRGB) : (HBRUSH)GetStockObject(NULL_BRUSH);
                SelectObject(hdc, strokePen);
                SelectObject(hdc, fillBrush);
                Ellipse(hdc, rcNewEllipse.left, rcNewEllipse.top, rcNewEllipse.right, rcNewEllipse.bottom);
                DeleteObject(strokePen);
                DeleteObject(fillBrush);

                hRgLastRegion = GetLastEllipticRegion(rcNewEllipse, stroke_weight, &bCanDeleteRegion);

                bDrawingEllipse = false;
            }

            if(bDrawingBezierFirst)
            {
                point = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);
                ptBezierPoints[1] = point;
                bDrawingBezierFirst  = false;
                bDrawingBezierSecond = true;
                bDrawingBezierThird  = false;
            }
            return 0;

        case WM_RBUTTONDOWN:
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);
            fillRGB = GetPixel(hdc, xFillPreview + 20, yFillPreview + 20);

            if(IsInArea(ptMouse, rectRED))
            {
                fillRED = (ptMouse.y - rectRED.top) * 255 / (rectRED.bottom - rectRED.top);
                fillGREEN = GetGValue(fillRGB);
                fillBLUE = GetBValue(fillRGB);
                UpdateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                return 0;
            }

            if(IsInArea(ptMouse, rectGREEN))
            {
                fillRED = GetRValue(fillRGB);
                fillGREEN = (ptMouse.y - rectGREEN.top) * 255 / (rectGREEN.bottom - rectGREEN.top);
                fillBLUE = GetBValue(fillRGB);
                UpdateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                return 0;
            }

            if(IsInArea(ptMouse, rectBLUE))
            {
                fillRED = GetRValue(fillRGB);
                fillGREEN = GetGValue(fillRGB);
                fillBLUE = (ptMouse.y - rectBLUE.top) * 255 / (rectBLUE.bottom - rectBLUE.top);
                UpdateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                return 0;
            }

            if(IsInArea(ptMouse, rcDrawingArea))
            {
                stroke_weight = GetIntValue(hWndStrokeWeight);
                ptMouse = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);

                // If Bezier tool is selected & first half is selected
                if((wParam == MK_RBUTTON)&&(Button_GetCheck(hWndBezierTool) == BST_CHECKED)&&(bDrawingBezierSecond))
                {
                    ptBezierPoints[2] = ptMouse;
                    bDrawingBezierFirst  = false;
                    bDrawingBezierSecond = false;
                    bDrawingBezierThird  = true;
                }
            }
            if((bCanDeleteRegion)&&(wParam & MK_CONTROL))
            {
                InvalidateRgn(hwnd, hRgLastRegion, TRUE);
                DeleteObject(hRgLastRegion);
                bCanDeleteRegion = false;
            }
            return 0;

        case WM_RBUTTONUP:
            ptMouse.x       = GET_X_LPARAM(lParam);
            ptMouse.y       = GET_Y_LPARAM(lParam);
            strokeRGB       = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);
            fillRGB         = GetPixel(hdc, xFillPreview + 5, yFillPreview + 5);
            stroke_weight   = GetIntValue(hWndStrokeWeight);
            ptMouse         = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);

            if(bDrawingBezierThird)
            {
                ptBezierPoints[3] = ptMouse;
                strokePen       = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                SelectObject(hdc, strokePen);
                PolyBezier(hdc, ptBezierPoints, 4);
                DeleteObject(strokePen);
                bDrawingBezierFirst  = false;
                bDrawingBezierSecond = false;
                bDrawingBezierThird  = false;
            }
            return 0;

        case WM_MOUSEMOVE:
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);

            if(IsInArea(ptMouse, rcDrawingArea))
            {
                strokeRGB = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);
                fillRGB   = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hWndPenTool) == BST_CHECKED)) {
                    strokePen = CreatePen(PS_SOLID, 1, strokeRGB);
                    SelectObject(hdc, strokePen);
                    MoveToEx(hdc, ptMouse.x, ptMouse.y, NULL);
                    LineTo(hdc, ptStartPen.x, ptStartPen.y);
                    DeleteObject(strokePen);
                    ptStartPen = ptMouse;
                }

                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hWndEraserTool) == BST_CHECKED))
                {
                    stroke_weight   = GetIntValue(hWndEraserWeight);
                    ptMouse         = AdjustDrawLimits(ptMouse, rcDrawingArea, stroke_weight);
                    rect.left       = ptMouse.x - (stroke_weight/2);
                    rect.right      = ptMouse.x + (stroke_weight/2);
                    rect.top        = ptMouse.y - (stroke_weight/2);
                    rect.bottom     = ptMouse.y + (stroke_weight/2);
                    InvalidateRect(hwnd, &rect, FALSE);
                    SendMessage(hwnd, WM_PAINT, 0, 0);
                    ValidateRect(hwnd, &rect);
                }
            }
            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);

            UpdateColorPreview(hdc, RGB(255, 255, 255), xFillPreview, yFillPreview);

            UpdateColorPreview(hdc, RGB(0, 0, 0), xStrokePreview, yStrokePreview);

            rectTemp.left    = rectRED.left;
            rectTemp.right   = rectRED.right;
            for(int i = 0; i < (rectRED.bottom - rectRED.top); i++)
            {
                int red;
                red = i * 255 / (rectRED.bottom - rectRED.top);
                rectTemp.top    = rectRED.top  + i;
                rectTemp.bottom = rectRED.top + i + 1;
                hBrush = CreateSolidBrush(RGB(red, 0, 0));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            rectTemp.left    = rectGREEN.left;
            rectTemp.right   = rectGREEN.right;
            for(int i = 0; i < (rectGREEN.bottom - rectGREEN.top); i++)
            {
                int green;
                green = i * 255 / (rectGREEN.bottom - rectGREEN.top);
                rectTemp.top     = rectGREEN.top  + i;
                rectTemp.bottom  = rectGREEN.top + i + 1;
                hBrush = CreateSolidBrush(RGB(0, green, 0));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            rectTemp.left   = rectBLUE.left;
            rectTemp.right  = rectBLUE.right;
            for(int i = 0; i < (rectBLUE.bottom - rectBLUE.top); i++)
            {
                int blue;
                blue = i * 255 / (rectBLUE.bottom - rectBLUE.top);
                rectTemp.top     = rectBLUE.top  + i;
                rectTemp.bottom  = rectBLUE.top + i + 1;
                hBrush = CreateSolidBrush(RGB(0, 0, blue));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0,0,0)));
            SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, rcDrawingArea.left, rcDrawingArea.top, rcDrawingArea.right, rcDrawingArea.bottom);

            // Draw Senju Clan image
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hbmpImage);
            BitBlt(hdc, 640, 180, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);

            // Copy the bits from the memory DC (hdcMem) into the current dc
            BitBlt(hdc,
                rcDrawingArea.right - bitmap.bmWidth - 1,
                rcDrawingArea.bottom - bitmap.bmHeight - 1,
                bitmap.bmWidth, bitmap.bmHeight,
                hdcMem, 0, 0, SRCCOPY);

            // Restore the old bitmap
            DeleteDC(hdcMem);

            EndPaint(hwnd, &ps); // End main window DC paint
            return 0;

        case WM_DESTROY:
            PostQuitMessage (0);
            return 0;

        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

// Function for updating the preview rectangles
void UpdateColorPreview(HDC hdc, COLORREF rgb, int xLeft, int yTop)
{
    HBRUSH hBrush = CreateSolidBrush(rgb);
    SelectObject(hdc, hBrush);
    Rectangle(hdc, xLeft, yTop, xLeft + 45, yTop + 45);
    DeleteObject(hBrush);
}

// Retrieve the input integer from the edit box
int GetIntValue(HWND input)
{
    int result;
    int iLength = SendMessage(input, WM_GETTEXTLENGTH, 0, 0);
    char* szText = (char*)malloc(iLength+1);
    SendMessage(input, WM_GETTEXT, iLength+1, (LPARAM)szText);
    result = atoi(szText);
    _itoa(result, szText, 10);
    SendMessage(input, WM_SETTEXT, 0, (LPARAM)szText);
    free(szText);
    return result;
}

// Adjust the mouse position for drawing according to the limits of the rectangle
// Thus we prevent from drawing outside the Drawing Area
POINT AdjustDrawLimits(POINT ptMouse, RECT limit, int stroke)
{
    POINT result;
    stroke = stroke / 2 + 1;

    if(ptMouse.x - stroke < limit.left)
    {
        result.x = limit.left + stroke;
    } else if(ptMouse.x + stroke > limit.right)
    {
        result.x = limit.right - stroke;
    } else result.x = ptMouse.x;

    if(ptMouse.y - stroke < limit.top)
    {
        result.y = limit.top + stroke;
    } else if(ptMouse.y + stroke > limit.bottom)
    {
        result.y = limit.bottom - stroke;
    } else result.y = ptMouse.y;

    return result;
}

// Returns the last elliptinc region drawn according 
// to the rectangle in which it was filled
HRGN GetLastEllipticRegion(RECT rect, int stroke, BOOL* flag)
{
    HRGN result = CreateEllipticRgn(
        rect.left - (stroke / 2) - 1,
        rect.top - (stroke / 2) - 1,
        rect.right + (stroke / 2) + 1,
        rect.bottom + (stroke / 2) + 1);
    *flag = TRUE;
    return result;
}

// Returns the last rectangular region which was drawn
HRGN GetLastRectRegion(RECT rect, int stroke, BOOL* flag)
{
    HRGN result = CreateRectRgn(
        rect.left - (stroke / 2) - 1,
        rect.top - (stroke / 2) - 1,
        rect.right + (stroke / 2) + 1,
        rect.bottom + (stroke / 2) + 1);
    *flag = TRUE;
    return result;
}

// Verifies if a point is interior of a given rectangle
bool IsInArea(POINT point, RECT rect)
{
    if ((point.x > rect.left) && (point.x < rect.right) &&
        (point.y > rect.top) && (point.y < rect.bottom))
    {
        return true;
    } else return false;
}
