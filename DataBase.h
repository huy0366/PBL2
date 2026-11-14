#pragma once
#include "ChucVu.h"
#include "Date.h"
#include "FullTime.h"
#include "PartTime.h"
#include "Employee.h"
#include "Hash.h"
#include "LinkedList.h"
#include "MucLuong.h"
#include "MucLuong_PartTime.h"
#include "MucLuong_FullTime.h"
#include "PhongBan.h"
#include "ViTriViecLam.h"
#include <sstream>
#include <string>
#include <fstream>
class DataBase
{
private:
    List<PhongBan> *ListPB;
    List<ChucVu> *ListCV;
    List<ViTriViecLam> *ListVTVL;
    List<MucLuong> *ListML;
    List<Employee> *ListEmployee;
    Hash<Employee> *HashEmployee;
    int soFT, soPT;

public:
    DataBase();
    ~DataBase();
    int get_soFT() const;
    int get_soPT() const;
    // thêm
    void Insert(PhongBan *, Node<PhongBan> * = nullptr);
    void Insert(ChucVu *, Node<ChucVu> * = nullptr);
    void Insert(ViTriViecLam *, Node<ViTriViecLam> * = nullptr);
    void Insert(MucLuong *, Node<MucLuong> * = nullptr);
    void Insert(Employee *, Node<Employee> * = nullptr);

    // xóa
    void Remove(PhongBan *);
    void Remove(ChucVu *);
    void Remove(ViTriViecLam *);
    void Remove(MucLuong *);
    void Remove(Employee *);

    // tìm
    PhongBan *FindPB(const string &);
    ChucVu *FindCV(const string &);
    ViTriViecLam *FindVTVL(const string &);
    MucLuong *FindML(const string &, const int &, const string &); // mã VT, bậc, loại nv
    Employee *FindEmployee(const string &);

    // sửa
    void Repair(PhongBan *);
    void Repair(ChucVu *);
    void Repair(ViTriViecLam *);
    void Repair(MucLuong *);
    void Repair(Employee *);

    // lấy list
    List<PhongBan> *get_ListPB();
    List<ChucVu> *get_ListCV();
    List<ViTriViecLam> *get_ListVTVL();
    List<MucLuong> *get_ListML();
    List<Employee> *get_ListEmployee();

    // đọc file
    void ReadFilePB(const string & = "data/PhongBan.txt");
    void ReadFileCV(const string & = "data/ChucVu.txt");
    void ReadFileVTVL(const string & = "data/ViTriViecLam.txt");
    void ReadFileML(const string & = "data/MucLuong_FullTime.txt", const string & = "data/MucLuong_PartTime.txt");
    void ReadFileEmployee(const string & = "data/Employee_FullTime.txt", const string & = "data/Employee_PartTime.txt");

    // lưu file
    void SaveFilePB(const string & = "data/PhongBan.txt");
    void SaveFileCV(const string & = "data/ChucVu.txt");
    void SaveFileVTVL(const string & = "data/ViTriViecLam.txt");
    void SaveFileML(const string & = "data/MucLuong_FullTime.txt", const string & = "data/MucLuong_PartTime.txt");
    void SaveFileEmployee(const string & = "data/Employee_FullTime.txt", const string & = "data/Employee_PartTime.txt");

    // đọc all
    void ReadAll();
    // lưu all
    void SaveAll();
};

DataBase::DataBase() : soFT(0), soPT(0)
{
    this->ListPB = new List<PhongBan>();
    this->ListCV = new List<ChucVu>();
    this->ListVTVL = new List<ViTriViecLam>();
    this->ListML = new List<MucLuong>();
    this->ListEmployee = new List<Employee>();
    this->HashEmployee = new Hash<Employee>(101);
}

DataBase::~DataBase()
{
    delete ListPB;
    delete ListCV;
    delete ListVTVL;
    delete ListML;
    delete ListEmployee;
}

int DataBase::get_soFT() const { return soFT; }
int DataBase::get_soPT() const { return soPT; }

// p = null thì insert đầu
void DataBase::Insert(PhongBan *pb, Node<PhongBan> *p)
{
    if (pb != nullptr)
        if (p == nullptr)
            this->ListPB->insert(pb);
        else
            this->ListPB->insert(pb, p);
}
void DataBase::Insert(ChucVu *cv, Node<ChucVu> *p)
{
    if (cv != nullptr)
        if (p == nullptr)
            this->ListCV->insert(cv);
        else
            this->ListCV->insert(cv, p);
}
void DataBase::Insert(ViTriViecLam *vt, Node<ViTriViecLam> *p)
{
    if (vt != nullptr)
        if (p == nullptr)
            this->ListVTVL->insert(vt);
        else
            this->ListVTVL->insert(vt, p);
}
void DataBase::Insert(MucLuong *ml, Node<MucLuong> *p)
{
    if (ml != nullptr)
        if (p == nullptr)
            this->ListML->insert(ml);
        else
            this->ListML->insert(ml, p);
}
void DataBase::Insert(Employee *e, Node<Employee> *p)
{
    if (e != nullptr)
    {
        if (p == nullptr)
            this->ListEmployee->insert(e);
        else
            this->ListEmployee->insert(e, p);
        this->HashEmployee->insert(e->get_mnv(), e);
        if (e->get_LoaiNV() == "FullTime")
            ++soFT;
        else if (e->get_LoaiNV() == "PartTime")
            ++soPT;
        if (e->get_PhongBan() != nullptr)
            e->get_PhongBan()->ThemNV(e);
        if (e->get_ChucVu() != nullptr)
            e->get_ChucVu()->ThemNV(e);
        if (e->get_ViTri() != nullptr)
            e->get_ViTri()->ThemNV(e);
    }
}

void DataBase::Remove(PhongBan *pb)
{
    Node<PhongBan> *p = this->ListPB->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data == pb)
        {
            this->ListPB->remove_item(p);
            break;
        }
        p = p->Next;
    }
}
void DataBase::Remove(ChucVu *cv)
{
    Node<ChucVu> *p = this->ListCV->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data == cv)
        {
            this->ListCV->remove_item(p);
            break;
        }
        p = p->Next;
    }
}
void DataBase::Remove(ViTriViecLam *vt)
{
    Node<ViTriViecLam> *p = this->ListVTVL->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data == vt)
        {
            this->ListVTVL->remove_item(p);
            break;
        }
        p = p->Next;
    }
}
void DataBase::Remove(MucLuong *ml)
{
    Node<MucLuong> *p = this->ListML->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data == ml)
        {
            this->ListML->remove_item(p);
            break;
        }
        p = p->Next;
    }
}
void DataBase::Remove(Employee *e)
{
    if (e == nullptr)
        return;
    this->HashEmployee->erase(e->get_mnv(), FindID);
    if (e->get_LoaiNV() == "FullTime")
        --soFT;
    else if (e->get_LoaiNV() == "PartTime")
        --soPT;
    Node<Employee> *p = this->ListEmployee->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data == e)
        {
            this->ListEmployee->remove_item(p);
            break;
        }
        p = p->Next;
    }
    PhongBan *pb = e->get_PhongBan();
    pb->XoaNV(e);
    ViTriViecLam *vt = e->get_ViTri();
    vt->XoaNV(e);
    ChucVu *cv = e->get_ChucVu();
    cv->XoaNV(e);
}

PhongBan *DataBase::FindPB(const string &ma)
{
    Node<PhongBan> *p = this->ListPB->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr && p->Data->get_MaPhongBan() == ma)
        {
            return p->Data;
        }
        p = p->Next;
    }
    return nullptr;
}
ChucVu *DataBase::FindCV(const string &ma)
{
    Node<ChucVu> *p = this->ListCV->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr && p->Data->get_MaChucVu() == ma)
        {
            return p->Data;
        }
        p = p->Next;
    }
    return nullptr;
}
ViTriViecLam *DataBase::FindVTVL(const string &ma)
{
    Node<ViTriViecLam> *p = this->ListVTVL->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr && p->Data->get_MaVTVL() == ma)
        {
            return p->Data;
        }
        p = p->Next;
    }
    return nullptr;
}
MucLuong *DataBase::FindML(const string &maVT, const int &bac, const string &loaiNV)
{
    if (this->ListML == nullptr)
        return nullptr;

    Node<MucLuong> *p = this->ListML->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr)
        {
            // MucLuong *ml = dynamic_cast<MucLuong *>(p->Data);
            if (p->Data != nullptr)
            {
                if (p->Data->get_Ma_VTVL() == maVT &&
                    p->Data->get_Bac_VTVL() == bac &&
                    p->Data->get_Loai_NV() == loaiNV)
                {
                    return p->Data;
                }
            }
        }
        p = p->Next;
    }
    return nullptr;
}
Employee *DataBase::FindEmployee(const string &ma)
{
    return this->HashEmployee->find(ma, FindID);
}

void DataBase::Repair(PhongBan *pb)
{
    PhongBan *pb_old = this->FindPB(pb->get_MaPhongBan());
    if (pb_old == nullptr)
        return;
    pb_old->set_TenPhongBan(pb->get_TenPhongBan());
    pb_old->set_NgayThanhLap(pb->get_NgayThanhLap());
}
void DataBase::Repair(ChucVu *cv)
{
    ChucVu *cv_old = this->FindCV(cv->get_MaChucVu());
    if (cv_old == nullptr)
        return;
    *cv_old = *cv;
}
void DataBase::Repair(ViTriViecLam *vt)
{
    ViTriViecLam *vt_old = this->FindVTVL(vt->get_MaVTVL());
    if (vt_old == nullptr)
        return;
    *vt_old = *vt;
}
void DataBase::Repair(MucLuong *ml)
{
    MucLuong *ml_old = this->FindML(ml->get_Ma_VTVL(), ml->get_Bac_VTVL(), ml->get_Loai_NV());
    if (ml_old == nullptr)
        return;
    ml_old->set_Luong(ml->get_Luong());
}
void DataBase::Repair(Employee *e)
{
    Employee *e_old = FindEmployee(e->get_mnv());
    if (e_old == nullptr)
        return;
    *e_old = *e;
    // if (e->get_PhongBan() != nullptr)
    //     e->get_PhongBan()->SuaNV(e);
}

List<PhongBan> *DataBase::get_ListPB()
{
    return this->ListPB;
}
List<ChucVu> *DataBase::get_ListCV()
{
    return this->ListCV;
}
List<ViTriViecLam> *DataBase::get_ListVTVL()
{
    return this->ListVTVL;
}
List<MucLuong> *DataBase::get_ListML()
{
    return this->ListML;
}
List<Employee> *DataBase::get_ListEmployee()
{
    return this->ListEmployee;
}

void DataBase::ReadFilePB(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        string maPB, tenPB, ngay;
        stringstream ss(line);
        getline(ss, maPB, '\t');
        getline(ss, tenPB, '\t');
        getline(ss, ngay, '\t');
        Date ngayTL(ngay);
        this->Insert(new PhongBan(maPB, tenPB, ngayTL));
    }
    file.close();
}
void DataBase::ReadFileCV(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        string maCV, tenCV, temp;
        double PC = 0;

        stringstream ss(line);
        ss >> maCV;

        while (ss >> temp)
        {
            if (isdigit((unsigned char)temp[0]))
            {
                PC = stod(temp);
                break;
            }
            if (!tenCV.empty())
                tenCV += " ";
            tenCV += temp;
        }
        this->Insert(new ChucVu(maCV, tenCV, PC));
    }
    file.close();
}
void DataBase::ReadFileVTVL(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
        return;

    string line;
    while (getline(file, line))
    {
        string maVT;
        string tenVT;
        if (line.empty())
            continue;

        stringstream ss(line);

        ss >> maVT;
        ss >> ws;
        getline(ss, tenVT);
        this->Insert(new ViTriViecLam(maVT, tenVT));
    }
    file.close();
}
void DataBase::ReadFileML(const string &filePath1, const string &filePath2)
{
    ifstream file1(filePath1);
    if (!file1.is_open())
        return;

    string line;
    while (getline(file1, line))
    {
        string MaVTVL;
        int bac;
        double luong;
        if (line.empty())
            continue;

        stringstream ss(line);
        ss >> MaVTVL;
        ss >> bac;
        ss >> luong;
        ViTriViecLam *VTVL = FindVTVL(MaVTVL);
        this->Insert(new MucLuong_FullTime(VTVL, bac, luong));
    }
    file1.close();
    ifstream file2(filePath2);
    if (!file2.is_open())
        return;

    while (getline(file2, line))
    {
        string MaVTVL;
        int bac;
        double luong;
        if (line.empty())
            continue;

        stringstream ss(line);
        ss >> MaVTVL;
        ss >> bac;
        ss >> luong;
        ViTriViecLam *VTVL = FindVTVL(MaVTVL);
        this->Insert(new MucLuong_PartTime(VTVL, bac, luong));
    }
    file2.close();
}
void DataBase::ReadFileEmployee(const string &filePath1, const string &filePath2) // 1 full  2part
{
    // full
    ifstream file1(filePath1);
    if (!file1.is_open())
        return;

    string line;
    while (getline(file1, line))
    {
        string mnv, hoTen, ngay1, gioiTinh, maChucVu, maViTri, maPhongBan, ngay2;
        int bac;
        int thuong;
        // fulltime
        int songaycong;
        double heSoLuong = 1.0;
        stringstream ss(line);
        getline(ss, mnv, '\t');
        getline(ss, hoTen, '\t');
        getline(ss, ngay1, '\t');
        getline(ss, gioiTinh, '\t');
        getline(ss, maChucVu, '\t');
        getline(ss, maViTri, '\t');
        getline(ss, maPhongBan, '\t');
        getline(ss, ngay2, '\t');
        ss >> bac >> thuong >> songaycong >> heSoLuong;
        Date ngaySinh(ngay1);
        Date ngayVaoLam(ngay2);
        PhongBan *pb = FindPB(maPhongBan);
        ViTriViecLam *vt = FindVTVL(maViTri);
        ChucVu *cv = FindCV(maChucVu);
        FullTime *e = new FullTime(mnv, hoTen, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac, thuong, songaycong, heSoLuong);
        this->Insert(e);
    }
    file1.close();

    // part
    ifstream file2(filePath2);
    if (!file2.is_open())
        return;

    while (getline(file2, line))
    {
        string mnv, hoTen, ngay1, gioiTinh, maChucVu, maViTri, maPhongBan, ngay2;
        int bac;
        int thuong;
        int tglam;
        stringstream ss(line);
        getline(ss, mnv, '\t');
        getline(ss, hoTen, '\t');
        getline(ss, ngay1, '\t');
        getline(ss, gioiTinh, '\t');
        getline(ss, maChucVu, '\t');
        getline(ss, maViTri, '\t');
        getline(ss, maPhongBan, '\t');
        getline(ss, ngay2, '\t');
        ss >> bac >> thuong >> tglam;

        Date ngaySinh(ngay1);
        Date ngayVaoLam(ngay2);
        PhongBan *pb = FindPB(maPhongBan);
        ViTriViecLam *vt = FindVTVL(maViTri);
        ChucVu *cv = FindCV(maChucVu);
        PartTime *e = new PartTime(mnv, hoTen, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac, thuong, tglam);
        this->Insert(e);
    }
    file2.close();
}

void DataBase::SaveFilePB(const string &filePath)
{
    ofstream file(filePath);
    if (!file.is_open())
    {
        cout << "Khong the ghi file: " << filePath << endl;
        return;
    }
    Node<PhongBan> *p = this->ListPB->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr)
            file << p->Data->get_MaPhongBan() << "\t" << p->Data->get_TenPhongBan() << "\t" << p->Data->get_NgayThanhLap() << "\n";
        p = p->Next;
    }
    file.close();
}
void DataBase::SaveFileCV(const string &filePath)
{
    ofstream file(filePath);
    if (!file.is_open())
    {
        cout << "Khong the ghi file: " << filePath << endl;
        return;
    }
    Node<ChucVu> *p = this->ListCV->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr)
            file << p->Data->get_MaChucVu() << "\t"
                 << p->Data->get_TenChucVu() << "\t"
                 << (long long)p->Data->get_PhuCap() << "\n";
        p = p->Next;
    }
    file.close();
}
void DataBase::SaveFileVTVL(const string &filePath)
{
    ofstream file(filePath);
    if (!file.is_open())
    {
        cout << "Khong the ghi file: " << filePath << endl;
        return;
    }
    Node<ViTriViecLam> *p = ListVTVL->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data != nullptr)
            file << p->Data->get_MaVTVL() << "\t" << p->Data->get_TenVTVL() << "\n";
        p = p->Next;
    }
    file.close();
}
void DataBase::SaveFileML(const string &filePath1, const string &filePath2) // 1 full, 2 part
{
    ofstream file1(filePath1);
    ofstream file2(filePath2);
    if (!file1.is_open() || !file2.is_open())
    {
        cout << "Khong the ghi file muc luong" << endl;
        return;
    }

    Node<MucLuong> *p = ListML->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data->get_Loai_NV() == "FullTime")
        {
            file1 << p->Data->get_Ma_VTVL() << "\t"
                  << p->Data->get_Bac_VTVL() << "\t"
                  << (long long)p->Data->get_Luong() << "\n";
        }
        else // if (p->Data->get_Loai_NV() == "PartTime")
        {
            file2 << p->Data->get_Ma_VTVL() << "\t"
                  << p->Data->get_Bac_VTVL() << "\t"
                  << (long long)p->Data->get_Luong() << "\n";
        }
        p = p->Next;
    }
    file1.close();
    file2.close();
}
void DataBase::SaveFileEmployee(const string &filePath1, const string &filePath2)
{
    ofstream file1(filePath1);
    ofstream file2(filePath2);
    if (!file1.is_open() || !file2.is_open())
    {
        cout << "Khong the ghi file nhan vien" << endl;
        return;
    }
    Node<Employee> *p = ListEmployee->get_header()->Next;
    while (p != nullptr)
    {
        if (p->Data->get_LoaiNV() == "FullTime")
        {
            FullTime *ft = dynamic_cast<FullTime *>(p->Data);
            file1
                << ft->get_mnv() << "\t" << ft->get_name() << "\t"
                << ft->get_NgaySinh().ToString() << "\t" << ft->get_sex() << "\t"
                << ft->get_ChucVu()->get_MaChucVu() << "\t"
                << ft->get_ViTri()->get_MaVTVL() << "\t"
                << ft->get_PhongBan()->get_MaPhongBan() << "\t"
                << ft->get_NgayLamViec().ToString() << "\t"
                << ft->get_BacViTri() << "\t"
                << ft->get_Thuong() << "\t"
                << ft->get_SoNgayCong() << "\t"
                << ft->get_HeSo() << "\n";
        }
        else if (p->Data->get_LoaiNV() == "PartTime")
        {
            PartTime *pt = dynamic_cast<PartTime *>(p->Data);
            file2 << pt->get_mnv() << "\t" << pt->get_name() << "\t"
                  << pt->get_NgaySinh().ToString() << "\t" << pt->get_sex() << "\t"
                  << pt->get_ChucVu()->get_MaChucVu() << "\t"
                  << pt->get_ViTri()->get_MaVTVL() << "\t"
                  << pt->get_PhongBan()->get_MaPhongBan() << "\t"
                  << pt->get_NgayLamViec().ToString() << "\t"
                  << pt->get_BacViTri() << "\t"
                  << pt->get_Thuong() << "\t"
                  << pt->get_TimeWork() << "\n";
        }
        p = p->Next;
    }
    file1.close();
    file2.close();
}

void DataBase::ReadAll()
{
    this->ReadFilePB();
    this->ReadFileCV();
    this->ReadFileVTVL();
    this->ReadFileML();
    this->ReadFileEmployee();
}
void DataBase::SaveAll()
{
    this->SaveFilePB();
    this->SaveFileCV();
    this->SaveFileVTVL();
    this->SaveFileML();
    this->SaveFileEmployee();
}

inline long long FullTime::get_ThucLinh(const long long &lcb)
{
    long long luongOT = 0.0; // lương làm thêm giờ
    int songaycong = 0;
    if (this->SoNgayCong > 26)
    {
        songaycong = this->SoNgayCong - 26;
        luongOT = (songaycong) * ((lcb / 26.0) * 2.0); // lương 1 ngày x2
    }
    long long luongHS = (lcb * this->SoNgayCong - songaycong) / 26 * this->HeSo + this->Thuong + this->CV->get_PhuCap();
    long long luong = luongOT + luongHS;
    return luong;
}

inline long long PartTime::get_ThucLinh(const long long &luonggio)
{
    double luongOT = 0.0; // lương làm thêm giờ
    int timework = 0;
    if (this->Time_Work > 80)
    {
        timework = this->Time_Work - 80;
        luongOT = timework * luonggio * 1.5; // làm vượt thời gian x1.5
    }
    double luongtheogio = (this->Time_Work - timework) * luonggio + this->Thuong + this->CV->get_PhuCap();
    double luong = luongOT + luongtheogio;
    return luong;
}