// KeyBoardHook.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "KeyBoardHook.h"
#include "commons.h"
#include <shellapi.h>

#define MAX_LOADSTRING 100
HDC hdc;
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND mhWnd;
HMENU hMenu;
int totalNum = 0;

std::wstring StoWs(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r.c_str();
}

int getKeyBroadValue(std::string mapping) {

	LPCTSTR count = L"";
	std::wstring tmp = L"";
	int countTemp = 0;
	int idx = -1;
	if ((idx = mapping.find("|")) > -1) {
		std::string k1 = mapping.substr(0, idx);
		std::string k2 = mapping.substr(idx + 1, mapping.length() - 1);
		if (keyRecord.count(k1) != 0) {
			countTemp += keyRecord.at(k1);
		}
		if (keyRecord.count(k2) != 0) {
			countTemp += keyRecord.at(k2);
		}
	}
	else {
		if (keyRecord.count(mapping) != 0) {
			countTemp = keyRecord.at(mapping);
		}
	}
	totalNum += countTemp;
	return countTemp;
}

void drawRectang(std::string mapping, HDC hdc, int x, int y, LPCTSTR text, int width = 0, int height = 0) {
	int tmp = 0;
	if (mapping != "N/A")
		tmp = getKeyBroadValue(mapping);
	Rectangle(hdc, (x == 0 ? 3 : x + keyPadingX), (y == 0 ? 2 : y + keyPadingY), x + keyWidth + width, y + keyHeight + height);
	k_max_v = k_max_v < tmp ? tmp : k_max_v;
	int r = 128, g = 128, b = 128;
	if (!firstRun) {
		r = (1 - max(0.1, (0.4 * tmp / k_max_v))) * 255;
		g = (1 - max(0.1, (0.8 * tmp / k_max_v))) * 255;
		b = (1 - max(0.1, (0.9 * tmp / k_max_v))) * 255;
	}
	RECT temp;
	temp.left = (x == 0 ? 3 : x + keyPadingX) + 1;
	temp.top = (y == 0 ? 2 : y + keyPadingY) + 1;
	temp.right = x + keyWidth + width - 1;
	temp.bottom = y + keyHeight + height - 1;
	HBRUSH rgb = CreateSolidBrush(RGB(r, g, b));
	FillRect(hdc, &temp, rgb);
	DeleteObject(rgb); // 内存泄漏
	COLORREF BGColor = SetBkColor(hdc, RGB(r, g, b));


	size_t len = wcslen(text);
	/*
	 right  x + width + keyWidth;
	 left   x
	 mid    (2*x+width+48)/2 - 6*len
	*/

	if (mapping != "N/A") {
		TextOut(hdc, (2 * x + width + keyWidth) / 2 - 3 * len, y + 4, text, len);
	}
	else {
		TextOut(hdc, x+6, y + 4, text, len);
	}
	std::wstring vstr = StoWs(std::to_string(tmp));
	if (mapping != "N/A")
		TextOut(hdc, (2 * x + width + keyWidth) / 2 - 3 * vstr.length(), y + keyHeight / 2, vstr.c_str(), vstr.length());


}
void drawUI(HDC hdc) {
	totalNum = 0;
	drawRectang("[ESC]", hdc, keyWidth * 0, keyHeight * 0, L"ESC");
	drawRectang("[F1]", hdc, keyWidth * 1 + (keyWidth - keyWidth / 6) * 1, keyHeight * 0, L"F1");
	drawRectang("[F2]", hdc, keyWidth * 2 + (keyWidth - keyWidth / 6) * 1, keyHeight * 0, L"F2");
	drawRectang("[F3]", hdc, keyWidth * 3 + (keyWidth - keyWidth / 6) * 1, keyHeight * 0, L"F3");
	drawRectang("[F4]", hdc, keyWidth * 4 + (keyWidth - keyWidth / 6) * 1, keyHeight * 0, L"F4");
	drawRectang("[F5]", hdc, keyWidth * 5 + (keyWidth - keyWidth / 6) * 2, keyHeight * 0, L"F5");
	drawRectang("[F6]", hdc, keyWidth * 6 + (keyWidth - keyWidth / 6) * 2, keyHeight * 0, L"F6");
	drawRectang("[F7]", hdc, keyWidth * 7 + (keyWidth - keyWidth / 6) * 2, keyHeight * 0, L"F7");
	drawRectang("[F8]", hdc, keyWidth * 8 + (keyWidth - keyWidth / 6) * 2, keyHeight * 0, L"F8");
	drawRectang("[F9]", hdc, keyWidth * 9 + (keyWidth - keyWidth / 6) * 3, keyHeight * 0, L"F9");
	drawRectang("[F10]", hdc, keyWidth * 10 + (keyWidth - keyWidth / 6) * 3, keyHeight * 0, L"F10");
	drawRectang("[F11]", hdc, keyWidth * 11 + (keyWidth - keyWidth / 6) * 3, keyHeight * 0, L"F11");
	drawRectang("[F12]", hdc, keyWidth * 12 + (keyWidth - keyWidth / 6) * 3, keyHeight * 0, L"F12");

	drawRectang("`|~", hdc, keyWidth * 0, keyHeight * 1 + 2 * 2, L"~");
	drawRectang("1|!", hdc, keyWidth * 1, keyHeight * 1 + 2 * 2, L"1");
	drawRectang("2|@", hdc, keyWidth * 2, keyHeight * 1 + 2 * 2, L"2");
	drawRectang("3|#", hdc, keyWidth * 3, keyHeight * 1 + 2 * 2, L"3");
	drawRectang("4|$", hdc, keyWidth * 4, keyHeight * 1 + 2 * 2, L"4");
	drawRectang("5|%", hdc, keyWidth * 5, keyHeight * 1 + 2 * 2, L"5");
	drawRectang("6|^", hdc, keyWidth * 6, keyHeight * 1 + 2 * 2, L"6");
	drawRectang("7|&", hdc, keyWidth * 7, keyHeight * 1 + 2 * 2, L"7");
	drawRectang("8|*", hdc, keyWidth * 8, keyHeight * 1 + 2 * 2, L"8");
	drawRectang("9|(", hdc, keyWidth * 9, keyHeight * 1 + 2 * 2, L"9");
	drawRectang("0|)", hdc, keyWidth * 10, keyHeight * 1 + 2 * 2, L"0");
	drawRectang("-|_", hdc, keyWidth * 11, keyHeight * 1 + 2 * 2, L"-");
	drawRectang("=|+", hdc, keyWidth * 12, keyHeight * 1 + 2 * 2, L"=");
	drawRectang("[BACKSPACE]", hdc, keyWidth * 13, keyHeight * 1 + 2 * 2, L"BackSpace", 1.5 * keyWidth);

	drawRectang("[TAB]", hdc, keyWidth * 0, keyHeight * 2 + 2 * 3, L"TAB", keyWidth * 0.75);
	drawRectang("q|Q", hdc, keyWidth * 1 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"q");
	drawRectang("w|W", hdc, keyWidth * 2 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"w");
	drawRectang("e|E", hdc, keyWidth * 3 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"e");
	drawRectang("r|R", hdc, keyWidth * 4 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"r");
	drawRectang("t|T", hdc, keyWidth * 5 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"t");
	drawRectang("y|Y", hdc, keyWidth * 6 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"y");
	drawRectang("u|U", hdc, keyWidth * 7 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"u");
	drawRectang("i|I", hdc, keyWidth * 8 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"i");
	drawRectang("o|O", hdc, keyWidth * 9 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"o");
	drawRectang("p|P", hdc, keyWidth * 10 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"p");
	drawRectang("[|{", hdc, keyWidth * 11 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"[");
	drawRectang("]|}", hdc, keyWidth * 12 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"]");
	drawRectang("\\||", hdc, keyWidth * 13 + keyWidth * 0.75, keyHeight * 2 + 2 * 3, L"\\", keyWidth * 0.75);

	drawRectang("[CAPITAL]", hdc, keyWidth * 0, keyHeight * 3 + 2 * 4, L"Caps", keyWidth * 1.25);
	drawRectang("a|A", hdc, keyWidth * 1 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"a");
	drawRectang("s|S", hdc, keyWidth * 2 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"s");
	drawRectang("d|D", hdc, keyWidth * 3 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"d");
	drawRectang("f|F", hdc, keyWidth * 4 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"f");
	drawRectang("g|G", hdc, keyWidth * 5 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"g");
	drawRectang("h|H", hdc, keyWidth * 6 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"h");
	drawRectang("j|J", hdc, keyWidth * 7 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"j");
	drawRectang("k|K", hdc, keyWidth * 8 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"k");
	drawRectang("l|L", hdc, keyWidth * 9 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"l");
	drawRectang(";|:", hdc, keyWidth * 10 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L";");
	drawRectang("'|\"", hdc, keyWidth * 11 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"'");
	drawRectang("[ENTER]|[rEnter]", hdc, keyWidth * 12 + keyWidth * 1.25, keyHeight * 3 + 2 * 4, L"Enter", keyWidth * 1.25);

	drawRectang("[LSHIFT]", hdc, keyWidth * 0 + 0, keyHeight * 4 + 2 * 5, L"LShift", keyWidth * 1.75);
	drawRectang("z|Z", hdc, keyWidth * 1 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"z");
	drawRectang("x|X", hdc, keyWidth * 2 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"x");
	drawRectang("c|C", hdc, keyWidth * 3 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"c");
	drawRectang("v|V", hdc, keyWidth * 4 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"v");
	drawRectang("b|B", hdc, keyWidth * 5 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"b");
	drawRectang("n|N", hdc, keyWidth * 6 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"n");
	drawRectang("m|M", hdc, keyWidth * 7 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"m");
	drawRectang(",|<", hdc, keyWidth * 8 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L",");
	drawRectang(".|>", hdc, keyWidth * 9 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L".");
	drawRectang("/|?", hdc, keyWidth * 10 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"/");
	drawRectang("[RSHIFT]", hdc, keyWidth * 11 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"RShift", keyWidth * 1.75);

	drawRectang("[LCTRL]", hdc, keyWidth * 0 + 0, keyHeight * 5 + 2 * 6, L"LCtrl", keyWidth * 0.75);
	drawRectang("[LWIN]", hdc, keyWidth * 1 + keyWidth * 0.75, keyHeight * 5 + 2 * 6, L"LWin", keyWidth * 0.5);
	drawRectang("[LALT]", hdc, keyWidth * 2 + keyWidth * 1.25, keyHeight * 5 + 2 * 6, L"LAlt", keyWidth * 0.5);
	drawRectang("[SPACE]", hdc, keyWidth * 3 + keyWidth * 1.75, keyHeight * 5 + 6 * 2, L"Space", keyWidth * 5);
	drawRectang("[RALT]", hdc, keyWidth * 4 + keyWidth * 6.75, keyHeight * 5 + 2 * 6, L"RAlt", keyWidth * 0.5);
	drawRectang("[RWIN]", hdc, keyWidth * 5 + keyWidth * 7.25, keyHeight * 5 + 2 * 6, L"RWin", keyWidth * 0.5);
	drawRectang("[RCTRL]", hdc, keyWidth * 6 + keyWidth * 7.75, keyHeight * 5 + 2 * 6, L"RCtrl", keyWidth * 0.75);

	drawRectang("[PRT SC]", hdc, keyWidth * 14 + keyWidth * 1.75, 0, L"PrtSc");
	drawRectang("[SCRLK]", hdc, keyWidth * 15 + keyWidth * 1.75, 0, L"ScrLK");
	drawRectang("[PAUSE]", hdc, keyWidth * 16 + keyWidth * 1.75, 0, L"Pause");

	drawRectang("[INSERT]", hdc, keyWidth * 14 + keyWidth * 1.75, keyHeight * 1 + 2 * 2, L"Ins");
	drawRectang("[HOME]", hdc, keyWidth * 15 + keyWidth * 1.75, keyHeight * 1 + 2 * 2, L"Home");
	drawRectang("[PG UP]", hdc, keyWidth * 16 + keyWidth * 1.75, keyHeight * 1 + 2 * 2, L"Pg UP");

	drawRectang("[DELETE]", hdc, keyWidth * 14 + keyWidth * 1.75, keyHeight * 2 + 2 * 3, L"Del");
	drawRectang("[END]", hdc, keyWidth * 15 + keyWidth * 1.75, keyHeight * 2 + 2 * 3, L"End");
	drawRectang("[PG DN]", hdc, keyWidth * 16 + keyWidth * 1.75, keyHeight * 2 + 2 * 3, L"Pg DN");

	drawRectang("[UP]", hdc, keyWidth * 15 + keyWidth * 1.75, keyHeight * 4 + 2 * 5, L"↑");
	drawRectang("[LEFT]", hdc, keyWidth * 14 + keyWidth * 1.75, keyHeight * 5 + 2 * 6, L"←");
	drawRectang("[DOWN]", hdc, keyWidth * 15 + keyWidth * 1.75, keyHeight * 5 + 2 * 6, L"↓");
	drawRectang("[RIGHT]", hdc, keyWidth * 16 + keyWidth * 1.75, keyHeight * 5 + 2 * 6, L"→");


	drawRectang("[NUMLOCK]", hdc, keyWidth * 19, keyHeight + 2 * 2, L"NumL");
	drawRectang("r/", hdc, keyWidth * 20, keyHeight * 1 + 2 * 2, L"/");
	drawRectang("r*", hdc, keyWidth * 21, keyHeight * 1 + 2 * 2, L"*");
	drawRectang("r-", hdc, keyWidth * 22, keyHeight * 1 + 2 * 2, L"-");

	drawRectang("N7", hdc, keyWidth * 19, keyHeight * 2 + 2 * 3, L"7");
	drawRectang("N8", hdc, keyWidth * 20, keyHeight * 2 + 2 * 3, L"8");
	drawRectang("N9", hdc, keyWidth * 21, keyHeight * 2 + 2 * 3, L"9");
	drawRectang("r+", hdc, keyWidth * 22, keyHeight * 2 + 2 * 3, L"+", 0, keyHeight + 2);

	drawRectang("N4", hdc, keyWidth * 19, keyHeight * 3 + 2 * 4, L"4");
	drawRectang("N5", hdc, keyWidth * 20, keyHeight * 3 + 2 * 4, L"5");
	drawRectang("N6", hdc, keyWidth * 21, keyHeight * 3 + 2 * 4, L"6");

	drawRectang("N1", hdc, keyWidth * 19, keyHeight * 4 + 2 * 5, L"1");
	drawRectang("N2", hdc, keyWidth * 20, keyHeight * 4 + 2 * 5, L"2");
	drawRectang("N3", hdc, keyWidth * 21, keyHeight * 4 + 2 * 5, L"3");
	drawRectang("[rEnter]", hdc, keyWidth * 22, keyHeight * 4 + 2 * 5, L"Enter", 0, keyHeight + 2);

	drawRectang("N0", hdc, keyWidth * 19, keyHeight * 5 + 2 * 6, L"0", keyWidth);
	drawRectang("r.", hdc, keyWidth * 21, keyHeight * 5 + 2 * 6, L".");

	drawRectang("N/A", hdc, keyWidth * 19, 0, StoWs("键盘按键次数总计：" + std::to_string(totalNum)).c_str(), keyWidth * 3);

	if (firstRun) {
		firstRun = false;
		drawUI(hdc);
	}
	else {
		RECT rect;
		GetWindowRect(mhWnd, &rect);
		SetWindowPos(mhWnd, nullptr, rect.left, rect.top, keyWidth * 23 + 18, keyHeight * 6 + 73, NULL);
	}
}





// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    CONFIG(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    LogWindow(HWND, UINT, WPARAM, LPARAM);
NOTIFYICONDATA m_Notify;
int WindowStatus = SW_SHOWNORMAL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。



	importConfig();
	import();

	DWORD tskid;
	HANDLE tskThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CreateKHookThread, NULL, 0, &tskid);



	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_KeyBoardHook, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KeyBoardHook));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		drawUI(hdc);
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	HBRUSH bgColor = CreateSolidBrush(RGB(238, 238, 238));
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KeyBoardHook));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = bgColor;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KeyBoardHook);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, keyWidth * 23 + 18, keyHeight * 6 + 73, nullptr, nullptr, hInstance, nullptr);
	mhWnd = hWnd;
	if (!hWnd)
	{
		return FALSE;
	}
	if (autoTray) {
		ShowWindow(mhWnd, HIDE_WINDOW);
		WindowStatus = HIDE_WINDOW;
	}
	else {
		ShowWindow(hWnd, nCmdShow);
	}
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//



void CreateTray(HWND hwnd) {
	m_Notify.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_Notify.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_KeyBoardHook));

	m_Notify.hWnd = hwnd;
	m_Notify.uCallbackMessage = WM_USER;
	m_Notify.uVersion = NOTIFYICON_VERSION;
	m_Notify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_Notify.uID = 1;
	lstrcpy(m_Notify.szTip, L"KeyBoardHook");
	Shell_NotifyIcon(NIM_ADD, &m_Notify);
}
void DestroyTray(HWND hwnd) {
	Shell_NotifyIcon(NIM_DELETE, &m_Notify);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		CreateTray(hWnd);
		break;
	case WM_USER:
		if (lParam == WM_LBUTTONDOWN || lParam == WM_LBUTTONDBLCLK) {
			::SetForegroundWindow(hWnd);//解决在菜单外单击左键菜单不消失的问题
			ShowWindow(mhWnd, SW_SHOWNORMAL);
			WindowStatus = SW_SHOWNORMAL;
		}
		if (lParam == WM_RBUTTONDOWN)
		{
			::SetForegroundWindow(hWnd);//解决在菜单外单击左键菜单不消失的问题
			tagPOINT point;//鼠标坐标结构
			GetCursorPos(&point);//得到鼠标当前位置
			HMENU hMenu;//菜单句柄
			hMenu = CreatePopupMenu();//创建下拉式菜单
			//在指定的菜单条、下拉式菜单、子菜单或快捷菜单的末尾追加一个新菜单项
			AppendMenu(hMenu, MF_STRING, SHOW, L"显示/隐藏");//菜单项 字符形式
			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);//增加一条分割线
			AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"退出");
			TrackPopupMenu(hMenu, TPM_LEFTALIGN, point.x, point.y, NULL, hWnd, NULL);

		}
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) {
			ShowWindow(hWnd, SW_HIDE);
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case SHOW_CONFIG:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CONFIG), hWnd, CONFIG);
			break;
		case IDM_EXIT:
			save();
			DestroyTray(hWnd);
			DestroyWindow(hWnd);
			Shell_NotifyIcon(NIM_DELETE, &m_Notify);
			break;
		case SETUP_HOOK:
			MessageBoxA(hWnd, KeyBoardHookOper(1), "提示", MB_OK | MB_ICONINFORMATION);
			break;
		case REMOVE_HOOK:
			MessageBoxA(hWnd, KeyBoardHookOper(0), "提示", MB_OK | MB_ICONINFORMATION);
			break;
		case REFRESH:
			InvalidateRect(hWnd, NULL, NULL);
			UpdateWindow(hWnd);
			break;
		case SHOW_LOG:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_LOGS), hWnd, LogWindow);
			break;
		case TRUNCATEDATA:
			if (MessageBoxA(hWnd, "这将会清除按键记录，你确定吗？", "提示", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				keyRecord.clear();
				k_max_v = 1;
				drawUI(hdc);
				remove(logFile.c_str());
				InvalidateRect(hWnd, NULL, NULL);
				UpdateWindow(hWnd);
			}
			break;
		case SHOW:
			if (WindowStatus == SW_SHOWNORMAL) {
				ShowWindow(hWnd, SW_HIDE);
				WindowStatus = SW_HIDE;
			}
			else {
				ShowWindow(hWnd, SW_SHOWNORMAL);
				WindowStatus = SW_SHOWNORMAL;
			}
			break;
		default:
			return WndProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		COLORREF TextColor = SetTextColor(hdc, RGB(12, 14, 16));

		HFONT hFont = CreateFont(12, 6, 0, 0, 0, false, false, false, DEFAULT_CHARSET, 0, 0, 0, 0, LPCWSTR("黑体"));
		HFONT hOFont = (HFONT)SelectObject(hdc, hFont);
		drawUI(hdc);
		DeleteObject(hFont);
		DeleteObject(hOFont);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:

		DestroyTray(hWnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		//SetDlgItemTextW(hDlg, CONTEXT, L"基于Hook的键盘记录按键次数器\r\n");
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	default:
		break;
	}
	return (INT_PTR)FALSE;
}
// “日志”框的消息处理程序。
INT_PTR CALLBACK LogWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			print();
			SetDlgItemText(hDlg, IDC_EDIT1, StoWs(getLog()).c_str());
			return 0;
			break;

		case IDC_BUTTON2:
			clearLog();
			SetDlgItemText(hDlg, IDC_EDIT1, StoWs(getLog()).c_str());
			return 0;
			break;
		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
		SetDlgItemText(hDlg, IDC_EDIT1, StoWs(getLog()).c_str());
		break;
	}

	return (INT_PTR)FALSE;
}
// “设置”框的消息处理程序。
INT_PTR CALLBACK CONFIG(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPWSTR kw = LPWSTR(std::to_wstring(keyWidth).c_str()), kh = LPWSTR(std::to_wstring(keyHeight).c_str());
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		SetDlgItemText(hDlg, IDC_EDIT_CONF_WIDTH, std::to_wstring(keyWidth).c_str());
		SetDlgItemText(hDlg, IDC_EDIT_CONF_HEIGHT, std::to_wstring(keyHeight).c_str());
		if (autoTray) {
			HWND dn = FindWindowEx(hDlg, NULL, L"Button", L"启动后自动缩小至托盘");
			SendMessage(dn, BM_SETCHECK, 1, 0);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case SAVE_CONFIG:
			GetDlgItemTextW(hDlg, IDC_EDIT_CONF_WIDTH, kw, sizeof(kw));
			GetDlgItemTextW(hDlg, IDC_EDIT_CONF_HEIGHT, kh, sizeof(kh));
			autoTray = (int)SendMessage(FindWindowEx(hDlg, NULL, L"Button", L"启动后自动缩小至托盘"), BM_GETCHECK, 0, 0) == 1;
			keyWidth = _wtoi(kw);
			keyHeight = _wtoi(kh);
			EndDialog(hDlg, LOWORD(wParam));
			{
				std::string cfg = "autoTray->" + std::string(autoTray ? "1" : "0") + std::string("\r\nkeyWidth->" + std::to_string(keyWidth)) + std::string("\r\nkeyHeight->" + std::to_string(keyHeight) + "\r\n");
				saveConfig(cfg);
			}
			return (INT_PTR)TRUE;
			break;

		case IDC_BUTTON2:
			clearLog();
			SetDlgItemText(hDlg, IDC_EDIT1, StoWs(getLog()).c_str());
			return 0;
			break;
		}
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
		SetDlgItemText(hDlg, IDC_EDIT1, StoWs(getLog()).c_str());
		break;
	}

	return (INT_PTR)FALSE;
}
