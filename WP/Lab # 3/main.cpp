#include <windows.h>
#include <windowsx.h>

#define IDW_DRAWING_AREA    100

#define IDB_PEN_TOOL        110
#define IDB_LINE_TOOL       111
#define IDB_RECTANGLE_TOOL    112
#define IDB_ELLIPSE_TOOL    113
#define IDB_BEZIER_TOOL     114
#define IDB_ERASER_TOOL     115

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void updateColorPreview(HDC, COLORREF, int, int);
bool isInArea(POINT, RECT);
int getIntValue(HWND);
POINT adjustDrawLimits(POINT, RECT, int);
HRGN getLastEllipticRegion(RECT, int, BOOL*);
HRGN getLastRectRegion(RECT, int, BOOL*);

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
    // Child windows' handles
    static HWND hwndEraserWeight;
    static HWND hwndStrokeWeight;
    static HWND hwndPenTool;
    static HWND hwndLineTool;
    static HWND hwndRectanTool;
    static HWND hwndEllipseTool;
    static HWND hwndBezierTool;
    static HWND hwndEraserTool;
    static HWND hwndFillCheck;

    // Drawing area rectangle
    static RECT drawingArea = {10, 15, 610, 415};

    // Drawing states
    static POINT startPen;

    static BOOL drawingLine;
    static POINT newLine;

    static BOOL drawingRectangle;
    static RECT newPolygon;

    static BOOL drawingEllipse;
    static RECT newEllipse;

    static BOOL drawingBezierFirst;
    static BOOL drawingBezierSecond;
    static BOOL drawingBezierThird;
    static POINT bezierPoints[4];

    static HRGN lastRegion;
    static BOOL canDeleteRegion;

    // Mouse point position
    POINT ptMouse;

    // Color variabes
    UINT fillRED      = 255;
    UINT fillGREEN    = 255;
    UINT fillBLUE     = 255;
    UINT strokeRED    = 0;
    UINT strokeGREEN  = 0;
    UINT strokeBLUE   = 0;

    // Color preview rectangles
    HBRUSH hBrush;
    int xFillPreview   = 635;
    int yFillPreview   = 277;
    int xStrokePreview = 698;
    int yStrokePreview = 277;

    // Color picker rectangles
    RECT rectRED     = {640, 342, 665, 402};
    RECT rectGREEN   = {680, 342, 705, 402};
    RECT rectBLUE    = {720, 342, 745, 402};
    RECT rectTemp;

    // Painting stuff
    HDC hdc = GetDC(hwnd);
    COLORREF strokeRGB;
    int stroke_weight;
    COLORREF fillRGB;
    HBRUSH fillBrush;
    PAINTSTRUCT ps;
    HPEN strokePen;
    POINT point;
    RECT rect;

    // Logo stuff
    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldBitmap;
    HBITMAP hbmpImage = NULL;
    hbmpImage = (HBITMAP)LoadImage(hInstance, "Senju_Clan_Symbol.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hbmpImage, sizeof(bitmap), &bitmap);

    switch(message)
    {
        case WM_CREATE:
            // Tools group box
            CreateWindowEx(0, "Button", "Tools",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                615, 10, 145, 150,
                hwnd, 0, hInstance, NULL);

            // Pen tool
            hwndPenTool = CreateWindowEx(0, "Button", "Pen",
                WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
                620, 30, 90, 20,
                hwnd, (HMENU)IDB_PEN_TOOL, hInstance, NULL);
            Button_SetCheck(hwndPenTool, BST_CHECKED);

            // Line tool
            hwndLineTool = CreateWindowEx(0, "Button", "Line",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                620, 50, 90, 20,
                hwnd, (HMENU)IDB_LINE_TOOL, hInstance, NULL);

            // Polygon tool
            hwndRectanTool = CreateWindowEx(0, "Button", "Rectangle",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                620, 70, 90, 20,
                hwnd, (HMENU)IDB_RECTANGLE_TOOL, hInstance, NULL);

            // Ellipse tool
            hwndEllipseTool = CreateWindowEx(0, "Button", "Ellipse",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                620, 90, 90, 20,
                hwnd, (HMENU)IDB_ELLIPSE_TOOL, hInstance, NULL);

            // Bezier tool
            hwndBezierTool = CreateWindowEx(0, "Button", "Bezier",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                620, 110, 90, 20,
                hwnd, (HMENU)IDB_BEZIER_TOOL, hInstance, NULL);

            // Eraser tool
            hwndEraserTool = CreateWindowEx(0, "Button", "Eraser",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                620, 130, 90, 20,
                hwnd, (HMENU)IDB_ERASER_TOOL, hInstance, NULL);

            // Eraser weight input
            hwndEraserWeight = CreateWindowEx(0, "Edit", "5",
                WS_VISIBLE | WS_CHILD | WS_BORDER,
                730, 133, 20, 19,
                hwnd, (HMENU)0, hInstance, NULL);

            // Style group box
            CreateWindowEx(0, "Button", "",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                615, 190, 145, 50,
                hwnd, 0, hInstance, NULL);

            // Fill checkbox
            hwndFillCheck = CreateWindowEx(0, "Button", "Fill",
                WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
                620, 210, 50, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            // Stroke label
            CreateWindowEx(0, "Static", "Margin",
                WS_VISIBLE | WS_CHILD,
                680, 210, 45, 20,
                hwnd, (HMENU)0, hInstance, NULL);

            // Stroke weight input
            hwndStrokeWeight = CreateWindowEx(0, "Edit", "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
                730, 212, 20, 19,
                hwnd, (HMENU)0, hInstance, NULL);

            // Color group box
            CreateWindowEx(0, "Button", "Color",
                WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                615, 240, 145, 170,
                hwnd, 0, hInstance, NULL);

            // Fill color label
            CreateWindowEx(0, "Static", "Fill",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                640, 260, 30, 17,
                hwnd, (HMENU)0, hInstance, NULL);

            // Stroke color label
            CreateWindowEx(0, "Static", "Stroke",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                700, 260, 40, 17,
                hwnd, (HMENU)0, hInstance, NULL);

            // RED color label
            CreateWindowEx(0, "Static", "R",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                644, 325, 10, 15,
                hwnd, (HMENU)0, hInstance, NULL);

            // GREEN color label
            CreateWindowEx(0, "Static", "G",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                684, 325, 10, 15,
                hwnd, (HMENU)0, hInstance, NULL);

            // BLUE color label
            CreateWindowEx(0, "Static", "B",
                WS_VISIBLE | WS_CHILD | SS_LEFT,
                724, 325, 10, 15,
                hwnd, (HMENU)0, hInstance, NULL);

            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                default:
                    DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
                    break;
            }
            return 0;

        case WM_LBUTTONDOWN:
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);

            strokeRGB = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);

            // If it's RED color picker
            if (isInArea(ptMouse, rectRED))
            {
                strokeRED   = (ptMouse.y - rectRED.top) * 255 / (rectRED.bottom - rectRED.top);
                strokeGREEN = GetGValue(strokeRGB);
                strokeBLUE  = GetBValue(strokeRGB);
                updateColorPreview(hdc, RGB(strokeRED, strokeGREEN, strokeBLUE), xStrokePreview, yStrokePreview);
                return 0;
            }

            // If it's GREEN color picker
            if (isInArea(ptMouse, rectGREEN))
            {
                strokeRED   = GetRValue(strokeRGB);
                strokeGREEN = (ptMouse.y - rectGREEN.top) * 255 / (rectGREEN.bottom - rectGREEN.top);
                strokeBLUE  = GetBValue(strokeRGB);
                updateColorPreview(hdc, RGB(strokeRED, strokeGREEN, strokeBLUE), xStrokePreview, yStrokePreview);
                return 0;
            }

            // If it's BLUE color picker
            if (isInArea(ptMouse, rectBLUE))
            {
                strokeRED   = GetRValue(strokeRGB);
                strokeGREEN = GetGValue(strokeRGB);
                strokeBLUE  = (ptMouse.y - rectBLUE.top) * 255 / (rectBLUE.bottom - rectBLUE.top);
                updateColorPreview(hdc, RGB(strokeRED, strokeGREEN, strokeBLUE), xStrokePreview, yStrokePreview);
                return 0;
            }

            // If it's on drawing area
            if (isInArea(ptMouse, drawingArea))
            {
                stroke_weight = getIntValue(hwndStrokeWeight);
                ptMouse = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);

                //Pen tool selected
                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndPenTool) == BST_CHECKED))
                {
                    startPen = ptMouse;
                }

                // If Line tool is selected
                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndLineTool) == BST_CHECKED))
                {
                    newLine = ptMouse;
                    drawingLine = true;
                }

                // If Rectangle tool is selected
                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndRectanTool) == BST_CHECKED)) {
                    newPolygon.left = ptMouse.x;
                    newPolygon.top = ptMouse.y;
                    drawingRectangle = true;
                }

                // If Ellipse tool is selected
                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndEllipseTool) == BST_CHECKED)) {
                    newEllipse.left = ptMouse.x;
                    newEllipse.top = ptMouse.y;
                    drawingEllipse = true;
                }

                // If Bezier tool is selected
                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndBezierTool) == BST_CHECKED)) {
                    bezierPoints[0] = ptMouse;
                    drawingBezierFirst  = true;
                    drawingBezierSecond = false;
                    drawingBezierThird  = false;
                }
            }
             return 0;

        case WM_LBUTTONUP:
            ptMouse.x       = GET_X_LPARAM(lParam);
            ptMouse.y       = GET_Y_LPARAM(lParam);
            strokeRGB       = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);
            fillRGB         = GetPixel(hdc, xFillPreview + 5, yFillPreview + 5);
            stroke_weight   = getIntValue(hwndStrokeWeight);

            if(drawingLine)
            {
                ptMouse     = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);

                strokePen   = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                SelectObject(hdc, strokePen);
                MoveToEx(hdc, ptMouse.x, ptMouse.y, NULL);
                LineTo(hdc, newLine.x, newLine.y);
                DeleteObject(strokePen);

                drawingLine = false;
            }

            if(drawingRectangle)
            {
                point               = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);
                newPolygon.right    = point.x;
                newPolygon.bottom   = point.y;

                strokePen = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                fillBrush = (Button_GetCheck(hwndFillCheck) == BST_CHECKED)? CreateSolidBrush(fillRGB) : (HBRUSH)GetStockObject(NULL_BRUSH);
                SelectObject(hdc, strokePen);
                SelectObject(hdc, fillBrush);
                Rectangle(hdc, newPolygon.left, newPolygon.top, newPolygon.right, newPolygon.bottom);
                DeleteObject(strokePen);
                DeleteObject(fillBrush);

                lastRegion = getLastRectRegion(newPolygon, stroke_weight, &canDeleteRegion);

                drawingRectangle = false;
            }

            if(drawingEllipse)
            {
                point = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);
                newEllipse.right = point.x;
                newEllipse.bottom = point.y;

                strokePen = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                fillBrush = (Button_GetCheck(hwndFillCheck) == BST_CHECKED)? CreateSolidBrush(fillRGB) : (HBRUSH)GetStockObject(NULL_BRUSH);
                SelectObject(hdc, strokePen);
                SelectObject(hdc, fillBrush);
                Ellipse(hdc, newEllipse.left, newEllipse.top, newEllipse.right, newEllipse.bottom);
                DeleteObject(strokePen);
                DeleteObject(fillBrush);

                lastRegion = getLastEllipticRegion(newEllipse, stroke_weight, &canDeleteRegion);

                drawingEllipse = false;
            }

            if(drawingBezierFirst)
            {
                point = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);
                bezierPoints[1] = point;
                drawingBezierFirst  = false;
                drawingBezierSecond = true;
                drawingBezierThird  = false;
            }
            return 0;

        case WM_RBUTTONDOWN:
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);
            fillRGB = GetPixel(hdc, xFillPreview + 20, yFillPreview + 20);

            // If RED color picker
            if(isInArea(ptMouse, rectRED))
            {
                fillRED = (ptMouse.y - rectRED.top) * 255 / (rectRED.bottom - rectRED.top);
                fillGREEN = GetGValue(fillRGB);
                fillBLUE = GetBValue(fillRGB);
                updateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                return 0;
            }

            // If GREEN color picker
            if(isInArea(ptMouse, rectGREEN))
            {
                fillRED = GetRValue(fillRGB);
                fillGREEN = (ptMouse.y - rectGREEN.top) * 255 / (rectGREEN.bottom - rectGREEN.top);
                fillBLUE = GetBValue(fillRGB);
                updateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                return 0;
            }

            // If BLUE color picker
            if(isInArea(ptMouse, rectBLUE))
            {
                fillRED = GetRValue(fillRGB);
                fillGREEN = GetGValue(fillRGB);
                fillBLUE = (ptMouse.y - rectBLUE.top) * 255 / (rectBLUE.bottom - rectBLUE.top);
                updateColorPreview(hdc, RGB(fillRED, fillGREEN, fillBLUE), xFillPreview, yFillPreview);
                return 0;
            }

            if(isInArea(ptMouse, drawingArea))
            {
                stroke_weight = getIntValue(hwndStrokeWeight);
                ptMouse = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);

                // If Bezier tool is selected & first half is selected
                if((wParam == MK_RBUTTON)&&(Button_GetCheck(hwndBezierTool) == BST_CHECKED)&&(drawingBezierSecond))
                {
                    bezierPoints[2] = ptMouse;
                    drawingBezierFirst  = false;
                    drawingBezierSecond = false;
                    drawingBezierThird  = true;
                }
            }
            if((canDeleteRegion)&&(wParam & MK_CONTROL && wParam & MK_SHIFT)) {
                InvalidateRgn(hwnd, lastRegion, TRUE);
                DeleteObject(lastRegion);
                canDeleteRegion = false;
            }
            return 0;

        case WM_RBUTTONUP:
            ptMouse.x       = GET_X_LPARAM(lParam);
            ptMouse.y       = GET_Y_LPARAM(lParam);
            strokeRGB       = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);
            fillRGB         = GetPixel(hdc, xFillPreview + 5, yFillPreview + 5);
            stroke_weight   = getIntValue(hwndStrokeWeight);
            ptMouse         = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);

            if(drawingBezierThird)
            {
                bezierPoints[3] = ptMouse;
                strokePen       = CreatePen(PS_SOLID, stroke_weight, strokeRGB);
                SelectObject(hdc, strokePen);
                PolyBezier(hdc, bezierPoints, 4);
                DeleteObject(strokePen);
                drawingBezierFirst  = false;
                drawingBezierSecond = false;
                drawingBezierThird  = false;
            }
            return 0;

        case WM_MOUSEMOVE:
            ptMouse.x = GET_X_LPARAM(lParam);
            ptMouse.y = GET_Y_LPARAM(lParam);

            // Check if on the drawing area
            if(isInArea(ptMouse, drawingArea))
            {
                strokeRGB = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);
                fillRGB   = GetPixel(hdc, xStrokePreview + 5, yStrokePreview + 5);

                // If Pen tool is selected
                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndPenTool) == BST_CHECKED)) {
                    strokePen = CreatePen(PS_SOLID, 1, strokeRGB);
                    SelectObject(hdc, strokePen);
                    MoveToEx(hdc, ptMouse.x, ptMouse.y, NULL);
                    LineTo(hdc, startPen.x, startPen.y);
                    DeleteObject(strokePen);
                    startPen = ptMouse;
                }

                // If Eraser tool is selected
                if((wParam == MK_LBUTTON)&&(Button_GetCheck(hwndEraserTool) == BST_CHECKED))
                {
                    stroke_weight   = getIntValue(hwndEraserWeight);
                    ptMouse         = adjustDrawLimits(ptMouse, drawingArea, stroke_weight);
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

            // Fill color preview rectangle
            updateColorPreview(hdc, RGB(255, 255, 255), xFillPreview, yFillPreview);

            // Stroke color preview rectangle
            updateColorPreview(hdc, RGB(0, 0, 0), xStrokePreview, yStrokePreview);

            // Gradient for RED color picker
            rectTemp.left    = rectRED.left;
            rectTemp.right   = rectRED.right;
            for(int i = 0; i < (rectRED.bottom - rectRED.top); i++)
            {
                int r;
                r = i * 255 / (rectRED.bottom - rectRED.top);
                rectTemp.top    = rectRED.top  + i;
                rectTemp.bottom = rectRED.top + i + 1;
                hBrush = CreateSolidBrush(RGB(r, 0, 0));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            // Gradient for GREEN color picker
            rectTemp.left    = rectGREEN.left;
            rectTemp.right   = rectGREEN.right;
            for(int i = 0; i < (rectGREEN.bottom - rectGREEN.top); i++)
            {
                int g;
                g = i * 255 / (rectGREEN.bottom - rectGREEN.top);
                rectTemp.top     = rectGREEN.top  + i;
                rectTemp.bottom  = rectGREEN.top + i + 1;
                hBrush = CreateSolidBrush(RGB(0, g, 0));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            // Gradient for BLUE color picker
            rectTemp.left   = rectBLUE.left;
            rectTemp.right  = rectBLUE.right;
            for(int i = 0; i < (rectBLUE.bottom - rectBLUE.top); i++)
            {
                int b;
                b = i * 255 / (rectBLUE.bottom - rectBLUE.top);
                rectTemp.top     = rectBLUE.top  + i;
                rectTemp.bottom  = rectBLUE.top + i + 1;
                hBrush = CreateSolidBrush(RGB(0, 0, b));
                FillRect(hdc, &rectTemp, hBrush);
                DeleteObject(hBrush);
            }

            // Blank draw area
            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0,0,0)));
            SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, drawingArea.left, drawingArea.top, drawingArea.right, drawingArea.bottom);

            // Draw image
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hbmpImage);
            BitBlt(hdc, 640, 160, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);

            // Copy the bits from the memory DC into the current dc
            BitBlt(hdc,
                drawingArea.right - bitmap.bmWidth - 1,
                drawingArea.bottom - bitmap.bmHeight - 1,
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

void updateColorPreview(HDC hdc, COLORREF rgb, int xLeft, int yTop)
{
    HBRUSH hBrush = CreateSolidBrush(rgb);
    SelectObject(hdc, hBrush);
    Rectangle(hdc, xLeft, yTop, xLeft + 45, yTop + 45);
    DeleteObject(hBrush);
}

int getIntValue(HWND input)
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

POINT adjustDrawLimits(POINT ptMouse, RECT limit, int stroke)
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

HRGN getLastEllipticRegion(RECT rect, int stroke, BOOL* flag)
{
    HRGN result = CreateEllipticRgn(
        rect.left - (stroke / 2) - 1,
        rect.top - (stroke / 2) - 1,
        rect.right + (stroke / 2) + 1,
        rect.bottom + (stroke / 2) + 1);
    *flag = TRUE;
    return result;
}

HRGN getLastRectRegion(RECT rect, int stroke, BOOL* flag)
{
    HRGN result = CreateRectRgn(
        rect.left - (stroke / 2) - 1,
        rect.top - (stroke / 2) - 1,
        rect.right + (stroke / 2) + 1,
        rect.bottom + (stroke / 2) + 1);
    *flag = TRUE;
    return result;
}

bool isInArea(POINT point, RECT rect)
{
    if ((point.x > rect.left) && (point.x < rect.right) &&
        (point.y > rect.top) && (point.y < rect.bottom))
    {
        return true;
    } else return false;
}
