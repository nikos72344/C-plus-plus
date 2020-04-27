#include <Windows.h>
#include "lab_2.h"

char szClassName[] = "CG_WAPI_Template";

int screen_size = 700;

const unsigned int TIMER_SEC = 1;

const unsigned int INTERVAL = 1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Object act;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  HWND hWnd;
  MSG lpMsg;
  WNDCLASS wc;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = (LPCWSTR)szClassName;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, (LPCWSTR)L"Не могу зарегистрировать класс окна!", (LPCWSTR)L"Ошибка", MB_OK);
    return 1;
  }

  hWnd = CreateWindow((LPCWSTR)szClassName, L"E - удлиннить, Q - уменьшить", WS_OVERLAPPEDWINDOW, 50, 50, screen_size, screen_size, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, NULL);

  if (!hWnd)
  {
    MessageBox(NULL, (LPCWSTR)L"Не удается создать главное окно!", (LPCWSTR)L"Ошибка", MB_OK);
    return 2;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  while (GetMessage(&lpMsg, NULL, 0, 0))
  {
    TranslateMessage(&lpMsg);
    DispatchMessage(&lpMsg);
  }
  return (lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  RECT Rect;
  HDC hdc, hCmpDC;
  HBITMAP hBmp;
  switch (messg)
  {
  case WM_CREATE:
    SetTimer(hWnd, TIMER_SEC, INTERVAL, NULL);

  case WM_TIMER:
    act.DataUpdate();
    InvalidateRect(hWnd, NULL, FALSE);
    break;

  case WM_KEYDOWN:
    switch (int(wParam))
    {
    case 'W':
    case VK_UP: act += speed_unit; break;
    case 'S':
    case VK_DOWN: act -= speed_unit; break;
    case 'A':
    case VK_LEFT: act << PI / 18; break;
    case 'D':
    case VK_RIGHT: act >> PI / 18; break;
    case 'E': act.operator++(); break;
    case 'Q': act.operator--(); break;
    }
    InvalidateRect(hWnd, NULL, true);
    break;

  case WM_PAINT:
    GetClientRect(hWnd, &Rect);
    hdc = BeginPaint(hWnd, &ps);

    hCmpDC = CreateCompatibleDC(hdc);
    hBmp = CreateCompatibleBitmap(hdc, Rect.right - Rect.left,
      Rect.bottom - Rect.top);
    SelectObject(hCmpDC, hBmp);

    LOGBRUSH br;
    br.lbStyle = BS_SOLID;
    br.lbColor = RGB(224,255,255);
    HBRUSH brush;
    brush = CreateBrushIndirect(&br);
    FillRect(hCmpDC, &Rect, brush);
    DeleteObject(brush);

    hCmpDC << act;

    SetStretchBltMode(hdc, COLORONCOLOR);
    BitBlt(hdc, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top,
      hCmpDC, 0, 0, SRCCOPY);

    DeleteDC(hCmpDC);
    DeleteObject(hBmp);
    hCmpDC = NULL;

    EndPaint(hWnd, &ps);
    break;

  case WM_SIZE:
    GetClientRect(hWnd, &Rect);
    SetWindowSize(Rect.right - Rect.left, Rect.bottom - Rect.top);
    break;

  case WM_ERASEBKGND:
    return 1;
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return (DefWindowProc(hWnd, messg, wParam, lParam));
  }
  return (0);
}