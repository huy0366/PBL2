#pragma once
#include "Entity.h"
#include <iostream>
#include <iomanip>
using namespace std;
class ChucVu : public Entity
{
private:
    string MaChucVu, Name;
    double PhuCap;

public:
    ChucVu();
    ChucVu(string, string, long long);
    void set_MaChucVu(const string &);
    string get_MaChucVu() const;

    void set_TenChucVu(const string &);
    string get_TenChucVu() const;

    void set_PhuCap(const double &);
    double get_PhuCap() const;
    void set_Luong_Base(long long phucap);

    void Show();
    ~ChucVu();
};

ChucVu::ChucVu() {}

ChucVu::ChucVu(string ma, string ten, long long phucap) : MaChucVu(ma), Name(ten), PhuCap(phucap)
{
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
void ChucVu::set_PhuCap(const double &PC)
{
    this->PhuCap = PC;
}
double ChucVu::get_PhuCap() const
{
    return this->PhuCap;
}
void ChucVu::set_Luong_Base(long long phucap)
{
    this->PhuCap = phucap;
}
void ChucVu::Show()
{
    cout << setw(10) << this->MaChucVu
         << setw(10) << this->Name
         << setw(10) << fixed << setprecision(0) << this->PhuCap << endl;
}
ChucVu::~ChucVu() {}