//
// Created by 123 on 2019/12/15.
//

#ifndef B_PC_TABLE_H
#define B_PC_TABLE_H

#include <iostream>
#include <io.h>
#include <fstream>
#include <direct.h>
#include <queue>
using std::queue;
using std::ofstream ;
using std::ifstream ;
using std::cin;
using std::cout;
#include "default.h"
#include "ListBTree.h"
#include "ROW.h"


struct Table: public ListBTree<ROW>
{
    Table() = default;
    Table(string _name, vector<vector<string>> init_dic, string _store_path)
    {
        WORKING = true;
        dic = init_dic;
        name = _name;
        statement = Statement(init_dic);
        store_path = _store_path;
    }

    
    Statement statement;
    int getFilesNum(string path);
    void getAllFiles(string path, vector<string>& files);
    void Load(string store_path, string table_name);  // and init Table
    void Save();

    string get_name() const { return name; }
protected:
    vector<vector<string>> dic;
    string name;
    bool WORKING = false;
    string store_path;
};


void Table::Load(string store_path,string table_name)
{
    cout << "Load from " << store_path << "\n";
    this->store_path = store_path;
    ifstream inFile;
    string tmp;

    // 检测文件夹下是否存在dic文件,，并读取内容初始化Table
    string dic_file_path = store_path + "dic.txt";
    if (_access(dic_file_path.c_str(), 0) != 0)
    {
        cout << "dic file is not exist!\n";
        return;
    }
    inFile.open(dic_file_path, std::ios::in);
    vector<vector<string>> dic;
    while (std::getline(inFile,tmp))
    {
        stringstream In(tmp);
        dic.push_back(vector<string>());
        while (In)
        {
            In >> tmp;
            dic[dic.size()-1].push_back(tmp);
        }
    }
    inFile.close();
    this->dic = dic;
    statement = Statement(dic);
    WORKING = true;
    name = table_name;
    cout << "table " <<name <<" is init done! Load data ...\n";


    
    // 加载数据
    string data_path = store_path + "data";
    if (_access(data_path.c_str(), 0) != 0)
    {
        cout << "data file is not exist!\n";
        return;
    }
    inFile.open(data_path, std::ios::in);
    while (getline(inFile,tmp))
    {
        statement.row << tmp;
        ADD(statement.row);
    }

    cout << "load done!\n";
    return;
}

void Table::Save()
{
    if (root == nullptr)
    {
        return;
    }
    //vector<Node*> tmp;
    Node_LBTree<ROW>* tmp = root;
    
    string dst_file = store_path + "data";
    ofstream outFile;
    
    outFile.open(dst_file.c_str());
    Print2(tmp, outFile);
    outFile.close();
    return;
}



void Table::getAllFiles(string path, vector<string>& files)
{
    // 文件句柄
    long hFile = 0;
    // 文件信息
    struct _finddata_t fileinfo;

    string p;

    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
        do {
            // 保存文件的全路径
            files.push_back(p.assign(path).append("\\").append(fileinfo.name));

        } while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

        _findclose(hFile);
    }
}


/*
	@ define
		获得文件夹下文件数量,
	@ para
		path 文件夹路径
	@ return
		返回数量值
*/
int Table::getFilesNum(string path)
{
    // 文件句柄
    long hFile = 0;
    size_t num = 0;
    // 文件信息
    struct _finddata_t fileinfo;
    string p;
    string fileType = ".txt";
    if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
        do {
            // 保存文件的全路径
            num += 1;
        } while (_findnext(hFile, &fileinfo) == 0); //寻找下一个，成功返回0，否则-1

        _findclose(hFile);
    }
    return num;
}

#endif //B_PC_TABLE_H
