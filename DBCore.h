//
// Created by 123 on 2019/12/15.
//

#ifndef B_PC_DBCORE_H
#define B_PC_DBCORE_H

#include <iostream>
#include <io.h>
#include <fstream>
#include <direct.h>
#include <ctime>
#include <regex>

using std::ofstream ;
using std::ifstream ;
using std::cin;
using std::cout;

#include "default.h"
#include "Table.h"

struct DBCore
{ 
    void RUN();
   
protected:
    vector<Table> db;
    size_t current_table = 0;
    string root_path = ".\\DB\\";
    bool EXIT = false;    // 若输入为exit则EXIT==TRUE

    PrepareResult  prepare_statement(const string& command);
    ExecuteResult execute_statement(const string& command);

    void create_new_table(string table_name);
    void load_old_table(string table_name);
    string now_time();
    StatementType running_statement = STATEMENT_NONE;  // DBCore对输入命令的处理结果
    bool select_init(string);
    bool insert_init(string);
    bool create_init(string);
    bool load_init(string);
    bool save_init();
    bool update_init(string);   // 待添加
    bool delete_init(string);   // 待添加
};


void DBCore::RUN()
{
    ifstream inFile;
    inFile.open("test.txt");

    string command_inputs;
    while (EXIT != true)
    {
        printf("db > ");
        getline(cin, command_inputs);

        switch (prepare_statement(command_inputs))
        {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            cout << "Unrecognized keyword at start of " << command_inputs << " .\n";
            continue;
            break;
        default:
            break;
        }

        switch (execute_statement(command_inputs))
        {
        case (EXECUTE_SUCCESS):
            cout << "Executed!\n";
            break;
        case (EXECUTE_TABLE_FULL):
            cout << "Error : Table full.\n";
            break;
        default:
            break;
        }
    }
}




void DBCore::load_old_table(string table_name)
{
    string store_path = root_path + table_name + "\\";
    db.push_back(Table());
    db.back().Load(store_path,table_name);
}

void DBCore::create_new_table(string table_name)
{
    cout <<"----------------------------------------\n";
    cout << "support type : int , string\n";
    cout <<" WARNING : Case sensitive\n";
    cout << "example:\n";
    cout <<"command  label   type    optional\n";
    cout <<"  ADD    name    string     16       (16 is name length)\n";
    cout <<"  ADD    year     int \n";
    cout <<" SEARCH   name \n";
    cout <<"  EXIT \n";
    cout <<"----------------------------------------\n";

    string str;
    vector<vector<string>> dic;
    vector<string> tmp;
    while (std::getline(cin,str) && str != "EXIT")
    {
        stringstream In(str);
        while (In >> str)
        {
            tmp.push_back(str);
        }
        dic.push_back(tmp);
        tmp.clear();
    }

    string store_path = root_path + table_name + "\\";

    db.push_back(Table(table_name,dic,store_path));

    cout << "init done!\n";

    // 将dic保存至 DB 路径下的dic.txt文件，留作下次加载数据时的依据
    if (_access(store_path.c_str(), 0) == -1)
    {
        _mkdir(store_path.c_str());
    }
    string dic_path = store_path + "dic.txt";
    ofstream outFile;
    outFile.open(dic_path.c_str());
    for (auto el : dic)
    {
        for (auto s : el)
        {
            outFile << s <<" ";
        }
        outFile << "\n";
    }
    outFile.close();
}


PrepareResult DBCore::prepare_statement(const string &command)
{
    if (command == "show tables")
    {
        for (auto el : db)
        {
            cout << el.get_name() << "\n";
        }
        running_statement = STATEMENT_INVALID; // 无后续操作
    }
    else if (command.substr(0, 6) == "insert")
    {
        running_statement = STATEMENT_INSERT;
    }
    else if (command.substr(0,6) == "select")
    {
        running_statement = STATEMENT_SELECT;
    }
    else if(command.substr(0,6) == "delete")
    {
        running_statement = STATEMENT_DELETE;
    }
    else if (command == "exit")
    {
        running_statement = STATEMENT_EXIT;
    }
    else if (command.substr(0, 6) == "create")
    {
        running_statement = STATEMENT_CREATE;
    }
    else if (command.substr(0, 4) == "load")
    {
        running_statement = STATEMENT_LOAD;
    }

    
    if (running_statement != STATEMENT_NONE)
    {
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}
ExecuteResult DBCore::execute_statement(const string &command)
{
    switch (running_statement)
    {
        case STATEMENT_INSERT:
            insert_init(command);
            break;
        case STATEMENT_SELECT:
            select_init(command);
            break;
        case STATEMENT_CREATE:
            create_init(command);
            break;
        case STATEMENT_EXIT:
            save_init();
            EXIT = true;
            break;
        case STATEMENT_LOAD:
            load_init(command);
            break;
        case STATEMENT_DELETE:
            delete_init(command);
            break;
        case STATEMENT_NONE_TABLE:
            cout << "not exist any table!\n";
            break;
    }

    if (running_statement != STATEMENT_NONE)
    {
        running_statement = STATEMENT_NONE;
        return EXECUTE_SUCCESS;
    }
    return EXECUTE_TABLE_FULL;
}

string DBCore::now_time()
{
    time_t time_now = time(NULL);
    return asctime(localtime(&time_now));
}


bool DBCore::select_init(string command)
{
    vector<string> pattern{ "select (.*) from (.*)",         // select LABEL from TABLE_NAME, 打印table内的所有内容
                            "select now()",                 // 以string格式输出当前时间
    };
    

    size_t pattern_match;
    std::smatch match_result;

    for (pattern_match = 0; pattern_match < pattern.size(); ++pattern_match)
    {
        if (std::regex_match(command, std::regex(pattern[pattern_match])))
        {
            break;
        }
    }

    // 若命令输入不匹配，则输出false
    
    if (pattern_match < pattern.size())
    {
        std::regex_search(command, match_result, std::regex(pattern[pattern_match]));
        if (pattern_match == 0)
        {
            for (auto &tmp_table : db)
            {
                if (tmp_table.get_name() == match_result[2])
                {   
                    if (match_result[1] == "*")
                    {
                        tmp_table.Print();
                    }
                    else
                    {
                        cout << "command is not support now!\n";
                    }
                    return true;
                }
            }
        }
        else if (pattern_match == 1)
        {
            cout << now_time();
            return true;
        }
        
    }
    return false;
}

/*
    根据表插入相应数据
*/
bool DBCore::insert_init(string command)
{
    vector<string> pattern = {
        "insert into (.*)",
    };

    size_t pattern_match = 0;
    std::smatch match_result;
    string tmp_insert_data;
    for (pattern_match = 0; pattern_match < pattern.size(); ++pattern_match)
    {
        if (std::regex_match(command, std::regex(pattern[pattern_match])))
        {
            break;
        }
    }

    if (pattern_match < pattern.size())
    {
        if (pattern_match == 0)  
        {
            std::regex_search(command, match_result, std::regex(pattern[pattern_match]));
            
            cout << " ->";
            getline(cin, tmp_insert_data);

            for (auto &tmp_table : db)
            {
                if (tmp_table.get_name() == match_result[1])
                {
                    tmp_table.statement.row << tmp_insert_data;
                    tmp_table.ADD(tmp_table.statement.row);
                    return true;
                }
            }
        }
    }
    return false;
}

/*
    根据表名创建新表
*/
bool DBCore::create_init(string command)
{

    if (std::regex_match(command, std::regex("create table (.*)")))
    {
        std::smatch match_result;
        std::regex_search(command, match_result, std::regex("create table (.*)"));
        for (auto tmp_table : db)
        {
            // 若表存在则停止创建新表
            if (tmp_table.get_name() == match_result[1])
            {
                return true;
            }
        }
        create_new_table(match_result[1]);
        return true;
    }
    return false;
}

/*
    根据表名加载相应表
*/
bool DBCore::load_init(string command)
{
    if (std::regex_match(command, std::regex("load table (.*)")))
    {
        std::smatch match_result;
        std::regex_search(command, match_result, std::regex("load table (.*)"));
        load_old_table(match_result[1]);
        return true;
    }
    return false;
}

/*
    保存所有已加载表
*/
bool DBCore::save_init()
{
    for (auto t : db)
    {
        t.Save();
    }
    return true;
}


/*
待添加
    @future function
*/
bool DBCore::update_init(string command)
{
    //目前仅支持修改整条数据
    return true;
}

/*
待添加
    @ delete X from T(TABLE)
*/
bool DBCore::delete_init(string)
{

    return true;
}

#endif //B_PC_DBCORE_H
