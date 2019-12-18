#ifndef LIST_H
#define LIST_H


template <typename T>
struct Node_List
{
	Node_List* next = nullptr;
	T element;
	Node_List(T el) {
		element = el;
	}
	Node_List(T el,Node_List<T> *_next)
    {
	    element = el;
	    next = _next;
    }
    void operator=(const Node_List<T> &node)
    {
	    element = node.element;
        next = node.next;
    }
};

template <typename T>
struct List
{
	List() {}	// init List
	void Print();
	void push_back(T el);
	void push_front(T el);
	void insert(T el,size_t index);
	void remove_index(size_t index);
    T&  operator[](size_t i);  // return element
    Node_List<T>& operator()(size_t i); // return Node_list

private:
    size_t Length = 0;
	Node_List<T>* head = nullptr;
	Node_List<T>* tail = nullptr;

	Node_List<T>* mem_optimize = nullptr;
};


template <typename T>
void List<T>::Print()
{
	Node_List<T>* tmp = head;
	while (tmp != nullptr)
	{
		cout << tmp->element << " ";
		tmp = tmp->next;
	}
	cout << "\n";
}

template <typename T>
void List<T>::push_back(T el)
{
    if(head == nullptr)
    {
        head = tail = new Node_List<T>(el);
    } else
    {
        tail->next = new Node_List<T>(el);
        tail = tail->next;
    }
    ++Length;
    return;
}

template <typename T>
void List<T>::push_front(T el)
{
    if(head == nullptr)
    {
        head = tail = new Node_List<T>(el);
    } else{
        head = new Node_List<T>(el,head);
    }
    ++Length;
}

template <typename T>
void List<T>::insert(T el,size_t index)
{
    if(index == 0)
    {
        push_front(el);
    }
    else if(index >= Length)
    {
        push_back(el);
    } else{
        Node_List<T> *tmp = head;
        while (index>0)
        {
            tmp = tmp->next;
            --index;
        }
        tmp->next = new Node_List<T>(el,tmp->next);
    }
}

template <typename T>
Node_List<T>& List<T>::operator()(size_t i)
{
    Node_List<T> *t;
    if(i < Length)
    {
        if(i == 0)
        {
            t = head;
        }
        else if(i == Length-1)
        {
            t = tail;
        } else{
            Node_List<T> *tmp = head;
            while (i>0)
            {
                tmp=tmp->next;
                i-=1;
            }
            t = tmp;
        }
    }
    return *t;
}
template <typename T>
void List<T>::remove_index(size_t index)
{
    if(index > Length)
    {
        return;
    }
    if(index == 0)
    {
        mem_optimize = head;
        head = head->next;
    } else{
        Node_List<T> *tmp = head;
        while (index > 1)
        {
            tmp = tmp->next;
            --index;
        }
        mem_optimize = tmp->next;
        tmp->next = tmp->next->next;
    }
    --Length;
    if(mem_optimize != nullptr)
    {
        free(mem_optimize);
        mem_optimize = nullptr;
    }
}

template <typename T>
T& List<T>::operator[](size_t i)
{
    T t;
    if(i < Length)
    {
        if(i == 0)
        {
            return head->element;
        }
        else if(i == Length-1)
        {
            return tail->element;
        } else{
            Node_List<T> *tmp = head;
            while (i>0)
            {
                tmp=tmp->next;
                i-=1;
            }
            return tmp->element;
        }
    }
    return t;
}


#endif // !LIST_H
#pragma once
