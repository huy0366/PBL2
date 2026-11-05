#include "QLNV.h"
int main()
{
    QLNV *ql = new QLNV();
    // Node<ChucVu> *p = ql->get_ListCV()->get_header()->Next;
    // while (p != nullptr)
    // {
    //     cout << p->Data->get_MaChucVu() << " " << p->Data->get_TenChucVu() << " " << p->Data->get_PhuCap();
    //     cout << endl;
    //     p = p->Next;
    // }
    // ChucVu *cv = new ChucVu("CV9999", "Phòng Game", 1123231);
    // ql->ThemChucVu(cv);
    // Node<ChucVu> *ptr = ql->get_ListCV()->get_header()->Next;
    // while (ptr != nullptr)
    // {
    //     cout << ptr->Data->get_MaChucVu() << " " << ptr->Data->get_TenChucVu() << " " << ptr->Data->get_PhuCap();
    //     cout << endl;
    //     ptr = ptr->Next;
    // }
    // Node<Employee> *p = ql->get_ListEmployee()->get_header()->Next;
    // while (p != nullptr)
    // {
    //     cout << p->Data->get_mnv() << p->Data->get_name() << p->Data->get_NgaySinh() << p->Data->get_sex()
    //          << p->Data->get_ChucVu()->get_TenChucVu() << p->Data->get_ViTri()->get_TenVTVL() << p->Data->get_PhongBan()->get_TenPhongBan()
    //          << p->Data->get_NgayLamViec() << p->Data->get_ThuongPhat() << p->Data->get_BacViTri() << endl;
    //     ;
    //     cout << endl;
    //     p = p->Next;
    // }
    // int size_nv = ql->get_ListEmployee()->get_size();
    // List<Employee> *e = ql->get_ListEmployee();
    // for (int i = 0; i < size_nv; i++)
    // {
    //     cout << (*e)[i]->Data->get_mnv() << (*e)[i]->Data->get_name() << (*e)[i]->Data->get_NgaySinh() << (*e)[i]->Data->get_sex()
    //          << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
    //          << (*e)[i]->Data->get_NgayLamViec() << (*e)[i]->Data->get_ThuongPhat() << (*e)[i]->Data->get_BacViTri() << endl;
    // }
    // ql->SapXep(TieuChiSapXep::MNV_TANG);
    // cout << "=============================================" << endl;
    // for (int i = 0; i < size_nv; i++)
    // {
    //     cout << (*e)[i]->Data->get_mnv() << (*e)[i]->Data->get_name() << (*e)[i]->Data->get_NgaySinh() << (*e)[i]->Data->get_sex()
    //          << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
    //          << (*e)[i]->Data->get_NgayLamViec() << (*e)[i]->Data->get_ThuongPhat() << (*e)[i]->Data->get_BacViTri() << endl;
    // }
    // cout << " ========================================" << endl;
    // Date d1 = Date("5/8/1991");
    // Date d2 = Date("18/8/2010");
    // int thuong = 0, vtri = 3, ngaycong = 26, heso = 1.9;
    // ChucVu *cv = ql->get_DB()->FindCV("CV04");
    // ViTriViecLam *vt = ql->get_DB()->FindVTVL("VTVL03");
    // PhongBan *pb = ql->get_DB()->FindPB("PB03");
    // Employee *e1 = new FullTime("NV015", "Đinh Quang Phúc", d1, "Nam", cv, vt, pb, d2, thuong, vtri, ngaycong, heso);

    // ql->SuaEmployee(e1);
    // // ql->XoaEmployee(e1);
    // cout << "main.cpp";
    // for (int i = 0; i < size_nv; i++)
    // {
    //     // if ((*e)[i]->Data->get_mnv() == "NV015")

    //     cout << (*e)[i]->Data->get_mnv() << (*e)[i]->Data->get_name() << (*e)[i]->Data->get_NgaySinh() << (*e)[i]->Data->get_sex()
    //          << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
    //          << (*e)[i]->Data->get_NgayLamViec() << (*e)[i]->Data->get_ThuongPhat() << (*e)[i]->Data->get_BacViTri() << endl;
    // }
    // Date d1 = Date("5/8/2000");
    // Date d2 = Date("18/8/2020");
    // int thuong = 0, vtri = 3, ngaycong = 26, heso = 1.9;
    // ChucVu *cv = ql->get_DB()->FindCV("CV04");
    // ViTriViecLam *vt = ql->get_DB()->FindVTVL("VTVL03");
    // PhongBan *pb = ql->get_DB()->FindPB("PB03");
    // Employee *e2 = new FullTime("NV016", "he he", d1, "Nam", cv, vt, pb, d2, thuong, vtri, ngaycong, heso);
    // ql->ThemEmployee(e2);
    // size_nv = ql->get_ListEmployee()->get_size();
    // ql->XoaEmployee(e2);
    // size_nv = ql->get_ListEmployee()->get_size();
    // for (int i = 0; i < size_nv; i++)
    // {
    //     // if ((*e)[i]->Data->get_mnv() == "NV015")

    //     cout << (*e)[i]->Data->get_mnv() << (*e)[i]->Data->get_name() << (*e)[i]->Data->get_NgaySinh() << (*e)[i]->Data->get_sex()
    //          << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
    //          << (*e)[i]->Data->get_NgayLamViec() << (*e)[i]->Data->get_ThuongPhat() << (*e)[i]->Data->get_BacViTri() << endl;
    // }
    // cout << size_nv;
    // ql->SapXep(TieuChiSapXep::NGAYSINH_GIAM);
    // for (int i = 0; i < e->get_size(); i++)
    // {
    //     // if ((*e)[i]->Data->get_mnv() == "NV015")

    //     cout << (*e)[i]->Data->get_mnv() << (*e)[i]->Data->get_name() << (*e)[i]->Data->get_NgaySinh() << (*e)[i]->Data->get_sex()
    //          << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
    //          << (*e)[i]->Data->get_NgayLamViec() << (*e)[i]->Data->get_ThuongPhat() << (*e)[i]->Data->get_BacViTri() << endl;
    // }
    // ChucVu *cv = new ChucVu("CV05", "Phó Giám đốc", 2500000);
    // ql->SuaChucVu(cv);
    // PhongBan *pb = new PhongBan("PB01", "Phòng IT");
    // ql->SuaPhongBan(pb);
    // ViTriViecLam *vt = new ViTriViecLam("VTVL01", "cc");
    // ql->SuaVTVL(vt);
    // for (int i = 0; i < e->get_size(); i++)
    // {
    //     // if ((*e)[i]->Data->get_mnv() == "NV015")

    //     cout << " " << (*e)[i]->Data->get_mnv() << " " << (*e)[i]->Data->get_name() << " " << (*e)[i]->Data->get_NgaySinh() << " " << (*e)[i]->Data->get_sex()
    //          << " " << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << " " << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << " " << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
    //          << " " << (*e)[i]->Data->get_NgayLamViec() << " " << (*e)[i]->Data->get_ThuongPhat() << " " << (*e)[i]->Data->get_BacViTri() << " " << endl;
    // }
    // Phong
    // delete ql;
    // PhongBan *pb = ql->get_DB()->FindPB("PB03");
    // cout << pb->get_sizeNV() << "dain";
    cout << "==================================================" << endl;
    PhongBan *pb = ql->get_DB()->FindPB("PB03");
    List<Employee> *e = pb->get_ListNV();
    int size = e->get_size();
    cout << (ql->get_ListEmployee()->get_size());
    for (int i = 0; i < e->get_size(); i++)
        cout << " " << (*e)[i]->Data->get_mnv() << " " << (*e)[i]->Data->get_name() << " " << (*e)[i]->Data->get_NgaySinh() << " " << (*e)[i]->Data->get_sex()
             << " " << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << " " << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << " " << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
             << " " << (*e)[i]->Data->get_NgayLamViec() << " " << (*e)[i]->Data->get_ThuongPhat() << " " << (*e)[i]->Data->get_BacViTri() << " " << endl;
    Employee *e1 = ql->TimTheoMa("NV005");
    ql->XoaEmployee(e1);
    cout << "daxoa====================================" << endl;
    size = e->get_size();
    for (int i = 0; i < e->get_size(); i++)
        cout << " " << (*e)[i]->Data->get_mnv() << " " << (*e)[i]->Data->get_name() << " " << (*e)[i]->Data->get_NgaySinh() << " " << (*e)[i]->Data->get_sex()
             << " " << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << " " << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << " " << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
             << " " << (*e)[i]->Data->get_NgayLamViec() << " " << (*e)[i]->Data->get_ThuongPhat() << " " << (*e)[i]->Data->get_BacViTri() << " " << endl;
    cout << size;
    cout << (ql->get_ListEmployee()->get_size());
    cout << "sua nhan vien==========================" << endl;
    Date d1 = Date("5/8/1991");
    Date d2 = Date("18/8/2010");
    int thuong = 100000, vtri = 3, ngaycong = 10, heso = 1.9;
    ChucVu *cv = ql->get_DB()->FindCV("CV04");
    ViTriViecLam *vt = ql->get_DB()->FindVTVL("VTVL03");
    pb = ql->get_DB()->FindPB("PB03");
    Employee *esua = new FullTime("NV015", "Đinh Quang Phúc", d1, "Nam", cv, vt, pb, d2, vtri, thuong, ngaycong, heso);
    ql->SuaEmployee(esua);
    size = e->get_size();
    for (int i = 0; i < e->get_size(); i++)
        cout << " " << (*e)[i]->Data->get_mnv() << " " << (*e)[i]->Data->get_name() << " " << (*e)[i]->Data->get_NgaySinh() << " " << (*e)[i]->Data->get_sex()
             << " " << (*e)[i]->Data->get_ChucVu()->get_TenChucVu() << " " << (*e)[i]->Data->get_ViTri()->get_TenVTVL() << " " << (*e)[i]->Data->get_PhongBan()->get_TenPhongBan()
             << " " << (*e)[i]->Data->get_NgayLamViec() << " " << (*e)[i]->Data->get_ThuongPhat() << " " << (*e)[i]->Data->get_BacViTri() << " " << endl;
    cout << size;
    cout << (ql->get_ListEmployee()->get_size());
}