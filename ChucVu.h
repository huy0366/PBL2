#pragma once
#include "Entity.h"
#include "LinkedList.h"
#include <iostream>
#include <iomanip>
using namespace std;
class Employee;
class ChucVu : public Entity
{
private:
    string MaChucVu, Name;
    long long PhuCap;
    List<Employee> *L;

public:
    ChucVu();
    ChucVu(string, string, long long);
    void set_MaChucVu(const string &);
    string get_MaChucVu() const;

    void set_TenChucVu(const string &);
    string get_TenChucVu() const;

    void set_PhuCap(const long long &);
    long long get_PhuCap() const;
    void set_Luong_Base(long long phucap);
    List<Employee> *get_ListNV();
    bool ThemNV(Employee *);
    bool XoaNV(Employee *);
    void Show();
    ~ChucVu();
};

ChucVu::ChucVu() {}

ChucVu::ChucVu(string ma, string ten, long long phucap) : MaChucVu(ma), Name(ten), PhuCap(phucap)
{
    this->L = new List<Employee>();
}

void ChucVu::set_MaChucVu(const string &ma)
{
    this->MaChucVu = ma;
}
string ChucVu::get_MaChucVu() const
{
    return this->MaChucVu;
}

void ChucVu::set_TenChucVu(const string &name)
{
    this->Name = name;
}
string ChucVu::get_TenChucVu() const
{
    return this->Name;
}
void ChucVu::set_PhuCap(const long long &PC)
{
    this->PhuCap = PC;
}
long long ChucVu::get_PhuCap() const
{
    return this->PhuCap;
}
void ChucVu::set_Luong_Base(long long phucap)
{
    this->PhuCap = phucap;
}
List<Employee> *ChucVu::get_ListNV()
{
    return this->L;
}
bool ChucVu::ThemNV(Employee *e)
{
    if (e == nullptr)
        return 0;
    this->L->insert(e);
    return 1;
}
bool ChucVu::XoaNV(Employee *e)
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
void ChucVu::Show()
{
    cout << setw(10) << this->MaChucVu
         << setw(10) << this->Name
         << setw(10) << fixed << setprecision(0) << this->PhuCap << endl;
}
ChucVu::~ChucVu() {}