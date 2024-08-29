// Viewer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "Viewer.h"
#include "WndPicture.h"

#include <commdlg.h>

#define MAX_LOADSTRING 100

#define ID_BTN_BIG               (100)
#define ID_BTN_SMALL             (101)
#define ID_BTN_SELECT            (102)

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

TCHAR szSelectedFileName[MAX_PATH];

HWND hBtnBig, hBtnSmall, hBtnSelect, hEdit, hWndPicture;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

TCHAR strScaleTable[10][10] = {
    L"10%",
    L"25%",
    L"50%",
    L"75%",
    L"100%",
    L"125%",
    L"150%",
    L"200%",
    L"250%",
    L"300%",
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VIEWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VIEWER));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VIEWER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VIEWER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

HFONT CreateFontMainWnd()
{
    HFONT hFont = CreateFont(
        18,								// 文字の高さ
        0,								// 平均文字幅 → 0なら自動
        0,								// 文字送りの方向とX軸との角度
        0,								// 文字の角度
        FW_NORMAL,						// フォントの太さ
        FALSE,							// イタリック体
        FALSE,							// 下線
        FALSE,							// 打ち消し線
        SHIFTJIS_CHARSET,					// キャラクタセット
        OUT_DEFAULT_PRECIS,				// 精度（出力）
        CLIP_DEFAULT_PRECIS,				// 精度（クリッピング）
        DEFAULT_QUALITY,					// 出力品質
        (DEFAULT_PITCH | FF_DONTCARE),  // ピッチとファミリ
        L"メイリオ");					// フォント名
    return hFont;
}

void OnCreate(HWND hWnd)
{
    // 1. ウィンドウを登録する
    // 2. ウィンドウを作成する
    // 3. ウィンドウに機能を付ける

    // エディットボックス
    hEdit = CreateWindow(L"edit",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        50, 50,
        200, 30,
        hWnd,
        (HMENU)103,
        hInst,
        NULL);
    HFONT hFont = CreateFontMainWnd();
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, NULL);

    // 拡大用ボタン
    hBtnBig = CreateWindow(L"button",
        L"▲",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        300, 50,
        30, 30,
        hWnd,
        (HMENU)ID_BTN_BIG,
        hInst,
        NULL);

    // 縮小用ボタン
    hBtnSmall = CreateWindow(L"button",
        L"▼",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        300, 100,
        30, 30,
        hWnd,
        (HMENU)ID_BTN_SMALL,
        hInst,
        NULL);

    // 画像選択用ボタン
    hBtnSelect = CreateWindow(L"button",
        L"…",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        250, 50,
        30, 30,
        hWnd,
        (HMENU)ID_BTN_SELECT,
        hInst,
        NULL);

    {
        RegisterPictureWnd(hInst);

        hWndPicture = CreateWindow(L"PictureWnd",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            50, 100,
            200, 300,
            hWnd,
            (HMENU)104,
            hInst,
            NULL);

    }
}

void OnSize(HWND hWnd)
{
    RECT rc;
    GetClientRect(hWnd, &rc);

    MoveWindow(hBtnBig,
        rc.right - 50 - 60,
        (rc.bottom / 2) - 20 - 60,
        60,
        60,
        TRUE);

    MoveWindow(hBtnSmall,
        rc.right - 50 - 60,
        (rc.bottom / 2) + 20,
        60,
        60,
        TRUE);

    MoveWindow(hBtnSelect,
        rc.right - 50 - 30,
        rc.bottom - 50 - 30,
        30,
        30,
        TRUE);

    MoveWindow(hEdit,
        50,
        rc.bottom - 50 - 30,
        rc.right - 50 - 30 - 50,
        30,
        TRUE);

    MoveWindow(hWndPicture,
        50,
        50,
        rc.right - 50 - 60 - 20 - 50,
        rc.bottom - 50 - 30 - 20 - 50,
        TRUE);
}

void OnDestroy(HWND hWnd)
{

    if (hBtnBig != NULL) {
        DestroyWindow(hBtnBig);
        hBtnBig = NULL;
    }

    if (hBtnSmall != NULL) {
        DestroyWindow(hBtnSmall);
        hBtnSmall = NULL;
    }

    if (hBtnSelect != NULL) {
        DestroyWindow(hBtnSelect);
        hBtnSelect = NULL;
    }

    if (hEdit != NULL) {
        DestroyWindow(hEdit);
        hEdit = NULL;
    }

    if (hWndPicture != NULL) {
        DestroyWindow(hWndPicture);
        hWndPicture = NULL;
    }
}

void OnBtnSelect(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    int code = HIWORD(wParam);
    if (code == BN_CLICKED) {

        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFile = szSelectedFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST;
        ofn.lpstrFilter =
            L"Bitmap {*.bmp}\0*.bmp\0"
            L"All files {*.*}\0*.*\0"
            L"\0";

        BOOL ret = GetOpenFileName(&ofn);
        if (ret == TRUE) {
            SetWindowText(hEdit, szSelectedFileName);
            SetBitmapFilePath(szSelectedFileName);

            InvalidateRect(hWndPicture, NULL, TRUE);
        }
    }
}

void OnBtnBig(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    int code = HIWORD(wParam);
    if (code == BN_CLICKED) {
        int scale = GetNowScale();
        scale++;
        SetNowScale(scale);

        InvalidateRect(hWndPicture, NULL, TRUE);

        InvalidateRect(hWnd, NULL, TRUE);
    }
}

void OnBtnSmall(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    int code = HIWORD(wParam);
    if (code == BN_CLICKED) {
        int scale = GetNowScale();
        scale--;
        SetNowScale(scale);

        InvalidateRect(hWndPicture, NULL, TRUE);

        InvalidateRect(hWnd, NULL, TRUE);
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

    TCHAR strScale[10];
    int scale = GetNowScale();
    if (scale >= 0 && scale < 10) {
        wsprintf(strScale, L"%s", strScaleTable[scale]);

        HFONT hFont = CreateFontMainWnd();
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        COLORREF oldColor = SetTextColor(hdc, RGB(255, 255, 255));
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc,
            (rc.right - 50) - 40,
            (rc.bottom / 2) - 10,
            strScale, wcslen(strScale));
        SetBkMode(hdc, OPAQUE);
        SetTextColor(hdc, oldColor);

        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
    }
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // ウィンドウが作成された時
    case WM_CREATE:
        OnCreate(hWnd);
        break;

    case WM_SIZE:
        OnSize(hWnd);
        break;
 
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case ID_BTN_BIG:
            OnBtnBig(hWnd, wParam, lParam);
            break;

        case ID_BTN_SMALL:
            OnBtnSmall(hWnd, wParam, lParam);
            break;

        case ID_BTN_SELECT:
            OnBtnSelect(hWnd, wParam, lParam);
            break;

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: HDC を使用する描画コードをここに追加してください...

        OnPaint(hWnd, hdc);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:

        OnDestroy(hWnd);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
