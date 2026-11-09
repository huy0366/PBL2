#pragma once
#include <iostream>
#include <string>
#include "MucLuong.h"
using namespace std;
class MucLuong_FullTime : public MucLuong
{
protected:
    double Luong_Thang;

public:
    MucLuong_FullTime(ViTriViecLam *, const int &, const double &);
    ~MucLuong_FullTime();
    double get_Luong();
    void set_Luong(double luong) override { this->Luong_Thang = luong; }
    string get_Loai_NV() override;
    void Show() override;
};

MucLuong_FullTime::MucLuong_FullTime(ViTriViecLam *VTVL, const int &bac, const double &luong)
{
    this->VTVL = VTVL;
    this->Bac_VTVL = bac;
    this->Luong_Thang = luong;
}
MucLuong_FullTime::~MucLuong_FullTime()
{
}
double MucLuong_FullTime::get_Luong()
{
    return this->Luong_Thang;
}
string MucLuong_FullTime::get_Loai_NV()
{
    return "FullTime";
}
void MucLuong_FullTime::Show()
{
    cout << left
         << setw(15) << this->get_Ma_VTVL()
         << setw(20) << this->VTVL->get_TenVTVL()
         << setw(10) << this->Bac_VTVL
         << setw(15) << this->get_Loai_NV()
         << setw(15) << fixed << setprecision(0) << this->Luong_Thang
         << endl;
}