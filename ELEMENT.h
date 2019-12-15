//
// Created by 123 on 2019/12/14.
//


/*
 * 表格最小存储单位，支持int，double，char和string格式。
 * 提供运算符
 *
 *
 */




#ifndef B_PC_ELEMENT_H
#define B_PC_ELEMENT_H

#include <string>

using std::string ;

union element{
    int INT_DB;
    double DOUBLE_DB;
    char CHAR_DB;
    char *STR;
};
enum ele_format{INT_DB,DOUBLE_DB,CHAR_DB,CHARS_DB};

struct ELEMENT
{
    element ele;
    ele_format format;
    ELEMENT(const ele_format &type,const string &el);
    void operator>>(string &str);
    bool operator==(const ELEMENT &el);
    bool operator>(const ELEMENT &el);
    bool operator<(const ELEMENT &el);
};

/*
 * @define
 *      初始化ELEMENT
 * @para [type]
 * @para [el]   
 */
ELEMENT::ELEMENT(const ele_format &type,const string &el)
{
    format = type;
    switch (type)
    {
        case INT_DB:
            ele.INT_DB = std::stoi(el);
            break;
        case DOUBLE_DB:
            ele.DOUBLE_DB = std::stod(el);
            break;
        case CHAR_DB:
            ele.CHAR_DB = el[0];
            break;
        case CHARS_DB: // string
            ele.STR = (char*)malloc(1 + el.length()* sizeof(char));
            memcpy(ele.STR,el.c_str(),el.length());
            ele.STR[el.length()] = '\0';
            break;
    }
}

void ELEMENT::operator >>(string &str)
{
    switch (format)
    {
        case INT_DB:
            str = std::to_string(ele.INT_DB);
            break;
        case DOUBLE_DB:
            str = std::to_string(ele.DOUBLE_DB);
            break;
        case CHAR_DB:
            str = std::to_string(ele.CHAR_DB);
            break;
        case CHARS_DB: // string
            str = ele.STR;
            break;
    }
}

/*
 * @define
 *  若格式不相同，则返回false
 */
bool ELEMENT::operator==(const ELEMENT &el)
{
    if(format != el.format)
    {
        return false;
    }
    switch (format)
    {
        case INT_DB:
            return ele.INT_DB == el.ele.INT_DB;
            break;
        case DOUBLE_DB:
            return ele.DOUBLE_DB == el.ele.DOUBLE_DB;
            break;
        case CHAR_DB:
            return ele.CHAR_DB == el.ele.CHAR_DB;
            break;
        case CHARS_DB: // string
            if(strcmp(ele.STR,el.ele.STR) == 0)
            {
                return 1;
            }
            return 0;
            break;
    }
}
bool ELEMENT::operator>(const ELEMENT &el)
{
    if(format != el.format)
    {
        return false;
    }
    switch (format)
    {
        case INT_DB:
            return ele.INT_DB > el.ele.INT_DB;
            break;
        case DOUBLE_DB:
            return ele.DOUBLE_DB > el.ele.DOUBLE_DB;
            break;
        case CHAR_DB:
            return ele.CHAR_DB > el.ele.CHAR_DB;
            break;
        case CHARS_DB: // string
            if(strcmp(ele.STR,el.ele.STR) == 1)
            {
                return true;
            }
            return false;
            break;
    }
}
bool ELEMENT::operator<(const ELEMENT &el)
{
    if(format != el.format)
    {
        return false;
    }
    switch (format)
    {
        case INT_DB:
            return ele.INT_DB < el.ele.INT_DB;
            break;
        case DOUBLE_DB:
            return ele.DOUBLE_DB < el.ele.DOUBLE_DB;
            break;
        case CHAR_DB:
            return ele.CHAR_DB < el.ele.CHAR_DB;
            break;
        case CHARS_DB: // string
            if(strcmp(ele.STR,el.ele.STR) == -1)
            {
                return 1;
            }
            return 0;
            break;
    }
}

#endif //B_PC_ELEMENT_H
