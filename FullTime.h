#pragma once
#include <iostream>
#include "ChucVu.h"
#include "Date.h"
#include "Employee.h"
#include "PhongBan.h"
#include "MucLuong.h"

using namespace std;
class DataBase;
class FullTime : public Employee
{
private:
    int SoNgayCong;
    double HeSo;

public:
    FullTime();
    FullTime(const string &, const string &, const Date &, const string &, ChucVu * = nullptr,
             ViTriViecLam * = nullptr, PhongBan * = nullptr, const Date & = Date(),
             const int & = 1, const int & = 0, const int & = 0, const double & = 1.0);
    int get_SoNgayCong();
    void set_SoNgayCong(const int &);
    double get_HeSo();
    double get_ThucLinh(const double &) override;
    string get_LoaiNV() override;
    void Show() override;
    double tinhHS(const Date &);
    virtual ~FullTime();
};

FullTime::FullTime(const string &mnv, const string &ten, const Date &ngay_sinh, const string &gioitinh,
                   ChucVu *cv, ViTriViecLam *vt, PhongBan *pb, const Date &ngay_lam, const int &bac,
                   const int &thuong, const int &songaycong, const double &heso)
    : Employee(mnv, ten, ngay_sinh, gioitinh, cv, vt, pb, ngay_lam, bac, thuong)
{
    this->SoNgayCong = songaycong;
    this->HeSo = tinhHS(ngay_lam);
}
int FullTime::get_SoNgayCong()
{
    return this->SoNgayCong;
}
void FullTime::set_SoNgayCong(const int &snc)
{
    this->SoNgayCong = snc;
}

double FullTime::get_HeSo()
{
    return this->HeSo;
}

// double FullTime::get_ThucLinh(DataBase *bl)
// {
//     MucLuong *ml = bl->FindML(this->get_ViTri()->get_MaVTVL(), this->get_BacViTri(), "FullTime");
//     return (ml->get_Luong() * this->SoNgayCong) / 26 * this->HeSo;
// }

string FullTime::get_LoaiNV()
{
    return "FullTime";
}

void FullTime::Show()
{
    Employee::Show();
    cout << setw(10) << get_LoaiNV()
         << setw(10) << get_SoNgayCong()
         << setw(10) << get_HeSo() << endl;
}
double FullTime::tinhHS(const Date &d)
{
    int year = Date::DateHienTai().get_year() - d.get_year();
    return (1.0 + ((year / 3) * 0.3));
}
FullTime::~FullTime()
{
}