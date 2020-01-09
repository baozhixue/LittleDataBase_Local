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
#include "Message.h"
using baozhixue::Table;


namespace baozhixue
{
    class DBCore
    {
    public:
        void RUN();
    protected:
        vector<Table> db;
        size_t current_table = 0;
        string root_path = ".\\DB\\";
        bool EXIT = false;    // 若输入为exit则EXIT==TRUE

        baozhixue::PrepareResult  prepare_statement(const string& command);
        baozhixue::ExecuteResult execute_statement(const string& command);

        void create_new_table(string table_name);
        void load_old_table(string table_name);
        string now_time();
        baozhixue::StatementType running_statement = baozhixue::StatementType::STATEMENT_NONE;  // DBCore对输入命令的处理结果
        bool select_init(string);
        bool insert_init(string);
        bool create_init(string);
        bool load_init(string);
        bool save_init();
        bool update_init(string);   // 待添加
        bool delete_init(string);   // 待添加
        bool server_init(string);   // 实现服务器版本
        bool client_init(string);   // 实现客户端版本

        bzx_message::Message message;
    };


    void DBCore::RUN()
    {
        
        string command_inputs;
        message.test_mode("test2.txt");
        while (EXIT != true)
        {
            command_inputs = message.INPUT();    
            switch (prepare_statement(command_inputs))
            {
            case (baozhixue::PrepareResult::PREPARE_SUCCESS):
                break;
            case (baozhixue::PrepareResult::PREPARE_UNRECOGNIZED_STATEMENT):
                message.OUTPUT("Unrecognized keyword at start of " + command_inputs + ".");
                message.OUTPUT("Executed!");
                continue;
                break;
            case baozhixue::PrepareResult::STATEMENT_INIT_CLIENT:
            case baozhixue::PrepareResult::STATEMENT_INIT_SERVER:
                continue;
                break;
            case baozhixue::PrepareResult::STATEMENT_CLIENT:
                message.OUTPUT("");
                continue;
            default:
                break;
            }
            
            switch (execute_statement(command_inputs))
            {
            case (baozhixue::ExecuteResult::EXECUTE_SUCCESS):
                message.OUTPUT("Executed!");
                break;
            case (baozhixue::ExecuteResult::EXECUTE_TABLE_FULL):
                message.OUTPUT("Error : Table full.");
                break;
            default:
                break;
            }
        }
        
        system("pause");
    }




    void DBCore::load_old_table(string table_name)
    {
        string store_path = root_path + table_name + "\\";
        db.push_back(Table());
        db.back().Load(store_path, table_name);
    }

    void DBCore::create_new_table(string table_name)
    {
        string str = "----------------------------------------\n"
                    "support type : int , string\n"
                    " WARNING : Case sensitive\n"
                    "example:\n"
                    "command  label   type    optional\n"
                    "  ADD    name    string     16       (16 is name length)\n"
                    "  ADD    year     int \n"
                    "  SEARCH   name \n"
                    "  end table \n"
                    "----------------------------------------";

        
        vector<vector<string>> dic;
        vector<string> tmp;
        //while (std::getline(cin, str) && str != "EXIT")
        message.OUTPUT(str);
        if (message.get_rm() == bzx_message::RUNNING_MODE::SERVER)
        {
            message.SERVER_NEXT_INPUT();
        }
        
        str = message.INPUT("  >");
        if (message.get_rm() == bzx_message::RUNNING_MODE::SERVER)
        {
            message.SERVER_NEXT_INPUT();
        }

        while(str != "end table")
        {
            stringstream In(str);
            while (In >> str)
            {
                tmp.push_back(str);
            }
            dic.push_back(tmp);
            tmp.clear();
            str = message.INPUT("  >");
            if (message.get_rm() == bzx_message::RUNNING_MODE::SERVER && str != "end table")
            {
                message.SERVER_NEXT_INPUT();
            }
        }

        string store_path = root_path + table_name + "\\";

        db.push_back(Table(table_name, dic, store_path));
        message.OUTPUT("init done!");

        // 将dic保存至 DB 路径下的dic.txt文件，留作下次加载数据时的依据
        if (_access(store_path.c_str(), 0) == -1)
        {
            if (_mkdir(store_path.c_str())==-1)
            {
                message.OUTPUT("failed to mkdir " + store_path);
                return;
            }

        }
        string dic_path = store_path + "dic.txt";
        ofstream outFile;
        outFile.open(dic_path.c_str());
        for (auto el : dic)
        {
            for (auto s : el)
            {
                outFile << s << " ";
            }
            outFile << "\n";
        }
        outFile.close();
        message.OUTPUT("save dic done!");
    }


    baozhixue::PrepareResult DBCore::prepare_statement(const string& command)
    {
        if (command == "exit")
        {
            running_statement = baozhixue::StatementType::STATEMENT_EXIT;
            return baozhixue::PrepareResult::PREPARE_SUCCESS;
        }
        else if (message.get_rm() != bzx_message::RUNNING_MODE::USER)
        {
            if (command == "show tables")
            {
                for (auto el : db)
                {
                    message.OUTPUT(el.get_name());
                }
                running_statement = baozhixue::StatementType::STATEMENT_INVALID; // 无后续操作
            }
            else if (command.substr(0, 6) == "insert")
            {
                running_statement = baozhixue::StatementType::STATEMENT_INSERT;
            }
            else if (command.substr(0, 6) == "select")
            {
                running_statement = baozhixue::StatementType::STATEMENT_SELECT;
            }
            else if (command.substr(0, 6) == "delete")
            {
                running_statement = baozhixue::StatementType::STATEMENT_DELETE;
            }
            else if (command == "exit")
            {
                running_statement = baozhixue::StatementType::STATEMENT_EXIT;
            }
            else if (command.substr(0, 6) == "create")
            {
                running_statement = baozhixue::StatementType::STATEMENT_CREATE;
            }
            else if (command.substr(0, 4) == "load")
            {
                running_statement = baozhixue::StatementType::STATEMENT_LOAD;
            }
            else if (command.substr(0, 6) == "server" && server_init(command))
            {
                return baozhixue::PrepareResult::STATEMENT_INIT_SERVER;
            }
            else if (command.substr(0, 6) == "client" && client_init(command))
            {
                return baozhixue::PrepareResult::STATEMENT_INIT_CLIENT;
            }

            if (running_statement != baozhixue::StatementType::STATEMENT_NONE)
            {
                return baozhixue::PrepareResult::PREPARE_SUCCESS;
            }
            return baozhixue::PrepareResult::PREPARE_UNRECOGNIZED_STATEMENT;
        }
        return baozhixue::PrepareResult::STATEMENT_CLIENT;
    }

    baozhixue::ExecuteResult DBCore::execute_statement(const string& command)
    {
        if (running_statement == baozhixue::StatementType::STATEMENT_EXIT)
        {
            save_init();
            if (message.get_rm() == bzx_message::RUNNING_MODE::SERVER)
            {
                message.SERVER_Kill(0);
                return baozhixue::ExecuteResult::EXECUTE_CLIENT_EXIT;
            }
            else
            {
                if (message.get_rm() == bzx_message::RUNNING_MODE::USER) {
                    message = bzx_message::Message();
                }
                EXIT = true;
                return baozhixue::ExecuteResult::EXECUTE_SUCCESS;
            }
        }
        else if(message.get_rm() != bzx_message::RUNNING_MODE::USER)
        {
            switch (running_statement)
            {
            case baozhixue::StatementType::STATEMENT_INSERT:
                insert_init(command);
                break;
            case baozhixue::StatementType::STATEMENT_SELECT:
                select_init(command);
                break;
            case baozhixue::StatementType::STATEMENT_CREATE:
                create_init(command);
                break;
            //case baozhixue::StatementType::STATEMENT_EXIT:
            //    save_init();
            //    if (message.get_rm() == bzx_message::RUNNING_MODE::SERVER)
            //    {
            //        message.SERVER_Kill(0);
            //        return baozhixue::ExecuteResult::EXECUTE_CLIENT_EXIT;
            //    }
            //    else
            //    {
            //        EXIT = true;
            //        return baozhixue::ExecuteResult::EXECUTE_SUCCESS;
            //    }
            //    break;
            case baozhixue::StatementType::STATEMENT_LOAD:
                load_init(command);
                break;
            case baozhixue::StatementType::STATEMENT_DELETE:
                delete_init(command);
                break;
            case baozhixue::StatementType::STATEMENT_NONE_TABLE:
                message.OUTPUT("not exist any table!\n");
                break;
            }

            if (running_statement != baozhixue::StatementType::STATEMENT_NONE)
            {
                running_statement = baozhixue::StatementType::STATEMENT_NONE;
                return baozhixue::ExecuteResult::EXECUTE_SUCCESS;
            }
            return baozhixue::ExecuteResult::EXECUTE_TABLE_FULL;
        }        
    }

    string DBCore::now_time()
    {
        time_t time_now = time(NULL);
        char* dst = asctime(localtime(&time_now));
        return dst == NULL ? "" : dst;
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
                for (auto& tmp_table : db)
                {
                    if (tmp_table.get_name() == match_result[2])
                    {
                        if (match_result[1] == "*")
                        {
                            tmp_table.Print(message);
                        }
                        else
                        {
                            message.OUTPUT("command is not support now!\n");
                        }
                        return true;
                    }
                }
            }
            else if (pattern_match == 1)
            {
                message.OUTPUT(now_time() + "\n");
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
            "insert into ([^ ]*)",
             "insert into ([^ ]*) (.*)",
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
            std::regex_search(command, match_result, std::regex(pattern[pattern_match]));
            if (pattern_match == 0)
            {
                message.OUTPUT(" ->");
                if (message.get_rm() == bzx_message::RUNNING_MODE::SERVER) {
                    message.SERVER_NEXT_INPUT();
                }
                tmp_insert_data = message.INPUT();
                for (auto& tmp_table : db)
                {
                    if (tmp_table.get_name() == match_result[1])
                    {
                        tmp_table.statement.row << tmp_insert_data;
                        tmp_table.ADD(tmp_table.statement.row);
                        return true;
                    }
                }
            }
            else if (pattern_match == 1)
            {
                for (auto& tmp_table : db)
                {
                    if (tmp_table.get_name() == match_result[1])
                    {
                        tmp_table.statement.row << match_result[2];
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
        return false;
    }

    /*
    待添加
        @ delete X from T(TABLE)
    */
    bool DBCore::delete_init(string command)
    {
        vector<string> pattern = {
           "delete (.*) from (.*)",
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
            std::regex_search(command, match_result, std::regex(pattern[pattern_match]));
            if (pattern_match == 0)
            {
                for (auto& tmp_table : db)
                {
                    if (tmp_table.get_name() == match_result[2])
                    {
                        tmp_table.statement.row << match_result[1];
                        tmp_table.Delete(tmp_table.statement.row);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /*
        传入port，
    */
    bool DBCore::server_init(string command)   // 实现服务器版本
    {
        // server [可选，默认接口6666]
        if (command == "server")
        {
            return message.SERVER();
        }
        return false;
    }
    bool DBCore::client_init(string command)   // 实现客户端版本
    {
        if (command == "client")
        {
            return message.CLIENT();
        }
        return false;
    }
}



#endif //B_PC_DBCORE_H
