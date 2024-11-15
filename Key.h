#ifndef KEY_H
#define KEY_H

#include <windows.h>
#include <string>
#include <iostream>

class Key {
public:
    Key();
    ~Key();

    Key& down(int vk_code);
    Key& up(int vk_code);
    Key& press(int vk_code);
    Key& combination(int vk_code);
    Key& combination(int vk_code_1, int vk_code_2);
    Key& combination(int vk_code_1, int vk_code_2, int vk_code_3);
    Key& combination(int vk_code_1, int vk_code_2, int vk_code_3, int vk_code_4);
    Key& sleep(int _time);
    Key& caps();
    Key& Caps();
    Key& setSleepTime(int _time);
    Key& period();
    Key& comma();
    int getTime();
    Key& bearStr(std::string str);
    Key& bearWStr(std::wstring wstr); //Ö§³Ö¿í×Ö·û´®

private:
    int time;
};

#endif // KEY_H