//
// Created by 123 on 2019/12/15.
//

#ifndef B_PC_DBCORE_H
#define B_PC_DBCORE_H

#include <iostream>
#include <io.h>
#include <fstream>
#include <direct.h>

using std::ofstream ;
using std::ifstream ;
using std::cin;
using std::cout;

#include "default.h"
#include "Table.h"

struct DBCore
{
    void create_new_table(string table_name);
    void load_old_table(string table_name);
    void RUN();
    MetaCommandResult do_meta_command(const string &command);
    PrepareResult  prepare_statement(const string &command);
    ExecuteResult execute_statement(const string &command);

protected:
    vector<Table> db;
    size_t current_table = 0;
    string root_path = ".\\DB\\";
};


void DBCore::RUN()
{
    //create_new_table("LDB");   // 创建一个表
    load_old_table("LDB");
    current_table = 0;                    // 当前表

    string command_inputs;
    while (true)
    {
        printf("db > ");
        getline(cin,command_inputs);

        if (command_inputs[0] == '.')
        {
            switch (do_meta_command(command_inputs))
            {
                case (META_COMMAND_EXIT_SUCCESS):
                    for(auto t:db)
                    {
                        t.Save();
                    }
                    exit(EXIT_SUCCESS);
                    break;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    cout << "Unrecognized keyword at start of " << command_inputs << " .\n";
                    break;
                case (META_COMMAND_LOAD_SUCCESS):
                    cout << "Load Done!\n";
                    break;
                default:
                    break;
            }
            continue;
        }

        switch (prepare_statement(command_inputs))
        {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                cout << "Unrecognized keyword at start of " << command_inputs << " .\n";
                break;
            default:
                break;
        }

        switch (execute_statement(command_inputs))
        {
            case (EXECUTE_SUCCESS):
                cout<<"Executed!\n";
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
    db[current_table].Load(store_path);
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

MetaCommandResult DBCore::do_meta_command(const string &command)
{
    if (strcmp(command.c_str(), ".exit") == 0)
    {
        return META_COMMAND_EXIT_SUCCESS;
    }
    else if (strcmp(command.c_str(), ".load") == 0)
    {
        return META_COMMAND_LOAD_SUCCESS;
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}
PrepareResult  DBCore::prepare_statement(const string &command)
{
    if (command.substr(0, 6) == "insert")
    {
        db[current_table].statement.type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (command.substr(0,6) == "select")
    {
        db[current_table].statement.type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    if(command.substr(0,6) == "delete")
    {
        db[current_table].statement.type = STATEMENT_DELETE;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}
ExecuteResult DBCore::execute_statement(const string &command)
{
    switch (db[current_table].statement.type)
    {
        case STATEMENT_INSERT:
            //btree->ADD(statement.row_to_insert);
            db[current_table].add_init(command);
            return EXECUTE_SUCCESS;
            break;
        case STATEMENT_SELECT:
            //db[current_table].Print();
            db[current_table].select_init(command);
            return EXECUTE_SUCCESS;
            break;
        case STATEMENT_DELETE:
            db[current_table].delete_init(command);
            return EXECUTE_SUCCESS;
            break;
    }
}

#endif //B_PC_DBCORE_H
