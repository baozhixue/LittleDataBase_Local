//
// Created by 123 on 2019/12/14.
//

#ifndef B_PC_TEST_H
#define B_PC_TEST_H

#include <iostream>
#include <thread>
#include <mutex>

#include <windows.h>
using std::cin;
using std::cout;
using std::thread;
using std::mutex;

void run(size_t *flag)
{
    int i = 0;
    while (*flag > 0)
    {
        cout<<*flag << "\n";
        (*flag) -=1;
        Sleep(1000);
    }
}


void test()
{
    size_t flag = 10;
    for(int i = 0; i < 3;)
    {
        thread t(run,&flag);
        t.join();
    }
}

#endif //B_PC_TEST_H
