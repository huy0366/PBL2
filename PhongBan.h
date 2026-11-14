#pragma once
#include "LinkedList.h"
#include "Date.h"
using namespace std;
class Employee;
class PhongBan
{
private:
    string MaPhongBan, TenPhongBan;
    Date NgayThanhLap;
    List<Employee> *L;

public:
    PhongBan();
    PhongBan(const string &, const string &, const Date &);
    ~PhongBan();
    void set_MaPhongBan(const string &);
    string get_MaPhongBan() const;
    void set_TenPhongBan(const string &);
    string get_TenPhongBan() const;
    void set_NgayThanhLap(const Date &);
    Date get_NgayThanhLap() const;

    List<Employee> *get_ListNV();
    bool ThemNV(Employee *);
    bool XoaNV(Employee *);
    // bool SuaNV(Employee *);
    int get_sizeNV();
    // Hàm so sánh kiểm tra Tên Chi Nhánh
    // static bool checkPhongBanName(Entity *entity, const string &name);
};

PhongBan::PhongBan() : L(new List<Employee>()) {}
PhongBan::PhongBan(const string &ma, const string &name, const Date &d)
{
    this->MaPhongBan = ma;
    this->TenPhongBan = name;
    this->NgayThanhLap = d;
    this->L = new List<Employee>();
}
PhongBan::~PhongBan()
{
    delete L;
}

void PhongBan::set_MaPhongBan(const string &ma)
{
    this->MaPhongBan = ma;
}
string PhongBan::get_MaPhongBan() const
{
    return this->MaPhongBan;
}

void PhongBan::set_TenPhongBan(const string &name)
{
    this->TenPhongBan = name;
}
string PhongBan::get_TenPhongBan() const
{
    return this->TenPhongBan;
}
void PhongBan::set_NgayThanhLap(const Date &d)
{
    this->NgayThanhLap = d;
}
Date PhongBan::get_NgayThanhLap() const
{
    return this->NgayThanhLap;
}
List<Employee> *PhongBan::get_ListNV()
{
    return this->L;
}
bool PhongBan::ThemNV(Employee *e)
{
    if (e == nullptr)
        return 0;
    this->L->insert(e);
    return 1;
}
bool PhongBan::XoaNV(Employee *e)
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
// bool PhongBan::SuaNV(Employee *e)
// {
//     Node<Employee> *p = this->L->get_header()->Next;
//     while (p != nullptr)
//     {
//         if (p->Data->get_mnv() == e->get_mnv())
//         {
//             p->Data = e;
//             return 1;
//         }
//         p = p->Next;
//     }
//     return 0;
// }
int PhongBan::get_sizeNV()
{
    return this->L->get_size();
}
// Hàm so sánh kiểm tra Tên Chi Nhánh
// bool PhongBan::checkPhongBanName(Entity *entity, const string &name)
// {
//     PhongBan *b = dynamic_cast<PhongBan *>(entity);
//     return (b != nullptr) && (b->get_MaPhongBan() == name);
// }
