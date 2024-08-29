
#include "framework.h"
#include "WndPicture.h"

LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

TCHAR szFilePath[MAX_PATH];

int nowScale = 4;

float pictureScaleTable[] = {
    0.10,
    0.25,
    0.50,
    0.75,
    1.00,   // 等倍
    1.25,
    1.50,
    2.00,
    2.50,
    3.00
};


void RegisterPictureWnd(HINSTANCE hInst)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcChild;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"PictureWnd";
    wcex.hIconSm = NULL;
    RegisterClassExW(&wcex);
}

void SetBitmapFilePath(TCHAR* pFilePath)
{
    wsprintf(szFilePath, L"%s", pFilePath);
}

int GetNowScale()
{
    return nowScale;
}

void SetNowScale(int scale)
{
    if (scale >= 0 && scale < 10) {
        nowScale = scale;
    }
}

static void OnPaint(HWND hWnd, HDC hdc)
{
    RECT rc;
    GetClientRect(hWnd, &rc);

    HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);

    HBITMAP hBmp = (HBITMAP)LoadImage(NULL, szFilePath,
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBmp) {

        BITMAP bmp;
        GetObject(hBmp, sizeof(BITMAP), &bmp);

        float scale = pictureScaleTable[nowScale];
        float width = bmp.bmWidth * scale;
        float height = bmp.bmHeight * scale;

        HDC hdc2 = CreateCompatibleDC(hdc);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdc2, hBmp);

        SetStretchBltMode(hdc, COLORONCOLOR);
        StretchBlt(hdc,
            (rc.right / 2) - (width / 2),
            (rc.bottom / 2) - (height / 2),
            (int)width,
            (int)height,
            hdc2, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

        SelectObject(hdc2, hOldBmp);
        DeleteDC(hdc2);
        DeleteObject(hBmp);
    }
}


LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...

        OnPaint(hWnd, hdc);

        EndPaint(hWnd, &ps);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
