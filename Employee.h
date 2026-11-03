#pragma once
#include <string>
#include <iomanip>
#include <iostream>
#include "LinkedList.h"
#include "ChucVu.h"
#include "PhongBan.h"
#include "ViTriViecLam.h"
#include "Entity.h"
#include "Date.h"

using namespace std;
class DataBase;
class Employee : public Entity
{
protected:
    string mnv, name, sex;
    Date NgaySinh;
    ChucVu *CV;
    ViTriViecLam *VTVL;
    PhongBan *PB;
    Date NgayLamViec;
    int Bac_VTVL;
    double ThuongPhat;

public:
    Employee(const string &, const string &, const Date &, const string &,
             ChucVu * = nullptr, ViTriViecLam * = nullptr, PhongBan * = nullptr,
             const Date & = Date(), const int & = 1, const double & = 0);
    Employee();
    virtual ~Employee();
    void set_mnv(const string &);
    string get_mnv();
    void set_name(const string &);
    string get_name();
    void set_NgaySinh(const Date &);
    Date get_NgaySinh();
    void set_sex(const string &);
    string get_sex();

    void set_ChucVu(ChucVu *);
    ChucVu *get_ChucVu() const;
    string get_MaChucVu() const;

    void set_ViTri(ViTriViecLam *);
    ViTriViecLam *get_ViTri() const;
    string get_MaViTri() const;

    void set_PhongBan(PhongBan *);
    PhongBan *get_PhongBan() const;
    string get_MaPhongBan() const;

    void set_NgayLamViec(const Date &);
    Date get_NgayLamViec() const;

    void set_BacViTri(const int &);
    int get_BacViTri() const;

    void set_ThuongPhat(const int &);
    int get_ThuongPhat() const;

    virtual string get_LoaiNV() = 0;
    virtual double get_ThucLinh(const double &) = 0;
    static bool checkEmpID(Entity *entity, const string &id);
    void Show() override;
};

Employee::Employee(const string &mnv, const string &ten, const Date &ngay_sinh, const string &gioitinh,
                   ChucVu *cv, ViTriViecLam *vt, PhongBan *pb, const Date &ngay_lam, const int &bac, const double &thuong)
    : mnv(mnv), name(ten), NgaySinh(ngay_sinh), sex(gioitinh), CV(cv), VTVL(vt),
      PB(pb), NgayLamViec(ngay_lam), Bac_VTVL(bac), ThuongPhat(thuong)
{
}
Employee::~Employee() {}
void Employee::set_name(const string &name)
{
    this->name = name;
}
string Employee::get_name()
{
    return name;
}

void Employee::set_NgaySinh(const Date &date)
{
    this->NgaySinh = date;
}
Date Employee::get_NgaySinh()
{
    return NgaySinh;
}

void Employee::set_sex(const string &sex)
{
    this->sex = sex;
}
string Employee::get_sex()
{
    return sex;
}
void Employee::set_mnv(const string &mnv)
{
    this->mnv = mnv;
}

string Employee::get_mnv()
{
    return mnv;
}
void Employee::set_ChucVu(ChucVu *cv)
{
    this->CV = cv;
}
ChucVu *Employee::get_ChucVu() const
{
    return this->CV;
}
string Employee::get_MaChucVu() const
{
    return this->CV->get_MaChucVu();
}

void Employee::set_ViTri(ViTriViecLam *vt)
{
    this->VTVL = vt;
}
ViTriViecLam *Employee::get_ViTri() const
{
    return this->VTVL;
}
string Employee::get_MaViTri() const

{
    return this->VTVL->get_MaVTVL();
}

void Employee::set_PhongBan(PhongBan *pb)
{
    this->PB = pb;
}
PhongBan *Employee::get_PhongBan() const
{
    return this->PB;
}
string Employee::get_MaPhongBan() const
{
    return this->PB->get_MaPhongBan();
}

void Employee::set_NgayLamViec(const Date &date)
{
    this->NgayLamViec = date;
}
Date Employee::get_NgayLamViec() const
{
    return this->NgayLamViec;
}

void Employee::set_BacViTri(const int &bac)
{
    this->Bac_VTVL = bac;
}
int Employee::get_BacViTri() const
{
    return this->Bac_VTVL;
}
void Employee::set_ThuongPhat(const int &thuong)
{
    this->ThuongPhat = thuong;
}
int Employee::get_ThuongPhat() const
{
    return this->ThuongPhat;
}
bool Employee::checkEmpID(Entity *entity, const string &id)

{
    Employee *emp = dynamic_cast<Employee *>(entity);
    return (emp != nullptr) && (emp->get_mnv() == id);
}

void Employee::Show()
{
    cout << left
         << setw(10) << get_mnv()
         << setw(20) << get_name()
         << setw(10) << get_NgaySinh()
         << setw(10) << get_sex()
         << setw(10) << (get_ChucVu() ? get_ChucVu()->get_MaChucVu() : "N/A")
         << setw(10) << (get_ViTri() ? get_ViTri()->get_TenVTVL() : "N/A")
         << setw(15) << (get_PhongBan() ? get_PhongBan()->get_TenPhongBan() : "N/A")
         << setw(10) << get_NgayLamViec()
         << setw(10) << get_BacViTri();
}

bool FindID(Entity *e, const string &s)
{
    Employee *emp = static_cast<Employee *>(e);
    if (e == NULL)
    {
        return false;
    }
    return emp->get_mnv() == s;
}

bool FindName(Entity *e, const string &s)
{
    Employee *emp = static_cast<Employee *>(e);
    if (e == NULL)
    {
        return false;
    }
    return emp->get_name() == s;
}

bool FindDate(Entity *e, const Date &s)
{
    Employee *emp = static_cast<Employee *>(e);
    if (e == NULL)
    {
        return false;
    }
    return emp->get_NgaySinh() == s;
}
// Các tiêu chí so sánh
bool FindID(Entity *, const string &);
bool FindName(Entity *, const string &);
bool FindDate(Entity *, const Date &);