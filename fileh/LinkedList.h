#pragma once
#include <iostream>
#include <string>
#include "Entity.h"
using namespace std;
template <typename T>
struct Node
{
    T *Data;
    Node<T> *Next;
    Node<T> *Prev;

    Node(T *x);
};
template <typename T>
class List : public Entity
{
private:
    Node<T> *header, *tail; // đầu cuối
    int size;

public:
    List();
    ~List();

    int insert(T *);
    int insert(T *, Node<T> *);
    int remove_item(Node<T> *);

    // Bổ sung hàm find dùng con trỏ hàm so sánh
    Node<T> *find(const string &, bool (*compare)(T *, const string &));

    int get_size();
    void Show() override;
    Node<T> *get_header();

    void Quicksort(const int, const int, bool(cmp)(T *, T *));
    void sort(bool(cmp)(T *, T *));
    Node<T> *operator[](int);
};

template <typename T>
Node<T>::Node(T *x) : Data(x), Next(nullptr), Prev(nullptr) {}

template <typename T>
List<T>::List() : size(0)
{
    header = tail = new Node<T>(nullptr);
}

template <typename T>
List<T>::~List()
{
    Node<T> *p = header;
    while (p != nullptr)
    {
        Node<T> *tmp = p;
        p = p->Next;
        delete tmp;
    }
}

template <typename T>
int List<T>::insert(T *e)
{
    return this->insert(e, this->tail);
}
template <typename T>
int List<T>::insert(T *e, Node<T> *p)
{
    Node<T> *NewItem = new Node<T>(e);
    if (NewItem == nullptr)
        return 0;

    if (p == nullptr)
        p = this->header;
    NewItem->Next = p->Next;
    NewItem->Prev = p;
    if (p->Next != nullptr)
        p->Next->Prev = NewItem;
    else
        this->tail = NewItem; // p ở node cuối mới gán
    p->Next = NewItem;
    this->size++;
    return 1;
}

template <typename T>
int List<T>::remove_item(Node<T> *p)
{
    if (p == nullptr || p == header)
        return 0;
    if (p->Prev != nullptr)
        p->Prev->Next = p->Next;
    if (p->Next != nullptr)
        p->Next->Prev = p->Prev;
    else if (p == this->tail)
    {
        this->tail = p->Prev; // p là node cuối thì xóa
    }
    delete p;
    this->size--;
    return 1;
}

// Thực thi hàm find
template <typename T>
Node<T> *List<T>::find(const string &s, bool (*compare)(T *, const string &))
{
    Node<T> *p = header->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr && compare(p->Data, s))
        {
            return p; // Tìm thấy
        }
        p = p->Next;
    }
    return nullptr; // Không tìm thấy
}

template <typename T>
int List<T>::get_size()
{
    return this->size;
}

template <typename T>
void List<T>::Show()
{
    Node<T> *p = header->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr)
        {
            p->Data->Show();
        }
        p = p->Next;
    }
}

template <typename T>
Node<T> *List<T>::get_header()
{
    return this->header;
}

template <typename T>
void List<T>::Quicksort(const int l, const int r, bool(cmp)(T *, T *))
{
    if (l >= r)
    {
        return;
    }
    int i = l - 1, j = l;
    int pivot = r;
    while (j < pivot)
    {
        if (cmp((*this)[j]->Data, (*this)[pivot]->Data))
        {
            ++i;
            T *tmp = (*this)[j]->Data;
            (*this)[j]->Data = (*this)[i]->Data;
            (*this)[i]->Data = tmp;
        }
        ++j;
    }
    ++i;

    T *tmp = (*this)[i]->Data;
    (*this)[i]->Data = (*this)[pivot]->Data;
    (*this)[pivot]->Data = tmp;

    pivot = i;

    Quicksort(l, pivot - 1, cmp);
    Quicksort(pivot + 1, r, cmp);
}

template <typename T>
void List<T>::sort(bool(cmp)(T *, T *))
{
    Quicksort(0, this->size - 1, cmp);
}

template <typename T>
Node<T> *List<T>::operator[](int index)
{
    Node<T> *p = this->header;
    p = p->Next;
    while (index-- != 0)
        p = p->Next;
    return p;
}
// template class List<PhongBan *>;
// template class List<ChucVu *>;
// template class List<ViTriViecLam *>;
// template class List<MucLuong *>;
// template class List<Employee *>;

// bool MNVIncrease(Entity *x, Entity *y)
// {
//     Employee *xx = static_cast<Employee *>(x);
//     Employee *yy = static_cast<Employee *>(y);
//     return xx->get_mnv() < yy->get_mnv();
// }
// bool MNVDecrease(Entity *x, Entity *y)
// {
//     Employee *xx = static_cast<Employee *>(x);
//     Employee *yy = static_cast<Employee *>(y);
//     return xx->get_mnv() > yy->get_mnv();
// }

// bool NameIncrease(Entity *x, Entity *y)
// {
//     Employee *xx = static_cast<Employee *>(x);
//     Employee *yy = static_cast<Employee *>(y);
//     return xx->get_name() < yy->get_name();
// }
// bool NameDecrease(Entity *x, Entity *y)
// {
//     Employee *xx = static_cast<Employee *>(x);
//     Employee *yy = static_cast<Employee *>(y);
//     return xx->get_name() > yy->get_name();
// }

// bool DateIncrease(Entity *x, Entity *y)
// {
//     Employee *xx = static_cast<Employee *>(x);
//     Employee *yy = static_cast<Employee *>(y);
//     return xx->get_date() < yy->get_date();
// }
// bool DateDecrease(Entity *x, Entity *y)
// {
//     Employee *xx = static_cast<Employee *>(x);
//     Employee *yy = static_cast<Employee *>(y);
//     return xx->get_date() > yy->get_date();
// }

// bool MNVIncrease(Entity *, Entity *);
// bool MNVDecrease(Entity *, Entity *);

// bool NameIncrease(Entity *, Entity *);
// bool NameDecrease(Entity *, Entity *);

// bool DateIncrease(Entity *, Entity *);
// bool DateDecrease(Entity *, Entity *);