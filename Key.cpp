#include "Key.h"
#include <string>
#include <iostream>

Key::Key() {
    this->time = 0; // 默认没有延迟
    std::cout << "Delay: " << this->time << std::endl;
}

Key::~Key() {}

Key& Key::down(int vk_code) {
    keybd_event(vk_code, 0, 0, 0);
    return *this;
}

Key& Key::up(int vk_code) {
    keybd_event(vk_code, 0, KEYEVENTF_KEYUP, 0);
    return *this;
}

Key& Key::press(int vk_code) {
    if (this->time) {
        this->sleep(this->time);
    }

    if (islower(vk_code)) { // 小写字母
        vk_code -= 32;
    }
    down(vk_code);
    up(vk_code);
    return *this;
}

Key& Key::combination(int vk_code) {
    press(vk_code);
    return *this;
}

Key& Key::combination(int vk_code_1, int vk_code_2) {
    down(vk_code_1);
    press(vk_code_2);
    up(vk_code_1);
    return *this;
}

Key& Key::combination(int vk_code_1, int vk_code_2, int vk_code_3) {
    down(vk_code_1);
    down(vk_code_2);
    press(vk_code_3);
    up(vk_code_2);
    up(vk_code_1);
    return *this;
}

Key& Key::combination(int vk_code_1, int vk_code_2, int vk_code_3, int vk_code_4) {
    down(vk_code_1);
    down(vk_code_2);
    down(vk_code_3);
    press(vk_code_4);
    up(vk_code_3);
    up(vk_code_2);
    up(vk_code_1);
    return *this;
}

Key& Key::sleep(int _time) {
    Sleep(_time);
    return *this;
}

Key& Key::caps() {
    if (GetKeyState(VK_CAPITAL) & 0x8000) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
    return *this;
}

Key& Key::Caps() {
    if (!(GetKeyState(VK_CAPITAL) & 0x8000)) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
    return *this;
}

Key& Key::setSleepTime(int _time) {
    this->time = _time;
    return *this;
}

Key& Key::period() {
    keybd_event(VK_OEM_PERIOD, 0, 0, 0);
    keybd_event(VK_OEM_PERIOD, 0, KEYEVENTF_KEYUP, 0);
    return *this;
}

Key& Key::comma() {
    keybd_event(VK_OEM_COMMA, 0, 0, 0);
    keybd_event(VK_OEM_COMMA, 0, KEYEVENTF_KEYUP, 0);
    return *this;
}

int Key::getTime() {
    return this->time;
}

Key& Key::bearStr(std::string str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (isupper(str[i])) { // 大写字母
            this->Caps();
            press(toupper(str[i]));
            this->caps();
        }
        else if (str[i] == '.') {
            period();
        }
        else if (str[i] == ',') {
            comma();
        }
        else {
            press(tolower(str[i]));
        }
    }
    return *this;
}

//宽字符
Key& Key::bearWStr(std::wstring wstr) {
    for (size_t i = 0; i < wstr.length(); ++i) {
        wchar_t ch = wstr[i];
        std::wcout << L"Inputting character: " << ch << L" (Unicode: " << (unsigned int)ch << L")" << std::endl;

        if (ch == L'，') { // 特殊处理中文逗号
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
            if (this->time) {
                this->sleep(this->time);
            }
        }
        else {
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
            if (this->time) {
                this->sleep(this->time);
            }
        }
    }
    return *this;
}