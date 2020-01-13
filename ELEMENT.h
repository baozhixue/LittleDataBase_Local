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

#include "DB_STRING.h"

namespace baozhixue
{
    enum class ele_format { 
        INT_DB, DOUBLE_DB, CHAR_DB, CHARS_DB,NONE };
    
    union element {
        int INT_DB;
        double DOUBLE_DB;
        char CHAR_DB;
        //char* STR;  
        baozhixue::DB_STRING* STR2;
    };
    

    class ELEMENT
    {
    public:
        ELEMENT() = default;
        ELEMENT(const ele_format& type, const string& el);
        
        element ele;
        ele_format format;
        
        bool operator==(const ELEMENT& el);
        bool operator>(const ELEMENT& el);
        bool operator<(const ELEMENT& el);
        operator string () const
        {
            string str;
            switch (format)
            {
            case ele_format::INT_DB:
                str = std::to_string(ele.INT_DB);
                break;
            case ele_format::DOUBLE_DB:
                str = std::to_string(ele.DOUBLE_DB);
                break;
            case ele_format::CHAR_DB:
                str = std::to_string(ele.CHAR_DB);
                break;
            case ele_format::CHARS_DB: // string
                //str = ele.STR;
                str = *ele.STR2;
                break;
            }
            return str;
        }
    };

    /*
     * @define
     *      更新ELEMENT
     * @para [type]
     * @para [el]
     */
    ELEMENT::ELEMENT(const ele_format& type, const string& el)
    {
        format = type;
        switch (type)
        {
        case ele_format::INT_DB:
            ele.INT_DB = std::stoi(el);
            break;
        case ele_format::DOUBLE_DB:
            ele.DOUBLE_DB = std::stod(el);
            break;
        case ele_format::CHAR_DB:
            ele.CHAR_DB = el[0];
            break;
        case ele_format::CHARS_DB: // string
            //ele.STR = (char*)malloc(1 + el.length() * sizeof(char));
            //if (ele.STR != nullptr)
            //{
            //    memcpy(ele.STR, el.c_str(), el.length() + 1);
            //    ele.STR[el.length()] = '\0';
            //}
            ele.STR2 = new baozhixue::DB_STRING(el);
            break;
        }
    }



    /*
     * @define
     *  若格式不相同，则返回false
     */
    bool ELEMENT::operator==(const ELEMENT& el)
    {
        if (format != el.format)
        {
            return false;
        }
        switch (format)
        {
        case ele_format::INT_DB:
            return ele.INT_DB == el.ele.INT_DB;
            break;
        case ele_format::DOUBLE_DB:
            return ele.DOUBLE_DB == el.ele.DOUBLE_DB;
            break;
        case ele_format::CHAR_DB:
            return ele.CHAR_DB == el.ele.CHAR_DB;
            break;
        case ele_format::CHARS_DB: // string
            return ele.STR2 == el.ele.STR2;
            break;
        }
        return false;
    }
    bool ELEMENT::operator>(const ELEMENT& el)
    {
        if (format != el.format)
        {
            return false;
        }
        switch (format)
        {
        case ele_format::INT_DB:
            return ele.INT_DB > el.ele.INT_DB;
            break;
        case ele_format::DOUBLE_DB:
            return ele.DOUBLE_DB > el.ele.DOUBLE_DB;
            break;
        case ele_format::CHAR_DB:
            return ele.CHAR_DB > el.ele.CHAR_DB;
            break;
        case ele_format::CHARS_DB: // string
            return ele.STR2 > el.ele.STR2;
            break;
        }
        return false;
    }
    bool ELEMENT::operator<(const ELEMENT& el)
    {
        if (format != el.format)
        {
            return false;
        }
        switch (format)
        {
        case ele_format::INT_DB:
            return ele.INT_DB < el.ele.INT_DB;
            break;
        case ele_format::DOUBLE_DB:
            return ele.DOUBLE_DB < el.ele.DOUBLE_DB;
            break;
        case ele_format::CHAR_DB:
            return ele.CHAR_DB < el.ele.CHAR_DB;
            break;
        case ele_format::CHARS_DB: // string
            return ele.STR2 < el.ele.STR2;
            break;
        }
        return false;
    }
}


#endif //B_PC_ELEMENT_H
