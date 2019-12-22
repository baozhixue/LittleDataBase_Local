#ifndef BTREE_H
#define BTREE_H

const int MAX_KEYS_SIZE = 5;  // btree
const int KEYS_MID = MAX_KEYS_SIZE / 2;  //btree

template <typename T>
struct Node
{
	Node(T el) { keys[0] = el; ++keys_size; }
	Node() {};
	
	T keys[MAX_KEYS_SIZE];
	Node* keys_child[MAX_KEYS_SIZE + 1];
	uint32_t index = 0;
	size_t keys_size = 0;
	size_t keys_child_size = 0; 
};


template <typename T>
struct Btree
{
	bool ADD(T el);
	void Print();
	int Compare(T A, T B,bool mode); // mode == 0, return 0/1. mode == 1, return -1/0/1
	Node<T>* Find(T row);
	Node<T>* root = nullptr;
	size_t index = 0;

private:
	Node<T>* add(T* el, Node<T> *tmp);
	void slide_add_key(T* row, Node<T>* tmp, const size_t index);
	void slide_add_child(Node<T>* src, Node<T>* tmp, const size_t index);
	void Print2(Node<T>* node);
};




template <typename T>
bool Btree<T>::ADD(T el)
{
	if (root == nullptr)
	{
		root = new Node<T>(el);
		return true;
	}
	else
	{
		Node<T>* tmp = root;
		tmp = add(&el, tmp);
		if (tmp != nullptr)
		{
			root = tmp;
		}
		return true;
	}
	return false;
}

template <typename T>
Node<T>* Btree<T>::add(T* row, Node<T>* tmp)
{
	Node<T>* re = nullptr;
	if (tmp->keys_child_size == 0)    // 叶节点
	{
		size_t i = 0;
		while (i < tmp->keys_size)
		{
			if (Compare(*row, tmp->keys[i],false) == 0)
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
			re = new Node<T>(tmp->keys[KEYS_MID]);
			re->index = ++index;
			Node<T>* key2 = new Node<T>(); key2->index = ++index;
			for (size_t i2 = KEYS_MID + 1; i2 < MAX_KEYS_SIZE; ++i2)
			{
				key2->keys[key2->keys_size] = tmp->keys[i2];
				key2->keys_size += 1;
			}

			tmp->keys_size = KEYS_MID;
			if (tmp == root)
			{
				re->keys_child[re->keys_child_size] = tmp;
				re->keys_child_size += 1;
			}
			re->keys_child[re->keys_child_size] = key2;
			re->keys_child_size += 1;
		}
	}
	else
	{
		size_t j = 0;
		while (j < tmp->keys_size)
		{
			if (Compare(*row, tmp->keys[j],false) == 0)
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
				if (Compare(re->keys[0], tmp->keys[k],false) == 0)
				{
					break;
				}
				++k;
			}
			slide_add_key(re->keys + 0, tmp, k);
			slide_add_child(re->keys_child[0], tmp, k + 1);
			re = nullptr;

			if (tmp->keys_size == MAX_KEYS_SIZE)
			{
				re = new Node<T>(tmp->keys[KEYS_MID]);
				re->index = ++index;

				Node<T>* key2 = new Node<T>(); key2->index = ++index;
				for (size_t i2 = KEYS_MID + 1; i2 < MAX_KEYS_SIZE; ++i2)
				{
					key2->keys[key2->keys_size] = tmp->keys[i2];
					key2->keys_size += 1;
					key2->keys_child[key2->keys_child_size] = tmp->keys_child[i2];
					key2->keys_child_size += 1;
				}
				key2->keys_child[key2->keys_child_size] = tmp->keys_child[MAX_KEYS_SIZE];
				key2->keys_child_size += 1;

				tmp->keys_size = KEYS_MID;
				tmp->keys_child_size = KEYS_MID + 1;
				if (tmp == root)
				{
					re->keys_child[0] = tmp;
					re->keys_child_size += 1;
				}
				re->keys_child[re->keys_child_size] = key2;
				re->keys_child_size += 1;
			}
		}
	}
	return re;
}


template <typename T>
int Btree<T>::Compare(T A, T B,bool mode)
{
	// mode == 0, return 0/1. mode == 1, return -1/0/1

	/*
	for (size_t i = 0; i < MaxIdLength; ++i)
	{
		if (A.id[i] > B.id[i])
		{
			return true;
		}
	}
	*/
	if (A > B)
	{
		return 1;
	}
	else if(mode == 1 && A < B)
	{
		return -1;
	}
	
	return 0;
}



template <typename T>
void Btree<T>::slide_add_key(T* row, Node<T>* tmp, const size_t index)
{
	for (size_t i2 = tmp->keys_size; i2 > index; --i2)
	{
		tmp->keys[i2] = tmp->keys[i2 - 1];
	}
	tmp->keys[index] = *row;
	tmp->keys_size += 1;
}

template <typename T>
void Btree<T>::slide_add_child(Node<T>* src, Node<T>* tmp, const size_t index)
{
	for (size_t i2 = tmp->keys_child_size; i2 > index; --i2)
	{
		tmp->keys_child[i2] = tmp->keys_child[i2 - 1];
	}
	tmp->keys_child[index] = src;
	tmp->keys_child_size += 1;
}

template <typename T>
void Btree<T>::Print()
{
	if (root != nullptr)
	{
		Node<T>* tmp = root;
		Print2(tmp);
		cout << "\n";
	}
}


template <typename T>
void Btree<T>::Print2(Node<T>* node)
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
		//printf("%s %s %s\n", node->keys[i].id, node->keys[i].UserName, node->keys[i].Email);
		cout << node->keys[i] <<" ";
		++i;
	}
	if (node->keys_child_size > i)
	{
		Print2(node->keys_child[i]);
	}
}

template <typename T>
Node<T>* Btree<T>::Find(T row)
{
	Node<T>* tmp = root;

	while (tmp != nullptr)
	{
		int i = 0;
		for (; i < tmp->keys_size; ++i)
		{
			if (Compare(row, tmp->keys[i]) == 0)
			{
				return tmp;
			}
			else if (Compare(row, tmp->keys[i]) == -1,true)  // row < keys[i]
			{
				break;
			}
		}

		if (tmp->keys_child_size > 0)
		{
			tmp = tmp->keys_child[i];
		}
		else
		{
			tmp = nullptr;
		}
	}
	return tmp;
}


#endif // !BTREE_H

#pragma once
