#include <windows.h>
#include <tchar.h>
#include <shellapi.h>

// ������������� ������ � ����
#define ID_TRAY_ICON 1
// ��������� ��� ������ � ����
#define WM_TRAYMESSAGE (WM_USER + 1)
// �������������� ����
#define IDM_OPEN 1
#define IDM_EXIT 2
#define IDM_ABOUT 3
#define IDM_HELP 4
#define IDM_SHOW 5
#define IDM_HIDE 6

// ��������� ��� ������ � ����
NOTIFYICONDATA nid;

// ���������� ��������� ��� ������ � ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_DESTROY:
            // �������� ������ �� ���� ��� �������� ����
            Shell_NotifyIcon(NIM_DELETE, &nid);
            break;
        case WM_TRAYMESSAGE:
            // ��������� ��������� �� ������ � ����
            switch (lParam) {
                case WM_LBUTTONDOWN:
                    // ��������� ������� ����� ������ ���� �� ������
                    break;
                case WM_RBUTTONDOWN:
                    // ��������� ������� ������ ������ ���� �� ������
                    {
                        POINT pt;
                        GetCursorPos(&pt);
                        HMENU hMenu = CreatePopupMenu();
                        AppendMenu(hMenu, MF_STRING, IDM_OPEN, _T("�������"));
                        AppendMenu(hMenu, MF_STRING, IDM_SHOW, _T("�������� ����"));
                        AppendMenu(hMenu, MF_STRING, IDM_HIDE, _T("������ ����"));
                        AppendMenu(hMenu, MF_STRING, IDM_ABOUT, _T("� ���������"));
                        AppendMenu(hMenu, MF_STRING, IDM_HELP, _T("�������"));
                        AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                        AppendMenu(hMenu, MF_STRING, IDM_EXIT, _T("�����"));
                        TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
                        DestroyMenu(hMenu);
                    }
                    break;
            }
            break;
        case WM_COMMAND:
            // ��������� ������ �� ����
            switch (LOWORD(wParam)) {
                case IDM_OPEN:
                    // ��������� ������� "�������"
                    MessageBox(hWnd, _T("�������� ���� ������ ���"), _T("��������"), MB_OK);
                    break;
                case IDM_SHOW:
                    // ��������� ������� "�������� ����"
                    ShowWindow(hWnd, SW_SHOW);
                    break;
                case IDM_HIDE:
                    // ��������� ������� "������ ����"
                    // ���������� � ��������� ����!!!
                    // ���� �������� ��� �� X-������, ��������� "���������" � ������� � ����� ������������ ������ � ������ �����!!!
                    // ����� ����� �� ����� ����� ����� ������ �� ���������� �����!!!
                    ShowWindow(hWnd, SW_HIDE);
                    break;
                case IDM_ABOUT:
                    // ��������� ������� "� ���������"
                    MessageBox(
                        hWnd, _T("��������� �� C"), _T("� ���������"), MB_OK);
                    break;
                case IDM_HELP:
                    // ��������� ������� "�������"
                    MessageBox(hWnd, _T("������� �� ���������"), _T("�������"), MB_OK);
                    break;
                case IDM_EXIT:
                    // ��������� ������� "�����"
                    PostQuitMessage(0);
                    break;
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// ��������� ����� ������ � ����
void ChangeIcon(HWND hWnd, HICON hIcon)
{
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = ID_TRAY_ICON;
    nid.uFlags = NIF_ICON;
    nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    // ����������� ������ ����
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = _T("MyTestClass");
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    // �������� ����
    HWND hWnd = CreateWindowEx(0, _T("MyTestClass"), _T("�������� ���������"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

    // �������� ICO-�����
    HICON hIcon = LoadImage(hInstance, _T("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    // ������������� ������ � ����
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = ID_TRAY_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYMESSAGE;
    nid.hIcon = hIcon;
    _tcscpy(nid.szTip, _T("�������� ���������"));
    Shell_NotifyIcon(NIM_ADD, &nid);

    // ����� ��������� �� ���������� ����� ������
    nid.uFlags = NIF_INFO;
    _tcscpy(nid.szInfo, _T("��������� ������� � ���������� � ������"));
    _tcscpy(nid.szInfoTitle, _T("����������"));
    Shell_NotifyIcon(NIM_MODIFY, &nid);

    // ���� ��������� ���������
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

