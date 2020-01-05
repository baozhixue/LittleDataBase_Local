#ifndef LIST_H
#define LIST_H

namespace baozhixue
{
    template <typename T>
    struct Node_List
    {
        Node_List* next = nullptr;
        T element;
        Node_List(T el) {
            element = el;
        }
        Node_List(T el, Node_List<T>* _next)
        {
            element = el;
            next = _next;
        }
        void operator=(const Node_List<T>& node)
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
        void pop_back();
        void pop_front();
        void push_back(T el);
        void push_front(T el);
        void insert(T el, size_t index);
        void remove_index(size_t index);
        T& operator[](size_t i);  // return element
        Node_List<T>& operator()(size_t i); // return Node_list
        void resize(size_t new_Size);
        List<T>* cut(size_t index);


        size_t Size = 0;
        Node_List<T>* head = nullptr;
        Node_List<T>* tail = nullptr;
        Node_List<T>* mem_optimize = nullptr;
        size_t index_optimize = 0;    //记录上次访问时的下表
    };


    template <typename T>
    void List<T>::Print()
    {
        Node_List<T>* tmp = head;
        while (tmp != tail->next)
        {
            cout << tmp->element << " ";
            tmp = tmp->next;
        }
    }

    template <typename T>
    void List<T>::push_back(T el)
    {
        if (head == nullptr)
        {
            head = tail = new Node_List<T>(el);
        }
        else
        {
            tail->next = new Node_List<T>(el);
            tail = tail->next;
        }
        ++Size;
        return;
    }

    template <typename T>
    void List<T>::push_front(T el)
    {
        if (head == nullptr)
        {
            head = tail = new Node_List<T>(el);
        }
        else {
            head = new Node_List<T>(el, head);
        }
        ++Size;
    }

    template <typename T>
    void List<T>::insert(T el, size_t index)
    {
        if (index == 0)
        {
            push_front(el);
        }
        else if (index >= Size)
        {
            push_back(el);
        }
        else {
            Node_List<T>* tmp = head;
            while (index > 1)
            {
                tmp = tmp->next;
                --index;
            }
            tmp->next = new Node_List<T>(el, tmp->next);
            ++Size;
        }
        
    }

    template <typename T>
    Node_List<T>& List<T>::operator()(size_t i)
    {
        Node_List<T>* t;
        if (i < Size)
        {
            if (i == 0)
            {
                t = head;
            }
            else if (i == Size - 1)
            {
                t = tail;
            }
            else {
                Node_List<T>* tmp = head;
                while (i > 0)
                {
                    tmp = tmp->next;
                    i -= 1;
                }
                t = tmp;
            }
        }
        return *t;
    }

    template <typename T>
    void List<T>::remove_index(size_t index)
    {
        if (index > Size)
        {
            return;
        }
        Node_List<T>* dst = nullptr;
        if (index == 0)
        {
            dst = head;
            if (head == tail)
            {
                head = tail = nullptr;
            }
            else
            {
                head = head->next;
            }
        }
        else {
            
            while (index > 1)
            {
                mem_optimize = mem_optimize->next;
                --index;
            }
            dst = mem_optimize->next;
            if (mem_optimize->next == tail)
            {
                tail = mem_optimize;
            }
            else
            {
                mem_optimize->next = mem_optimize->next->next;
            }
        }
        --Size;
        delete dst;
    }

    template <typename T>
    T& List<T>::operator[](size_t i)
    {
        T t;
        if (i < Size)
        {
            if (i == 0)
            {
                return head->element;
            }
            else if (i == Size - 1)
            {
                return tail->element;
            }
            else {
                Node_List<T>* tmp = head;
                while (i > 0)
                {
                    tmp = tmp->next;
                    i -= 1;
                }
                return tmp->element;
            }
        }
        return t;
    }

    /*
        将List剪切至newSize
    */
    template <typename T>
    void List<T>::resize(size_t new_Size)
    {
        if (new_Size >= Size)
        {
            return;
        }
        //free(cut(new_Size));
        List<T>* dst = cut(new_Size);
        while (dst->Size>0)
        {
            dst->pop_front();
        }
    }

    /*
        将链表索引为index的节点返回，作为另一个链表。
        原链表resize（index）
    */
    template <typename T>
    List<T>* List<T>::cut(size_t index)
    {
        if (index < Size)
        {
            Node_List<T> *pre = nullptr;
            List<T>* tmp = new List();
            mem_optimize = head;
            tmp->Size = Size - index;
            this->Size = index;

            while (index > 0)
            {
                pre = mem_optimize;
                mem_optimize = mem_optimize->next;
                --index;
            }
            
            tmp->head = mem_optimize;
            tmp->tail = tail;
            
            if (pre == nullptr)
            {
                head = tail = pre;
            }
            else
            {
                pre->next = nullptr;
                tail = pre;
            }
            
            return tmp;
        }
        return nullptr;
    }

    template <typename T>
    void List<T>::pop_back()
    {
        if (head == nullptr)
        {
            return;
        }
        else if (head != nullptr && head == tail)
        {
            mem_optimize = head;
            head = tail = nullptr;
        }
        else
        {
            mem_optimize = head;
            while (mem_optimize->next != tail)
            {
                mem_optimize = mem_optimize->next;
            }
            tail = mem_optimize;
            mem_optimize = tail->next;
            tail->next = nullptr;
        }
        delete mem_optimize;
        --Size;
    }

    template <typename T>
    void List<T>::pop_front()
    {
        mem_optimize = head;
        if (head == nullptr)
        {
            return;
        }
        else if (head == tail)
        {
            //free(head);
            head = tail = nullptr;
        }
        else
        {
            mem_optimize = head;
            head = head->next;
            //free(mem_optimize);
        }
        free(mem_optimize);
        --Size;
    }
}

#endif // !LIST_H
#pragma once
