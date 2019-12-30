#define _CRT_SECURE_NO_WARNINGS

/*
// 20191230, 10s   -> laptop
             5s    -> PC

*/


#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>

using std::vector;
using std::string ;
using std::cin;
using std::cout;
using std::stringstream ;


#include "DBCore.h"

int main() {

    baozhixue::DBCore db;
    db.RUN();


    return 0;
}

/*
   在ListNode中加入析构函数

*/