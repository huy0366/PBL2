#pragma once
#include <iostream>
#include <iomanip>
#include "Entity.h"
#include "ViTriViecLam.h"
using namespace std;
class MucLuong : public Entity
{
protected:
    ViTriViecLam *VTVL;
    int Bac_VTVL;

public:
    MucLuong(/* args */);
    virtual ~MucLuong();
    virtual double get_Luong() = 0;
    int get_Bac_VTVL();
    string get_Ma_VTVL();
    virtual string get_Loai_NV() = 0;
    void Show() override;
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
string MucLuong::get_Ma_VTVL()
{
    return this->VTVL->get_MaVTVL();
}
void MucLuong::Show()
{
    cout << left
         << setw(15) << this->get_Ma_VTVL()
         << setw(20) << this->VTVL->get_TenVTVL()
         << setw(10) << this->Bac_VTVL
         << setw(15) << this->get_Loai_NV()
         << setw(15) << fixed << setprecision(0) << this->get_Luong()
         << endl;
}