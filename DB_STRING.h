#ifndef DB_STRING_H
#define DB_STRING_H

/*
 ������2020��1��5�գ���� ELEMENT.h �� element �ڵ�char* STR;
 ��1��ֻ�п�����ֵ��������ָ��ָ���ڴ档
 ��2����δ�����ܼ������ʹ���Թ����ڴ�
*/

#include <memory>
#include <string>
#include <iostream>

namespace baozhixue {

    //bool operator==(const char *ptr,const DB_STRING& R);
    //bool operator>(const DB_STRING& L, const DB_STRING& R);
    //bool operator<(const DB_STRING& L, const DB_STRING& R);
    //bool operator>=(const DB_STRING& L, const DB_STRING& R);
    //bool operator<=(const DB_STRING& L, const DB_STRING& R);
    //bool operator!=(const DB_STRING& L, const DB_STRING& R);

    class DB_STRING
    {
        friend std::ostream& operator<<(std::ostream& out, const DB_STRING&);
        friend void operator>>(std::istream& in, DB_STRING&);
    public:
        DB_STRING() = default;
        DB_STRING(const char*);
        DB_STRING(const string&);
        DB_STRING(DB_STRING& R);
        DB_STRING(DB_STRING&& R) noexcept;

        ~DB_STRING()
        {
            free(this->db_string);
        }
        DB_STRING& operator=(const char*);
        DB_STRING& operator=(const DB_STRING& R);
        DB_STRING& operator=(const std::string &s);
        DB_STRING* operator=(const DB_STRING* R);
        
        bool operator==(const DB_STRING& R);
        bool operator>(const DB_STRING& R);
        bool operator<(const DB_STRING& R);

        operator string() const;
        
        size_t Length();   // ���ز�����'\0'���ַ�������
        size_t Size();     // ����Length()+1
        
    private:
        char* db_string = nullptr;
    };

    bool DB_STRING::operator<(const DB_STRING& R)
    {
        return strcmp(this->db_string, R.db_string) == -1;
    }
    bool DB_STRING::operator>(const DB_STRING& R)
    {
        return strcmp(this->db_string, R.db_string) == 1;
    }
    bool DB_STRING::operator==(const DB_STRING& R)
    {
        return strcmp(this->db_string, R.db_string) == 0;
    }
    DB_STRING::operator string() const
    {
        return std::string(this->db_string);
    }
    DB_STRING& DB_STRING::operator=(const std::string& s)
    {
        *this = s.c_str();
        return *this;
    }
    size_t DB_STRING::Length()
    {
        return strlen(db_string);
    }
    size_t DB_STRING::Size()
    {
        return Length() + 1;
    }
    DB_STRING& DB_STRING::operator=(const char* ptr)
    {
        char *new_db_string = (char*)malloc((1 + strlen(ptr)) * sizeof(char));
        if (new_db_string != nullptr)
        {
            memcpy(new_db_string, ptr, 1 + strlen(ptr));
            if (this->db_string != nullptr)
            {
                free(this->db_string);
            }
            this->db_string = new_db_string;
        }
        return *this;
    }
    DB_STRING& DB_STRING::operator=(const DB_STRING& R)
    {
        *this = R.db_string;
        return *this;
    }
    DB_STRING::DB_STRING(const string& s)
    {
        *this = s;
    }
    DB_STRING::DB_STRING(const char* ptr)
    {
        char* tmp = (char*)malloc((1+strlen(ptr))*sizeof(char));
        if (tmp != nullptr)
        {
            memcpy(tmp, ptr, 1 + strlen(ptr));
            free(this->db_string);
            this->db_string = tmp;
        }
    }
    DB_STRING::DB_STRING(DB_STRING& R)
    {
        if (this->db_string == R.db_string)
        {
            return;
        }
        *this = R.db_string;
    }
    DB_STRING::DB_STRING(DB_STRING&& R) noexcept
    {
        if (this->db_string != nullptr)
        {
            free(this->db_string);
        }
        *this = R;
    }
    std::ostream& operator<<(std::ostream& out, const DB_STRING& tmp)
    {
        out << tmp.db_string;
        return out;
    }
    void operator>>(std::istream& in, DB_STRING& tmp)
    {
        std::string str_tmp;
        in >> str_tmp;
        tmp = str_tmp;
    }
    DB_STRING* DB_STRING::operator=(const DB_STRING* R)
    {
        *this = R->db_string;
        return this;
    }
}



#endif // DB_STRING_H
#pragma once
