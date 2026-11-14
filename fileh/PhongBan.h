
#pragma once
#include "LinkedList.h"
#include "Entity.h"
using namespace std;

class PhongBan : public Entity
{
private:
    string MaPhongBan, TenPhongBan;

public:
    PhongBan();
    PhongBan(string, string);
    void set_MaPhongBan(string);
    string get_MaPhongBan() const;
    void set_TenPhongBan(string);
    string get_TenPhongBan() const;
    // Hàm so sánh kiểm tra Tên Chi Nhánh
    static bool checkPhongBanName(Entity *entity, const string &name);
    void Show() override;
};

PhongBan::PhongBan() {}
PhongBan::PhongBan(string ma, string name)
{
    this->MaPhongBan = ma;
    this->TenPhongBan = name;
}

void PhongBan::set_MaPhongBan(string ma)
{
    this->MaPhongBan = ma;
}
string PhongBan::get_MaPhongBan() const
{
    return this->MaPhongBan;
}

void PhongBan::set_TenPhongBan(string name)
{
    this->TenPhongBan = name;
}
string PhongBan::get_TenPhongBan() const
{
    return this->TenPhongBan;
}

void PhongBan::Show()
{
    cout << this->MaPhongBan << this->TenPhongBan << endl;
}

// Hàm so sánh kiểm tra Tên Chi Nhánh
bool PhongBan::checkPhongBanName(Entity *entity, const string &name)
{
    PhongBan *b = dynamic_cast<PhongBan *>(entity);
    return (b != nullptr) && (b->get_MaPhongBan() == name);
}
