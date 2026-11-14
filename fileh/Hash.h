#pragma once
#include <string>
#include "LinkedList.h"
using namespace std;
class Hash : public Entity
{
private:
    int M;
    List<Entity> *L; // Mảng cList<Entity *> chứa Entity*

public:
    Hash(int = 10);
    ~Hash();

    int hash_function(const string &s);
    void insert(const string &s, Entity *X);
    Entity *find(const string &key, bool (*cmp)(Entity *, const string &));
    bool erase(const string &key, bool (*cmp)(Entity *, const string &));
    List<Entity> *get_list();
    void Show() override;
};

Hash::Hash(int size)
{
    this->M = size;
    this->L = new List<Entity>[this->M];
}

Hash::~Hash()
{
    delete[] L;
}

int Hash::hash_function(const string &s)
{
    long long hash = 0;
    for (unsigned char c : s)
    {
        hash = (hash * 31 + c) % M;
    }
    return hash;
}

void Hash::insert(const string &s, Entity *X)
{
    int key = hash_function(s);
    this->L[key].insert(X); ///////////////////////////////////////?????????????????????????????????????????????????????????????????????????
}

Entity *Hash::find(const string &key, bool (*cmp)(Entity *, const string &))
{
    Node<Entity> *p = this->L[hash_function(key)].find(key, cmp);
    return (p == nullptr) ? nullptr : p->Data;
}

bool Hash::erase(const string &key, bool (*cmp)(Entity *, const string &))
{
    List<Entity> &bucket = this->L[hash_function(key)];
    Node<Entity> *p = bucket.find(key, cmp);
    if (p == nullptr)
        return false;
    bucket.remove_item(p);
    return true;
}

List<Entity> *Hash::get_list()
{
    return this->L;
}

void Hash::Show()
{
    for (int i = 0; i < this->M; i++)
    {
        this->L[i].Show();
    }
}