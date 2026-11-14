#pragma once
#include <string>
#include "LinkedList.h"
using namespace std;
template <typename T>
class Hash
{
private:
    int M;
    List<T> *L; // Mảng cList<Entity *> chứa Entity*

public:
    Hash(int = 10);
    ~Hash();

    int hash_function(const string &s);
    void insert(const string &s, T *X);
    T *find(const string &key, bool (*cmp)(T *, const string &));
    bool erase(const string &key, bool (*cmp)(T *, const string &));
};

template <typename T>
Hash<T>::Hash(int size)
{
    this->M = size;
    this->L = new List<T>[this->M];
}

template <typename T>
Hash<T>::~Hash()
{
    delete[] L;
}

template <typename T>
int Hash<T>::hash_function(const string &s)
{
    long long hash = 0;
    for (unsigned char c : s)
    {
        hash = (hash * 31 + c) % M;
    }
    return hash;
}

template <typename T>
void Hash<T>::insert(const string &s, T *X)
{
    int key = hash_function(s);
    this->L[key].insert(X); ///////////////////////////////////////?????????????????????????????????????????????????????????????????????????
}

template <typename T>
T *Hash<T>::find(const string &key, bool (*cmp)(T *, const string &))
{
    Node<T> *p = this->L[hash_function(key)].find(key, cmp);
    return (p == nullptr) ? nullptr : p->Data;
}

template <typename T>
bool Hash<T>::erase(const string &key, bool (*cmp)(T *, const string &))
{
    List<T> &bucket = this->L[hash_function(key)];
    Node<T> *p = bucket.find(key, cmp);
    if (p == nullptr)
        return false;
    bucket.remove_item(p);
    return true;
}
