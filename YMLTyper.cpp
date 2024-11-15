#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <windows.h>
#include <locale>
#include <codecvt>
#include "Key.h"

// 设置 wifstream 读取 UTF-8 编码的文件

std::locale getUTF8Locale() {
   return std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
   // return std::locale(std::locale(), new std::codecvt_byname<wchar_t, char, mbstate_t>("zh_CN.UTF-8"));
}


/*
std::locale getUTF8Locale() {
    return std::locale(std::locale(), new std::codecvt_byname<wchar_t, char, mbstate_t>("zh_CN.gbk"));
}
*/
// 切换输入法到中文模式
void switchToChineseInput(HWND hwnd) {
    HIMC himc = ImmGetContext(hwnd);
    if (himc) {
        ImmSetConversionStatus(himc, IME_CMODE_CHINESE, 0); // 切换到中文模式
        ImmReleaseContext(hwnd, himc);
    }
}

// 切换输入法到英文模式
void switchToEnglishInput(HWND hwnd) {
    HIMC himc = ImmGetContext(hwnd);
    if (himc) {
        ImmSetConversionStatus(himc, IME_CMODE_ALPHANUMERIC, 0); // 切换到英文模式
        ImmReleaseContext(hwnd, himc);
    }
}

int main() {
    std::cout << "YMLTyper by Yum1era" << std::endl;
    // 初始延迟，确保光标已经放置在目标窗口中
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 获取当前活动窗口的句柄
    HWND hwnd = GetForegroundWindow();

    // 指定文件的完整路径
    const std::string filename = "D:\\example.txt";

    // 打开文件，确保使用 UTF-8 编码
    std::wifstream file(filename);
    file.imbue(getUTF8Locale());
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return 1;
    }

    Key key;
    key.setSleepTime(100); // 设置每次按键之间的延迟为100毫秒

    std::wstring line;
    while (std::getline(file, line)) {
        for (size_t i = 0; i < line.length(); ++i) {
            wchar_t ch = line[i];
            std::wcout << L"Inputting character: " << ch << L" (Unicode: " << (unsigned int)ch << L")" << std::endl;

            // 切换输入法状态
            if (ch >= 0x4E00 && ch <= 0x9FA5) { // 中文字符
                switchToChineseInput(hwnd);
            }
            else {
                switchToEnglishInput(hwnd);
            }

            // 输入字符
            INPUT input[2] = { 0 };
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = 0;
            input[0].ki.wScan = ch;
            input[0].ki.dwFlags = KEYEVENTF_UNICODE;
            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = 0;
            input[1].ki.wScan = ch;
            input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

            SendInput(2, input, sizeof(INPUT));
            if (key.getTime()) {
                key.sleep(key.getTime());
            }
        }
        key.press(VK_RETURN); // 模拟回车键
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 每行之间间隔1秒
    }

    // 关闭文件
    file.close();

    return 0;
}