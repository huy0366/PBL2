#include "DataBase.h"
int main()
{
    DataBase *DB = new DataBase();
    DB->ReadAll();
    cout << "CHUA SUA" << endl;
    List<Employee> *e = DB->get_ListEmployee();
    int sizenv = e->get_size();
    cout << "Nhan vien " << sizenv << endl;
    for (int i = 0; i < sizenv; i++)
        cout << (*e)[i]->Data->get_mnv() << " " << (*e)[i]->Data->get_name() << " " << (*e)[i]->Data->get_NgaySinh() << " " << (*e)[i]->Data->get_sex()
             << " " << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << " " << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << " " << "|||" << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
             << "|||" << " " << (*e)[i]->Data->get_NgayLamViec() << " " << (*e)[i]->Data->get_ThuongPhat() << " " << (*e)[i]->Data->get_BacViTri() << " " << endl;
    cout << "================================================" << endl;
    List<PhongBan> *e1 = DB->get_ListPB();
    int sizepb = e1->get_size();
    cout << "Phong Ban " << sizepb << endl;
    for (int i = 0; i < sizepb; i++)
        cout << (*e1)[i]->Data->get_MaPhongBan() << " " << (*e1)[i]->Data->get_TenPhongBan() << " " << (*e1)[i]->Data->get_NgayThanhLap() << endl;
    cout << "================================================" << endl;
    Date d("25/01/2026");
    PhongBan *pb = new PhongBan("PB03", "Phong IT", d);
    DB->Repair(pb);
    e = DB->get_ListEmployee();
    sizenv = e->get_size();
    cout << "DA SUA" << endl;
    cout << "Nhan vien " << sizenv << endl;
    for (int i = 0; i < sizenv; i++)
        cout << (*e)[i]->Data->get_mnv() << " " << (*e)[i]->Data->get_name() << " " << (*e)[i]->Data->get_NgaySinh() << " " << (*e)[i]->Data->get_sex()
             << " " << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << " " << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << " " << "|||" << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
             << "|||" << " " << (*e)[i]->Data->get_NgayLamViec() << " " << (*e)[i]->Data->get_ThuongPhat() << " " << (*e)[i]->Data->get_BacViTri() << " " << endl;
    cout << "================================================" << endl;
    e1 = DB->get_ListPB();
    sizepb = e1->get_size();
    cout << "Phong Ban" << sizepb << endl;
    for (int i = 0; i < sizepb; i++)
        cout << (*e1)[i]->Data->get_MaPhongBan() << " " << (*e1)[i]->Data->get_TenPhongBan() << " " << (*e1)[i]->Data->get_NgayThanhLap() << endl;
    cout << "================================================" << endl;
    PhongBan *pb1 = DB->FindPB("PB03");
    List<Employee> *pb2 = pb1->get_ListNV();
    sizepb = pb2->get_size();
    for (int i = 0; i < sizepb; i++)
        cout << (*pb2)[i]->Data->get_mnv() << " " << (*pb2)[i]->Data->get_name() << " " << (*pb2)[i]->Data->get_NgaySinh() << " " << (*pb2)[i]->Data->get_sex()
             << " " << (*pb2)[i]->Data->get_ChucVu()->get_TenChucVu() << " " << (*pb2)[i]->Data->get_ViTri()->get_TenVTVL() << " " << "|||" << (*pb2)[i]->Data->get_PhongBan()->get_TenPhongBan()
             << "|||" << " " << (*pb2)[i]->Data->get_NgayLamViec() << " " << (*pb2)[i]->Data->get_ThuongPhat() << " " << (*pb2)[i]->Data->get_BacViTri() << " " << endl;
    // Employee *e = DB->FindEmployee("NV003");
    // List<Employee> *e1 = e->get_PhongBan()->get_ListNV();
    // Date d("1/4/2025");
    // PhongBan *pb = new PhongBan("PB06", "Phong IT", d);
    // DB->Insert(pb);
    // int size = e->get_size();

    // DB->SaveAll();
    // for (int i = 0; i < size; i++)
    //     cout << (*e1)[i]->Data->get_mnv() << (*e1)[i]->Data->get_name() << (*e1)[i]->Data->get_NgaySinh() << (*e1)[i]->Data->get_sex()
    //          << (*e1)[i]->Data->get_ChucVu()->get_TenChucVu() << (*e1)[i]->Data->get_ViTri()->get_TenVTVL() << (*e1)[i]->Data->get_PhongBan()->get_TenPhongBan()
    //          << (*e1)[i]->Data->get_NgayLamViec() << (*e1)[i]->Data->get_ThuongPhat() << (*e1)[i]->Data->get_BacViTri() << endl;
    // cout << size;
}