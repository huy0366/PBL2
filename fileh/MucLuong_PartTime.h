#pragma once
#include <iostream>
#include <string>
#include "MucLuong.h"
using namespace std;
class MucLuong_PartTime : public MucLuong
{
protected:
    double Luong_Gio;

public:
    MucLuong_PartTime(ViTriViecLam *, const int &, const double &);
    ~MucLuong_PartTime();
    double get_Luong();
    string get_Loai_NV() override;
    void Show() override;
};

MucLuong_PartTime::MucLuong_PartTime(ViTriViecLam *VTVL, const int &bac, const double &luong)
{
    this->VTVL = VTVL;
    this->Bac_VTVL = bac;
    this->Luong_Gio = luong;
}
MucLuong_PartTime::~MucLuong_PartTime()
{
}
double MucLuong_PartTime::get_Luong()
{
    return this->Luong_Gio;
}
string MucLuong_PartTime::get_Loai_NV()
{
    return "PartTime";
}
void MucLuong_PartTime::Show()
{
    cout << left
         << setw(15) << this->get_Ma_VTVL()
         << setw(20) << this->VTVL->get_TenVTVL()
         << setw(10) << this->Bac_VTVL
         << setw(15) << this->get_Loai_NV()
         << setw(15) << fixed << setprecision(0) << this->Luong_Gio
         << endl;
}