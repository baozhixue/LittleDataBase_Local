//
// Created by 123 on 2019/12/13.
//

#ifndef B_PC_LISTBTREE_H
#define B_PC_LISTBTREE_H


#include "List.h"

template <typename T>
struct Node_LBTree
{
    Node_LBTree(T el){keys.push_back(el); ++keys_size;}
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
    Node_LBTree<T> *father = nullptr,*find_node = nullptr;
    int index = -1;
    Find2_return_LB(Node_LBTree<T> * node){father = find_node = node;}
};

template <typename  T>
struct ListBTree
{
public:

    ListBTree<T>(int max_keys_size)
    {
        this->MAX_KEYS_SIZE = max_keys_size;
        this->KEYS_MID = max_keys_size/2;
    }
    ListBTree<T>() = default;

    void Print();
    int Compare(T A, T B); // return -1/0/1
    bool ADD(T el);
    bool Delete(T el);
    Node_LBTree<T>* Find(T row);
    Find2_return_LB<T> Find2(T row);    // return find_node, father_node
    void set_root(Node_LBTree<T>* r){root = r;}
    Node_LBTree<T>* get_root(){ return root;}

protected:
    Node_LBTree<T>* add(T el, Node_LBTree<T> *tmp);
    void slide_add_key(T row,Node_LBTree<T>* tmp, const size_t index);
    void slide_add_child(Node_LBTree<T>* src, Node_LBTree<T>* tmp, const size_t index);
    virtual void Print2(Node_LBTree<T>* node);
    bool Balance_Leaf_Nodes(Node_LBTree<T> * father_node);
    void Merge_father_2_child_node(Node_LBTree<T>* father_node);
    Node_LBTree<T>* root = nullptr;
    size_t index = 0;
    void operator +=(T el);  //equals B.ADD(el)

    size_t MAX_KEYS_SIZE = 5;
    size_t KEYS_MID = 2;
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
Node_LBTree<T>* ListBTree<T>::add(T row, Node_LBTree<T> *tmp)
{
    Node_LBTree<T>* re = nullptr;
    if (tmp->keys_child_size == 0)    // 叶节点
    {
        size_t i = 0;
        while (i < tmp->keys_size)
        {
            if(Compare(row,tmp->keys[i]) == -1)
            {
                break;
            }
            ++i;
        }

        // 将新元素加入相应位置
        slide_add_key(row, tmp, i);
        // 检测是否达到最大容量，即是否需要拆分
        if (tmp->keys_size == MAX_KEYS_SIZE)
        {
            re = new Node_LBTree<T>(tmp->keys[KEYS_MID]);
            re->index = ++index;

            Node_LBTree<T>* key2 = new Node_LBTree<T>(); key2->index = ++index;
            for (size_t i2 = KEYS_MID + 1; i2 < MAX_KEYS_SIZE; ++i2)
            {
                key2->keys.push_back(tmp->keys[i2]);
                key2->keys_size += 1;
            }

            tmp->keys_size = KEYS_MID;
            if (tmp == root)
            {
                re->keys_child.push_back(tmp);
                re->keys_child_size += 1;
            }
            re->keys_child.push_back(key2);
            re->keys_child_size += 1;
        }
    }
    else
    {
        size_t j = 0;
        while (j < tmp->keys_size)
        {
            if(Compare(row,tmp->keys[j]) == -1)
            {
                break;
            }
            ++j;
        }
        re = add(row, tmp->keys_child[j]);

        if (re != nullptr)
        {
            size_t k = 0;
            while (k < tmp->keys_size)
            {
                if (Compare(re->keys[0], tmp->keys[k]) == -1)
                {
                    break;
                }
                ++k;
            }
            slide_add_key(re->keys[0], tmp, k);
            slide_add_child(re->keys_child[0], tmp, k + 1);
            re = nullptr;

            if (tmp->keys_size == MAX_KEYS_SIZE)
            {
                re = new Node_LBTree<T>(tmp->keys[KEYS_MID]);
                re->index = ++index;

                Node_LBTree<T>* key2 = new Node_LBTree<T>(); key2->index = ++index;
                for (size_t i2 = KEYS_MID + 1; i2 < MAX_KEYS_SIZE; ++i2)
                {
                    key2->keys.push_back(tmp->keys[i2]);
                    key2->keys_size += 1;
                    key2->keys_child.push_back(tmp->keys_child[i2]);
                    key2->keys_child_size += 1;
                }
                key2->keys_child.push_back(tmp->keys_child[MAX_KEYS_SIZE]);
                key2->keys_child_size += 1;

                tmp->keys_size = KEYS_MID;
                tmp->keys_child_size = KEYS_MID + 1;
                if (tmp == root)
                {
                    re->keys_child.push_back(tmp);
                    re->keys_child_size += 1;
                }
                re->keys_child.push_back(key2);
                re->keys_child_size += 1;
            }
        }
    }
    return re;
}

template <typename T>
void ListBTree<T>::slide_add_key(T row,Node_LBTree<T>* tmp, const size_t index)
{
    tmp->keys.insert(row,index);
    tmp->keys_size += 1;
}
template <typename T>
void ListBTree<T>::slide_add_child(Node_LBTree<T> *src, Node_LBTree<T>* tmp, const size_t index)
{
    tmp->keys_child.insert(src,index);
    tmp->keys_child_size += 1;
}


template <typename  T>
void ListBTree<T>::Print() {
    if (root == nullptr)
    {
        return;
    }
    Node_LBTree<T> *tmp = root;
    Print2(tmp);
    cout<<"\n";
}
template <typename  T>
int ListBTree<T>::Compare(T A, T B) {
    if(A > B)
    {
        return 1;
    }
    else if(A < B)
    {
        return -1;
    }
    return 0;
}

template <typename T>
void ListBTree<T>::Print2(Node_LBTree<T>* node)
{
    if (node == nullptr)
    {
        return;
    }
    size_t i = 0;
    while (i < node->keys_size)
    {
        if (node->keys_child_size > i)
            Print2(node->keys_child[i]);
        cout << node->keys[i] << "\n";
        ++i;
    }
    if (node->keys_child_size > i)
    {
        Print2(node->keys_child[i]);
    }
}

template <typename T>
Find2_return_LB<T> ListBTree<T>::Find2(T row)
{
    Find2_return_LB<T> FR2(root);
    FR2.index = -1;

    while (FR2.find_node != nullptr)
    {
        int i = 0;
        for (; i < FR2.find_node->keys_size; ++i)
        {
            if (Compare(row, FR2.find_node->keys[i]) == 0)
            {
                FR2.index = i;
                return FR2;
            }
            else if (Compare(row, FR2.find_node->keys[i]) == -1)  // row < keys[i]
            {
                break;
            }
        }
        FR2.father = FR2.find_node;
        if (FR2.find_node->keys_child_size > 0)
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
bool ListBTree<T>::Balance_Leaf_Nodes(Node_LBTree<T> * father_node)
{
    if (father_node->keys_child_size == 0)
    {
        return false;
    }
    size_t index = 0;
    Node_LBTree<T>* Left, * Right;
    for (; index < father_node->keys_size; ++index)
    {
        if (father_node->keys_child[index]->keys_size < KEYS_MID)
        {
            break;
        }
    }

    // 检测左侧
    if (index > 0)
    {
        Right = father_node->keys_child[index];
        if (father_node->keys_child[index - 1]->keys_size > KEYS_MID)
        {
            Left = father_node->keys_child[index - 1];

            for (int i = Right->keys_size; i > 0; --i)
            {
                Right->keys[i] = Right->keys[i - 1];
            }
            Right->keys_size += 1;
            Right->keys[0] = father_node->keys[index - 1];
            Left->keys_size -= 1;
            father_node->keys[index - 1] = Left->keys[Left->keys_size];

            if (Left->keys_child_size > 0)
            {
                Left->keys_child_size -= 1;
                for (int j = Right->keys_child_size; j > 0; ++j)
                {
                    Right->keys_child[j] = Right->keys_child[j - 1];
                }
                Right->keys_child[0] = Left->keys_child[Left->keys_child_size];
                Right->keys_child_size += 1;
            }
            return true;
        }
    }
    // 检测右侧
    if (index < father_node->keys_size)
    {
        Left = father_node->keys_child[index];
        if (father_node->keys_child[index + 1]->keys_size > KEYS_MID)
        {
            Right = father_node->keys_child[index+1];
            Right->keys[Right->keys_size] = father_node->keys[index];
            Right->keys_size += 1;
            father_node->keys[index] = Left->keys[0];
            Left->keys_size -= 1;

            if (Left->keys_child_size > 0)
            {
                Right->keys_child[Right->keys_child_size] = Left->keys_child[0];
                Right->keys_child_size += 1;
                Left->keys_child_size -= 1;
                for (int i = 0; i < Left->keys_child_size; ++i)
                {
                    Left->keys_child[i] = Left->keys_child[i + 1];
                }
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
    int index = 0;
    for (; index < father_node->keys_size - 1; ++index)
    {
        if (father_node->keys_child[index]->keys_size < KEYS_MID)
        {
            break;
        }
    }
    Node_LBTree<T>* A, * B;
    A = father_node->keys_child[index];
    B = father_node->keys_child[index + 1];

    A->keys[A->keys_size] = father_node->keys[index];

    A->keys_size += 1;
    for (int i = 0; i < B->keys_size; ++i)
    {
        A->keys[A->keys_size] = B->keys[i];
        A->keys_size += 1;
    }
    if (B->keys_child_size > 0)
    {
        for (int i = 0; i < B->keys_child_size; ++i)
        {
            A->keys_child[A->keys_child_size] = B->keys_child[i];
            A->keys_child_size += 1;
        }
    }
    for (; index < father_node->keys_size-1; ++index)
    {
        father_node->keys[index] = father_node->keys[index + 1];
        father_node->keys_child[index + 1] = father_node->keys_child[index + 2];
    }

    father_node->keys_size -= 1;
    father_node->keys_child_size -= 1;
}

template <typename T>
bool ListBTree<T>::Delete(T el)
{
    Find2_return_LB<T> FR2 = Find2(el);
    Node_LBTree<T> *father_node = FR2.father;
    Node_LBTree<T> *find_node = FR2.find_node;
    size_t index = FR2.index;
    Node_LBTree<T> *tmp = nullptr;
    if(find_node != nullptr)
    {
        if(find_node->keys_child_size > 0)   // find 不是叶节点
        {
            // 寻找最接近的后继节点,并交换存储的值
            while (find_node->keys_child_size > 0)
            {
                father_node = find_node; // 父节点下移
                tmp = find_node->keys_child[index + 1];
                find_node->keys[index] = tmp->keys[0];
                find_node = tmp;
                index = 0;
            }
        }

        // 在叶节点中删除该值
        find_node->keys_size -=1;
        find_node->keys.remove_index(index);

        while (true)
        {
            if(find_node->keys_size >= KEYS_MID || find_node == root) // 没有发生下溢，即节点数量>=MAX_KEYS_SIZE/2; 或者节点为根节点
            {
                return true;
            }
            else  if(Balance_Leaf_Nodes(father_node))   //find_node 左节点或右节点可以足够平衡
            {
                return true;
            }
            else if(father_node == root)    // find_node 父节点是根节点
            {
                if(father_node->keys_size == 1) // 父节点只有一个值
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

#endif //B_PC_LISTBTREE_H
