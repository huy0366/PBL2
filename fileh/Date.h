#pragma once
#include <iostream>
#include <string>
#include <ctime>
using namespace std;
class Date
{
protected:
    int day, month, year;

public:
    Date(const int & = 1, const int & = 1, const int & = 1900);
    Date(const string &);
    static Date DateHienTai();
    static bool ktrayear(const int &);
    static int ktramonth(const int &, const int &);
    static bool ktraday(const int &, const int &, const int &);
    friend istream &operator>>(istream &, Date &);
    friend ostream &operator<<(ostream &, const Date &);
    bool operator>(const Date &) const;
    bool operator<(const Date &) const;
    bool operator==(const Date &) const;
    Date operator-(const Date &);
    string ToString();
    int get_day() const;
    int get_month() const;
    int get_year() const;
    bool empty() const;
    void Show();
    ~Date();
};

Date::Date(const int &day, const int &month, const int &year)
{
    if (ktraday(day, month, year))
    {
        this->day = day;
        this->month = month;
        this->year = year;
    }
    else
    {
        // ngoại lệ
    }
}
Date::Date(const string &s)
{
    this->day = this->month = this->year = 0;
    // string temp;
    // for (int i = 0; i < s.size(); i++)
    // {
    //     if (s[i] != '/')
    //         temp += s[i];
    //     else
    //     {
    //         if (this->day == 0)
    //             this->day = stoi(temp);
    //         else if (this->month == 0)
    //             this->month = stoi(temp);

    //         temp = "";
    //     }
    // }
    // this->year = stoi(temp);
    sscanf(s.c_str(), "%d/%d/%d", &day, &month, &year);
}
Date Date::DateHienTai()
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    int currentDay = now->tm_mday;
    int currentMonth = now->tm_mon + 1;
    int currentYear = now->tm_year + 1900;
    return Date(currentDay, currentMonth, currentYear);
}
bool Date::ktrayear(const int &year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}
int Date::ktramonth(const int &month, const int &year)
{
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;

    case 4:
    case 6:
    case 9:
    case 11:
        return 30;

    case 2:
        return ktrayear(year) ? 29 : 28;

    default:
        return 0;
    }
}
bool Date::ktraday(const int &day, const int &month, const int &year)
{
    if (year < 1 || month < 1 || month > 12)
        return false;

    return day >= 1 && day <= ktramonth(month, year);
}

istream &operator>>(istream &in, Date &d)
{
    in >> d.day >> d.month >> d.year;
    return in;
}
ostream &operator<<(ostream &o, const Date &d)
{
    o << d.day << "/" << d.month << "/" << d.year;
    return o;
}
bool Date::operator>(const Date &d) const
{
    if (this->year != d.year)
        return this->year > d.year;
    if (this->month != d.month)
        return this->month > d.month;
    return this->day > d.day;
}
bool Date::operator<(const Date &d) const
{
    if (this->year != d.year)
        return this->year < d.year;
    if (this->month != d.month)
        return this->month < d.month;
    return this->day < d.day;
}
bool Date::operator==(const Date &d) const
{
    return this->year == d.year && this->month == d.month && this->day == d.day;
}
Date Date::operator-(const Date &d)
{
    Date d1 = *this;
    Date d2 = d;
    if (d1 < d2)
    {
        Date temp = d1;
        d1 = d2;
        d2 = temp;
    }
    int resDay = d1.day - d2.day;
    int resMonth = d1.month - d2.month;
    int resYear = d1.year - d2.year;

    if (resDay < 0)
    {
        resMonth--;

        // Lấy số ngày của tháng trước tháng hiện tại của d1
        int prevMonth = d1.month - 1;
        int prevYear = d1.year;

        if (prevMonth < 1)
        {
            prevMonth = 12;
            prevYear--;
        }

        resDay += ktramonth(prevMonth, prevYear);
    }

    if (resMonth < 0)
    {
        resYear--;
        resMonth += 12;
    }
    return Date(resDay, resMonth, resYear);
}

string Date::ToString()
{
    string s = "";
    s += to_string(this->day) + '/' + to_string(this->month) + '/' + to_string(this->year);
    return s;
}
int Date::get_day() const
{
    return this->day;
}
int Date::get_month() const
{
    return this->month;
}
int Date::get_year() const
{
    return this->year;
}
bool Date::empty() const
{
    return !ktraday(this->day, this->month, this->year);
}
void Date::Show()
{
    cout << this->day << '/' << this->month << '/' << this->year;
}
Date::~Date()
{
}