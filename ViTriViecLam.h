#pragma once
#include <iostream>
#include <iomanip>
#include "Entity.h"
using namespace std;
class ViTriViecLam : public Entity
{
private:
    string MaVTVL, TenVTVL;

public:
    ViTriViecLam(const string &, const string &);
    ~ViTriViecLam();
    void set_MaVTVL(const string &);
    string get_MaVTVL();
    void set_TenVTVL(const string &);
    string get_TenVTVL();
    void Show() override;
};

ViTriViecLam::ViTriViecLam(const string &ma, const string &ten) : MaVTVL(ma), TenVTVL(ten)
{
}

ViTriViecLam::~ViTriViecLam()
{
}
void ViTriViecLam::set_MaVTVL(const string &ma)
{
    this->MaVTVL = ma;
}
string ViTriViecLam::get_MaVTVL()
{
    return this->MaVTVL;
}
void ViTriViecLam::set_TenVTVL(const string &ten)
{
    this->TenVTVL = ten;
}
string ViTriViecLam::get_TenVTVL()
{
    return this->TenVTVL;
}
void ViTriViecLam::Show()
{
    cout << left
         << setw(15) << this->MaVTVL
         << setw(30) << this->TenVTVL
         << endl;
}