//
// Created by 123 on 2019/12/13.
//

#ifndef B_PC_LISTBTREE_H
#define B_PC_LISTBTREE_H

#include "List2.h"
#include "Message.h"
using baozhixue::List;
using std::ofstream;

namespace baozhixue
{
    template <typename T>
    struct Node_LBTree
    {
        Node_LBTree(T el) { keys.push_back(el); }
        Node_LBTree() = default;

        List<T> keys;
        List<Node_LBTree<T>*>  keys_child;
        size_t index = 0;
        size_t keys_size = 0;
        size_t keys_child_size = 0;
    };

    template <typename T>
    struct Find2_return_LB
    {
        Node_LBTree<T>* father = nullptr, * find_node = nullptr;
        int index = -1;
        Find2_return_LB(Node_LBTree<T>* node) { father = find_node = node; }
    };

    template <typename  T>
    class ListBTree
    {
    public:

        ListBTree<T>(int max_keys_size)
        {
            this->MAX_KEYS_SIZE = max_keys_size;
            this->KEYS_MID = max_keys_size / 2;
        }
        ListBTree<T>() = default;

        void Print();
        void Print(bzx_message::Message& message);
        bool ADD(T el);
        bool Delete(T el);
        Find2_return_LB<T> Find2(T row);    // return find_node, father_node
        void set_root(Node_LBTree<T>* r) { root = r; }
        Node_LBTree<T>* get_root() { return root; }

    protected:
        Node_LBTree<T>* add(T el, Node_LBTree<T>* tmp);
        virtual void Print2(Node_LBTree<T>* node);
        void Print2(Node_LBTree<T>* node, ofstream& out);
        void Print2(Node_LBTree<T>* node, bzx_message::Message& message);
        bool Balance_Leaf_Nodes(Node_LBTree<T>* father_node);
        void Merge_father_2_child_node(Node_LBTree<T>* father_node);
        Node_LBTree<T>* root = nullptr;
        size_t index = 0;
        //void operator +=(T el);  //equals B.ADD(el)

        size_t MAX_KEYS_SIZE = 49;
        size_t KEYS_MID = 24;
    };

    
    template <typename  T>
    bool ListBTree<T>::ADD(T el) {
        if (root == nullptr)
        {
            root = new Node_LBTree<T>(el);
            return true;
        }
        Node_LBTree<T>* tmp = root;
        tmp = add(el, tmp);
        if (tmp != nullptr)
        {
            root = tmp;
        }
        return true;
    }

    template <typename T>
    Node_LBTree<T>* ListBTree<T>::add(T row, Node_LBTree<T>* tmp)
    {
        Node_LBTree<T>* re = nullptr;
        if (tmp->keys_child.Size == 0)    // 叶节点
        {
            size_t i = 0;
            while (i < tmp->keys.Size)
            {
                if (row < tmp->keys[i])
                {
                    break;
                }
                if (row == tmp->keys[i])
                {
                    return nullptr;
                }
                ++i;
            }

            // 将新元素加入相应位置
            tmp->keys.insert(row, i);
            // 检测是否达到最大容量，即是否需要拆分
            if (tmp->keys.Size == MAX_KEYS_SIZE)
            {
                re = new Node_LBTree<T>(tmp->keys[KEYS_MID]);
                Node_LBTree<T>* key2 = new Node_LBTree<T>();
                key2->keys = *tmp->keys.cut(KEYS_MID + 1);
                tmp->keys.resize(KEYS_MID);
                if (tmp == root)
                {
                    re->keys_child.push_back(tmp);
                }
                re->keys_child.push_back(key2);
            }
        }
        else
        {
            size_t j = 0;
            while (j < tmp->keys.Size)
            {
                if (row < tmp->keys[j])
                {
                    break;
                }
                if (row == tmp->keys[j])
                {
                    return nullptr;
                }
                ++j;
            }
            re = add(row, tmp->keys_child[j]);

            if (re != nullptr)
            {
                size_t k = 0;
                while (k < tmp->keys.Size)
                {
                    if (re->keys[0] < tmp->keys[k])
                    {
                        break;
                    }
                    ++k;
                }
                tmp->keys.insert(re->keys[0], k);
                tmp->keys_child.insert(re->keys_child[0], k + 1);
                re = nullptr;

                if (tmp->keys.Size == MAX_KEYS_SIZE)
                {
                    re = new Node_LBTree<T>(tmp->keys[KEYS_MID]);
                    Node_LBTree<T>* key2 = new Node_LBTree<T>();
                    key2->keys = *tmp->keys.cut(KEYS_MID + 1);
                    key2->keys_child = *(tmp->keys_child.cut(KEYS_MID + 1));
                    key2->keys.resize(KEYS_MID);
                    key2->keys_child.resize(KEYS_MID + 1);
                    tmp->keys.resize(KEYS_MID);
                    if (tmp == root)
                    {
                        re->keys_child.push_back(tmp);
                    }
                    re->keys_child.push_back(key2);
                }
            }
        }
        return re;
    }

    template <typename  T>
    void ListBTree<T>::Print() {
        if (root == nullptr)
        {
            return;
        }
        Node_LBTree<T>* tmp = root;
        Print2(tmp);
    }

    template <typename  T>
    void ListBTree<T>::Print(bzx_message::Message& message)
    {
        if (root == nullptr)
        {
            return;
        }
        Node_LBTree<T>* tmp = root;
        Print2(tmp,message);
    }
    template <typename T>
    void ListBTree<T>::Print2(Node_LBTree<T>* node)
    {
        if (node == nullptr)
        {
            return;
        }
        size_t i = 0;
        while (i < node->keys.Size)
        {
            if (node->keys_child.Size > i)
                Print2(node->keys_child[i]);
            cout << node->keys[i] << "\n";
         
            ++i;
        }
        if (node->keys_child.Size > i)
        {
            Print2(node->keys_child[i]);
        }
    }

    
    template <typename  T>
    void ListBTree<T>::Print2(Node_LBTree<T>* node, bzx_message::Message& message)
    {
        if (node == nullptr)
        {
            return;
        }
        size_t i = 0;
        std::string str;
        while (i < node->keys.Size)
        {
            if (node->keys_child.Size > i)
                Print2(node->keys_child[i],message);
            message.OUTPUT(node->keys[i]);
            ++i;
        }
        if (node->keys_child.Size > i)
        {
            Print2(node->keys_child[i],message);
        }
    }

    template <typename T>
    void ListBTree<T>::Print2(Node_LBTree<T>* node, ofstream& out)
    {
        if (node == nullptr)
        {
            return;
        }
        size_t i = 0;
        while (i < node->keys.Size)
        {
            if (node->keys_child.Size > i)
                Print2(node->keys_child[i], out);
            out << node->keys[i] << "\n";
            ++i;
        }
        if (node->keys_child.Size > i)
        {
            Print2(node->keys_child[i], out);
        }
    }



    template <typename T>
    Find2_return_LB<T> ListBTree<T>::Find2(T row)
    {
        Find2_return_LB<T> FR2(root);
        FR2.index = -1;

        while (FR2.find_node != nullptr)
        {
            size_t i = 0;
            for (; i < FR2.find_node->keys.Size; ++i)
            {
                if (row == FR2.find_node->keys[i])
                {
                    FR2.index = i;
                    return FR2;
                }
                else if (row < FR2.find_node->keys[i])  // row < keys[i]
                {
                    break;
                }
            }
            FR2.father = FR2.find_node;
            if (FR2.find_node->keys_child.Size > 0)
            {
                FR2.find_node = FR2.find_node->keys_child[i];
            }
            else
            {
                FR2.find_node = nullptr;
            }
        }
        return FR2;
    }


    template <typename T>
    bool ListBTree<T>::Balance_Leaf_Nodes(Node_LBTree<T>* father_node)
    {
        if (father_node->keys_child.Size == 0)
        {
            return false;
        }
        // 寻找第一个不满足条件的节点
        size_t index = 0;
        Node_LBTree<T>* Left, * Right;
        for (; index < father_node->keys.Size; ++index)
        {
            if (father_node->keys_child[index]->keys.Size < KEYS_MID)
            {
                break;
            }
        }
        // 检测节点左侧兄弟是否满足替换
        if (index > 0 || index == father_node->keys.Size)
        {
            Right = father_node->keys_child[index];
            if (father_node->keys_child[index - 1]->keys.Size > KEYS_MID)
            {
                Left = father_node->keys_child[index - 1];
                Right->keys.push_front(father_node->keys[index - 1]);

                father_node->keys[index - 1] = Left->keys.tail->element;
                Left->keys.pop_back();

                if (Left->keys_child.Size > 0)
                {
                    Right->keys_child.push_front(Left->keys_child.tail->element);
                    Left->keys_child.pop_back();
                }
                return true;
            }
        }
        // 检测右侧
        
        if (index < father_node->keys.Size)
        {
            Left = father_node->keys_child[index];
            if (father_node->keys_child[index + 1]->keys.Size > KEYS_MID)
            {
                Right = father_node->keys_child[index + 1];
                Left->keys.push_back(father_node->keys[index]);

                father_node->keys[index] = Left->keys[0];
                Right->keys.pop_front();

                if (Right->keys_child.Size > 0)
                {
                    Left->keys_child.push_back(Left->keys_child.tail->element);
                    Right->keys_child.pop_front();
                }
                return true;
            }
        }
        return false;
    }

    template <typename T>
    void ListBTree<T>::Merge_father_2_child_node(Node_LBTree<T>* father_node)
    {
        //
        size_t index = 0;
        for (; index < father_node->keys.Size - 1; ++index)
        {
            if (father_node->keys_child[index]->keys.Size < KEYS_MID)
            {
                break;
            }
        }
        Node_LBTree<T>* A, * B;
        A = father_node->keys_child[index];
        B = father_node->keys_child[index + 1];

        A->keys.push_back(father_node->keys[index]);

        for (size_t i = 0; i < B->keys.Size; ++i)
        {
            A->keys.push_back(B->keys[i]);
        }
        for (size_t i  = 0; i < B->keys_child.Size; ++i)
        {
            A->keys_child.push_back(B->keys_child[i]);
        }
        father_node->keys.remove_index(index);
        father_node->keys_child.remove_index(index + 1);
    }

    template <typename T>
    bool ListBTree<T>::Delete(T el)
    {
        // 根据 statement中的索引 进行删除
        Find2_return_LB<T> FR2 = Find2(el);
        Node_LBTree<T>* father_node = FR2.father;
        Node_LBTree<T>* find_node = FR2.find_node;
        size_t index = FR2.index;
        Node_LBTree<T>* tmp = nullptr;
        if (find_node != nullptr)
        {
            if (find_node->keys_child.Size > 0)   // find 不是叶节点
            {
                // 寻找最接近的后继节点,并交换存储的值
                father_node = find_node;
                find_node = find_node->keys_child[index + 1];
                index = 0;
                while (find_node->keys_child.Size > 0)
                {
                    father_node = find_node; // 父节点下移
                    find_node = find_node->keys_child[0];
                }
                FR2.find_node->keys[FR2.index] = find_node->keys[0];
                find_node->keys.pop_front();
            }
            else
            {
                // 在叶节点中删除该值
                find_node->keys.remove_index(index);
            }

            while (true)
            {
                if (find_node->keys.Size >= KEYS_MID || find_node == root) // 没有发生下溢，即节点数量>=MAX_KEYS_SIZE/2; 或者节点为根节点
                {
                    return true;
                }
                else  if (Balance_Leaf_Nodes(father_node))   //find_node 左节点或右节点可以足够平衡
                {
                    return true;
                }
                else if (father_node == root)    // find_node 父节点是根节点
                {
                    if (father_node->keys.Size == 1) // 父节点只有一个值
                    {
                        // 合并父节点和子节点
                        Merge_father_2_child_node(root);
                    }
                    else
                    {
                        // 合并同级节点
                        Merge_father_2_child_node(father_node);
                    }
                    return true;
                }
                else  //合并find_node和同级节点,node等于父亲节点
                {
                    Merge_father_2_child_node(father_node);
                    FR2 = Find2(father_node->keys[0]);
                    father_node = FR2.father;
                    find_node = FR2.find_node;
                    index = FR2.index;
                }
            }
        }
        return false;
    }
}
#endif //B_PC_LISTBTREE_H
