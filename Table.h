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
    Table(string _name,vector<vector<string>> init_dic,string _store_path)
    {
        WORKING = true;
        dic = init_dic;
        name = _name;
        statement = Statement(init_dic);
        store_path = _store_path;
    }
    Statement statement;
    int getFilesNum(string path);
    void Load(string store_path);  // and init Table
    void Save();
    void Table::getAllFiles(string path, vector<string>& files);

protected:
    vector<vector<string>> dic;
    string name;
    bool WORKING = false;
    string store_path;
};


void Table::Load(string store_path)
{

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
    this->dic = dic;
    statement = Statement(dic);
    WORKING = true;


    // 获得当前目录下的文件数量
    size_t total_file_nums = getFilesNum(store_path);
    if(total_file_nums <= 1)
    {
        return;
    }

    string load_path = store_path;
    string dst_file = load_path + "root.txt";

    vector<vector<size_t>> vec_1, vec_2;
    vector<Node_LBTree<ROW>*> vec_n;

    ROW tmp_Row = statement.row;

    inFile.open(dst_file, std::ios::in);

    // 读取root index
    getline(inFile, tmp);
    root = new Node_LBTree<ROW>();
    root->index = std::atoi(tmp.c_str());
    getline(inFile,name);
    inFile.close();

    // 读取第一条数据
    dst_file = load_path + std::to_string(root->index) + ".txt";
    inFile.open(dst_file, std::ios::in);
    vec_1.push_back(vector<size_t>());
    vec_1[0].push_back(root->index);
    getline(inFile, tmp);
    stringstream In(tmp);
    while (In)
    {
        In >> tmp;
        vec_1[0].push_back(std::atoi(tmp.c_str()));
    }

    while (getline(inFile, tmp))
    {
        tmp_Row <<tmp;
        root->keys.push_back(tmp_Row);
        root->keys_size += 1;
    }

    vec_1[0].pop_back();
    vec_n.push_back(root);
    inFile.close();

    // 加载所有数据
    size_t k = 0;

    while (vec_1.size() < total_file_nums - 2)  //root和dic文件不算在内
    {
        // 添加子index

        for (int i = k; i < vec_1.size(); ++i)  // father
        {
            int father_index = vec_1[i][0];
            Node_LBTree<ROW>* father = nullptr;
            for (size_t g = 0; g < vec_n.size(); ++g)
            {
                if (vec_n[g]->index == father_index)
                {
                    father = vec_n[g];
                    break;
                }
            }

            for (int j = 1; j < vec_1[i].size(); ++j)  // child
            {
                dst_file = load_path + std::to_string(vec_1[i][j]) + ".txt";
                inFile.open(dst_file.c_str(), std::ios::in);
                // step 1  ,读取index
                getline(inFile, tmp);

                vec_2.push_back(vector<size_t>());
                vec_2.back().push_back(vec_1[i][j]);
                In.clear();
                In.str(tmp);
                while (In)
                {
                    In >> tmp;
                    vec_2.back().push_back(std::atoi(tmp.c_str()));
                }
                vec_2.back().pop_back();

                // step 2 ，加载数据
                Node_LBTree<ROW>* tmp_Node2 = new Node_LBTree<ROW>();
                father->keys_child[father->keys_child_size] = tmp_Node2; father->keys_child_size += 1;
                tmp_Node2->index = vec_1[i][j];

                while (getline(inFile, tmp))
                {
                    tmp_Row <<tmp;
                    tmp_Node2->keys.push_back(tmp_Row);
                    tmp_Node2->keys_size += 1;
                }
                if (vec_1[i].size() > 1)
                {
                    vec_n.push_back(tmp_Node2);
                }
                // end
                inFile.close();
            }
        }
        k = vec_1.size();
        vec_1.insert(vec_1.end(), vec_2.begin(), vec_2.end());
        vec_2.clear();
    }
    return;
}

void Table::Save()
{
    if (root == nullptr)
    {
        return;
    }

    //vector<Node*> tmp;
    queue<Node_LBTree<ROW>*> tmp;
    tmp.push(root);
    string dst_file;
    string save_path = store_path;// string(path);
    ofstream outFile;
    while (tmp.size() > 0)
    {
        dst_file = save_path + std::to_string(tmp.front()->index) + ".txt";
        outFile.open(dst_file.c_str());
        // 写入keys_child 的index
        for (size_t i = 0; i < tmp.front()->keys_child_size; ++i)
        {
            outFile << tmp.front()->keys_child[i]->index << " ";
            tmp.push(tmp.front()->keys_child[i]);
        }
        outFile << "\n";
        // 写入本节点数据
        string write_buffer;
        for (size_t i = 0; i < tmp.front()->keys_size; ++i)
        {
            tmp.front()->keys[i]>>write_buffer;
            outFile << tmp.front()->keys[i] << "\n";
        }
        tmp.pop();
        outFile.close();
    }
    dst_file = save_path + "root.txt";
    outFile.open(dst_file.c_str());
    outFile << root->index<<"\n";
    outFile << name;
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
