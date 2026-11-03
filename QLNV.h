#pragma once
#include <iostream>
#include "DataBase.h"
#include <ctime>
using namespace std;

enum class TieuChiSapXep
{
    MNV_TANG,
    MNV_GIAM,
    TEN_TANG,
    TEN_GIAM,
    NGAYSINH_TANG,
    NGAYSINH_GIAM,
    THAM_NIEN_GIAM,
    THAM_NIEN_TANG
};

class QLNV : public Entity
{
private:
    DataBase *DB;

    int tuoiToiThieu;
    int tuoiToiDa;
    int bacToiDa;
    int soNgayCongChuan;
    int soGioToiDaPartTime;

    string tienToMa;
    int maTiepTheo;

    Date ngayHienTai;

    // string fileLog; // "data/log.txt"
    // string nguoiDangNhap;
    // int quyenHan; // vd 0: xem, 1: sửa, 2: admin

    double lcb(Employee *) const;
    static bool MNVIncrease(Employee *x, Employee *y) { return x->get_mnv() < y->get_mnv(); }
    static bool MNVDecrease(Employee *x, Employee *y) { return x->get_mnv() > y->get_mnv(); }

    static bool NameIncrease(Employee *x, Employee *y) { return x->get_name() < y->get_name(); }
    static bool NameDecrease(Employee *x, Employee *y) { return x->get_name() > y->get_name(); }

    static bool DateIncrease(Employee *x, Employee *y) { return x->get_NgaySinh() < y->get_NgaySinh(); }
    static bool DateDecrease(Employee *x, Employee *y) { return x->get_NgaySinh() > y->get_NgaySinh(); }

    static bool NgayLamIncrease(Employee *x, Employee *y) { return x->get_NgayLamViec() < y->get_NgayLamViec(); }
    static bool NgayLamDecrease(Employee *x, Employee *y) { return x->get_NgayLamViec() > y->get_NgayLamViec(); }

    bool CheckEmployee(Employee *); // có hợp lệ không, không thì throw (LƯU Ý: chưa check trùng)
    bool CheckPhongBan(PhongBan *);
    bool CheckChucVu(ChucVu *);
    bool CheckVTVL(ViTriViecLam *);
    bool CheckMucLuong(MucLuong *);

public:
    QLNV(/* args */);

    // đếm nv
    int DemNVThamChieuPB(PhongBan *);
    int DemNVThamChieuCV(ChucVu *);
    int DemNVThamChieuVT(ViTriViecLam *);
    int DemNVThamChieuML(const string &, int, const string &);
    // thêm
    bool ThemEmployee(Employee *, Node<Employee> * = nullptr);
    bool ThemPhongBan(PhongBan *);
    bool ThemChucVu(ChucVu *);
    bool ThemViTri(ViTriViecLam *);
    bool ThemMucLuong(MucLuong *);
    // xóa
    bool XoaEmployee(Employee *);
    bool XoaPhongBan(PhongBan *);
    bool XoaChucVu(ChucVu *);
    bool XoaViTri(ViTriViecLam *);
    // bool XoaMucLuong(const string &, int, const string &);
    // sửa
    bool SuaEmployee(Employee *);
    bool SuaChucVu(ChucVu *);
    bool SuaPhongBan(PhongBan *);
    bool SuaVTVL(ViTriViecLam *);
    bool SuaMucLuong(MucLuong *);

    // find
    Employee *TimTheoMa(const string &);
    List<Employee> *TimTheoTen(const string &);
    List<Employee> *TimTheoPhongBan(const string &);
    List<Employee> *TimTheoChucVu(const string &);

    // double TinhLuong(const string &);
    // double TongQuyLuong();
    // double TongQuyLuongPhongBan(const string &);
    // Employee *LuongCaoNhat();
    // Employee *LuongThapNhat();
    // int DemTheoPhongBan(const string &);
    // void ThongKeTheoPhongBan();
    // void DanhSachTheoThamNien();

    // ================= Sắp xếp & hiển thị =================
    List<PhongBan> *get_ListPB() const;
    List<ChucVu> *get_ListCV() const;
    List<ViTriViecLam> *get_ListVTVL() const;
    List<MucLuong> *get_ListML() const;
    List<Employee> *get_ListEmployee() const;
    DataBase *get_DB() const;
    void SapXep(TieuChiSapXep);
    void Show() override;
    ~QLNV();
};

QLNV::QLNV(/* args */)
{
    this->DB = new DataBase();
    this->DB->ReadAll();
    tuoiToiThieu = 18;
    tuoiToiDa = 60;
    bacToiDa = 3;
    soNgayCongChuan = 26;
    soGioToiDaPartTime = 80;
    tienToMa = "NV";
    maTiepTheo = 1;
    ngayHienTai = Date::DateHienTai();
}

QLNV::~QLNV()
{
    this->DB->SaveAll();
    delete DB;
}

double QLNV::lcb(Employee *e) const
{
    if (!e || !e->get_ViTri()) // thêm check này
        return -1;             // trả về -1 báo hiệu không hợp lệ

    string mvt = e->get_MaViTri();
    int bac = e->get_BacViTri();
    string loainv = e->get_LoaiNV();

    MucLuong *ml = DB->FindML(mvt, bac, loainv);
    if (!ml)
        return -1; // thêm check này
    return ml->get_Luong();
}

bool QLNV::CheckEmployee(Employee *e)
{
    try
    {
        if (e == nullptr)
            throw "Chưa có thông tin nhân viên";
        if (e->get_mnv() == "")
            throw "Chưa nhập mã nhân viên";

        if (e->get_PhongBan() == nullptr)
            throw "Chưa chọn phòng ban";
        if (e->get_ChucVu() == nullptr)
            throw "Chưa chọn chức vụ";
        if (e->get_ViTri() == nullptr)
            throw "Chưa chọn vị trí việc làm";
        int tuoi = ngayHienTai.get_year() - e->get_NgaySinh().get_year();
        if (tuoi < this->tuoiToiThieu)
            throw "Chưa đủ 18 tuổi";
        if (tuoi > this->tuoiToiDa)
            throw "Vượt quá tuổi tối đa (60 tuổi)";
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::CheckPhongBan(PhongBan *pb)
{
    try
    {
        if (pb == nullptr)
            throw "Chưa có thông tin";
        if (pb->get_MaPhongBan() == "")
            throw "chưa nhập mã phòng ban";
        if (pb->get_TenPhongBan() == "")
            throw "chưa nhập tên phòng ban";
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::CheckChucVu(ChucVu *cv)
{
    try
    {
        if (cv == nullptr)
            throw "Chưa có thông tin";
        if (cv->get_MaChucVu() == "")
            throw "chưa nhập mã Chức Vụ";
        if (cv->get_TenChucVu() == "")
            throw "Chưa nhập tên chức vụ";
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::CheckVTVL(ViTriViecLam *vt)
{
    try
    {
        if (vt == nullptr)
            throw "Chưa có thông tin";
        if (vt->get_MaVTVL() == "")
            throw "chưa nhập mã Vị Trí Việc Làm";
        if (vt->get_TenVTVL() == "")
            throw "chưa nhập tên vị trí việc làm";
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::CheckMucLuong(MucLuong *ml)
{
    try
    {
        if (ml->get_Bac_VTVL() < 1)
            throw "Bậc nhỏ hơn 1";
        if (ml->get_Luong() < 0)
            throw "Lương âm";
    }
    catch (...)
    {
        throw;
    }
    return 1;
}

int QLNV::DemNVThamChieuPB(PhongBan *pb)
{
    if (pb == nullptr)
        return 0;
    return pb->get_ListNV()->get_size();
}
int QLNV::DemNVThamChieuCV(ChucVu *cv)
{
    if (cv == nullptr)
        return 0;
    int c = 0;
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data->get_ChucVu() == cv)
            ++c;
        p = p->Next;
    }
    return c;
}
int QLNV::DemNVThamChieuVT(ViTriViecLam *vt)
{
    if (vt == nullptr)
        return 0;
    int c = 0;
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data->get_ViTri() == vt)
            ++c;
        p = p->Next;
    }
    return c;
}
int QLNV::DemNVThamChieuML(const string &ma, int bac, const string &loai)
{
    int c = 0;
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data->get_ViTri() && p->Data->get_MaViTri() == ma && p->Data->get_BacViTri() == bac && p->Data->get_LoaiNV() == loai)
            ++c;
        p = p->Next;
    }
    return c;
}

bool QLNV::ThemEmployee(Employee *e, Node<Employee> *p)
{
    try
    {
        CheckEmployee(e);
        if (DB->FindEmployee(e->get_mnv()) != nullptr)
            throw "Trùng mã nhân viên đã tồn tại";
        this->DB->Insert(e, p);
    }
    catch (...)
    {
        throw;
    }

    return 1;
}
bool QLNV::ThemPhongBan(PhongBan *pb)
{
    try
    {
        CheckPhongBan(pb);
        if (DB->FindPB(pb->get_MaPhongBan()) != nullptr)
            throw "trùng mã phòng ban";
        this->DB->Insert(pb);
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::ThemChucVu(ChucVu *cv)
{
    try
    {
        CheckChucVu(cv);
        if (DB->FindCV(cv->get_MaChucVu()) != nullptr)
            throw "trùng mã Chức Vụ";
        this->DB->Insert(cv);
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::ThemViTri(ViTriViecLam *vt)
{
    try
    {
        CheckVTVL(vt);
        if (DB->FindVTVL(vt->get_MaVTVL()) != nullptr)
            throw "trùng mã Vị Trí Việc Làm";
        this->DB->Insert(vt);
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::ThemMucLuong(MucLuong *ml)
{
    try
    {
        CheckMucLuong(ml);
        if (DB->FindML(ml->get_Ma_VTVL(), ml->get_Bac_VTVL(), ml->get_Loai_NV()) != nullptr)
            throw "Trùng mức lương (vị trí/bậc/loại)";
        DB->Insert(ml);
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
    return 1;
}

bool QLNV::XoaEmployee(Employee *e)
{
    try
    {
        if (e == nullptr)
            throw "NULL";    //????
        this->DB->Remove(e); // gỡ khỏi Hash + List
        delete e;            // QuanLyNhanVien/DataBase là chủ sở hữu
        return true;
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
}
bool QLNV::XoaPhongBan(PhongBan *pb)
{
    try
    {
        if (pb == nullptr)
            throw "Không tìm thấy phòng ban mã " + pb->get_MaPhongBan();
        int n = DemNVThamChieuPB(pb);
        if (n > 0)
            throw "Không thể xóa phòng ban mã " + pb->get_MaPhongBan() + ": còn " + to_string(n) + " nhân viên";
        DB->Remove(pb);
        delete pb;
        return true;
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
}
bool QLNV::XoaChucVu(ChucVu *cv)
{
    try
    {
        if (cv == nullptr)
            throw "Không tìm thấy chức vụ mã " + cv->get_MaChucVu();
        int n = DemNVThamChieuCV(cv);
        if (n > 0)
            throw "Không thể xóa chức vụ mã " + cv->get_MaChucVu() + ": còn " + to_string(n) + " nhân viên";
        DB->Remove(cv);
        delete cv;
        return true;
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
}
bool QLNV::XoaViTri(ViTriViecLam *vt)
{
    try
    {
        if (!vt)
            throw "Không tìm thấy vị trí mã " + vt->get_MaVTVL();
        int n = DemNVThamChieuVT(vt);
        if (n > 0)
            throw "Không thể xóa vị trí " + vt->get_MaVTVL() + ": còn " + to_string(n) + " nhân viên";

        DB->Remove(vt);
        Node<MucLuong> *p = DB->get_ListML()->get_header()->Next;
        while (p != nullptr)
        {
            Node<MucLuong> *next = p->Next;
            if (p->Data && p->Data->get_Ma_VTVL() == vt->get_MaVTVL())
                DB->Remove(p->Data);
            p = next;
        }
        delete vt;
        return true;
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
}

bool QLNV::SuaEmployee(Employee *e)
{
    try
    {
        CheckEmployee(e);
        DB->Repair(e);
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::SuaChucVu(ChucVu *cv)
{
    try
    {
        CheckChucVu(cv);
        DB->Repair(cv);
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::SuaPhongBan(PhongBan *pb)
{
    try
    {
        CheckPhongBan(pb);
        DB->Repair(pb);
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::SuaVTVL(ViTriViecLam *vt)
{
    try
    {
        CheckVTVL(vt);
        DB->Repair(vt);
    }
    catch (...)
    {
        throw;
    }
    return 1;
}
bool QLNV::SuaMucLuong(MucLuong *ml)
{
    try
    {
        CheckMucLuong(ml);
        DB->Repair(ml);
    }
    catch (...)
    {
        throw;
    }
    return 1;
}

// xem lại
Employee *QLNV::TimTheoMa(const string &ma)
{
    return this->DB->FindEmployee(ma);
}
List<Employee> *QLNV::TimTheoTen(const string &ten)
{
    List<Employee> *res = new List<Employee>();
    if (!DB || !DB->get_ListEmployee())
        return res;

    string keyLower = ten;
    for (char &c : keyLower)
        c = (char)tolower((unsigned char)c);

    Node<Employee> *tail = res->get_header();
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr)
        {
            string nameLower = p->Data->get_name();
            for (char &c : nameLower)
                c = (char)tolower((unsigned char)c);
            if (nameLower.find(keyLower) != string::npos)
            {
                res->insert(p->Data, tail);
                tail = tail->Next;
            }
        }
        p = p->Next;
    }
    return res;
}
List<Employee> *QLNV::TimTheoPhongBan(const string &maPB)
{
    List<Employee> *res = new List<Employee>();
    if (!DB || !DB->get_ListEmployee())
        return res;

    Node<Employee> *tail = res->get_header();
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr && p->Data->get_PhongBan() && p->Data->get_MaPhongBan() == maPB)
        {
            res->insert(p->Data, tail);
            tail = tail->Next;
        }
        p = p->Next;
    }
    return res;
}
List<Employee> *QLNV::TimTheoChucVu(const string &maCV)
{
    List<Employee> *res = new List<Employee>();
    if (!DB || !DB->get_ListEmployee())
        return res;

    Node<Employee> *tail = res->get_header();
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr && p->Data->get_ChucVu() && p->Data->get_MaChucVu() == maCV)
        {
            res->insert(p->Data, tail);
            tail = tail->Next;
        }
        p = p->Next;
    }
    return res;
}
/*
double QLNV::TinhLuong(const string &ma) // mã nhân viên
{
    try
    {
        Employee *e = DB->FindEmployee(ma);
        if (!e)
            throw "Không tìm thấy nhân viên mã " + ma;
        double lcb = this->lcb(e);
        return lcb < 0 ? -1 : e->get_ThucLinh(lcb);
    }
    catch (const char *e)
    {
        throw;
    }
    catch (...)
    {
        throw;
    }
}
double QLNV::TongQuyLuong()
{
    double tong = 0;
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        Employee *e = p->Data;
        double lcb1 = lcb(e);
        if (lcb1 >= 0)
            tong += p->Data->get_ThucLinh(lcb1);
        p = p->Next;
    }
    return tong;
}
double QLNV::TongQuyLuongPhongBan(const string &maPB)
{
    double tong = 0;
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        Employee *e = p->Data;
        if (e && e->get_PhongBan() && e->get_MaPhongBan() == maPB)
        {
            double lcb1 = lcb(e);
            if (lcb1 >= 0)
                tong += e->get_ThucLinh(lcb1);
        }
        p = p->Next;
    }
    return tong;
}
Employee *QLNV::LuongCaoNhat()
{
    Employee *best = nullptr;
    double max = -1;
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        Employee *e = p->Data;
        if (e != nullptr)
        {
            double lcb1 = lcb(e);
            if (lcb1 >= 0)
            {
                double l = e->get_ThucLinh(lcb1);
                if (l > max)
                {
                    max = l;
                    best = e;
                }
            }
        }
        p = p->Next;
    }
    return best;
}
Employee *QLNV::LuongThapNhat()
{
    Employee *lowest = nullptr;
    double min = -1;
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    while (p != nullptr)
    {
        Employee *e = p->Data;
        if (e != nullptr)
        {
            double lcb1 = lcb(e);
            if (lcb1 >= 0)
            {
                double l = e->get_ThucLinh(lcb1);
                if (lowest == nullptr || l < min)
                {
                    min = l;
                    lowest = e;
                }
            }
        }
        p = p->Next;
    }
    return lowest;
}
void QLNV::ThongKeTheoPhongBan()
{
    cout << left
         << setw(8) << "Ma PB"
         << setw(25) << "Ten Phong Ban"
         << setw(10) << "So NV"
         << setw(20) << "Tong Quy Luong (VND)" << "\n";
    cout << string(63, '-') << "\n";
    Node<PhongBan> *pb = DB->get_ListPB()->get_header()->Next;
    while (pb != nullptr)
    {
        if (pb->Data == nullptr)
        {
            pb = pb->Next;
            continue;
        }
        string ma = pb->Data->get_MaPhongBan();
        int soNV = DemNVThamChieuPB(ma);
        double tong = TongQuyLuongPhongBan(ma);
        cout << left
             << setw(8) << ma
             << setw(25) << pb->Data->get_TenPhongBan()
             << setw(10) << soNV
             << fixed << setprecision(0) << tong << "\n";
        pb = pb->Next;
    }
}
void QLNV::DanhSachTheoThamNien()
{
    SapXep(TieuChiSapXep::THAM_NIEN_GIAM);
    cout << left
         << setw(5) << "STT"
         << setw(10) << "Ma NV"
         << setw(22) << "Ho Ten"
         << setw(14) << "Ngay Vao Lam"
         << "Tham Nien" << "\n";
    cout << string(65, '-') << "\n";
    Node<Employee> *p = DB->get_ListEmployee()->get_header()->Next;
    int stt = 1;
    while (p != nullptr)
    {
        if (p->Data == nullptr)
        {
            p = p->Next;
            continue;
        }
        Date ngayVao = p->Data->get_NgayLamViec();
        Date tn = ngayHienTai - ngayVao;
        cout << left
             << setw(5) << stt++
             << setw(10) << p->Data->get_mnv()
             << setw(22) << p->Data->get_name()
             << setw(14) << ngayVao
             << tn.get_year() << " nam " << tn.get_month() << " thang\n";
        p = p->Next;
    }
}
*/
List<PhongBan> *QLNV::get_ListPB() const
{
    return DB ? DB->get_ListPB() : nullptr;
}
List<ChucVu> *QLNV::get_ListCV() const
{
    return DB ? DB->get_ListCV() : nullptr;
}
List<ViTriViecLam> *QLNV::get_ListVTVL() const
{
    return DB ? DB->get_ListVTVL() : nullptr;
}
List<MucLuong> *QLNV::get_ListML() const
{
    return DB ? DB->get_ListML() : nullptr;
}
List<Employee> *QLNV::get_ListEmployee() const
{
    return DB ? DB->get_ListEmployee() : nullptr;
}
DataBase *QLNV::get_DB() const
{
    return DB;
}

void QLNV::SapXep(TieuChiSapXep t)
{
    List<Employee> *l = DB->get_ListEmployee();
    switch (t)
    {
    case TieuChiSapXep::MNV_TANG:
        l->sort(MNVIncrease);
        break;
    case TieuChiSapXep::MNV_GIAM:
        l->sort(MNVDecrease);
        break;
    case TieuChiSapXep::TEN_TANG:
        l->sort(NameIncrease);
        break;
    case TieuChiSapXep::TEN_GIAM:
        l->sort(NameDecrease);
        break;
    case TieuChiSapXep::NGAYSINH_TANG:
        l->sort(DateIncrease);
        break;
    case TieuChiSapXep::NGAYSINH_GIAM:
        l->sort(DateDecrease);
        break;
    case TieuChiSapXep::THAM_NIEN_GIAM:
        l->sort(NgayLamIncrease);
        break; // ngày vào sớm = thâm niên cao
    case TieuChiSapXep::THAM_NIEN_TANG:
        l->sort(NgayLamDecrease);
        break;
    }
}

void QLNV::Show()
{
    this->get_ListEmployee()->Show();
}

/* MÀN HÌNH EMPLOYEE
get_ListEmployee() trả về list
get_ListCV() trả về list
get_ListPB() trả về list
Timetheoma, Timtheoten, Timtheopb,timchucvu trả về lish để in ra tất cả những thứ tìm được (có thể gôm mớ này thày find)

MÀN HÌNH SỬA EMPLOYEE (giữ nguyên mnv)
2 cách
C1: gọi các hàm sửa ở trên ...
C2:(CHƯA VIẾT) tạo 1 employee mới để sẵn mnv rồi nhập lại employee đó, xóa nhân viên mã cũ, thêm lại nhân viên mã mới

XÓA EMPLOYEE (giữ nguyên mnv)
tạo 1 cái list gồm các nhân viên cần xóa (có thể tích vào ô bên cạnh tên nhân viên hay gì đó)
bấm xóa thì Xoa...(con trỏ Employee, cv,pb,...)

MÀN HÌNH THÊM EMPLOYEE
tạo new Part or Full r bỏ dô :)

--------------------------------
MÀN HÌNH PHÒNG BAN
làm giống bên Employee đi
cũng in list
bên dưới có thêm, sửa, xóa :)
--------------------------------
lương cũng vậy
-----------------------------
search ???
kiểu in full thông tin mức lương các kiur hmmm ?
*/