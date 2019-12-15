//
// Created by 123 on 2019/12/14.
//

#ifndef B_PC_ROW_H
#define B_PC_ROW_H

#include "ELEMENT.h"

#include <sstream>
#include <vector>
#include <string>
#include <sstream>
using std::string;
using std::vector;

struct ROW
{
    ROW(vector<ELEMENT> tmp,size_t i)
    {
        // 只能初始化一次
        if(row2.size()==0){
            row2=tmp;
            index=i;
        }
    }
    ROW() = default;
    void operator >>(string &str);
    bool operator<<(string str);
    bool operator>(const ROW &R);
    bool operator<(const ROW &R);
    bool operator==(const ROW &R);
    friend  std::ostream&  operator<<(std::ostream &output,const ROW& D)
    {
        string tmp;
        for(auto el:D.row2)
        {
            el>>tmp;
            output<<tmp << " ";
        }
        return output;
    }
    void operator=(const ROW &t)
    {
        row2 = t.row2;
        index  = t.index;
    }
protected:
    vector<ELEMENT> row2;
    size_t index = 0;
};


void ROW::operator>>(string &str)
{
    string tmp;
    for(auto el:row2)
    {
        el>> tmp;
        str += (tmp + ' ');
    }
}

bool ROW::operator>(const ROW &R)
{
    return row2[index]>R.row2[index];
}
bool ROW::operator<(const ROW &R)
{
    return row2[index]<R.row2[index];
}
bool ROW::operator==(const ROW &R)
{
    return row2[index]==R.row2[index];
}
bool ROW::operator<<(string str)
{
    if(row2.size()==0)
    {
        return false;
    }
    std::stringstream  IN(str);
    string tmp;
    for(auto &el:row2)
    {
        IN >>tmp;
        el = ELEMENT(el.format,tmp);
    }
    return true;
}

#endif //B_PC_ROW_H
