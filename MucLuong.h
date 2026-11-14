#pragma once
#include <iostream>
#include <iomanip>
#include "ViTriViecLam.h"
using namespace std;
class MucLuong
{
protected:
    ViTriViecLam *VTVL;
    int Bac_VTVL;

public:
    MucLuong(/* args */);
    virtual ~MucLuong();
    virtual double get_Luong() = 0;
    virtual void set_Luong(double) = 0;
    int get_Bac_VTVL();
    void set_Bac_VTVL(int);
    string get_Ma_VTVL();
    ViTriViecLam *get_VTVL();
    void set_VTVL(ViTriViecLam *v);
    virtual string get_Loai_NV() = 0;
};

MucLuong::MucLuong(/* args */)
{
}

MucLuong::~MucLuong()
{
}

int MucLuong::get_Bac_VTVL()
{
    return this->Bac_VTVL;
}
void MucLuong::set_Bac_VTVL(int b)
{
    this->Bac_VTVL = b;
}
string MucLuong::get_Ma_VTVL()
{
    return this->VTVL->get_MaVTVL();
}
ViTriViecLam *MucLuong::get_VTVL()
{
    return this->VTVL;
}
void MucLuong::set_VTVL(ViTriViecLam *v)
{
    this->VTVL = v;
}