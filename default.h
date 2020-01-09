//
// Created by zhixu on 2019/10/31.
//

#ifndef UNTITLED_DEFAULT_H
#define UNTITLED_DEFAULT_H


#include <sstream>
#include <regex>
#include <ctime>

#include "ROW.h"
#include "ELEMENT.h"

using baozhixue::ELEMENT;
using baozhixue::ROW;
using std::stringstream;

namespace baozhixue
{
    const int MaxIdLength = 16;		     //Row
    const int MaxUserNmae_length = 16;   // Row
    const int MaxEmail_Length = 32;      // Row
    const int ROW_SIZE = MaxIdLength + MaxUserNmae_length + MaxEmail_Length;
    const int PAGE_SIZE = 4096;
    const int TABLE_MAX_PAGES = 100;
    const int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
    const int TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;


    enum class StatementType {
        STATEMENT_INSERT,    // 插入一条数据
        STATEMENT_SELECT,    // 选择符合条件的数据
        STATEMENT_INVALID,  // 输入命令无效
        STATEMENT_DELETE,    // 删除符合条件的数据
        STATEMENT_EXIT,      // 退出
        STATEMENT_NONE,      // 无后续操作
        STATEMENT_CREATE,    // 创建一个新表
        STATEMENT_LOAD,      // 加载一个原有表
        STATEMENT_NONE_TABLE, // 未加载任何一个表
        STATEMENT_CLIENT        // 客户端模式
    };
    enum class PrepareResult { 
        PREPARE_SUCCESS, 
        PREPARE_UNRECOGNIZED_STATEMENT,
        STATEMENT_INIT_SERVER, // 初始化服务器
        STATEMENT_INIT_CLIENT,  // 初始化客户端
        STATEMENT_CLIENT,
    };
    enum class ExecuteResult { 
        EXECUTE_SUCCESS, 
        EXECUTE_TABLE_FULL,
        EXECUTE_CLIENT_EXIT  // 客户断开链接
    };

    /*
     *记录当前表的状态
     */
    struct Statement
    {
        Statement() { type = StatementType::STATEMENT_NONE; };
        Statement(vector<vector<string>> init_dic);
        ROW row;   // 插入ListBtree
        StatementType type;
        vector<ELEMENT> init_ROW;   // 存储原始row，在其他功能可能修改row的内容，使用init_ROW进行恢复
        vector<string> Labels;      // 存放标签
        size_t index = 0;
    };


    Statement::Statement(vector<vector<string>> init_dic)
    {
        for (int i = 0; i < init_dic.size() - 1; ++i)
        {
            if (init_dic[i][0] == "ADD")
            {
                if (init_dic[i][2] == "int")
                {
                    init_ROW.push_back(ELEMENT(baozhixue::ele_format::INT_DB, "0"));
                    Labels.push_back(init_dic[i][1]);
                }
                else if (init_dic[i][2] == "string")
                {
                    init_ROW.push_back(ELEMENT(baozhixue::ele_format::CHARS_DB, ""));
                    Labels.push_back(init_dic[i][1]);
                }
                else if (init_dic[i][2] == "char")
                {
                    init_ROW.push_back(ELEMENT(baozhixue::ele_format::CHAR_DB, ""));
                    Labels.push_back(init_dic[i][1]);
                }
            }
            if (init_dic[init_dic.size() - 1][1] == init_dic[i][1])
            {
                index = i;
            }
        }
        row = ROW(init_ROW, index);
    }
}



#endif //UNTITLED_DEFAULT_H

