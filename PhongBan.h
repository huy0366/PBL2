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
    int nam, nu;

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
    int get_nam() const;
    int get_nu() const;

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
    nam = nu = 0;
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
int PhongBan::get_nam() const
{
    return this->nam;
}
int PhongBan::get_nu() const
{
    return this->nu;
}
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
