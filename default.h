//
// Created by zhixu on 2019/10/31.
//

#ifndef UNTITLED_DEFAULT_H
#define UNTITLED_DEFAULT_H


#include <sstream>
#include <regex>
#include <ctime>



#include "ROW.h"
#include "ListBTree.h"

using std::stringstream;

const int MaxIdLength = 16;		     //Row
const int MaxUserNmae_length = 16;   // Row
const int MaxEmail_Length = 32;      // Row
const int ROW_SIZE = MaxIdLength + MaxUserNmae_length + MaxEmail_Length;
const int PAGE_SIZE = 4096;
const int TABLE_MAX_PAGES = 100;
const int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const int TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;


enum MetaCommandResult { META_COMMAND_EXIT_SUCCESS, META_COMMAND_UNRECOGNIZED_COMMAND, META_COMMAND_LOAD_SUCCESS };
enum StatementType { STATEMENT_INSERT, STATEMENT_SELECT, STATEMENT_INVALID ,STATEMENT_DELETE, STATEMENT_NONE};
enum PrepareResult { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT };
enum ExecuteResult { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL };

/*
 *记录当前表的状态
 */
struct Statement
{
    Statement() {type = STATEMENT_NONE; };
    Statement(vector<vector<string>> init_dic);
    ROW row;   // 插入ListBtree
	StatementType type;
    vector<ELEMENT> init_ROW;   // 存储原始row，在其他功能可能修改row的内容，使用init_ROW进行恢复
    vector<string> Labels;      // 存放标签
    size_t index = 0;
};




Statement::Statement(vector<vector<string>> init_dic)
{
    for(int i = 0; i < init_dic.size() - 1;++i)
    {
        if(init_dic[i][0] == "ADD")
        {
            if(init_dic[i][2] == "int")
            {
                init_ROW.push_back(ELEMENT(INT_DB,"0"));
            }
            else if(init_dic[i][2] == "string")
            {
                init_ROW.push_back(ELEMENT(CHARS_DB,""));
            }
            else if(init_dic[i][2] == "char")
            {
                init_ROW.push_back(ELEMENT(CHAR_DB,""));
            }
        }
        if(init_dic[init_dic.size()-1][1] == init_dic[i][1])
        {
            index = i;
        }
    }
    row = ROW(init_ROW,index);
}

#endif //UNTITLED_DEFAULT_H

