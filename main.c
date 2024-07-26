#include <windows.h>
#include <tchar.h>
#include <shellapi.h>

// Идентификатор иконки в трее
#define ID_TRAY_ICON 1
// Сообщение для иконки в трее
#define WM_TRAYMESSAGE (WM_USER + 1)
// Идентификаторы меню
#define IDM_OPEN 1
#define IDM_EXIT 2
#define IDM_ABOUT 3
#define IDM_HELP 4
#define IDM_SHOW 5
#define IDM_HIDE 6

// Структура для иконки в трее
NOTIFYICONDATA nid;

// Обработчик сообщений для иконки в трее
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_DESTROY:
            // Удаление иконки из трея при закрытии окна
            Shell_NotifyIcon(NIM_DELETE, &nid);
            break;
        case WM_TRAYMESSAGE:
            // Обработка сообщений от иконки в трее
            switch (lParam) {
                case WM_LBUTTONDOWN:
                    // Обработка нажатия левой кнопки мыши на иконку
                    break;
                case WM_RBUTTONDOWN:
                    // Обработка нажатия правой кнопки мыши на иконку
                    {
                        POINT pt;
                        GetCursorPos(&pt);
                        HMENU hMenu = CreatePopupMenu();
                        AppendMenu(hMenu, MF_STRING, IDM_OPEN, _T("Открыть"));
                        AppendMenu(hMenu, MF_STRING, IDM_SHOW, _T("Показать окно"));
                        AppendMenu(hMenu, MF_STRING, IDM_HIDE, _T("Скрыть окно"));
                        AppendMenu(hMenu, MF_STRING, IDM_ABOUT, _T("О программе"));
                        AppendMenu(hMenu, MF_STRING, IDM_HELP, _T("Справка"));
                        AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                        AppendMenu(hMenu, MF_STRING, IDM_EXIT, _T("Выход"));
                        TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
                        DestroyMenu(hMenu);
                    }
                    break;
            }
            break;
        case WM_COMMAND:
            // Обработка команд от меню
            switch (LOWORD(wParam)) {
                case IDM_OPEN:
                    // Обработка команды "Открыть"
                    MessageBox(hWnd, _T("Вставьте сюда нужный код"), _T("Заглушка"), MB_OK);
                    break;
                case IDM_SHOW:
                    // Обработка команды "Показать окно"
                    ShowWindow(hWnd, SW_SHOW);
                    break;
                case IDM_HIDE:
                    // Обработка команды "Скрыть окно"
                    // Осторожнее с закрытием окна!!!
                    // Если закроете его по X-кнопке, программа "спрячется" в системе и будет отображаться только в списке задач!!!
                    // После этого ее можно будет снять только из диспетчера задач!!!
                    ShowWindow(hWnd, SW_HIDE);
                    break;
                case IDM_ABOUT:
                    // Обработка команды "О программе"
                    MessageBox(
                        hWnd, _T("Программа на C"), _T("О программе"), MB_OK);
                    break;
                case IDM_HELP:
                    // Обработка команды "Справка"
                    MessageBox(hWnd, _T("Справка по программе"), _T("Справка"), MB_OK);
                    break;
                case IDM_EXIT:
                    // Обработка команды "Выход"
                    PostQuitMessage(0);
                    break;
            }
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// Процедура смены иконки в трее
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
    // Регистрация класса окна
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

    // Создание окна
    HWND hWnd = CreateWindowEx(0, _T("MyTestClass"), _T("Тестовая программа"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);

    // Загрузка ICO-файла
    HICON hIcon = LoadImage(hInstance, _T("icon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    // Инициализация иконки в трее
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = ID_TRAY_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYMESSAGE;
    nid.hIcon = hIcon;
    _tcscpy(nid.szTip, _T("Тестовая программа"));
    Shell_NotifyIcon(NIM_ADD, &nid);

    // Вывод сообщения из приложения около иконки
    nid.uFlags = NIF_INFO;
    _tcscpy(nid.szInfo, _T("Программа запущен и резидентна в памяти"));
    _tcscpy(nid.szInfoTitle, _T("Информация"));
    Shell_NotifyIcon(NIM_MODIFY, &nid);

    // Цикл обработки сообщений
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

