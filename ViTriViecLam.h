#pragma once
#include <iostream>
#include <iomanip>
#include "Entity.h"
using namespace std;
class ViTriViecLam : public Entity
{
private:
    string MaVTVL, TenVTVL;
    List<Employee> *L;

public:
    ViTriViecLam(const string &, const string &);
    ~ViTriViecLam();
    void set_MaVTVL(const string &);
    string get_MaVTVL();
    void set_TenVTVL(const string &);
    string get_TenVTVL();
    List<Employee> *get_ListNV();
    bool ThemNV(Employee *);
    bool XoaNV(Employee *);
    void Show() override;
};

ViTriViecLam::ViTriViecLam(const string &ma, const string &ten) : MaVTVL(ma), TenVTVL(ten)
{
    this->L = new List<Employee>();
}

ViTriViecLam::~ViTriViecLam()
{
}
void ViTriViecLam::set_MaVTVL(const string &ma)
{
    this->MaVTVL = ma;
}
string ViTriViecLam::get_MaVTVL()
{
    return this->MaVTVL;
}
void ViTriViecLam::set_TenVTVL(const string &ten)
{
    this->TenVTVL = ten;
}
string ViTriViecLam::get_TenVTVL()
{
    return this->TenVTVL;
}
List<Employee> *ViTriViecLam::get_ListNV()
{
    return this->L;
}
bool ViTriViecLam::ThemNV(Employee *e)
{
    if (e == nullptr)
        return 0;
    this->L->insert(e);
    return 1;
}
bool ViTriViecLam::XoaNV(Employee *e)
{
    Node<Employee> *p = this->L->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data == e)
        {
            this->L->remove_item(p);
            return 1;
        }
        p = p->Next;
    }
    return 0;
}
void ViTriViecLam::Show()
{
    cout << left
         << setw(15) << this->MaVTVL
         << setw(30) << this->TenVTVL
         << endl;
}