#include "raylib.h"
#include <ctime>
#include "QLNV.h"
#include <bits/stdc++.h>
using namespace std;
Font myFont, tieude;
string FormatSoTien(long long so);
void HienThongBao(const string &noiDung, bool laLoi = true);
void VeThongBao();
void DrawSidebar(int &currentScreen);
void DrawMainContent(int &currentScreen, Texture2D &BackGround, QLNV *QL);
void Draw_Screen_Home(Texture2D &BackGround);
void Draw_Screen_NhanVien(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_VTVL(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_Employee(QLNV *QL);
void Draw_Screen_PhongBan(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_ChucVu(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_Deparment(QLNV *QL);
bool DrawCalendar(Vector2 pos, int &selDay, int &month, int &year, Rectangle &Calendar, bool &isOpenCalendar);
void Draw_Screen_BangLuong(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_MucLuong(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_FullTime(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_PartTime(Vector2 mousePos, bool Clicked, QLNV *QL);
void Draw_Screen_Salary(QLNV *QL); // 2 cái duyệt cho nhau
// void Draw_Screen_Search(QLNV *QL);
void HieuUngGo(bool &active, Rectangle &btn, char input[], int fSize = 28);
int GetDaysInMonth(const int &m, const int &y);
int GetStartDay(const int &m, const int &y);

time_t now = time(0); // lấy thời gian thực tế
tm *t = localtime(&now);
int day = t->tm_mday;
int month = t->tm_mon + 1;
int year = t->tm_year + 1900;
// ================= HỆ THỐNG THÔNG BÁO GÓC TRÊN BÊN PHẢI =================
string tbNoiDung = "";
bool tbLaLoi = true;     // true: Báo lỗi (Đỏ), false: Thành công (Xanh lá)
bool tbDangHien = false; // Trạng thái đang bật hay tắt
float tbThoiGian = 0.0f; // Đếm thời gian hiển thị
float tbX = 2000.0f;     // Tọa độ X hiện tại của hộp thông báo

enum Screen
{
    SCREEN_HOME,       // Trang Chủ
    SCREEN_EMPLOYEE,   // Nhân viên
    SCREEN_DEPARTMENT, // Bộ phận
    SCREEN_SALARY,     // Bảng lương
    SCREEN_Search,     // Hệ thống
    Exit,              // Thoát hệ thống
};

int main()
{
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1792, 1008, "PBL2");
    SetTargetFPS(60);
    // lấy ảnh
    Texture2D BackGround = LoadTexture("BackGround.png");

    int codepoints[8000];
    for (int i = 0; i < 8000; i++)
        codepoints[i] = 32 + i;
    myFont = LoadFontEx("font/times.ttf", 32, codepoints, 8000);
    tieude = LoadFontEx("font/timesbd.ttf", 32, codepoints, 8000);
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);
    QLNV *QL = new QLNV;

    int currentScreen = SCREEN_HOME;

    while (!WindowShouldClose())
    {
        if (currentScreen == Exit)
        {
            break;
        }
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawSidebar(currentScreen);
        DrawMainContent(currentScreen, BackGround, QL);
        VeThongBao();
        EndDrawing();
    }
    UnloadTexture(BackGround);
    CloseWindow();
}
string FormatSoTien(long long so)
{
    if (so < 0)
        return "-" + FormatSoTien(-so);
    string s = to_string(so);
    string kq = "";
    int dem = 0;
    for (int i = s.size() - 1; i >= 0; i--)
    {
        kq = s[i] + kq;
        dem++;
        if (dem % 3 == 0 && i > 0)
            kq = "." + kq;
    }
    return kq;
}
// 1. Gọi hàm này mỗi khi muốn bật thông báo
void HienThongBao(const string &noiDung, bool laLoi)
{
    tbNoiDung = noiDung;
    tbLaLoi = laLoi;
    tbDangHien = true;
    tbThoiGian = 0.0f;
    tbX = (float)GetScreenWidth() + 20.0f; // Bắt đầu ở ngoài lề phải màn hình
}

// 2. Gọi hàm này ở cuối vòng lặp main() để vẽ và chạy hiệu ứng trượt
void VeThongBao()
{
    if (!tbDangHien)
        return;

    float dt = GetFrameTime(); // Thời gian trôi qua giữa 2 khung hình
    tbThoiGian += dt;

    // Tự động co giãn chiều rộng hộp theo độ dài chữ (tối thiểu 360px)
    float textW = MeasureTextEx(myFont, tbNoiDung.c_str(), 22, 1).x;
    float boxW = (textW + 65.0f > 360.0f) ? (textW + 65.0f) : 360.0f;
    float boxH = 72.0f;
    float boxY = 25.0f; // Cách mép trên 25px

    // Tọa độ đích: Trong 3.5 giây đầu trượt ra góc phải, sau 3.5 giây trượt về lại bên phải
    float dichX;
    if (tbThoiGian < 3.5f)
        dichX = (float)GetScreenWidth() - boxW - 25.0f; // Vị trí dừng (cách lề phải 25px)
    else
        dichX = (float)GetScreenWidth() + 30.0f; // Vị trí ẩn (ngoài màn hình)

    // Công thức trượt mượt (tự động giảm tốc khi gần tới đích)
    tbX += (dichX - tbX) * 10.0f * dt;

    // Khi đã hết giờ và hộp đã trượt hẳn ra ngoài màn hình -> Tắt thông báo
    if (tbThoiGian >= 3.5f && tbX >= (float)GetScreenWidth() + 5.0f)
    {
        tbDangHien = false;
        return;
    }

    // Vẽ bóng mờ & nền trắng của hộp thông báo
    Rectangle box = {tbX, boxY, boxW, boxH};
    DrawRectangleRounded({box.x + 3, box.y + 4, box.width, box.height}, 0.2f, 8, Fade(BLACK, 0.15f));
    DrawRectangleRounded(box, 0.2f, 8, WHITE);

    // Màu chủ đạo: Đỏ nếu là lỗi, Xanh lá nếu thành công
    Color mauChuDao = tbLaLoi ? Color{230, 55, 60, 255} : Color{34, 190, 90, 255};
    DrawRectangleRoundedLinesEx(box, 0.2f, 8, 1.5f, mauChuDao);

    // Vẽ thanh điểm nhấn bên trái hộp
    DrawRectangleRounded({box.x + 8, box.y + 10, 6, box.height - 20}, 0.5f, 4, mauChuDao);

    // Vẽ tiêu đề và nội dung thông báo
    const char *tieuDeTB = tbLaLoi ? "Thông báo lỗi" : "Thành công";
    DrawTextEx(tieude, tieuDeTB, {box.x + 26, box.y + 10}, 22, 1, mauChuDao);
    DrawTextEx(myFont, tbNoiDung.c_str(), {box.x + 26, box.y + 38}, 22, 1, Color{40, 45, 55, 255});
}
void DrawSidebar(int &currentScreen)
{
    static bool isLoaded = false;
    static Texture2D anhmenu;
    static Texture2D a21;
    static Texture2D a2;
    static Texture2D a7;
    static Texture2D iconmenu[6];
    if (!isLoaded)
    {
        anhmenu = LoadTexture("photo/anhto/Asset-3.png");
        a21 = LoadTexture("photo/icon/Asset-21.png");
        a2 = LoadTexture("photo/anhto/Asset-2.png");
        a7 = LoadTexture("photo/anhto/Asset-7.png");
        iconmenu[0] = LoadTexture("photo/icon/Asset-6.png");
        iconmenu[1] = LoadTexture("photo/icon/Asset-7.png");
        iconmenu[2] = LoadTexture("photo/icon/Asset-8.png");
        iconmenu[3] = LoadTexture("photo/icon/Asset-9.png");
        iconmenu[4] = LoadTexture("photo/icon/Asset-10.png");
        iconmenu[5] = LoadTexture("photo/icon/Asset-11.png");
        isLoaded = true; // Đánh dấu đã nạp xong
    }
    Vector2 panhmenu = {-80.0f, -47.0f}; // Tọa độ (x, y) postion
    float ranhmenu = 0.0f;               // Góc xoay (0 độ) rotation
    float sanhmenu = 1.96f;              // Tỉ lệ phóng to (2.0f = gấp đôi kích thước gốc)scale
    Vector2 pa21 = {40.0f, 70.0f};
    float ra21 = 0.0f;
    float sa21 = 0.2f;
    Vector2 pa2 = {30.0f, 650.0f};
    float ra2 = 0.0f;
    float sa2 = 0.54f;
    Vector2 pa7 = {30.0f, 650.0f};
    float ra7 = 0.0f;
    float sa7 = 0.54f;
    DrawTextureEx(anhmenu, panhmenu, ranhmenu, sanhmenu, WHITE);
    DrawTextureEx(a21, pa21, ra21, sa21, WHITE);
    DrawTextEx(tieude, "PBL2", {130.f, 65.f}, 35, 1, BLACK);
    DrawTextEx(myFont, "Quản lí nhân sự", {130.f, 100.f}, 25, 1, BLACK);
    DrawTextureEx(a2, pa2, ra2, sa2, WHITE);
    const char *menu[] = {"HOME", "EMPLOYEE", "DEPARTMENT", "SALARY", "SEARCH", "Exit"};
    int startX = 90;  // Vị trí X bắt đầu lề trái
    int startY = 220; // Vị trí Y của mục đầu tiên
    int spacing = 60;
    int sizeword = 30;

    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < 6; i++)
    {
        int currentY = startY + (i * spacing);

        Rectangle button_Rect{(float)startX - 10, (float)currentY - 5, 180, 35};

        bool Hover = CheckCollisionPointRec(mousePos, button_Rect);

        if (Hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            currentScreen = i;
        }

        if (Hover || (i == currentScreen))
        {
            // Vẽ nền highlight (ví dụ nền bo góc hoặc hình chữ nhật màu sáng)
            DrawRectangleRounded(button_Rect, 0.3f, 4, Fade(PURPLE, 0.2f));

            // Vẽ thanh vạch chỉ báo bên trái cho mục đang được highlight
            DrawRectangle((int)button_Rect.x, (int)button_Rect.y, 4, (int)button_Rect.height, PURPLE);

            // Vẽ chữ
            DrawTextureEx(iconmenu[i], {(float)(startX - 70), (float)currentY - 10}, 0, 0.9, WHITE);
            DrawTextEx(myFont, menu[i], {(float)(startX + 10), (float)currentY}, sizeword, 1, BLACK);
        }
        else
        {
            DrawTextureEx(iconmenu[i], {(float)(startX - 70), (float)currentY - 10}, 0, 0.9, WHITE);
            DrawTextEx(myFont, menu[i], {(float)(startX + 10), (float)currentY}, sizeword, 1, BLACK);
            DrawTextureEx(a7, {290.f, -20.f}, 0, 1.25, WHITE);
        }
    }
}
void DrawMainContent(int &currentScreen, Texture2D &BackGround, QLNV *QL)
{

    if (currentScreen == 0)
    {
        Draw_Screen_Home(BackGround);
    }
    else if (currentScreen == 1)
    {
        Draw_Screen_Employee(QL);
    }
    else if (currentScreen == 2)
    {
        Draw_Screen_Deparment(QL);
    }
    else if (currentScreen == 3)
    {
        Draw_Screen_Salary(QL);
    }
    else if (currentScreen == 4)
    {
        // Draw_Screen_Search(QL);
    }
    else
    {
        WindowShouldClose();
    }
}

void HieuUngGo(bool &active, Rectangle &btn, char input[], int fSize)
{
    float yOffset = btn.y + (btn.height - fSize) / 2.0f;
    if (active && ((int)(GetTime() * 2) % 2 == 0))
    {
        float textWidth = MeasureTextEx(myFont, input, fSize, 1).x;
        DrawTextEx(myFont, "_", {btn.x + 10 + textWidth, yOffset}, fSize, 1, MAROON);
    }
    DrawTextEx(myFont, input, {btn.x + 10, yOffset}, fSize, 1, BLACK);
}
// 1. Hàm tính số ngày trong tháng
int GetDaysInMonth(const int &m, const int &y)
{
    if (m == 2)
        return ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) ? 29 : 28;
    return (m == 4 || m == 6 || m == 9 || m == 11) ? 30 : 31;
}

// 2. Hàm lấy thứ của ngày mùng 1 (0 = Thứ 2, ..., 6 = Chủ nhật)
int GetStartDay(const int &m, const int &y)
{
    tm t = {0};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = 1;
    mktime(&t);
    return (t.tm_wday + 6) % 7;
}

// 3. Hàm vẽ & xử lý Lịch đơn giản (Gọi trực tiếp trong vòng lặp game)
bool DrawCalendar(Vector2 mousepos, int &selDay, int &month, int &year, Rectangle &Calendar, bool &isOpenCalendar)
{
    bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool change = false; // check xem có thay đổi ko
    static bool Year = false;
    Rectangle btnCalendar = {
        Calendar.x + 60,
        Calendar.y + 50,
        100,
        25,
    };

    // Khung nền & viền
    Rectangle Pattern = {Calendar.x, Calendar.y + 30.0f, 250.0f, 240.0f};
    DrawRectangle(Calendar.x, Calendar.y + 30, 250, 240, RAYWHITE);
    DrawRectangleLines(Calendar.x, Calendar.y + 30, 250, 240, DARKGRAY);
    DrawRectangle(btnCalendar.x, btnCalendar.y, btnCalendar.width, btnCalendar.height, RAYWHITE);

    if ((!CheckCollisionPointRec(mousepos, Pattern) && !CheckCollisionPointRec(mousepos, Calendar)) && clicked)
    {
        Year = false;
        isOpenCalendar = false;
    }

    // Header: Tiêu đề Tháng/Năm
    if (CheckCollisionPointRec(mousepos, btnCalendar) && clicked)
    {
        Year = !Year;
    }
    if (Year)
    {
        DrawTextEx(myFont, TextFormat("Nam %d", year), {Calendar.x + 85, Calendar.y + 50}, 18, 1, BLACK);
        // Biến này giúp lùi/tiến danh sách năm mà không làm thay đổi biến year gốc cho đến khi user thực sự click chọn
        static int yearOffset = 0;
        // ---------------- MÀN HÌNH CHỌN NĂM ----------------
        // Nút lùi 12 năm (<<)
        Rectangle btnPrevYear = {Calendar.x + 10, Calendar.y + 50, 30, 22};
        DrawRectangleRec(btnPrevYear, WHITE);
        DrawTextEx(myFont, "<<", {btnPrevYear.x + 8, btnPrevYear.y + 3}, 16, 1, BLACK);
        if (clicked && CheckCollisionPointRec(mousepos, btnPrevYear))
        {
            yearOffset -= 12;
        }

        // Nút tiến 12 năm (>>)
        Rectangle btnNextYear = {Calendar.x + 210, Calendar.y + 50, 30, 22};
        DrawRectangleRec(btnNextYear, WHITE);
        DrawTextEx(myFont, ">>", {btnNextYear.x + 8, btnNextYear.y + 3}, 16, 1, BLACK);
        if (clicked && CheckCollisionPointRec(mousepos, btnNextYear))
        {
            yearOffset += 12;
        }

        // Vẽ Lưới các năm (3 hàng x 4 cột = 12 năm)
        // Hiển thị dải năm xung quanh năm hiện tại
        int startDisplayYear = year - 5 + yearOffset;

        for (int i = 0; i < 12; i++)
        {
            int col = i % 4; // 4 cột
            int row = i / 4; // 3 hàng
            int displayYear = startDisplayYear + i;

            // Tính toán tọa độ từng ô chứa năm
            Rectangle cellYear = {Calendar.x + 15 + col * 55, Calendar.y + 90 + row * 45, 50, 30};

            // Đổi màu nền: Năm đang được chọn (Xanh), Di chuột qua (Xám)
            if (displayYear == year)
                DrawRectangleRec(cellYear, SKYBLUE);
            else if (CheckCollisionPointRec(mousepos, cellYear))
                DrawRectangleRec(cellYear, LIGHTGRAY);

            // In số năm
            DrawTextEx(myFont, TextFormat("%d", displayYear), {cellYear.x + 5, cellYear.y + 8}, 16, 1, BLACK);

            // Xử lý khi click chọn một năm
            if (clicked && CheckCollisionPointRec(mousepos, cellYear))
            {
                year = displayYear; // Cập nhật biến year chính thức của bạn
                Year = false;       // Tắt giao diện chọn năm, quay về giao diện tháng
                yearOffset = 0;     // Reset lại offset
                change = true;
            }
        }
    }
    else
    {
        DrawTextEx(myFont, TextFormat("Thang %d / %d", month, year), {Calendar.x + 45, Calendar.y + 50}, 18, 1, BLACK);
        Rectangle btnPrev = {Calendar.x + 10, Calendar.y + 50, 30, 22};
        DrawRectangleRec(btnPrev, WHITE);
        // Nút lùi tháng(<)
        DrawTextEx(myFont, "<", {btnPrev.x + 10, btnPrev.y + 3}, 16, 1, BLACK);
        if (clicked && CheckCollisionPointRec(mousepos, btnPrev))
        {
            if (--month < 1)
            {
                month = 12;
                year--;
            }
        }

        // Nút tiến tháng (>)
        Rectangle btnNext = {Calendar.x + 210, Calendar.y + 50, 30, 22};
        DrawRectangleRec(btnNext, WHITE);
        DrawTextEx(myFont, ">", {btnNext.x + 10, btnNext.y + 3}, 16, 1, BLACK);
        if (clicked && CheckCollisionPointRec(mousepos, btnNext))
        {
            if (++month > 12)
            {
                month = 1;
                year++;
            }
        }

        // Hàng tiêu đề thứ trong tuần
        const char *days[] = {"T2", "T3", "T4", "T5", "T6", "T7", "CN"};
        for (int i = 0; i < 7; i++)
        {
            DrawTextEx(myFont, days[i], {Calendar.x + 12 + i * 34, Calendar.y + 80}, 14, 1, DARKGRAY);
        }

        // Vẽ các ô ngày trong tháng
        int startOffset = GetStartDay(month, year);
        int totalDays = GetDaysInMonth(month, year);

        for (int d = 1; d <= totalDays; d++)
        {
            int idx = d - 1 + startOffset;
            int col = idx % 7;
            int row = idx / 7;

            Rectangle cell = {Calendar.x + 8 + col * 34, Calendar.y + 100 + row * 28, 30, 24};

            // Đổi màu nền: Ngày được chọn (Xanh), Di chuột qua (Xám nhẹ)
            if (d == selDay)
                DrawRectangleRec(cell, SKYBLUE);
            else if (CheckCollisionPointRec(mousepos, cell))
                DrawRectangleRec(cell, LIGHTGRAY);

            // In số ngày
            DrawTextEx(myFont, TextFormat("%d", d), {cell.x + (d < 10 ? 10 : 6), cell.y + 4}, 14, 1, BLACK);

            // Click chọn ngày
            if (clicked && CheckCollisionPointRec(mousepos, cell))
            {
                change = true;
                selDay = d;
                isOpenCalendar = false;
            }
        }
    }
    return change;
}
void Typing(bool &isActive, char *Input, int &letterCount, Vector2 &mousePos, Rectangle vung_go)
{
    // Kiểm tra click chuột
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Nếu click vào vùng textHoTen thì kích hoạt, ngược lại thì tắt
        if (CheckCollisionPointRec(mousePos, vung_go))
        {
            isActive = true;
        }
        else
        {
            isActive = false;
        }
    }

    // Xử lý gõ phím khi ô Họ Tên đang được kích hoạt
    if (isActive)
    {
        int key = GetCharPressed();
        // Bắt các ký tự (hỗ trợ bảng mã ASCII cơ bản)
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (letterCount < 49))
            {
                Input[letterCount] = (char)key;
                Input[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed(); // Lấy ký tự tiếp theo trong hàng đợi
        }
        // Xử lý nút Xóa (Backspace)
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0)
                letterCount = 0;
            Input[letterCount] = '\0';
        }
    }
}
bool CheckName(const string &input, const string &name)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (tolower(input[i]) != tolower(name[i]))
        {
            return false;
        }
    }
    return true;
}

// maCV / maPB rỗng = không lọc theo tiêu chí đó
bool CheckEmployee(Employee *emp, const string &maCV, const string &maPB, const string &keyword)
{
    if (!maCV.empty() && emp->get_MaChucVu() != maCV)
        return false;

    if (!maPB.empty() && emp->get_MaPhongBan() != maPB)
        return false;

    // tìm chuỗi con, không phân biệt hoa thường
    if (!CheckName(keyword, emp->get_name()))
    {
        return false;
    }

    return true;
}
void Draw_Screen_Home(Texture2D &BackGround)
{
    // 1. Khung nguồn: Lấy toàn bộ kích thước gốc của file ảnh
    Rectangle sourceRec = {
        0.0f,
        0.0f,
        (float)BackGround.width,
        (float)BackGround.height};

    // 2. Khung đích: Tọa độ và kích thước hiển thị trên màn hình
    // - x = 220.0f  (Nằm ngay sau thanh Sidebar)
    // - y = 0.0f    (Sát mép trên)
    // - width = GetScreenWidth() - 220 (Chiều rộng vùng bên phải)
    // - height = GetScreenHeight()     (Toàn bộ chiều cao)
    float sidebarWidth = 250.0f;

    Rectangle destRec = {
        sidebarWidth,
        0.0f,
        (float)GetScreenWidth() - sidebarWidth,
        (float)GetScreenHeight()};

    // 3. Tâm xoay
    Vector2 origin = {0.0f, 0.0f};

    // 4. Vẽ ảnh co giãn vừa vặn vào khung destRec
    DrawTexturePro(BackGround, sourceRec, destRec, origin, 0.0f, WHITE);
}
// Xử lý click (giống đoạn if (Clicked) {...} của bạn)
// Trả về true nếu click này đã được combo x    ử lý
void XuLyCombo(Rectangle combo, bool &isOpen, int &selected, int count,
               Vector2 mousePos, bool Clicked)
{
    if (!Clicked)
        return;

    if (isOpen)
    {
        bool clickedInsideList = false;
        for (int i = 0; i < count; i++)
        {
            Rectangle itemRect = {combo.x, combo.y + combo.height + (i * combo.height), combo.width, combo.height};
            if (CheckCollisionPointRec(mousePos, itemRect))
            {
                selected = i;
                isOpen = false;
                clickedInsideList = true;
                break;
            }
        }
        isOpen = false; // click ra ngoài -> đóng
        if (!clickedInsideList && !CheckCollisionPointRec(mousePos, combo))
        {
            isOpen = false;
        }
    }
    else if (CheckCollisionPointRec(mousePos, combo))
    {
        isOpen = true;
    }
}

// Vẽ ô + chữ đã chọn + tam giác (giống phần "Vẽ box" + "Vẽ tam giác")
void VeCombo(Rectangle combo, int selected, string items[], int count)
{
    DrawRectangleRounded(combo, 0.2f, 10, WHITE);
    if (selected >= 0 && selected < count)
    {
        DrawTextEx(myFont, items[selected].c_str(), {combo.x + 5, combo.y + 5}, 18, 1, DARKGRAY);
    }
    Vector2 p1 = {combo.x + combo.width - 15, combo.y + 12};
    Vector2 p2 = {combo.x + combo.width - 5, combo.y + 12};
    Vector2 p3 = {combo.x + combo.width - 10, combo.y + 20};
    DrawTriangle(p1, p3, p2, DARKGRAY);
}

// Vẽ bảng xổ xuống (giống đoạn if (isChucVuOpen) {...}), gọi CUỐI CÙNG
void VeDanhSachCombo(Rectangle combo, bool isOpen, string items[], int count, Vector2 mousePos, int wordsize)
{
    Vector2 p1 = {combo.x + combo.width - 15, combo.y + 12}; // Góc trên-trái
    Vector2 p2 = {combo.x + combo.width - 5, combo.y + 12};  // Góc trên-phải
    Vector2 p3 = {combo.x + combo.width - 10, combo.y + 20}; // Đỉnh dưới
    bool isHoverCombo = CheckCollisionPointRec(mousePos, combo);
    Color borderColor = Color{205, 215, 230, 255};
    if (isOpen)
    {
        borderColor = Color{98, 70, 234, 255}; // Khi đang mở menu: viền tím nổi bật
    }
    else if (isHoverCombo)
    {
        borderColor = Color{140, 165, 210, 255}; // Khi rê chuột vào: viền xanh nhạt
    }

    DrawRectangleRounded(combo, 0.35f, 8, WHITE); // Đổ nền trắng trước
    DrawRectangleRoundedLinesEx(combo, 0.35f, 8, 1.5f, borderColor);
    DrawTriangle(p1, p3, p2, DARKGRAY);
    if (!isOpen || count <= 0)
        return;

    // Vẽ viền bo tròn
    float itemHeight = combo.height;              // Chiều cao mỗi mục
    float startY = combo.y + combo.height + 3.0f; // Bắt đầu ngay dưới đáy hộp combo (cách 3px)
    float totalHeight = count * itemHeight;

    Rectangle menuBox = {combo.x, startY, combo.width, totalHeight};

    // 1. ĐỔ BÓNG MỜ NHẸ (Giúp bảng menu nổi bật hẳn lên trên nền)
    DrawRectangleRounded({menuBox.x + 2, menuBox.y + 3, menuBox.width, menuBox.height}, 0.15f, 6, Fade(BLACK, 0.08f));

    // 2. VẼ HỘP TRẮNG BO GÓC CHỨA TOÀN BỘ DANH SÁCH
    DrawRectangleRounded(menuBox, 0.15f, 6, WHITE);
    DrawRectangleRoundedLinesEx(menuBox, 0.15f, 6, 1.2f, Color{210, 218, 230, 255}); // Viền xám xanh nhẹ

    // 3. VẼ TỪNG MỤC BÊN TRONG
    for (int i = 0; i < count; i++)
    {
        Rectangle itemRect = {combo.x, startY + (i * itemHeight), combo.width, itemHeight};
        bool isHover = CheckCollisionPointRec(mousePos, itemRect);

        // Hiệu ứng rê chuột (Hover): Đổi sang màu tím nhạt thanh lịch
        if (isHover)
        {
            DrawRectangleRounded(itemRect, 0.12f, 4, Color{242, 240, 255, 255});
        }

        // Chữ: Khi hover đổi màu tím thương hiệu, bình thường màu xám đen rõ nét
        Color textColor = isHover ? Color{98, 70, 234, 255} : Color{50, 50, 60, 255};
        DrawTextEx(myFont, items[i].c_str(), {itemRect.x + 14, itemRect.y + 6}, wordsize, 1, textColor);

        // Vạch phân cách mỏng giữa các hàng (trừ hàng cuối)
        if (i < count - 1)
        {
            DrawLine(itemRect.x + 8, itemRect.y + itemHeight, itemRect.x + itemRect.width - 8, itemRect.y + itemHeight, Color{240, 243, 248, 255});
        }
    }
}

int TimIndex(string ds[], int n, const string &key)
{
    for (int i = 0; i < n; i++)
    {
        if (ds[i] == key)
            return i;
    }
    return -1;
}
void VeOInput(Rectangle r, const char *input, bool active, bool readOnly, const char *placeholder)
{
    Color nen = readOnly ? Color{241, 244, 249, 255} : WHITE;
    Color vien = readOnly ? nen : (active ? Color{98, 70, 234, 255} : Color{225, 230, 240, 255});
    DrawRectangleRounded(r, 0.25f, 8, nen);
    DrawRectangleRoundedLinesEx(r, 0.25f, 8, 1.5f, vien);
    if (input[0] == '\0')
        DrawTextEx(myFont, placeholder, {r.x + 16, r.y + 11}, 24, 1, Color{150, 158, 170, 255});
    else
        DrawTextEx(myFont, input, {r.x + 16, r.y + 11}, 24, 1, Color{31, 41, 55, 255});
    if (active && ((int)(GetTime() * 2) % 2 == 0))
    {
        float w = MeasureTextEx(myFont, input, 24, 1).x;
        DrawRectangle(r.x + 18 + w, r.y + 10, 2, 27, Color{98, 70, 234, 255});
    }
}

void VeODate(Rectangle r, const char *text, bool open, Vector2 mousePos, const char *placeholder)
{
    bool hover = CheckCollisionPointRec(mousePos, r);
    Color vien = open ? Color{98, 70, 234, 255} : (hover ? Color{140, 165, 210, 255} : Color{225, 230, 240, 255});
    DrawRectangleRounded(r, 0.25f, 8, WHITE);
    DrawRectangleRoundedLinesEx(r, 0.25f, 8, 1.5f, vien);
    if (text[0] == '\0')
        DrawTextEx(myFont, placeholder, {r.x + 16, r.y + 11}, 24, 1, Color{107, 114, 128, 255});
    else
        DrawTextEx(myFont, text, {r.x + 16, r.y + 11}, 24, 1, Color{31, 41, 55, 255});
    float ix = r.x + r.width - 34, iy = r.y + 14;
    DrawRectangleLinesEx({ix, iy + 2, 18, 17}, 1.8f, DARKGRAY);
    DrawLineEx({ix, iy + 8}, {ix + 18, iy + 8}, 1.8f, DARKGRAY);
    DrawLineEx({ix + 5, iy}, {ix + 5, iy + 4}, 1.8f, DARKGRAY);
    DrawLineEx({ix + 13, iy}, {ix + 13, iy + 4}, 1.8f, DARKGRAY);
}

void Draw_Screen_NhanVien(Vector2 mousePos, bool Clicked, QLNV *QL)
{
    static bool clickThem = false;
    static bool clickSua = false;
    static bool isOpenDelete = false;
    static bool isOpenTable = false;
    static bool needFill = false;
    int n = QL->get_ListEmployee()->get_size();
    static int *dsXoa = new int[n]();
    static int sizeXoa = 0;
    static string selectedMNV = "";
    // MeNu

    Rectangle comboChucVu = {350, 290, 250, 45};
    static bool isChucVuOpen = false;
    static int selectedChucVuIndex = 0;
    string default_choice = "Tất cả";

    Rectangle comboPhong = {620, 290, 250, 45};
    static bool isPhongOpen = false;
    static int selectedPhongIndex = 0;

    Rectangle textHoTen = {890, 290, 350, 45};
    // Các biến cho ô nhập liệu Họ Tên
    static bool isHoTenActive = false; // Trạng thái: ô có đang được click vào không
    static char hoTenInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount = 0;

    Rectangle comboSortTT = {1560, 290, 200, 45}; // thứ tự (a->z, z->a, tuổi ...)
    static bool isSortTTOpen = false;
    static int selectedSortTTIndex = 0;

    // 1. TẠO BIẾN CUỘN (Dùng 'static' để giữ nguyên giá trị qua mỗi khung hình)
    static float scrollY = 0.0f;
    float scrollSpeed = 30.0f; // Tốc độ cuộn mỗi nấc chuột

    // Lấy thao tác lăn chuột và cộng dồn vào scrollY
    float wheel = GetMouseWheelMove(); // Lăn chuột xuống trả về -1 ngược lại lên
    if (wheel != 0.0f)
    {
        scrollY += wheel * scrollSpeed;
    }

    // Chặn cuộn quá đà lên trên (danh sách không bị tụt xuống dưới tiêu đề)
    if (scrollY > 0.0f)
        scrollY = 0.0f;
    // Cấu hình khoảng cách và kích thước chữ
    float startX = 350.f; // Lề trái
    float startY = 400.f; // Lề trên
    int rowHeight = 60;   // Chiều cao mỗi dòng
    int fontSize = 30;    // Kích thước chữ

    // Tọa độ X của từng cột (Bạn cần tinh chỉnh các số này cho vừa với màn hình)
    float colX[] = {350.f, 470.f, 790.f, 930.f, 1055.f, 1290.f, 1490.f, 1700.f};

    // 1. VẼ TIÊU ĐỀ CỘT (Header)

    float lech = isOpenDelete ? 30.f : 0.f;
    DrawTextEx(myFont, "Mã NV", {colX[0] + lech, startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Họ và tên", {colX[1] + lech, startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Ngày sinh", {colX[2] + lech, startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Giới tính", {colX[3] + lech, startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Mã phòng", {colX[4] + lech, startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Chức vụ", {colX[5] + lech, startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Vị trí", {colX[6] + lech, startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Bậc", {colX[7] + lech, startY}, fontSize, 1, DARKGRAY);
    // ... (Thêm các tiêu đề khác tương tự)

    // Kẻ đường gạch ngang dưới tiêu đề
    DrawLine(startX, startY + 30, 1900, startY + 30, LIGHTGRAY);

    // 4. KHU VỰC VẼ DANH SÁCH BỊ CẮT (Scissor Mode)
    int listStartY = startY + 60;                         // Điểm bắt đầu vẽ dữ liệu (ngay dưới đường kẻ)
    int listHeight = GetScreenHeight() - listStartY - 80; // Chiều cao khu vực hiển thị dữ liệu

    // Bật chế độ Scissor: Bất cứ thứ gì vẽ bên ngoài hình chữ nhật này sẽ bị ẩn đi

    Rectangle listArea = {(float)startX, (float)listStartY, (float)(GetScreenWidth() - startX), (float)listHeight};
    // Chỉ hover khi bảng thông tin đóng, không có combo lọc đang mở, và chuột nằm trong vùng danh sách
    bool canHover = !isOpenTable && !isChucVuOpen && !isPhongOpen && CheckCollisionPointRec(mousePos, listArea);

    // Tọa độ Y hiện tại bắt đầu = Tọa độ gốc + Độ lệch cuộn
    float currentY = listStartY + scrollY;

    Node<Employee> *current = QL->get_DB()->get_ListEmployee()->get_header()->Next;

    // Đếm tổng số lượng dòng để chặn cuộn quá đà xuống dưới (tùy chọn nâng cao)
    int totalRows = 0;

    // lấy tiêu chí so sánh khi in
    static string tieuchi1 = "";
    static string tieuchi2 = "";
    if (selectedChucVuIndex != 0)
    {
        tieuchi1 = (*QL->get_DB()->get_ListCV())[selectedChucVuIndex - 1]->Data->get_MaChucVu();
    }
    else
    {
        tieuchi1 = "";
    }

    if (selectedPhongIndex != 0)
    {
        tieuchi2 = (*QL->get_DB()->get_ListPB())[selectedPhongIndex - 1]->Data->get_MaPhongBan();
    }
    else
    {
        tieuchi2 = "";
    }
    Employee *selEmp = nullptr;
    int idx = 0;
    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
    while (current != nullptr)
    {
        if (CheckEmployee(current->Data, tieuchi1, tieuchi2, hoTenInput))
        {
            string mnv = current->Data->get_mnv();
            string name = current->Data->get_name();
            string Ngaysinh = current->Data->get_NgaySinh().ToString();
            string mpb = current->Data->get_PhongBan()->get_TenPhongBan();
            string sex = current->Data->get_sex();
            string chucvu = current->Data->get_ChucVu()->get_TenChucVu();
            string vitri = current->Data->get_ViTri()->get_TenVTVL();
            string bac = to_string(current->Data->get_BacViTri());
            // ----- HOVER + CHỌN DÒNG -----
            Rectangle rowRect = {(float)startX, currentY - 5, (float)(GetScreenWidth() - startX - 20), 35};
            bool isHover = canHover && CheckCollisionPointRec(mousePos, rowRect);
            if (isHover && Clicked)
            {
                selectedMNV = mnv;
            }
            bool isSelected = (mnv == selectedMNV);

            if (isSelected)
            {
                // đã chọn: đậm hơn; vừa chọn vừa rê chuột: đậm thêm chút
                DrawRectangleRec(rowRect, isHover ? Color{180, 195, 230, 255} : Color{200, 210, 235, 255});
                // lưu nhân viên đang chọn
                selEmp = current->Data;
            }
            else if (isHover)
            {
                DrawRectangleRec(rowRect, Color{235, 235, 240, 255});
            }
            // ------------------------------
            DrawTextEx(myFont, mnv.c_str(), {colX[0] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, name.c_str(), {colX[1] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, Ngaysinh.c_str(), {colX[2] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, sex.c_str(), {colX[3] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, mpb.c_str(), {colX[4] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, chucvu.c_str(), {colX[5] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, vitri.c_str(), {colX[6] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, bac.c_str(), {colX[7] + lech, currentY}, fontSize, 1, BLACK);
            DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));
            currentY += rowHeight;
            totalRows++;
        }
        current = current->Next;
        idx++;
    }
    EndScissorMode();
    DrawTextEx(myFont, "Chức Vụ", {350, 240}, 40, 1, BLACK);

    int nCV = QL->get_DB()->get_ListCV()->get_size();
    string dsCVFilter[nCV + 1];
    dsCVFilter[0] = default_choice; // Mục đầu tiên: "Tất cả"
    for (int i = 0; i < nCV; i++)
    {
        dsCVFilter[i + 1] = (*QL->get_DB()->get_ListCV())[i]->Data->get_TenChucVu();
    }
    VeDanhSachCombo(comboChucVu, isChucVuOpen, dsCVFilter, nCV + 1, mousePos, 35);
    if (selectedChucVuIndex == 0)
    {
        DrawTextEx(myFont, default_choice.c_str(), {comboChucVu.x + 15, comboChucVu.y + 5}, 35, 1, DARKGRAY);
    }
    else
    {
        DrawTextEx(myFont, (*QL->get_DB()->get_ListCV())[selectedChucVuIndex - 1]->Data->get_TenChucVu().c_str(), {comboChucVu.x + 15, comboChucVu.y + 5}, 35, 1, DARKGRAY);
    }

    if (Clicked)
    {
        XuLyCombo(comboChucVu, isChucVuOpen, selectedChucVuIndex, nCV + 1, mousePos, Clicked);
    }
    DrawTextEx(myFont, "Phòng Ban", {620, 240}, 40, 1, BLACK);

    int nPB = QL->get_DB()->get_ListPB()->get_size();
    string dsPBFilter[nPB + 1];
    dsPBFilter[0] = default_choice; // Mục đầu tiên: "Tất cả"
    for (int i = 0; i < nPB; i++)
    {
        dsPBFilter[i + 1] = (*QL->get_DB()->get_ListPB())[i]->Data->get_TenPhongBan();
    }
    VeDanhSachCombo(comboPhong, isPhongOpen, dsPBFilter, nPB + 1, mousePos, 35);
    if (selectedPhongIndex == 0)
    {
        DrawTextEx(myFont, default_choice.c_str(), {comboPhong.x + 15, comboPhong.y + 5}, 35, 1, DARKGRAY);
    }
    else
    {
        DrawTextEx(myFont, (*QL->get_DB()->get_ListPB())[selectedPhongIndex - 1]->Data->get_TenPhongBan().c_str(), {comboPhong.x + 15, comboChucVu.y + 5}, 35, 1, DARKGRAY);
    }
    if (Clicked)
    {
        XuLyCombo(comboPhong, isPhongOpen, selectedPhongIndex, nPB + 1, mousePos, Clicked);
    }
    Typing(isHoTenActive, hoTenInput, letterCount, mousePos, textHoTen);

    // if (CheckCollisionPointRec(mousePos, btnLoc))
    // {
    //     // TODO: Viết code lọc danh sách nhân viên ở đây
    // }
    // 3. Cụm "Họ Tên"
    DrawTextEx(myFont, "Tìm kiếm", {890, 240}, 40, 1, BLACK);
    // Vẽ khung trắng bo góc cho ô nhập liệu, nếu đang active thì viền xám nhẹ
    DrawRectangleRounded(textHoTen, 0.5f, 10, isHoTenActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textHoTen, 0.3f, 12, 3.0f, LIGHTGRAY);
    // In chữ người dùng đã gõ ra màn hình
    DrawTextEx(myFont, hoTenInput, {textHoTen.x + 10, textHoTen.y + 8}, 35, 1, BLACK);

    // Nhấp nháy con trỏ chuột mô phỏng (dấu gạch dưới)
    if (isHoTenActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(hoTenInput, 35);
        DrawTextEx(myFont, "_", {textHoTen.x + 12 + textWidth, textHoTen.y + 8}, 35, 1, MAROON);
    }

    // 4. Nút bấm Lọc (Nút gradient/màu nổi)
    // Do Raylib không hỗ trợ sẵn gradient cho hình bo góc, ta dùng màu tím đậm tạm thời

    Rectangle comboSort = {1290, 290, 250, 45};
    static bool isSortOpen = false;
    static int selectedSortIndex = 0;
    string dsSort[] = {"...", "Mã NV", "Họ và Tên", "Ngày Sinh"};
    int nSort = 3;

    DrawTextEx(myFont, "Sắp xếp theo:", {1290, 240}, 40, 1, BLACK);
    // Vẽ đường gạch dưới
    // DrawLine(comboSort.x, comboSort.y + 30, comboSort.x + comboSort.width, comboSort.y + 30, GRAY);
    // Vẽ box
    VeDanhSachCombo(comboSort, isSortOpen, dsSort, nSort + 1, mousePos, 35);
    if (selectedSortIndex == 0)
    {
        DrawTextEx(myFont, dsSort[0].c_str(), {comboSort.x + 15, comboSort.y + 5}, 35, 1, DARKGRAY);
    }
    else
    {
        DrawTextEx(myFont, dsSort[selectedSortIndex].c_str(), {comboSort.x + 15, comboSort.y + 5}, 35, 1, DARKGRAY);
    }

    if (Clicked)
    {
        // Thay toàn bộ khối if (Clicked) cũ bằng:)
        XuLyCombo(comboSort, isSortOpen, selectedSortIndex, nSort + 1, mousePos, Clicked);
    }
    string dsSortTT[] = {"...", "tăng dần", "giảm dần"};
    int nSortTT = 2;
    VeDanhSachCombo(comboSortTT, isSortTTOpen, dsSortTT, nSortTT + 1, mousePos, 35);
    if (selectedSortTTIndex == 0)
    {
        DrawTextEx(myFont, dsSortTT[0].c_str(), {comboSortTT.x + 15, comboSortTT.y + 5}, 35, 1, DARKGRAY);
    }
    else
    {
        DrawTextEx(myFont, dsSortTT[selectedSortTTIndex].c_str(), {comboSortTT.x + 15, comboSortTT.y + 5}, 35, 1, DARKGRAY);
    }
    if (Clicked)
    {
        // Thay toàn bộ khối if (Clicked) cũ bằng:
        XuLyCombo(comboSortTT, isSortTTOpen, selectedSortTTIndex, nSort + 1, mousePos, Clicked);
    }
    if (selectedSortIndex == 1)
        if (selectedSortTTIndex == 1)
            QL->SapXep(TieuChiSapXep::MNV_TANG);
        else
            QL->SapXep(TieuChiSapXep::MNV_GIAM);
    else if (selectedSortIndex == 2)
        if (selectedSortTTIndex == 1)
            QL->SapXep(TieuChiSapXep::TEN_TANG);
        else
            QL->SapXep(TieuChiSapXep::TEN_GIAM);
    else if (selectedSortIndex == 3)
        if (selectedSortTTIndex == 1)
            QL->SapXep(TieuChiSapXep::NGAYSINH_TANG);
        else
            QL->SapXep(TieuChiSapXep::NGAYSINH_GIAM);
    // Tính toán chiều cao tổng của danh sách để chặn cuộn quá đáy
    float totalListHeight = totalRows * rowHeight;
    float maxScroll = listHeight - totalListHeight;
    // Nếu danh sách dài hơn màn hình và cuộn quá mức dưới, ép nó về mức tối đa
    if (totalListHeight > listHeight && scrollY < maxScroll)
    {
        scrollY = maxScroll;
    }
    // Nếu danh sách ngắn hơn màn hình, không cho cuộn
    else if (totalListHeight <= listHeight)
    {
        scrollY = 0.0f;
    }

    // Tắt chế độ Scissor để các phần vẽ sau (nếu có) không bị ảnh hưởng

    if (!isOpenDelete)
    {
        Rectangle btnThemNhanSu = {350, 940, 230, 50};
        DrawRectangleRounded(btnThemNhanSu, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Thêm nhân sự", {btnThemNhanSu.x + 32, btnThemNhanSu.y + 10}, 30, 1, WHITE);

        Rectangle btnSuaNhanSu = {610, 940, 230, 50};
        DrawRectangleRounded(btnSuaNhanSu, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Sửa nhân sự", {btnSuaNhanSu.x + 32, btnSuaNhanSu.y + 10}, 30, 1, WHITE);

        Rectangle btnXoaNhanSu = {870, 940, 230, 50};
        DrawRectangleRounded(btnXoaNhanSu, 0.2f, 10, RED);
        DrawTextEx(myFont, "   Xóa nhân sự", {btnXoaNhanSu.x + 32, btnXoaNhanSu.y + 10}, 30, 1, WHITE);

        if (Clicked && !isOpenTable && CheckCollisionPointRec(mousePos, btnThemNhanSu))
        {
            clickThem = true;
        }

        if (Clicked && !isOpenTable && CheckCollisionPointRec(mousePos, btnSuaNhanSu))
        {
            clickSua = true;
            needFill = true;
        }

        if (Clicked && !isOpenTable && CheckCollisionPointRec(mousePos, btnXoaNhanSu))
        {
            isOpenDelete = true;
        }

        if (clickSua || clickThem)
        {
            isOpenTable = true;
        }
    }

    if (isOpenTable)
    {
        // ================= 1. KHAI BÁO BIẾN LƯU TRẠNG THÁI FORM =================
        static char MaNhanVienInput[50] = "";
        static char HovaTenInput[50] = "";
        static int letterCountHovaTen = 0;
        static bool isHovaTenActive = false;

        // Trạng thái đóng/mở của 6 ô Combobox
        static bool openGT = false;
        static bool openPB = false;
        static bool openCV = false;
        static bool openLoai = false;
        static bool openVT = false;
        static bool openBac = false;

        // Vị trí mục đang chọn trong Combobox (-1 là chưa chọn)
        static int selGT = -1;
        static int selPB = -1;
        static int selCV = -1;
        static int selLoai = -1;
        static int selVT = -1;
        static int selBac = -1;

        // Trạng thái và dữ liệu của 2 bảng Lịch (Ngày sinh & Ngày vào làm)
        static bool isOpenCalendarNS = false;
        static bool isOpenCalendarNVL = false;
        static bool changeNS = false;
        static bool changeNVL = false;
        static int dayNS = day, monthNS = month, yearNS = year;
        static int dayNVL = day, monthNVL = month, yearNVL = year;

        static bool daSinhMa = false;
        static string thongBaoLoi = "";
        bool dongBang = false; // Cờ đánh dấu khi cần đóng và reset bảng

        // ================= 2. LẤY DỮ LIỆU TỪ DATABASE CHO COMBOBOX =================
        int nCV = QL->get_DB()->get_ListCV()->get_size();
        int nPB = QL->get_DB()->get_ListPB()->get_size();
        int nVT = QL->get_DB()->get_ListVTVL()->get_size();
        int nBac = 3;

        string dsCV[nCV], dsPB[nPB], dsVT[nVT], dsBac[3];
        for (int i = 0; i < nCV; i++)
            dsCV[i] = (*QL->get_DB()->get_ListCV())[i]->Data->get_TenChucVu();
        for (int i = 0; i < nPB; i++)
            dsPB[i] = (*QL->get_DB()->get_ListPB())[i]->Data->get_TenPhongBan();
        for (int i = 0; i < nVT; i++)
            dsVT[i] = (*QL->get_DB()->get_ListVTVL())[i]->Data->get_TenVTVL();
        for (int i = 0; i < nBac; i++)
            dsBac[i] = "Bậc " + to_string(i + 1);

        string dsGT[2] = {"Nam", "Nữ"};
        string dsLoai[2] = {"FullTime", "PartTime"};

        // ================= 3. KHỞI TẠO DỮ LIỆU KHI MỞ BẢNG (THÊM / SỬA) =================
        // Trường hợp THÊM: Tự động tạo mã nhân viên tăng dần (VD: NV016)
        if (clickThem && !daSinhMa)
        {
            int maxSo = 0;
            List<Employee> *ds = QL->get_ListEmployee();
            for (int i = 0; i < ds->get_size(); i++)
            {
                string ma = (*ds)[i]->Data->get_mnv();
                if (ma.length() > 2)
                {
                    int so = atoi(ma.substr(2).c_str());
                    if (so > maxSo)
                        maxSo = so;
                }
            }
            string maMoi = "NV";
            if (maxSo + 1 < 10)
                maMoi += "00";
            else if (maxSo + 1 < 100)
                maMoi += "0";
            maMoi += to_string(maxSo + 1);

            strcpy(MaNhanVienInput, maMoi.c_str());
            daSinhMa = true;
        }

        // Trường hợp SỬA: Đổ thông tin của nhân viên đang chọn vào các ô
        if (needFill)
        {
            needFill = false;
            if (selEmp != nullptr)
            {
                strcpy(MaNhanVienInput, selEmp->get_mnv().c_str());
                strcpy(HovaTenInput, selEmp->get_name().c_str());
                letterCountHovaTen = strlen(HovaTenInput);

                selCV = TimIndex(dsCV, nCV, selEmp->get_ChucVu()->get_TenChucVu());
                selPB = TimIndex(dsPB, nPB, selEmp->get_PhongBan()->get_TenPhongBan());
                selGT = TimIndex(dsGT, 2, selEmp->get_sex());
                selVT = TimIndex(dsVT, nVT, selEmp->get_ViTri()->get_TenVTVL());
                selBac = selEmp->get_BacViTri() - 1;
                selLoai = (dynamic_cast<FullTime *>(selEmp) != nullptr) ? 0 : 1;

                sscanf(selEmp->get_NgaySinh().ToString().c_str(), "%d/%d/%d", &dayNS, &monthNS, &yearNS);
                changeNS = true;

                sscanf(selEmp->get_NgayLamViec().ToString().c_str(), "%d/%d/%d", &dayNVL, &monthNVL, &yearNVL);
                changeNVL = true;
            }
        }

        // ================= 4. VẼ KHUNG NỀN MODAL & TIÊU ĐỀ =================
        float screenW = GetScreenWidth();
        float screenH = GetScreenHeight();

        // Lớp phủ mờ toàn màn hình phía sau
        DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.45f));

        // Khung trắng chính giữa màn hình
        Rectangle Table = {(screenW - 840) / 2, (screenH - 670) / 2, 840, 670};
        DrawRectangleRounded({Table.x + 3, Table.y + 5, Table.width, Table.height}, 0.04f, 10, Fade(BLACK, 0.15f));
        DrawRectangleRounded(Table, 0.04f, 10, WHITE);

        // Tiêu đề & dòng mô tả phụ
        DrawTextEx(tieude, "Thông tin nhân viên", {Table.x + 36, Table.y + 26}, 32, 1, Color{20, 25, 35, 255});
        if (clickSua)
            DrawTextEx(myFont, "Chỉnh sửa thông tin nhân viên", {Table.x + 36, Table.y + 64}, 22, 1, Color{110, 118, 130, 255});
        else
            DrawTextEx(myFont, "Nhập thông tin nhân viên mới", {Table.x + 36, Table.y + 64}, 22, 1, Color{110, 118, 130, 255});

        // Nút [X] góc phải trên
        Rectangle btnDong = {Table.x + Table.width - 60, Table.y + 26, 36, 36};
        bool hoverX = CheckCollisionPointRec(mousePos, btnDong);
        DrawRectangleRounded(btnDong, 0.3f, 8, hoverX ? Color{230, 235, 245, 255} : Color{243, 245, 249, 255});
        DrawLineEx({btnDong.x + 11, btnDong.y + 11}, {btnDong.x + 25, btnDong.y + 25}, 2.0f, DARKGRAY);
        DrawLineEx({btnDong.x + 25, btnDong.y + 11}, {btnDong.x + 11, btnDong.y + 25}, 2.0f, DARKGRAY);

        // ================= 5. TỌA ĐỘ 10 Ô NHẬP LIỆU (5 HÀNG x 2 CỘT) =================
        float boxW = 370.0f;
        float boxH = 45.0f;
        float col1X = Table.x + 36.0f;  // Cột trái
        float col2X = Table.x + 434.0f; // Cột phải

        float row1Y = Table.y + 125.0f; // Hàng 1
        float row2Y = Table.y + 207.0f; // Hàng 2
        float row3Y = Table.y + 289.0f; // Hàng 3
        float row4Y = Table.y + 371.0f; // Hàng 4
        float row5Y = Table.y + 453.0f; // Hàng 5

        Rectangle fMa = {col1X, row1Y, boxW, boxH};
        Rectangle fTen = {col2X, row1Y, boxW, boxH};

        Rectangle fNS = {col1X, row2Y, boxW, boxH};
        Rectangle fGT = {col2X, row2Y, boxW, boxH};

        Rectangle fPB = {col1X, row3Y, boxW, boxH};
        Rectangle fCV = {col2X, row3Y, boxW, boxH};

        Rectangle fLoai = {col1X, row4Y, boxW, boxH};
        Rectangle fVT = {col2X, row4Y, boxW, boxH};

        Rectangle fNVL = {col1X, row5Y, boxW, boxH};
        Rectangle fBac = {col2X, row5Y, boxW, boxH};

        // ================= 6. VẼ NHÃN (LABEL) VÀ CÁC Ô TRÊN FORM =================
        Color labelColor = Color{50, 60, 75, 255};
        Color placeColor = Color{150, 158, 170, 255};
        Color textColor = Color{31, 41, 55, 255};

        // --- HÀNG 1: Mã nhân viên & Họ và tên ---
        DrawTextEx(tieude, "Mã nhân viên", {fMa.x, fMa.y - 26}, 20, 1, labelColor);
        VeOInput(fMa, MaNhanVienInput, false, true, "");

        DrawTextEx(tieude, "Họ và tên", {fTen.x, fTen.y - 26}, 20, 1, labelColor);
        Typing(isHovaTenActive, HovaTenInput, letterCountHovaTen, mousePos, fTen);
        VeOInput(fTen, HovaTenInput, isHovaTenActive, false, "Nhập họ và tên");

        // --- HÀNG 2: Ngày sinh & Giới tính ---
        DrawTextEx(tieude, "Ngày sinh", {fNS.x, fNS.y - 26}, 20, 1, labelColor);
        string txtNS = changeNS ? TextFormat("%02d/%02d/%04d", dayNS, monthNS, yearNS) : "";
        VeODate(fNS, txtNS.c_str(), isOpenCalendarNS, mousePos, "mm/dd/yyyy");

        DrawTextEx(tieude, "Giới tính", {fGT.x, fGT.y - 26}, 20, 1, labelColor);
        VeDanhSachCombo(fGT, false, dsGT, 2, mousePos, 24);
        DrawTextEx(myFont, (selGT == -1) ? "Chọn giới tính" : dsGT[selGT].c_str(),
                   {fGT.x + 16, fGT.y + 11}, 24, 1, (selGT == -1) ? placeColor : textColor);

        // --- HÀNG 3: Phòng ban & Chức vụ ---
        DrawTextEx(tieude, "Phòng ban", {fPB.x, fPB.y - 26}, 20, 1, labelColor);
        VeDanhSachCombo(fPB, false, dsPB, nPB, mousePos, 24);
        DrawTextEx(myFont, (selPB == -1) ? "Chọn phòng ban" : dsPB[selPB].c_str(),
                   {fPB.x + 16, fPB.y + 11}, 24, 1, (selPB == -1) ? placeColor : textColor);

        DrawTextEx(tieude, "Chức vụ", {fCV.x, fCV.y - 26}, 20, 1, labelColor);
        VeDanhSachCombo(fCV, false, dsCV, nCV, mousePos, 24);
        DrawTextEx(myFont, (selCV == -1) ? "Chọn chức vụ" : dsCV[selCV].c_str(),
                   {fCV.x + 16, fCV.y + 11}, 24, 1, (selCV == -1) ? placeColor : textColor);

        // --- HÀNG 4: Loại nhân viên & Vị trí việc làm ---
        DrawTextEx(tieude, "Loại nhân viên", {fLoai.x, fLoai.y - 26}, 20, 1, labelColor);
        VeDanhSachCombo(fLoai, false, dsLoai, 2, mousePos, 24);
        DrawTextEx(myFont, (selLoai == -1) ? "Chọn loại nhân viên" : dsLoai[selLoai].c_str(),
                   {fLoai.x + 16, fLoai.y + 11}, 24, 1, (selLoai == -1) ? placeColor : textColor);

        DrawTextEx(tieude, "Vị trí việc làm", {fVT.x, fVT.y - 26}, 20, 1, labelColor);
        VeDanhSachCombo(fVT, false, dsVT, nVT, mousePos, 24);
        DrawTextEx(myFont, (selVT == -1) ? "Chọn vị trí" : dsVT[selVT].c_str(),
                   {fVT.x + 16, fVT.y + 11}, 24, 1, (selVT == -1) ? placeColor : textColor);

        // --- HÀNG 5: Ngày vào làm & Bậc lương ---
        DrawTextEx(tieude, "Ngày vào làm", {fNVL.x, fNVL.y - 26}, 20, 1, labelColor);
        string txtNVL = changeNVL ? TextFormat("%02d/%02d/%04d", dayNVL, monthNVL, yearNVL) : "";
        VeODate(fNVL, txtNVL.c_str(), isOpenCalendarNVL, mousePos, "mm/dd/yyyy");

        DrawTextEx(tieude, "Bậc lương", {fBac.x, fBac.y - 26}, 20, 1, labelColor);
        VeDanhSachCombo(fBac, false, dsBac, nBac, mousePos, 24);
        DrawTextEx(myFont, (selBac == -1) ? "Chọn bậc lương" : dsBac[selBac].c_str(),
                   {fBac.x + 16, fBac.y + 11}, 24, 1, (selBac == -1) ? placeColor : textColor);

        // ================= 7. VẼ NÚT "HỦY" VÀ NÚT "THÊM / SỬA" =================
        DrawLine(Table.x + 36, Table.y + 575, Table.x + Table.width - 36, Table.y + 575, Color{235, 240, 245, 255});

        Rectangle btnHuy = {Table.x + Table.width - 235, Table.y + 595, 95, 45};
        bool hoverHuy = CheckCollisionPointRec(mousePos, btnHuy);
        DrawRectangleRounded(btnHuy, 0.25f, 8, WHITE);
        DrawRectangleRoundedLinesEx(btnHuy, 0.25f, 8, 1.5f, hoverHuy ? Color{180, 190, 205, 255} : Color{225, 230, 240, 255});
        DrawTextEx(tieude, "Hủy", {btnHuy.x + 28, btnHuy.y + 11}, 22, 1, Color{50, 60, 75, 255});

        Rectangle btnLuu = {Table.x + Table.width - 125, Table.y + 595, 95, 45};
        bool hoverLuu = CheckCollisionPointRec(mousePos, btnLuu);
        DrawRectangleRounded(btnLuu, 0.25f, 8, hoverLuu ? Color{35, 80, 220, 255} : Color{45, 96, 235, 255});
        if (clickSua)
            DrawTextEx(tieude, "Sửa", {btnLuu.x + 28, btnLuu.y + 11}, 22, 1, WHITE);
        else
            DrawTextEx(tieude, "Thêm", {btnLuu.x + 22, btnLuu.y + 11}, 22, 1, WHITE);

        if (!thongBaoLoi.empty())
        {
            DrawTextEx(myFont, ("* Lỗi: " + thongBaoLoi).c_str(), {Table.x + 36, Table.y + 605}, 22, 1, RED);
        }

        // ================= 8. XỬ LÝ DANH SÁCH XỔ XUỐNG, LỊCH & SỰ KIỆN CLICK =================
        // Nếu đang có 1 Combobox hoặc Lịch mở -> Vẽ đè lên trên cùng và chỉ xử lý tương tác cho ô đó
        if (openGT)
        {
            VeDanhSachCombo(fGT, true, dsGT, 2, mousePos, 24);
            DrawTextEx(myFont, (selGT == -1) ? "Chọn giới tính" : dsGT[selGT].c_str(), {fGT.x + 16, fGT.y + 11}, 24, 1, (selGT == -1) ? placeColor : textColor);
            XuLyCombo(fGT, openGT, selGT, 2, mousePos, Clicked);
        }
        else if (openPB)
        {
            VeDanhSachCombo(fPB, true, dsPB, nPB, mousePos, 24);
            DrawTextEx(myFont, (selPB == -1) ? "Chọn phòng ban" : dsPB[selPB].c_str(), {fPB.x + 16, fPB.y + 11}, 24, 1, (selPB == -1) ? placeColor : textColor);
            XuLyCombo(fPB, openPB, selPB, nPB, mousePos, Clicked);
        }
        else if (openCV)
        {
            VeDanhSachCombo(fCV, true, dsCV, nCV, mousePos, 24);
            DrawTextEx(myFont, (selCV == -1) ? "Chọn chức vụ" : dsCV[selCV].c_str(), {fCV.x + 16, fCV.y + 11}, 24, 1, (selCV == -1) ? placeColor : textColor);
            XuLyCombo(fCV, openCV, selCV, nCV, mousePos, Clicked);
        }
        else if (openLoai)
        {
            VeDanhSachCombo(fLoai, true, dsLoai, 2, mousePos, 24);
            DrawTextEx(myFont, (selLoai == -1) ? "Chọn loại nhân viên" : dsLoai[selLoai].c_str(), {fLoai.x + 16, fLoai.y + 11}, 24, 1, (selLoai == -1) ? placeColor : textColor);
            XuLyCombo(fLoai, openLoai, selLoai, 2, mousePos, Clicked);
        }
        else if (openVT)
        {
            VeDanhSachCombo(fVT, true, dsVT, nVT, mousePos, 24);
            DrawTextEx(myFont, (selVT == -1) ? "Chọn vị trí" : dsVT[selVT].c_str(), {fVT.x + 16, fVT.y + 11}, 24, 1, (selVT == -1) ? placeColor : textColor);
            XuLyCombo(fVT, openVT, selVT, nVT, mousePos, Clicked);
        }
        else if (openBac)
        {
            VeDanhSachCombo(fBac, true, dsBac, nBac, mousePos, 24);
            DrawTextEx(myFont, (selBac == -1) ? "Chọn bậc lương" : dsBac[selBac].c_str(), {fBac.x + 16, fBac.y + 11}, 24, 1, (selBac == -1) ? placeColor : textColor);
            XuLyCombo(fBac, openBac, selBac, nBac, mousePos, Clicked);
        }
        else if (isOpenCalendarNS)
        {
            if (DrawCalendar(mousePos, dayNS, monthNS, yearNS, fNS, isOpenCalendarNS))
                changeNS = true;
        }
        else if (isOpenCalendarNVL)
        {
            if (DrawCalendar(mousePos, dayNVL, monthNVL, yearNVL, fNVL, isOpenCalendarNVL))
                changeNVL = true;
        }
        // Khi không có danh sách xổ xuống hay lịch nào mở -> Kiểm tra click mở ô hoặc bấm nút
        else if (Clicked)
        {
            if (CheckCollisionPointRec(mousePos, fGT))
                openGT = true;
            else if (CheckCollisionPointRec(mousePos, fPB))
                openPB = true;
            else if (CheckCollisionPointRec(mousePos, fCV))
                openCV = true;
            else if (CheckCollisionPointRec(mousePos, fLoai))
                openLoai = true;
            else if (CheckCollisionPointRec(mousePos, fVT))
                openVT = true;
            else if (CheckCollisionPointRec(mousePos, fBac))
                openBac = true;
            else if (CheckCollisionPointRec(mousePos, fNS))
                isOpenCalendarNS = true;
            else if (CheckCollisionPointRec(mousePos, fNVL))
                isOpenCalendarNVL = true;
            else if (hoverX || hoverHuy)
            {
                dongBang = true;
            }
            else if (hoverLuu)
            {
                Employee *nv = nullptr;
                try
                {
                    if (strlen(HovaTenInput) == 0)
                        throw string("Vui lòng nhập họ và tên!");
                    if (!changeNS)
                        throw string("Vui lòng chọn ngày sinh!");
                    if (selGT == -1)
                        throw string("Vui lòng chọn giới tính!");
                    if (selPB == -1)
                        throw string("Vui lòng chọn phòng ban!");
                    if (selCV == -1)
                        throw string("Vui lòng chọn chức vụ!");
                    if (selLoai == -1)
                        throw string("Vui lòng chọn loại nhân viên!");
                    if (selVT == -1)
                        throw string("Vui lòng chọn vị trí việc làm!");
                    if (!changeNVL)
                        throw string("Vui lòng chọn ngày vào làm!");
                    if (selBac == -1)
                        throw string("Vui lòng chọn bậc lương!");

                    Date ngaySinh(dayNS, monthNS, yearNS);
                    Date ngayVaoLam(dayNVL, monthNVL, yearNVL);

                    string mnv = MaNhanVienInput;
                    string ten = HovaTenInput;
                    string gioiTinh = dsGT[selGT];
                    ChucVu *cv = (*QL->get_DB()->get_ListCV())[selCV]->Data;
                    PhongBan *pb = (*QL->get_DB()->get_ListPB())[selPB]->Data;
                    ViTriViecLam *vt = (*QL->get_DB()->get_ListVTVL())[selVT]->Data;
                    int bac = selBac + 1;

                    Employee *oldEmp = QL->TimTheoMa(mnv);
                    int oldThuong = (oldEmp != nullptr) ? oldEmp->get_ThuongPhat() : 0;
                    int oldSNC = 0;
                    if (oldEmp != nullptr)
                    {
                        FullTime *oldFT = dynamic_cast<FullTime *>(oldEmp);
                        if (oldFT != nullptr)
                            oldSNC = oldFT->get_SoNgayCong();
                    }

                    if (selLoai == 0)
                        nv = new FullTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac, oldThuong, oldSNC);
                    else
                    {
                        int oldTime = 80;
                        if (oldEmp != nullptr)
                        {
                            PartTime *oldPT = dynamic_cast<PartTime *>(oldEmp);
                            if (oldPT != nullptr)
                                oldTime = oldPT->get_TimeWork();
                        }
                        nv = new PartTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac, oldThuong, oldTime);
                    }

                    if (clickThem)
                    {
                        QL->ThemEmployee(nv);
                        QL->get_DB()->SaveFileEmployee();
                        HienThongBao("Thêm nhân viên mới thành công!", false); // false = Màu xanh
                    }
                    else
                    {
                        QL->SuaEmployee(nv);
                        QL->get_DB()->SaveFileEmployee();
                        delete nv;
                        nv = nullptr;
                        HienThongBao("Cập nhật thông tin thành công!", false); // false = Màu xanh
                    }

                    dongBang = true;
                }
                catch (const string &loi)
                {
                    HienThongBao(loi, true); // true = Báo lỗi màu đỏ trượt ra
                    if (nv != nullptr)
                        delete nv;
                }
                catch (const char *loi)
                {
                    HienThongBao(string(loi), true);
                    if (nv != nullptr)
                        delete nv;
                }
            }
        }

        // ================= 9. ĐÓNG BẢNG & RESET BIẾN KHI HOÀN TẤT / HỦY =================
        if (dongBang)
        {
            isOpenTable = false;
            clickThem = false;
            clickSua = false;

            openGT = openPB = openCV = openLoai = openVT = openBac = false;
            selGT = selPB = selCV = selLoai = selVT = selBac = -1;

            isOpenCalendarNS = isOpenCalendarNVL = false;
            changeNS = changeNVL = false;

            MaNhanVienInput[0] = '\0';
            HovaTenInput[0] = '\0';
            letterCountHovaTen = 0;
            isHovaTenActive = false;

            daSinhMa = false;
            thongBaoLoi = "";
        }
    }

    if (isOpenDelete)
    {
        Rectangle btnHuyXoa = {350, 940, 230, 50};
        DrawRectangleRounded(btnHuyXoa, 0.2f, 10, WHITE);
        DrawTextEx(myFont, "Hủy xóa", {btnHuyXoa.x + 32, btnHuyXoa.y + 10}, 30, 1, BLACK);

        Rectangle btnXacNhanXoa = {610, 940, 230, 50};
        DrawRectangleRounded(btnXacNhanXoa, 0.2f, 10, RED);
        DrawTextEx(myFont, "Xác nhận xoá", {btnXacNhanXoa.x + 32, btnXacNhanXoa.y + 10}, 30, 1, WHITE);

        if (Clicked && CheckCollisionPointRec(mousePos, btnHuyXoa))
        {
            for (int i = 0; i < QL->get_ListEmployee()->get_size(); i++)
                dsXoa[i] = 0;
            isOpenDelete = false;
        }

        if (Clicked && CheckCollisionPointRec(mousePos, btnXacNhanXoa))
        {
            List<Employee> *ds = QL->get_ListEmployee();
            int n = ds->get_size();

            Employee **canXoa = new Employee *[n]; // gom con trỏ trước //* là mảng * con trỏ
            int dem = 0;
            for (int i = 0; i < n; i++)
                if (dsXoa[i] == 1)
                    canXoa[dem++] = (*ds)[i]->Data;

            for (int i = 0; i < dem; i++) // rồi mới xóa
                QL->XoaEmployee(canXoa[i]);
            delete[] canXoa;

            for (int i = 0; i < QL->get_ListEmployee()->get_size(); i++)
                dsXoa[i] = 0;
            isOpenDelete = false;
        }

        // vẽ
        BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
        float y = listStartY + scrollY;
        int idx = 0;
        Node<Employee> *p = QL->get_DB()->get_ListEmployee()->get_header()->Next;
        while (p != nullptr)
        {
            if (CheckEmployee(p->Data, tieuchi1, tieuchi2, hoTenInput))
            {
                Rectangle box = {(float)colX[0] + 2, y, 20, 20};
                DrawRectangleRec(box, WHITE);
                DrawRectangleLinesEx(box, 2, DARKGRAY);

                if (Clicked && canHover && CheckCollisionPointRec(mousePos, box))
                    dsXoa[idx] = !dsXoa[idx];
                if (dsXoa[idx] == 1)
                    DrawRectangleRec({box.x + 4, box.y + 4, box.width - 8, box.height - 8}, DARKGREEN);

                y += rowHeight;
            }
            p = p->Next;
            idx++;
        }
        EndScissorMode();
    }
    DrawTextEx(myFont, ("Tổng nhân viên: " + to_string(QL->get_DB()->get_ListEmployee()->get_size())).c_str(), {1550, 960}, 25, 1, BLACK);
}

void Draw_Screen_VTVL(Vector2 mousePos, bool Clicked, QLNV *QL)
{
    // bảng đầu
    static bool isMaVTActive = false;  // Trạng thái: ô có đang được click vào không
    static bool isTenVTActive = false; // Trạng thái: ô có đang được click vào không
    static bool needfill = false;
    static set<string> dsXoaVTVL;

    Rectangle panelBang1 = {330, 230, 1450.0f, 280.0f};
    DrawRectangleRounded({panelBang1.x + 3, panelBang1.y + 4, panelBang1.width, panelBang1.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang1, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang1, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Thông tin vị trí", {360.0f, 250.0f}, 40, 1, BLACK);

    static Texture2D a1;
    static bool isLoaded = false;
    if (!isLoaded)
    {
        a1 = LoadTexture("photo/anhto/vtri.png");
        isLoaded = true;
    }
    Vector2 pa1 = {360.0f, 300.0f}; // Tọa độ (x, y) postion
    float ra1 = 0.0f;               // Góc xoay (0 độ) rotation
    float sa1 = 0.6f;
    DrawTextureEx(a1, pa1, ra1, sa1, WHITE);

    Rectangle textMaVT = {850, 350, 300, 40};
    static char MaVTInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Ma = 0;
    DrawTextEx(myFont, "Mã vị trí:", {700.0f, 350.0f}, 35, 1, BLACK);
    DrawRectangle(textMaVT.x, textMaVT.y, textMaVT.width, textMaVT.height, isMaVTActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textMaVT, 0.3f, 12, 3.0f, LIGHTGRAY);

    Rectangle textTenVT = {850, 420, 300, 40};
    static char TenVTInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Ten = 0;
    DrawTextEx(myFont, "Tên vị trí:", {700.0f, 420.0f}, 35, 1, BLACK);
    DrawRectangle(textTenVT.x, textTenVT.y, textTenVT.width, textTenVT.height, isTenVTActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textTenVT, 0.3f, 12, 3.0f, LIGHTGRAY);
    if (Clicked)
    {
        if (CheckCollisionPointRec(mousePos, textMaVT))
        {
            isMaVTActive = true;
            isTenVTActive = false;
        }
        else if (CheckCollisionPointRec(mousePos, textTenVT))
        {
            isTenVTActive = true;
            isMaVTActive = false;
        }
    }

    Rectangle btnThemViTri = {350, 940, 230, 50};
    static bool OpenThem = false;

    Rectangle btnSuaViTri = {610, 940, 230, 50};
    static bool OpenSua = false;

    Rectangle btnXoaViTri = {870, 940, 230, 50};
    static bool OpenXoa = false;

    Rectangle btnXacNhan = {350, 940, 230, 50};
    static bool isXacNhan = false;

    Rectangle btnHuy = {610, 940, 230, 50};
    static bool isHuy = false;

    Rectangle btnHuyXoa = {350, 940, 230, 50};
    Rectangle btnXacNhanXoa = {610, 940, 230, 50};

    if (Clicked)
    {
        if (OpenXoa)
        {
            if (CheckCollisionPointRec(mousePos, btnHuyXoa))
            {
                dsXoaVTVL.clear();
                OpenXoa = false;
            }
            else if (CheckCollisionPointRec(mousePos, btnXacNhanXoa))
            {
                if (dsXoaVTVL.empty())
                {
                    HienThongBao("Chưa chọn vị trí nào để xóa!", true);
                }
                else
                {
                    vector<ViTriViecLam *> canXoa;
                    Node<ViTriViecLam> *p = QL->get_DB()->get_ListVTVL()->get_header()->Next;
                    while (p != nullptr)
                    {
                        if (p->Data && dsXoaVTVL.find(p->Data->get_MaVTVL()) != dsXoaVTVL.end())
                        {
                            canXoa.push_back(p->Data);
                        }
                        p = p->Next;
                    }

                    int dem = 0;
                    string loiCuoi = "";
                    for (ViTriViecLam *vt : canXoa)
                    {
                        try
                        {
                            QL->XoaViTri(vt);
                            dem++;
                        }
                        catch (const string &loi)
                        {
                            loiCuoi = loi;
                        }
                    }

                    if (dem > 0)
                    {
                        QL->get_DB()->SaveFileVTVL();
                        QL->get_DB()->SaveFileML();
                        if (loiCuoi.empty())
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " vị trí thành công!", false);
                        }
                        else
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " vị trí. " + loiCuoi, true);
                        }
                    }
                    else
                    {
                        HienThongBao(loiCuoi.empty() ? "Xóa không thành công!" : loiCuoi, true);
                    }

                    dsXoaVTVL.clear();
                    OpenXoa = false;
                    MaVTInput[0] = '\0';
                    TenVTInput[0] = '\0';
                    letterCount_Ma = 0;
                    letterCount_Ten = 0;
                }
            }
        }
        else if (OpenThem || OpenSua)
        {
            // Chỉ xử lý Xác nhận
            if (CheckCollisionPointRec(mousePos, btnXacNhan))
            {
                isXacNhan = true;
            }
            // Chỉ xử lý Hủy
            else if (CheckCollisionPointRec(mousePos, btnHuy))
            {
                isHuy = true;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mousePos, btnThemViTri))
            {
                OpenThem = true;
                MaVTInput[0] = '\0';
                TenVTInput[0] = '\0';
                letterCount_Ma = 0;
                letterCount_Ten = 0;
            }
            else if (CheckCollisionPointRec(mousePos, btnSuaViTri))
            {
                OpenSua = true;
            }
            else if (CheckCollisionPointRec(mousePos, btnXoaViTri))
            {
                OpenXoa = true;
                dsXoaVTVL.clear();
            }
        }
    }

    if (OpenXoa)
    {
        DrawRectangleRounded(btnHuyXoa, 0.2f, 10, WHITE);
        DrawRectangleRoundedLinesEx(btnHuyXoa, 0.2f, 10, 1.5f, DARKGRAY);
        DrawTextEx(myFont, "Hủy xóa", {btnHuyXoa.x + 55, btnHuyXoa.y + 10}, 30, 1, BLACK);

        DrawRectangleRounded(btnXacNhanXoa, 0.2f, 10, RED);
        DrawTextEx(myFont, "Xác nhận xoá", {btnXacNhanXoa.x + 32, btnXacNhanXoa.y + 10}, 30, 1, WHITE);
    }
    else if (OpenSua || OpenThem)
    {
        DrawRectangleRounded(btnXacNhan, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Xác nhận", {btnXacNhan.x + 32, btnXacNhan.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnHuy, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Hủy", {btnHuy.x + 32, btnHuy.y + 10}, 30, 1, WHITE);
    }
    else
    {
        DrawRectangleRounded(btnThemViTri, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Thêm vị trí", {btnThemViTri.x + 32, btnThemViTri.y + 10}, 30, 1, WHITE);
        DrawRectangleRounded(btnSuaViTri, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Sửa vị trí", {btnSuaViTri.x + 32, btnSuaViTri.y + 10}, 30, 1, WHITE);
        DrawRectangleRounded(btnXoaViTri, 0.2f, 10, RED);
        DrawTextEx(myFont, "   Xóa vị trí", {btnXoaViTri.x + 32, btnXoaViTri.y + 10}, 30, 1, WHITE);
    }

    // bảng 2
    Rectangle panelBang2 = {330, 530, 1450.0f, 390.0f};
    DrawRectangleRounded({panelBang2.x + 3, panelBang2.y + 4, panelBang2.width, panelBang2.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang2, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang2, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});
    static float scrollY = 0.0f;
    float scrollSpeed = 30.0f; // Tốc độ cuộn mỗi nấc chuột

    // Lấy thao tác lăn chuột và cộng dồn vào scrollY
    float wheel = GetMouseWheelMove(); // Lăn chuột xuống trả về -1 ngược lại lên
    if (wheel != 0.0f)
    {
        scrollY += wheel * scrollSpeed;
    }

    // Chặn cuộn quá đà lên trên (danh sách không bị tụt xuống dưới tiêu đề)
    if (scrollY > 0.0f)
        scrollY = 0.0f;
    // Cấu hình khoảng cách và kích thước chữ
    int startX = 350;   // Lề trái
    int startY = 600;   // Lề trên
    int rowHeight = 60; // Chiều cao mỗi dòng
    int fontSize = 30;  // Kích thước chữ

    // Tọa độ X của từng cột (Bạn cần tinh chỉnh các số này cho vừa với màn hình)
    int colX[] = {350, 600, 900, 950, 860, 1000, 1200, 1460};

    // 1. VẼ TIÊU ĐỀ CỘT (Header)
    float lech = OpenXoa ? 30.f : 0.f;
    DrawTextEx(myFont, "Mã Vị Trí", {colX[0] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Tên Vị Trí", {colX[1] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Số Lượng Nhân Viên", {colX[2] + lech, (float)startY}, fontSize, 1, DARKGRAY);

    // Kẻ đường gạch ngang dưới tiêu đề
    DrawLine(startX, startY + 30, 1900, startY + 30, LIGHTGRAY);

    // 4. KHU VỰC VẼ DANH SÁCH BỊ CẮT (Scissor Mode)
    int listStartY = startY + 60;                         // Điểm bắt đầu vẽ dữ liệu (ngay dưới đường kẻ)
    int listHeight = GetScreenHeight() - listStartY - 80; // Chiều cao khu vực hiển thị dữ liệu

    // Bật chế độ Scissor: Bất cứ thứ gì vẽ bên ngoài hình chữ nhật này sẽ bị ẩn đi
    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
    Rectangle listArea = {(float)startX, (float)listStartY, (float)(GetScreenWidth() - startX), (float)listHeight};
    // Chỉ hover khi chuột nằm trong vùng danh sách
    bool canHover = CheckCollisionPointRec(mousePos, listArea);

    // Tọa độ Y hiện tại bắt đầu = Tọa độ gốc + Độ lệch cuộn
    float currentY = listStartY + scrollY;

    Node<ViTriViecLam> *current = QL->get_DB()->get_ListVTVL()->get_header()->Next;

    // Đếm tổng số lượng dòng để chặn cuộn quá đà xuống dưới (tùy chọn nâng cao)
    int totalRows = 0;

    ViTriViecLam *selPB = nullptr;
    static string selectedPhongBan = "";
    while (current != nullptr)
    {
        string mpb = current->Data->get_MaVTVL();
        string name = current->Data->get_TenVTVL();
        int size = current->Data->get_ListNV()->get_size();
        string vt_size = to_string(size);

        // ----- HOVER + CHỌN DÒNG -----
        Rectangle rowRect = {(float)startX, currentY - 5, (float)(GetScreenWidth() - startX - 20), 35};
        bool isHover = canHover && CheckCollisionPointRec(mousePos, rowRect);
        if (isHover && Clicked)
        {
            if (OpenXoa)
            {
                if (dsXoaVTVL.find(mpb) != dsXoaVTVL.end())
                    dsXoaVTVL.erase(mpb);
                else
                    dsXoaVTVL.insert(mpb);
            }
            else
            {
                selectedPhongBan = mpb;
                needfill = true;
            }
        }
        bool isSelected = (mpb == selectedPhongBan);

        if (OpenXoa)
        {
            if (dsXoaVTVL.find(mpb) != dsXoaVTVL.end())
            {
                DrawRectangleRec(rowRect, Color{255, 230, 230, 255});
            }
            else if (isHover)
            {
                DrawRectangleRec(rowRect, Color{245, 245, 250, 255});
            }

            Rectangle box = {(float)colX[0] + 2, currentY + 4, 20, 20};
            DrawRectangleRec(box, WHITE);
            DrawRectangleLinesEx(box, 2, DARKGRAY);
            if (dsXoaVTVL.find(mpb) != dsXoaVTVL.end())
            {
                DrawRectangleRec({box.x + 4, box.y + 4, box.width - 8, box.height - 8}, DARKGREEN);
            }
        }
        else if (isSelected)
        {
            // đã chọn: đậm hơn; vừa chọn vừa rê chuột: đậm thêm chút
            DrawRectangleRec(rowRect, isHover ? Color{180, 195, 230, 255} : Color{200, 210, 235, 255});
            // lưu nhân viên đang chọn
            selPB = current->Data;
        }
        else if (isHover)
        {
            DrawRectangleRec(rowRect, Color{235, 235, 240, 255});
        }
        // ------------------------------

        DrawTextEx(myFont, mpb.c_str(), {colX[0] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, name.c_str(), {colX[1] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, vt_size.c_str(), {colX[2] + lech, currentY}, fontSize, 1, BLACK);
        DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));

        currentY += rowHeight;
        totalRows++;

        current = current->Next;
    }

    // Tính toán chiều cao tổng của danh sách để chặn cuộn quá đáy
    float totalListHeight = totalRows * rowHeight;
    float maxScroll = listHeight - totalListHeight;
    // Nếu danh sách dài hơn màn hình và cuộn quá mức dưới, ép nó về mức tối đa
    if (totalListHeight > listHeight && scrollY < maxScroll)
    {
        scrollY = maxScroll;
    }
    // Nếu danh sách ngắn hơn màn hình, không cho cuộn
    else if (totalListHeight <= listHeight)
    {
        scrollY = 0.0f;
    }

    // Tắt chế độ Scissor để các phần vẽ sau (nếu có) không bị ảnh hưởng
    EndScissorMode();

    if (needfill && selPB != nullptr)
    {
        strncpy(MaVTInput, selPB->get_MaVTVL().c_str(), 49);
        letterCount_Ma = strlen(MaVTInput);
        MaVTInput[letterCount_Ma] = '\0';

        strncpy(TenVTInput, selPB->get_TenVTVL().c_str(), 49);
        letterCount_Ten = strlen(TenVTInput);
        TenVTInput[letterCount_Ten] = '\0';

        needfill = false;
    }

    if (OpenThem)
    {
        Typing(isMaVTActive, MaVTInput, letterCount_Ma, mousePos, textMaVT);
        HieuUngGo(isMaVTActive, textMaVT, MaVTInput, 28);

        Typing(isTenVTActive, TenVTInput, letterCount_Ten, mousePos, textTenVT);
        HieuUngGo(isTenVTActive, textTenVT, TenVTInput, 28);
    }
    else if (OpenSua && selPB != nullptr)
    { // Ô nhập chữ chế độ sửa
        DrawTextEx(myFont, MaVTInput, {textMaVT.x + 10, textMaVT.y + 6}, 28, 1, BLACK);
        Typing(isTenVTActive, TenVTInput, letterCount_Ten, mousePos, textTenVT);
        HieuUngGo(isTenVTActive, textTenVT, TenVTInput, 28);
    }
    else if (selPB != nullptr || MaVTInput[0] != '\0')
    {
        // Khi click vào 1 đối tượng trong danh sách (chế độ xem)
        DrawTextEx(myFont, MaVTInput, {textMaVT.x + 10, textMaVT.y + 6}, 28, 1, BLACK);
        DrawTextEx(myFont, TenVTInput, {textTenVT.x + 10, textTenVT.y + 6}, 28, 1, BLACK);
    }

    if (isXacNhan)
    {
        if (OpenThem)
        {
            ViTriViecLam *VTnew = new ViTriViecLam(MaVTInput, TenVTInput);
            try
            {
                QL->ThemViTri(VTnew);
                QL->get_DB()->SaveFileVTVL();
                needfill = false;
                letterCount_Ma = 0;
                letterCount_Ten = 0;
                OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
                MaVTInput[0] = '\0';
                TenVTInput[0] = '\0';
                HienThongBao("Thêm vị trí mới thành công!", false);
            }
            catch (const string &loi)
            {
                needfill = false;
                letterCount_Ma = 0;
                letterCount_Ten = 0;
                OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
                MaVTInput[0] = '\0';
                TenVTInput[0] = '\0';
                HienThongBao(loi, true); // true màu đỏ
            }
        }
        else if (OpenSua && selPB != nullptr)
        {
            selPB->set_TenVTVL(TenVTInput);
            QL->get_DB()->SaveFileVTVL();
            OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
            MaVTInput[0] = '\0';
            TenVTInput[0] = '\0';
            letterCount_Ma = 0;
            letterCount_Ten = 0;
            HienThongBao("Cập nhật vị trí thành công!", false);
        }
    }

    if (isHuy && CheckCollisionPointRec(mousePos, btnHuy))
    {
        OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
        MaVTInput[0] = '\0';
        TenVTInput[0] = '\0';
        letterCount_Ma = 0;
        letterCount_Ten = 0;
    }

    DrawTextEx(myFont, ("Tổng vị trí: " + to_string(QL->get_DB()->get_ListVTVL()->get_size())).c_str(), {1550, 960}, 25, 1, BLACK);
}

void Draw_Screen_Employee(QLNV *QL)
{
    static Texture2D a1;
    static bool isLoaded = false;
    if (!isLoaded)
    {
        a1 = LoadTexture("photo/icon/Asset-1.png");
        isLoaded = true;
    }
    Vector2 pa1 = {370.0f, 45.0f}; // Tọa độ (x, y) postion
    float ra1 = 0.0f;              // Góc xoay (0 độ) rotation
    float sa1 = 0.33f;
    DrawTextureEx(a1, pa1, ra1, sa1, WHITE);
    DrawTextEx(tieude, "Quản Lý Nhân Sự/Vị Trí Việc Làm", {pa1.x + 100, pa1.y}, 45, 1, WHITE);
    DrawTextEx(myFont, "Danh sách và thông tin nhân sự/vị trí của công ty", {pa1.x + 100, pa1.y + 50}, 30, 1, WHITE);
    static bool isOpenNV = false;
    static bool isOpenVT = true;
    Vector2 mousePos = GetMousePosition();
    bool Clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float TabX = 330.0f;
    float TabY = 160.0f;
    float TabH = 90.0f;
    Rectangle TabNV = {TabX, TabY, 300.0f, TabH};
    Rectangle TabVTVL = {TabX + 310.0f, TabY, 400.0f, TabH};
    bool hoverNV = CheckCollisionPointRec(mousePos, TabNV);
    bool hoverVTVL = CheckCollisionPointRec(mousePos, TabVTVL);
    Color CTabNV = isOpenNV == 1 ? BLUE : WHITE;
    Color CTabVT = isOpenVT == 1 ? BLUE : WHITE;
    if (isOpenNV == true && hoverVTVL)
        CTabVT = Fade(BLUE, 0.4);
    if (isOpenVT == true && hoverNV)
        CTabNV = Fade(BLUE, 0.4);
    DrawRectangleRounded(TabNV, 0.3f, 10, CTabNV);
    DrawRectangleRoundedLinesEx(TabNV, 0.3f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Nhân viên", {TabNV.x + 70.0f, TabNV.y + 15.0f}, 40, 1, BLACK);

    DrawRectangleRounded(TabVTVL, 0.3f, 10, CTabVT);
    DrawRectangleRoundedLinesEx(TabNV, 0.3f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Vị trí việc làm", {TabVTVL.x + 90.0f, TabVTVL.y + 15.0f}, 40, 1, BLACK);

    Rectangle panelBang = {330, 230, 1450.0f, 690.0f};
    DrawRectangleRounded({panelBang.x + 3, panelBang.y + 4, panelBang.width, panelBang.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});

    if (Clicked && CheckCollisionPointRec(mousePos, TabNV))
    {
        isOpenNV = true;
        isOpenVT = false;
    }
    else if (Clicked && CheckCollisionPointRec(mousePos, TabVTVL))
    {
        isOpenVT = true;
        isOpenNV = false;
    }

    if (isOpenNV)
    {
        Draw_Screen_NhanVien(mousePos, Clicked, QL);
    }
    else if (isOpenVT)
    {
        Draw_Screen_VTVL(mousePos, Clicked, QL);
    }
}

void Draw_Screen_PhongBan(Vector2 mousePos, bool Clicked, QLNV *QL)
{
    // Vẽ tiêu đề
    static bool isMaPhongBanActive = false;  // Trạng thái: ô có đang được click vào không
    static bool isTenPhongBanActive = false; // Trạng thái: ô có đang được click vào không
    static bool isOpenCalendar = false;
    static bool needfill = false;
    static set<string> dsXoaPB;

    Rectangle panelBang1 = {330, 230, 1450.0f, 320.0f};
    DrawRectangleRounded({panelBang1.x + 3, panelBang1.y + 4, panelBang1.width, panelBang1.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang1, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang1, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Thông tin phòng ban", {360.0f, 250.0f}, 40, 1, BLACK);

    static Texture2D a1;
    static bool isLoaded = false;
    if (!isLoaded)
    {
        a1 = LoadTexture("photo/anhto/pban.png");
        isLoaded = true;
    }
    Vector2 pa1 = {360.0f, 320.0f}; // Tọa độ (x, y) postion
    float ra1 = 0.0f;               // Góc xoay (0 độ) rotation
    float sa1 = 0.6f;
    DrawTextureEx(a1, pa1, ra1, sa1, WHITE);

    Rectangle textMaPhongBan = {930, 350, 300, 40};
    static char MaPhongBanInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Ma = 0;
    DrawTextEx(myFont, "Mã Phòng Ban:", {700.0f, 350.0f}, 35, 1, BLACK);
    DrawRectangleRounded(textMaPhongBan, 0.5f, 10, isMaPhongBanActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textMaPhongBan, 0.3f, 12, 3.0f, LIGHTGRAY);

    Rectangle textTenPhongBan = {930, 420, 300, 40};
    static char TenPhongBanInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Ten = 0;
    DrawTextEx(myFont, "Tên Phòng Ban:", {700.0f, 420.0f}, 35, 1, BLACK);
    DrawRectangleRounded(textTenPhongBan, 0.5f, 10, isTenPhongBanActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textTenPhongBan, 0.3f, 12, 3.0f, LIGHTGRAY);

    Rectangle textNgayThanhLap = {930, 490, 300, 40};
    static int day1 = day, month1 = month, year1 = year;
    DrawTextEx(myFont, "Ngày thành lập:", {700, 490}, 35, 1, BLACK);
    DrawRectangleRoundedLinesEx(textNgayThanhLap, 0.3f, 12, 3.0f, LIGHTGRAY);
    string textCalendar = to_string(day1) + '/' + to_string(month1) + '/' + to_string(year1);
    if (day1 < 10)
    {
        textCalendar = '0' + textCalendar;
    }
    if (month1 < 10)
    {
        textCalendar.insert(3, "0");
    }

    if (Clicked)
    {
        if (CheckCollisionPointRec(mousePos, textMaPhongBan))
        {
            isMaPhongBanActive = true;
            isTenPhongBanActive = isOpenCalendar = false;
        }
        else if (CheckCollisionPointRec(mousePos, textTenPhongBan))
        {
            isTenPhongBanActive = true;
            isMaPhongBanActive = isOpenCalendar = false;
        }
        else if (CheckCollisionPointRec(mousePos, textNgayThanhLap))
        {
            isOpenCalendar = true;
            isMaPhongBanActive = isTenPhongBanActive = false;
        }
    }

    DrawRectangleRounded(textNgayThanhLap, 0.5f, 10, isOpenCalendar ? LIGHTGRAY : WHITE);

    // Vẽ nút chức năng để ở trên để không bị che
    Rectangle btnThemPhongBan = {350, 940, 280, 50};
    static bool OpenThem = false;

    Rectangle btnSuaPhongBan = {660, 940, 280, 50};
    static bool OpenSua = false;

    Rectangle btnXoaPhongBan = {970, 940, 280, 50};
    static bool OpenXoa = false;

    Rectangle btnXacNhan = {350, 940, 230, 50};
    static bool isXacNhan = false;
    Rectangle btnHuy = {610, 940, 230, 50};
    static bool isHuy = false;

    Rectangle btnHuyXoa = {350, 940, 230, 50};
    Rectangle btnXacNhanXoa = {610, 940, 230, 50};

    if (Clicked)
    {
        if (OpenXoa)
        {
            if (CheckCollisionPointRec(mousePos, btnHuyXoa))
            {
                dsXoaPB.clear();
                OpenXoa = false;
            }
            else if (CheckCollisionPointRec(mousePos, btnXacNhanXoa))
            {
                if (dsXoaPB.empty())
                {
                    HienThongBao("Chưa chọn phòng ban nào để xóa!", true);
                }
                else
                {
                    vector<PhongBan *> canXoa;
                    Node<PhongBan> *p = QL->get_DB()->get_ListPB()->get_header()->Next;
                    while (p != nullptr)
                    {
                        if (p->Data && dsXoaPB.find(p->Data->get_MaPhongBan()) != dsXoaPB.end())
                        {
                            canXoa.push_back(p->Data);
                        }
                        p = p->Next;
                    }

                    int dem = 0;
                    string loiCuoi = "";
                    for (PhongBan *pb : canXoa)
                    {
                        try
                        {
                            QL->XoaPhongBan(pb);
                            dem++;
                        }
                        catch (const string &loi)
                        {
                            loiCuoi = loi;
                        }
                    }

                    if (dem > 0)
                    {
                        QL->get_DB()->SaveFilePB();
                        if (loiCuoi.empty())
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " phòng ban thành công!", false);
                        }
                        else
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " phòng ban. " + loiCuoi, true);
                        }
                    }
                    else
                    {
                        HienThongBao(loiCuoi.empty() ? "Xóa không thành công!" : loiCuoi, true);
                    }

                    dsXoaPB.clear();
                    OpenXoa = false;
                    MaPhongBanInput[0] = '\0';
                    TenPhongBanInput[0] = '\0';
                    textCalendar = "";
                    letterCount_Ma = letterCount_Ten = 0;
                }
            }
        }
        else if (OpenThem || OpenSua)
        {
            // Chỉ xử lý Xác nhận
            if (CheckCollisionPointRec(mousePos, btnXacNhan))
            {
                isXacNhan = true;
            }
            // Chỉ xử lý Hủy
            else if (CheckCollisionPointRec(mousePos, btnHuy))
            {
                isHuy = true;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mousePos, btnThemPhongBan))
            {
                OpenThem = true;
                MaPhongBanInput[0] = '\0';
                TenPhongBanInput[0] = '\0';
                textCalendar = "";
                letterCount_Ma = 0;
                letterCount_Ten = 0;
            }
            else if (CheckCollisionPointRec(mousePos, btnSuaPhongBan))
            {
                OpenSua = true;
            }
            else if (CheckCollisionPointRec(mousePos, btnXoaPhongBan))
            {
                OpenXoa = true;
                dsXoaPB.clear();
            }
        }
    }

    if (OpenXoa)
    {
        DrawRectangleRounded(btnHuyXoa, 0.2f, 10, WHITE);
        DrawRectangleRoundedLinesEx(btnHuyXoa, 0.2f, 10, 1.5f, DARKGRAY);
        DrawTextEx(myFont, "Hủy xóa", {btnHuyXoa.x + 55, btnHuyXoa.y + 10}, 30, 1, BLACK);

        DrawRectangleRounded(btnXacNhanXoa, 0.2f, 10, RED);
        DrawTextEx(myFont, "Xác nhận xoá", {btnXacNhanXoa.x + 32, btnXacNhanXoa.y + 10}, 30, 1, WHITE);
    }
    else if (OpenSua || OpenThem)
    {
        DrawRectangleRounded(btnXacNhan, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Xác nhận", {btnXacNhan.x + 32, btnXacNhan.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnHuy, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Hủy", {btnHuy.x + 32, btnHuy.y + 10}, 30, 1, WHITE);
    }
    else
    {
        DrawRectangleRounded(btnThemPhongBan, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Thêm Phòng Ban", {btnThemPhongBan.x + 32, btnThemPhongBan.y + 10}, 30, 1, WHITE);
        DrawRectangleRounded(btnSuaPhongBan, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Sửa Phòng Ban", {btnSuaPhongBan.x + 32, btnSuaPhongBan.y + 10}, 30, 1, WHITE);
        DrawRectangleRounded(btnXoaPhongBan, 0.2f, 10, RED);
        DrawTextEx(myFont, "   Xóa Phòng Ban", {btnXoaPhongBan.x + 32, btnXoaPhongBan.y + 10}, 30, 1, WHITE);
    }

    // Bảng 2
    Rectangle panelBang2 = {330, 580, 1450.0f, 350.0f};
    DrawRectangleRounded({panelBang2.x + 3, panelBang2.y + 4, panelBang2.width, panelBang2.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang2, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang2, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});
    static float scrollY = 0.0f;
    float scrollSpeed = 30.0f; // Tốc độ cuộn mỗi nấc chuột

    // Lấy thao tác lăn chuột và cộng dồn vào scrollY
    float wheel = GetMouseWheelMove(); // Lăn chuột xuống trả về -1 ngược lại lên
    if (wheel != 0.0f)
    {
        scrollY += wheel * scrollSpeed;
    }

    // Chặn cuộn quá đà lên trên (danh sách không bị tụt xuống dưới tiêu đề)
    if (scrollY > 0.0f)
        scrollY = 0.0f;
    // Cấu hình khoảng cách và kích thước chữ
    int startX = 350;   // Lề trái
    int startY = 650;   // Lề trên
    int rowHeight = 60; // Chiều cao mỗi dòng
    int fontSize = 30;  // Kích thước chữ

    // Tọa độ X của từng cột (Bạn cần tinh chỉnh các số này cho vừa với màn hình)
    int colX[] = {350, 600, 900, 1250};

    // 1. VẼ TIÊU ĐỀ CỘT (Header)
    float lech = OpenXoa ? 30.f : 0.f;
    DrawTextEx(myFont, "Mã Phòng Ban", {colX[0] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Tên Phòng Ban", {colX[1] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Ngày Thành Lập", {colX[2] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Số Lượng Nhân Viên", {colX[3] + lech, (float)startY}, fontSize, 1, DARKGRAY);

    // Kẻ đường gạch ngang dưới tiêu đề
    DrawLine(startX, startY + 30, 1900, startY + 30, LIGHTGRAY);

    // 4. KHU VỰC VẼ DANH SÁCH BỊ CẮT (Scissor Mode)
    int listStartY = startY + 60;                         // Điểm bắt đầu vẽ dữ liệu (ngay dưới đường kẻ)
    int listHeight = GetScreenHeight() - listStartY - 80; // Chiều cao khu vực hiển thị dữ liệu

    // Bật chế độ Scissor: Bất cứ thứ gì vẽ bên ngoài hình chữ nhật này sẽ bị ẩn đi
    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
    Rectangle listArea = {(float)startX, (float)listStartY, (float)(GetScreenWidth() - startX), (float)listHeight};
    // Chỉ hover khi chuột nằm trong vùng danh sách
    bool canHover = CheckCollisionPointRec(mousePos, listArea);

    // Tọa độ Y hiện tại bắt đầu = Tọa độ gốc + Độ lệch cuộn
    float currentY = listStartY + scrollY;

    Node<PhongBan> *current = QL->get_DB()->get_ListPB()->get_header()->Next;

    // Đếm tổng số lượng dòng để chặn cuộn quá đà xuống dưới (tùy chọn nâng cao)
    int totalRows = 0;

    PhongBan *selPB = nullptr;
    static string selectedPhongBan = "";
    while (current != nullptr)
    {
        string mpb = current->Data->get_MaPhongBan();
        string name = current->Data->get_TenPhongBan();
        string NTL = current->Data->get_NgayThanhLap().ToString();
        int size = current->Data->get_ListNV()->get_size();
        string pb_size = to_string(size);

        // ----- HOVER + CHỌN DÒNG -----
        Rectangle rowRect = {(float)startX, currentY - 5, (float)(GetScreenWidth() - startX - 20), 35};
        bool isHover = canHover && CheckCollisionPointRec(mousePos, rowRect);
        if (isHover && Clicked)
        {
            if (OpenXoa)
            {
                if (dsXoaPB.find(mpb) != dsXoaPB.end())
                    dsXoaPB.erase(mpb);
                else
                    dsXoaPB.insert(mpb);
            }
            else
            {
                selectedPhongBan = mpb;
                needfill = true;
            }
        }
        bool isSelected = (mpb == selectedPhongBan);

        if (OpenXoa)
        {
            if (dsXoaPB.find(mpb) != dsXoaPB.end())
            {
                DrawRectangleRec(rowRect, Color{255, 230, 230, 255});
            }
            else if (isHover)
            {
                DrawRectangleRec(rowRect, Color{245, 245, 250, 255});
            }

            Rectangle box = {(float)colX[0] + 2, currentY + 4, 20, 20};
            DrawRectangleRec(box, WHITE);
            DrawRectangleLinesEx(box, 2, DARKGRAY);
            if (dsXoaPB.find(mpb) != dsXoaPB.end())
            {
                DrawRectangleRec({box.x + 4, box.y + 4, box.width - 8, box.height - 8}, DARKGREEN);
            }
        }
        else if (isSelected)
        {
            // đã chọn: đậm hơn; vừa chọn vừa rê chuột: đậm thêm chút
            DrawRectangleRec(rowRect, isHover ? Color{180, 195, 230, 255} : Color{200, 210, 235, 255});
            // lưu phòng ban đang chọn
            selPB = current->Data;
        }
        else if (isHover)
        {
            DrawRectangleRec(rowRect, Color{235, 235, 240, 255});
        }
        // ------------------------------

        DrawTextEx(myFont, mpb.c_str(), {colX[0] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, name.c_str(), {colX[1] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, NTL.c_str(), {colX[2] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, pb_size.c_str(), {colX[3] + lech, currentY}, fontSize, 1, BLACK);
        DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));

        currentY += rowHeight;
        totalRows++;

        current = current->Next;
    }

    // Tính toán chiều cao tổng của danh sách để chặn cuộn quá đáy
    float totalListHeight = totalRows * rowHeight;
    float maxScroll = listHeight - totalListHeight;
    // Nếu danh sách dài hơn màn hình và cuộn quá mức dưới, ép nó về mức tối đa
    if (totalListHeight > listHeight && scrollY < maxScroll)
    {
        scrollY = maxScroll;
    }
    // Nếu danh sách ngắn hơn màn hình, không cho cuộn
    else if (totalListHeight <= listHeight)
    {
        scrollY = 0.0f;
    }

    // Tắt chế độ Scissor để các phần vẽ sau (nếu có) không bị ảnh hưởng
    EndScissorMode();
    if (needfill && selPB != nullptr)
    {
        strncpy(MaPhongBanInput, selPB->get_MaPhongBan().c_str(), 49);
        letterCount_Ma = strlen(MaPhongBanInput);
        MaPhongBanInput[letterCount_Ma] = '\0';

        strncpy(TenPhongBanInput, selPB->get_TenPhongBan().c_str(), 49);
        letterCount_Ten = strlen(TenPhongBanInput);
        TenPhongBanInput[letterCount_Ten] = '\0';

        textCalendar = selPB->get_NgayThanhLap().ToString();
        needfill = false;
    }

    if (OpenThem)
    {
        Typing(isMaPhongBanActive, MaPhongBanInput, letterCount_Ma, mousePos, textMaPhongBan);
        HieuUngGo(isMaPhongBanActive, textMaPhongBan, MaPhongBanInput, 28);

        Typing(isTenPhongBanActive, TenPhongBanInput, letterCount_Ten, mousePos, textTenPhongBan);
        HieuUngGo(isTenPhongBanActive, textTenPhongBan, TenPhongBanInput, 28);
        DrawTextEx(myFont, textCalendar.c_str(), {textNgayThanhLap.x + 10, textNgayThanhLap.y + 6}, 28, 1, BLACK);
        if (isOpenCalendar)
        {
            DrawCalendar(mousePos, day1, month1, year1, textNgayThanhLap, isOpenCalendar);
        }
    }
    else if (OpenSua && selPB != nullptr)
    { // Ô nhập chữ chế độ sửa
        DrawTextEx(myFont, MaPhongBanInput, {textMaPhongBan.x + 10, textMaPhongBan.y + 6}, 28, 1, BLACK);
        Typing(isTenPhongBanActive, TenPhongBanInput, letterCount_Ten, mousePos, textTenPhongBan);
        HieuUngGo(isTenPhongBanActive, textTenPhongBan, TenPhongBanInput, 28);
        DrawTextEx(myFont, textCalendar.c_str(), {textNgayThanhLap.x + 10, textNgayThanhLap.y + 6}, 28, 1, BLACK);
        if (isOpenCalendar)
        {
            DrawCalendar(mousePos, day1, month1, year1, textNgayThanhLap, isOpenCalendar);
        }
    }
    else if (selPB != nullptr || MaPhongBanInput[0] != '\0')
    {
        // Khi click vào 1 đối tượng trong danh sách (chế độ xem)
        DrawTextEx(myFont, MaPhongBanInput, {textMaPhongBan.x + 10, textMaPhongBan.y + 6}, 28, 1, BLACK);
        DrawTextEx(myFont, TenPhongBanInput, {textTenPhongBan.x + 10, textTenPhongBan.y + 6}, 28, 1, BLACK);
        DrawTextEx(myFont, textCalendar.c_str(), {textNgayThanhLap.x + 10, textNgayThanhLap.y + 6}, 28, 1, BLACK);
    }

    if (isXacNhan)
    {
        if (OpenThem)
        {
            PhongBan *Pbnew = new PhongBan(MaPhongBanInput, TenPhongBanInput, Date(textCalendar));
            try
            {
                QL->ThemPhongBan(Pbnew);
                QL->get_DB()->SaveFilePB();
                needfill = false;
                letterCount_Ma = 0;
                letterCount_Ten = 0;
                OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
                MaPhongBanInput[0] = '\0';
                TenPhongBanInput[0] = '\0';
                day1 = day, month1 = month, year1 = year;
                HienThongBao("Thêm phòng ban mới thành công!", false); // true màu đỏ
            }
            catch (const string &loi)
            {
                needfill = false;
                letterCount_Ma = 0;
                letterCount_Ten = 0;
                OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
                MaPhongBanInput[0] = '\0';
                TenPhongBanInput[0] = '\0';
                day1 = day, month1 = month, year1 = year;
                HienThongBao(loi, true); // true màu đỏ
            }
        }
        else if (OpenSua && selPB != nullptr)
        {
            selPB->set_NgayThanhLap(textCalendar);
            selPB->set_TenPhongBan(TenPhongBanInput);
            QL->get_DB()->SaveFilePB();
            OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
            MaPhongBanInput[0] = '\0';
            TenPhongBanInput[0] = '\0';
            letterCount_Ma = 0;
            letterCount_Ten = 0;
            day1 = day, month1 = month, year1 = year;
            HienThongBao("Cập nhật phòng ban thành công!", false);
        }
    }

    if (isHuy && CheckCollisionPointRec(mousePos, btnHuy))
    {
        OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
        MaPhongBanInput[0] = '\0';
        TenPhongBanInput[0] = '\0';
        letterCount_Ma = 0;
        letterCount_Ten = 0;
        day1 = day, month1 = month, year1 = year;
    }

    DrawTextEx(myFont, ("Tổng phòng ban: " + to_string(QL->get_DB()->get_ListPB()->get_size())).c_str(), {1550, 960}, 25, 1, BLACK);
}

void Draw_Screen_ChucVu(Vector2 mousepos, bool Clicked, QLNV *QL)
{
    static bool isMaChucVuActive = false;
    static bool isTenChucVuActive = false;
    static bool isPhucCapActive = false;
    static bool needfill = false;
    static set<string> dsXoaCV;

    Rectangle panelBang1 = {330, 230, 1450.0f, 320.0f};
    DrawRectangleRounded({panelBang1.x + 3, panelBang1.y + 4, panelBang1.width, panelBang1.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang1, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang1, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Thông tin chức vụ", {360.0f, 250.0f}, 40, 1, BLACK);

    static Texture2D a1;
    static bool isLoaded = false;
    if (!isLoaded)
    {
        a1 = LoadTexture("photo/anhto/cvu.png");
        isLoaded = true;
    }
    Vector2 pa1 = {360.0f, 320.0f}; // Tọa độ (x, y) postion
    float ra1 = 0.0f;               // Góc xoay (0 độ) rotation
    float sa1 = 0.6f;
    DrawTextureEx(a1, pa1, ra1, sa1, WHITE);

    Rectangle MaChucVu = {930, 350, 300, 40};
    static char MaChucVuInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Ma = 0;
    DrawTextEx(myFont, "Mã Chức Vụ:", {MaChucVu.x - 230, MaChucVu.y}, 35, 1, BLACK);
    DrawRectangleRounded(MaChucVu, 0.5f, 10, isMaChucVuActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(MaChucVu, 0.3f, 12, 3.0f, LIGHTGRAY);

    Rectangle TenChucVu = {930, 420, 300, 40};
    static char TenChucVuInput[50] = "\0";
    static int letterCount_Ten = 0;
    DrawTextEx(myFont, "Tên Chức Vụ:", {TenChucVu.x - 230, TenChucVu.y}, 35, 1, BLACK);
    DrawRectangleRounded(TenChucVu, 0.5f, 10, isTenChucVuActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(TenChucVu, 0.3f, 12, 3.0f, LIGHTGRAY);

    Rectangle PhucCap = {930, 490, 300, 40};
    static char PhuCapInput[50] = "\0";
    static int letterCount_PhuCap = 0;
    DrawTextEx(myFont, "Phụ Cấp:", {PhucCap.x - 230, PhucCap.y}, 35, 1, BLACK);
    DrawRectangleRounded(PhucCap, 0.5f, 10, isPhucCapActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(PhucCap, 0.3f, 12, 3.0f, LIGHTGRAY);

    if (Clicked)
    {
        if (CheckCollisionPointRec(mousepos, MaChucVu))
        {
            isMaChucVuActive = true;
            isTenChucVuActive = isPhucCapActive = false;
        }
        else if (CheckCollisionPointRec(mousepos, TenChucVu))
        {
            isTenChucVuActive = true;
            isMaChucVuActive = isPhucCapActive = false;
        }
        else if (CheckCollisionPointRec(mousepos, PhucCap))
        {
            isPhucCapActive = true;
            isMaChucVuActive = isTenChucVuActive = false;
        }
    }

    Rectangle btnThemChucVu = {350, 940, 280, 50};
    static bool OpenThem = false;

    Rectangle btnSuaChucVu = {660, 940, 280, 50};
    static bool OpenSua = false;

    Rectangle btnXoa = {970, 940, 280, 50};
    static bool OpenXoa = false;

    Rectangle btnXacNhan = {350, 940, 230, 50};
    static bool isXacNhan = false;
    Rectangle btnHuy = {610, 940, 230, 50};
    static bool isHuy = false;

    Rectangle btnHuyXoa = {350, 940, 230, 50};
    Rectangle btnXacNhanXoa = {610, 940, 230, 50};

    if (Clicked)
    {
        if (OpenXoa)
        {
            if (CheckCollisionPointRec(mousepos, btnHuyXoa))
            {
                dsXoaCV.clear();
                OpenXoa = false;
            }
            else if (CheckCollisionPointRec(mousepos, btnXacNhanXoa))
            {
                if (dsXoaCV.empty())
                {
                    HienThongBao("Chưa chọn chức vụ nào để xóa!", true);
                }
                else
                {
                    vector<ChucVu *> canXoa;
                    Node<ChucVu> *p = QL->get_DB()->get_ListCV()->get_header()->Next;
                    while (p != nullptr)
                    {
                        if (p->Data && dsXoaCV.find(p->Data->get_MaChucVu()) != dsXoaCV.end())
                        {
                            canXoa.push_back(p->Data);
                        }
                        p = p->Next;
                    }

                    int dem = 0;
                    string loiCuoi = "";
                    for (ChucVu *cv : canXoa)
                    {
                        try
                        {
                            QL->XoaChucVu(cv);
                            dem++;
                        }
                        catch (const string &loi)
                        {
                            loiCuoi = loi;
                        }
                    }

                    if (dem > 0)
                    {
                        QL->get_DB()->SaveFileCV();
                        if (loiCuoi.empty())
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " chức vụ thành công!", false);
                        }
                        else
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " chức vụ. " + loiCuoi, true);
                        }
                    }
                    else
                    {
                        HienThongBao(loiCuoi.empty() ? "Xóa không thành công!" : loiCuoi, true);
                    }

                    dsXoaCV.clear();
                    OpenXoa = false;
                    MaChucVuInput[0] = '\0';
                    TenChucVuInput[0] = '\0';
                    PhuCapInput[0] = '\0';
                    letterCount_Ma = letterCount_Ten = letterCount_PhuCap = 0;
                }
            }
        }
        else if (OpenThem || OpenSua)
        {
            // Chỉ xử lý Xác nhận
            if (CheckCollisionPointRec(mousepos, btnXacNhan))
            {
                isXacNhan = true;
            }
            // Chỉ xử lý Hủy
            else if (CheckCollisionPointRec(mousepos, btnHuy))
            {
                isHuy = true;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mousepos, btnThemChucVu))
            {
                OpenThem = true;
                MaChucVuInput[0] = '\0';
                TenChucVuInput[0] = '\0';
                PhuCapInput[0] = '\0';
                letterCount_Ma = 0;
                letterCount_Ten = 0;
                letterCount_PhuCap = 0;
            }
            else if (CheckCollisionPointRec(mousepos, btnSuaChucVu))
            {
                OpenSua = true;
            }
            else if (CheckCollisionPointRec(mousepos, btnXoa))
            {
                OpenXoa = true;
                dsXoaCV.clear();
            }
        }
    }

    if (OpenXoa)
    {
        DrawRectangleRounded(btnHuyXoa, 0.2f, 10, WHITE);
        DrawRectangleRoundedLinesEx(btnHuyXoa, 0.2f, 10, 1.5f, DARKGRAY);
        DrawTextEx(myFont, "Hủy xóa", {btnHuyXoa.x + 55, btnHuyXoa.y + 10}, 30, 1, BLACK);

        DrawRectangleRounded(btnXacNhanXoa, 0.2f, 10, RED);
        DrawTextEx(myFont, "Xác nhận xoá", {btnXacNhanXoa.x + 32, btnXacNhanXoa.y + 10}, 30, 1, WHITE);
    }
    else if (OpenSua || OpenThem)
    {
        DrawRectangleRounded(btnXacNhan, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Xác nhận", {btnXacNhan.x + 32, btnXacNhan.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnHuy, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Hủy", {btnHuy.x + 70, btnHuy.y + 10}, 30, 1, WHITE);
    }
    else
    {
        DrawRectangleRounded(btnThemChucVu, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Thêm Chức Vụ", {btnThemChucVu.x + 15, btnThemChucVu.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnSuaChucVu, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Sửa Chức Vụ", {btnSuaChucVu.x + 20, btnSuaChucVu.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnXoa, 0.2f, 10, RED);
        DrawTextEx(myFont, "   Xóa Chức Vụ", {btnXoa.x + 65, btnXoa.y + 10}, 30, 1, WHITE);
    }

    Rectangle panelBang2 = {330, 580, 1450.0f, 350.0f};
    DrawRectangleRounded({panelBang2.x + 3, panelBang2.y + 4, panelBang2.width, panelBang2.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang2, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang2, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});
    // 1. TẠO BIẾN CUỘN (Dùng 'static' để giữ nguyên giá trị qua mỗi khung hình)
    static float scrollY = 0.0f;
    float scrollSpeed = 30.0f; // Tốc độ cuộn mỗi nấc chuột

    // Lấy thao tác lăn chuột và cộng dồn vào scrollY
    float wheel = GetMouseWheelMove(); // Lăn chuột xuống trả về -1 ngược lại lên
    if (wheel != 0.0f)
    {
        scrollY += wheel * scrollSpeed;
    }

    // Chặn cuộn quá đà lên trên (danh sách không bị tụt xuống dưới tiêu đề)
    if (scrollY > 0.0f)
        scrollY = 0.0f;
    // Cấu hình khoảng cách và kích thước chữ
    int startX = 350;   // Lề trái
    int startY = 650;   // Lề trên
    int rowHeight = 60; // Chiều cao mỗi dòng
    int fontSize = 30;  // Kích thước chữ

    // Tọa độ X của từng cột (Bạn cần tinh chỉnh các số này cho vừa với màn hình)
    int colX[] = {350, 600, 900, 1250};

    // 1. VẼ TIÊU ĐỀ CỘT (Header)
    float lech = OpenXoa ? 30.f : 0.f;
    DrawTextEx(myFont, "Mã Chức Vụ", {colX[0] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Tên Chức Vụ", {colX[1] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Phụ Cấp", {colX[2] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Số lượng", {colX[3] + lech, (float)startY}, fontSize, 1, DARKGRAY);

    // Kẻ đường gạch ngang dưới tiêu đề
    DrawLine(startX, startY + 30, 1900, startY + 30, LIGHTGRAY);

    // 4. KHU VỰC VẼ DANH SÁCH BỊ CẮT (Scissor Mode)
    int listStartY = startY + 60;                         // Điểm bắt đầu vẽ dữ liệu (ngay dưới đường kẻ)
    int listHeight = GetScreenHeight() - listStartY - 80; // Chiều cao khu vực hiển thị dữ liệu

    // Bật chế độ Scissor: Bất cứ thứ gì vẽ bên ngoài hình chữ nhật này sẽ bị ẩn đi
    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
    Rectangle listArea = {(float)startX, (float)listStartY, (float)(GetScreenWidth() - startX), (float)listHeight};
    // Chỉ hover khi chuột nằm trong vùng danh sách
    bool canHover = CheckCollisionPointRec(mousepos, listArea);

    // Tọa độ Y hiện tại bắt đầu = Tọa độ gốc + Độ lệch cuộn
    float currentY = listStartY + scrollY;

    Node<ChucVu> *current = QL->get_DB()->get_ListCV()->get_header()->Next;

    // Đếm tổng số lượng dòng để chặn cuộn quá đà xuống dưới (tùy chọn nâng cao)
    int totalRows = 0;

    ChucVu *selCV = nullptr;
    static string selectedMCV = "";
    while (current != nullptr)
    {
        string mcv = current->Data->get_MaChucVu();
        string name = current->Data->get_TenChucVu();
        string pc = to_string(current->Data->get_PhuCap());
        int size = current->Data->get_ListNV()->get_size();
        string cv_size = to_string(size);

        // ----- HOVER + CHỌN DÒNG -----
        Rectangle rowRect = {(float)startX, currentY - 5, (float)(GetScreenWidth() - startX - 20), 35};
        bool isHover = canHover && CheckCollisionPointRec(mousepos, rowRect);
        if (isHover && Clicked)
        {
            if (OpenXoa)
            {
                if (dsXoaCV.find(mcv) != dsXoaCV.end())
                    dsXoaCV.erase(mcv);
                else
                    dsXoaCV.insert(mcv);
            }
            else
            {
                selectedMCV = mcv;
                needfill = true;
            }
        }
        bool isSelected = (mcv == selectedMCV);

        if (OpenXoa)
        {
            if (dsXoaCV.find(mcv) != dsXoaCV.end())
            {
                DrawRectangleRec(rowRect, Color{255, 230, 230, 255});
            }
            else if (isHover)
            {
                DrawRectangleRec(rowRect, Color{245, 245, 250, 255});
            }

            Rectangle box = {(float)colX[0] + 2, currentY + 4, 20, 20};
            DrawRectangleRec(box, WHITE);
            DrawRectangleLinesEx(box, 2, DARKGRAY);
            if (dsXoaCV.find(mcv) != dsXoaCV.end())
            {
                DrawRectangleRec({box.x + 4, box.y + 4, box.width - 8, box.height - 8}, DARKGREEN);
            }
        }
        else if (isSelected)
        {
            // đã chọn: đậm hơn; vừa chọn vừa rê chuột: đậm thêm chút
            DrawRectangleRec(rowRect, isHover ? Color{180, 195, 230, 255} : Color{200, 210, 235, 255});
            // lưu chức vụ đang chọn
            selCV = current->Data;
        }
        else if (isHover)
        {
            DrawRectangleRec(rowRect, Color{235, 235, 240, 255});
        }
        // ------------------------------

        DrawTextEx(myFont, mcv.c_str(), {colX[0] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, name.c_str(), {colX[1] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, pc.c_str(), {colX[2] + lech, currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, cv_size.c_str(), {colX[3] + lech, currentY}, fontSize, 1, BLACK);
        DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));

        currentY += rowHeight;
        totalRows++;

        current = current->Next;
    }

    // Tính toán chiều cao tổng của danh sách để chặn cuộn quá đáy
    float totalListHeight = totalRows * rowHeight;
    float maxScroll = listHeight - totalListHeight;
    // Nếu danh sách dài hơn màn hình và cuộn quá mức dưới, ép nó về mức tối đa
    if (totalListHeight > listHeight && scrollY < maxScroll)
    {
        scrollY = maxScroll;
    }
    // Nếu danh sách ngắn hơn màn hình, không cho cuộn
    else if (totalListHeight <= listHeight)
    {
        scrollY = 0.0f;
    }

    // Tắt chế độ Scissor để các phần vẽ sau (nếu có) không bị ảnh hưởng
    EndScissorMode();

    if (needfill && selCV != nullptr)
    {
        strncpy(MaChucVuInput, selCV->get_MaChucVu().c_str(), 49);
        letterCount_Ma = strlen(MaChucVuInput);
        MaChucVuInput[letterCount_Ma] = '\0';

        strncpy(TenChucVuInput, selCV->get_TenChucVu().c_str(), 49);
        letterCount_Ten = strlen(TenChucVuInput);
        TenChucVuInput[letterCount_Ten] = '\0';

        snprintf(PhuCapInput, sizeof(PhuCapInput), "%lld", (long long)selCV->get_PhuCap());
        letterCount_PhuCap = strlen(PhuCapInput);

        needfill = false;
    }

    if (OpenThem)
    {
        Typing(isMaChucVuActive, MaChucVuInput, letterCount_Ma, mousepos, MaChucVu);
        HieuUngGo(isMaChucVuActive, MaChucVu, MaChucVuInput, 28);

        Typing(isTenChucVuActive, TenChucVuInput, letterCount_Ten, mousepos, TenChucVu);
        HieuUngGo(isTenChucVuActive, TenChucVu, TenChucVuInput, 28);

        Typing(isPhucCapActive, PhuCapInput, letterCount_PhuCap, mousepos, PhucCap);
        HieuUngGo(isPhucCapActive, PhucCap, PhuCapInput, 28);
    }
    else if (OpenSua && selCV != nullptr)
    { // Ô nhập chữ chế độ sửa
        DrawTextEx(myFont, MaChucVuInput, {MaChucVu.x + 10, MaChucVu.y + 6}, 28, 1, BLACK);

        Typing(isTenChucVuActive, TenChucVuInput, letterCount_Ten, mousepos, TenChucVu);
        HieuUngGo(isTenChucVuActive, TenChucVu, TenChucVuInput, 28);

        Typing(isPhucCapActive, PhuCapInput, letterCount_PhuCap, mousepos, PhucCap);
        HieuUngGo(isPhucCapActive, PhucCap, PhuCapInput, 28);
    }
    else if (selCV != nullptr || MaChucVuInput[0] != '\0')
    {
        // Khi click vào 1 đối tượng trong danh sách (chế độ xem)
        DrawTextEx(myFont, MaChucVuInput, {MaChucVu.x + 10, MaChucVu.y + 6}, 28, 1, BLACK);
        DrawTextEx(myFont, TenChucVuInput, {TenChucVu.x + 10, TenChucVu.y + 6}, 28, 1, BLACK);
        DrawTextEx(myFont, PhuCapInput, {PhucCap.x + 10, PhucCap.y + 6}, 28, 1, BLACK);
    }

    if (isXacNhan)
    {
        if (OpenThem)
        {
            double pc = 0.0;
            try
            {
                pc = stod(PhuCapInput);
            }
            catch (...)
            {
                pc = 0.0;
            }
            ChucVu *CVnew = new ChucVu(MaChucVuInput, TenChucVuInput, pc);
            try
            {
                QL->ThemChucVu(CVnew);
                QL->get_DB()->SaveFileCV();
                needfill = false;
                letterCount_Ma = 0;
                letterCount_Ten = 0;
                letterCount_PhuCap = 0;
                OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
                MaChucVuInput[0] = '\0';
                TenChucVuInput[0] = '\0';
                PhuCapInput[0] = '\0';
                HienThongBao("Thêm chức vụ mới thành công!", false);
            }
            catch (const string &loi)
            {
                needfill = false;
                letterCount_Ma = 0;
                letterCount_Ten = 0;
                letterCount_PhuCap = 0;
                OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
                MaChucVuInput[0] = '\0';
                TenChucVuInput[0] = '\0';
                PhuCapInput[0] = '\0';
                HienThongBao(loi, true);
            }
        }
        else if (OpenSua && selCV != nullptr)
        {
            selCV->set_TenChucVu(TenChucVuInput);
            try
            {
                selCV->set_PhuCap(stod(PhuCapInput));
            }
            catch (...)
            {
            }
            QL->get_DB()->SaveFileCV();
            OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
            MaChucVuInput[0] = '\0';
            TenChucVuInput[0] = '\0';
            PhuCapInput[0] = '\0';
            letterCount_Ma = 0;
            letterCount_Ten = 0;
            letterCount_PhuCap = 0;
            HienThongBao("Cập nhật chức vụ thành công!", false);
        }
    }

    if (isHuy && CheckCollisionPointRec(mousepos, btnHuy))
    {
        OpenSua = OpenThem = OpenXoa = isHuy = isXacNhan = false;
        MaChucVuInput[0] = '\0';
        TenChucVuInput[0] = '\0';
        PhuCapInput[0] = '\0';
        letterCount_Ma = 0;
        letterCount_Ten = 0;
        letterCount_PhuCap = 0;
    }

    DrawTextEx(myFont, ("Tổng chức vụ: " + to_string(QL->get_DB()->get_ListCV()->get_size())).c_str(), {1550, 960}, 25, 1, BLACK);
}

void Draw_Screen_Deparment(QLNV *QL)
{
    static Texture2D a1;
    static bool isLoaded = false;
    if (!isLoaded)
    {
        a1 = LoadTexture("photo/icon/Asset-2.png");
        isLoaded = true;
    }
    Vector2 pa1 = {370.0f, 45.0f}; // Tọa độ (x, y) postion
    float ra1 = 0.0f;              // Góc xoay (0 độ) rotation
    float sa1 = 0.33f;
    DrawTextureEx(a1, pa1, ra1, sa1, WHITE);
    DrawTextEx(tieude, "Quản Lý Phòng Ban/Chức Vụ", {pa1.x + 100, pa1.y}, 45, 1, WHITE);
    DrawTextEx(myFont, "Danh sách và thông tin phòng ban/chức vụ của công ty", {pa1.x + 100, pa1.y + 50}, 30, 1, WHITE);
    static bool isOpenPB = false;
    static bool isOpenCV = true;
    Vector2 mousePos = GetMousePosition();
    bool Clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float TabX = 330.0f;
    float TabY = 160.0f;
    float TabH = 90.0f;
    Rectangle TabPB = {TabX, TabY, 300.0f, TabH};
    Rectangle TabCV = {TabX + 310.0f, TabY, 300.0f, TabH};
    bool hoverPB = CheckCollisionPointRec(mousePos, TabPB);
    bool hoverCV = CheckCollisionPointRec(mousePos, TabCV);
    Color CTabPB = isOpenPB == 1 ? BLUE : WHITE;
    Color CTabVT = isOpenCV == 1 ? BLUE : WHITE;
    if (isOpenPB == true && hoverCV)
        CTabVT = Fade(BLUE, 0.4);
    if (isOpenCV == true && hoverPB)
        CTabPB = Fade(BLUE, 0.4);
    DrawRectangleRounded(TabPB, 0.3f, 10, CTabPB);
    DrawRectangleRoundedLinesEx(TabPB, 0.3f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Phòng ban", {TabPB.x + 70.0f, TabPB.y + 15.0f}, 40, 1, BLACK);

    DrawRectangleRounded(TabCV, 0.3f, 10, CTabVT);
    DrawRectangleRoundedLinesEx(TabPB, 0.3f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Chức vụ", {TabCV.x + 90.0f, TabCV.y + 15.0f}, 40, 1, BLACK);

    Rectangle panelBang = {330, 230, 1450.0f, 690.0f};
    DrawRectangleRounded({panelBang.x + 3, panelBang.y + 4, panelBang.width, panelBang.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});

    if (Clicked && CheckCollisionPointRec(mousePos, TabPB))
    {
        isOpenPB = true;
        isOpenCV = false;
    }
    else if (Clicked && CheckCollisionPointRec(mousePos, TabCV))
    {
        isOpenCV = true;
        isOpenPB = false;
    }

    if (isOpenPB)
    {
        Draw_Screen_PhongBan(mousePos, Clicked, QL);
    }
    else if (isOpenCV)
    {
        Draw_Screen_ChucVu(mousePos, Clicked, QL);
    }
}

void Draw_Screen_BangLuong(Vector2 mousePos, bool Clicked, QLNV *QL)
{
    static bool isOpenFT = true;
    static bool isOpenPT = false;

    // Khung chứa dài hết cỡ chiều ngang (1450px)
    Rectangle panelBang = {330, 230, 1450.0f, 95.0f};
    DrawRectangleRounded({panelBang.x + 3, panelBang.y + 4, panelBang.width, panelBang.height}, 0.04f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang, 0.04f, 10, Color{242, 245, 252, 255});
    DrawRectangleRoundedLinesEx(panelBang, 0.04f, 10, 1.5f, Color{218, 226, 242, 255});

    // 2 Tab chia đều dài hết cỡ theo chiều ngang
    float tabMargin = 10.0f;
    float tabGap = 10.0f;
    float tabW = (panelBang.width - 2 * tabMargin - tabGap) / 2.0f; // 710px mỗi tab
    float tabH = 75.0f;
    float tabY = panelBang.y + tabMargin;

    Rectangle TabFT = {panelBang.x + tabMargin, tabY, tabW, tabH};
    Rectangle TabPT = {panelBang.x + tabMargin + tabW + tabGap, tabY, tabW, tabH};

    bool hoverFT = CheckCollisionPointRec(mousePos, TabFT);
    bool hoverPT = CheckCollisionPointRec(mousePos, TabPT);

    // Màu sắc nổi bật, không dùng màu trắng để nhìn rõ ràng
    Color cActiveBg = Color{55, 100, 235, 255}; // Xanh dương nổi bật khi chọn
    Color cActiveBorder = Color{40, 80, 210, 255};
    Color cInactiveBg = Color{230, 236, 247, 255}; // Màu xám xanh nhẹ rõ nét khi chưa chọn
    Color cHoverBg = Color{218, 226, 242, 255};    // Màu khi rê chuột
    Color cInactiveBorder = Color{200, 212, 230, 255};

    Color CTabFT = isOpenFT ? cActiveBg : (hoverFT ? cHoverBg : cInactiveBg);
    Color borderFT = isOpenFT ? cActiveBorder : cInactiveBorder;
    Color textFT = isOpenFT ? WHITE : Color{55, 65, 85, 255};

    Color CTabPT = isOpenPT ? cActiveBg : (hoverPT ? cHoverBg : cInactiveBg);
    Color borderPT = isOpenPT ? cActiveBorder : cInactiveBorder;
    Color textPT = isOpenPT ? WHITE : Color{55, 65, 85, 255};

    // Vẽ Tab FullTime
    if (isOpenFT)
        DrawRectangleRounded({TabFT.x + 2, TabFT.y + 3, TabFT.width, TabFT.height}, 0.25f, 8, Fade(BLACK, 0.12f));
    DrawRectangleRounded(TabFT, 0.25f, 8, CTabFT);
    DrawRectangleRoundedLinesEx(TabFT, 0.25f, 8, 1.5f, borderFT);
    float twFT = MeasureTextEx(tieude, "Full Time", 36, 1).x;
    DrawTextEx(tieude, "Full Time", {TabFT.x + (TabFT.width - twFT) / 2.0f, TabFT.y + (TabFT.height - 36) / 2.0f}, 36, 1, textFT);

    // Vẽ Tab PartTime
    if (isOpenPT)
        DrawRectangleRounded({TabPT.x + 2, TabPT.y + 3, TabPT.width, TabPT.height}, 0.25f, 8, Fade(BLACK, 0.12f));
    DrawRectangleRounded(TabPT, 0.25f, 8, CTabPT);
    DrawRectangleRoundedLinesEx(TabPT, 0.25f, 8, 1.5f, borderPT);
    float twPT = MeasureTextEx(tieude, "Part Time", 36, 1).x;
    DrawTextEx(tieude, "Part Time", {TabPT.x + (TabPT.width - twPT) / 2.0f, TabPT.y + (TabPT.height - 36) / 2.0f}, 36, 1, textPT);

    if (Clicked && CheckCollisionPointRec(mousePos, TabFT))
    {
        isOpenFT = true;
        isOpenPT = false;
    }
    else if (Clicked && CheckCollisionPointRec(mousePos, TabPT))
    {
        isOpenPT = true;
        isOpenFT = false;
    }

    if (isOpenFT)
    {
        Draw_Screen_FullTime(mousePos, Clicked, QL);
    }
    else if (isOpenPT)
    {
        Draw_Screen_PartTime(mousePos, Clicked, QL);
    }
}

void Draw_Screen_MucLuong(Vector2 mousePos, bool Clicked, QLNV *QL)
{
    static bool isTabFullTime = true; // true: FullTime, false: PartTime

    static bool isMaVTActive = false;
    static bool isBacActive = false;
    static bool isLuongActive = false;
    static bool needfill = false;
    static set<string> dsXoaML;
    static string selectedMLKey = "";

    static char MaVTInput[50] = "\0";
    static int letterCount_Ma = 0;

    static char BacInput[50] = "\0";
    static int letterCount_Bac = 0;

    static char LuongInput[50] = "\0";
    static int letterCount_Luong = 0;

    static bool OpenThem = false;
    static bool OpenSua = false;
    static bool OpenXoa = false;

    // =========== 2 TAB SWITCHER: FULLTIME / PARTTIME ===========
    float tabMargin = 10.0f;
    float tabGap = 10.0f;
    float panelTabW = 1450.0f;
    float tabW = (panelTabW - 2 * tabMargin - tabGap) / 2.0f; // 710px
    float tabH = 60.0f;
    float tabY = 230.0f;

    Rectangle panelTabs = {330, tabY - 5, panelTabW, tabH + 10};
    DrawRectangleRounded({panelTabs.x + 3, panelTabs.y + 4, panelTabs.width, panelTabs.height}, 0.04f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelTabs, 0.04f, 10, Color{242, 245, 252, 255});
    DrawRectangleRoundedLinesEx(panelTabs, 0.04f, 10, 1.5f, Color{218, 226, 242, 255});

    Rectangle TabFT = {panelTabs.x + tabMargin, tabY, tabW, tabH};
    Rectangle TabPT = {panelTabs.x + tabMargin + tabW + tabGap, tabY, tabW, tabH};

    bool hoverFT = CheckCollisionPointRec(mousePos, TabFT);
    bool hoverPT = CheckCollisionPointRec(mousePos, TabPT);

    Color cActiveBg = Color{55, 100, 235, 255};
    Color cActiveBorder = Color{40, 80, 210, 255};
    Color cInactiveBg = Color{230, 236, 247, 255};
    Color cHoverBg = Color{218, 226, 242, 255};
    Color cInactiveBorder = Color{200, 212, 230, 255};

    Color CTabFT = isTabFullTime ? cActiveBg : (hoverFT ? cHoverBg : cInactiveBg);
    Color borderFT = isTabFullTime ? cActiveBorder : cInactiveBorder;
    Color textFT = isTabFullTime ? WHITE : Color{55, 65, 85, 255};

    Color CTabPT = !isTabFullTime ? cActiveBg : (hoverPT ? cHoverBg : cInactiveBg);
    Color borderPT = !isTabFullTime ? cActiveBorder : cInactiveBorder;
    Color textPT = !isTabFullTime ? WHITE : Color{55, 65, 85, 255};

    if (isTabFullTime)
        DrawRectangleRounded({TabFT.x + 2, TabFT.y + 3, TabFT.width, TabFT.height}, 0.25f, 8, Fade(BLACK, 0.12f));
    DrawRectangleRounded(TabFT, 0.25f, 8, CTabFT);
    DrawRectangleRoundedLinesEx(TabFT, 0.25f, 8, 1.5f, borderFT);
    float twFT = MeasureTextEx(tieude, "Mức Lương Full Time", 32, 1).x;
    DrawTextEx(tieude, "Mức Lương Full Time", {TabFT.x + (TabFT.width - twFT) / 2.0f, TabFT.y + (TabFT.height - 32) / 2.0f}, 32, 1, textFT);

    if (!isTabFullTime)
        DrawRectangleRounded({TabPT.x + 2, TabPT.y + 3, TabPT.width, TabPT.height}, 0.25f, 8, Fade(BLACK, 0.12f));
    DrawRectangleRounded(TabPT, 0.25f, 8, CTabPT);
    DrawRectangleRoundedLinesEx(TabPT, 0.25f, 8, 1.5f, borderPT);
    float twPT = MeasureTextEx(tieude, "Mức Lương Part Time", 32, 1).x;
    DrawTextEx(tieude, "Mức Lương Part Time", {TabPT.x + (TabPT.width - twPT) / 2.0f, TabPT.y + (TabPT.height - 32) / 2.0f}, 32, 1, textPT);

    if (Clicked && CheckCollisionPointRec(mousePos, TabFT) && !isTabFullTime)
    {
        isTabFullTime = true;
        selectedMLKey = "";
        MaVTInput[0] = BacInput[0] = LuongInput[0] = '\0';
        letterCount_Ma = letterCount_Bac = letterCount_Luong = 0;
        dsXoaML.clear();
        OpenThem = OpenSua = OpenXoa = false;
    }
    else if (Clicked && CheckCollisionPointRec(mousePos, TabPT) && isTabFullTime)
    {
        isTabFullTime = false;
        selectedMLKey = "";
        MaVTInput[0] = BacInput[0] = LuongInput[0] = '\0';
        letterCount_Ma = letterCount_Bac = letterCount_Luong = 0;
        dsXoaML.clear();
        OpenThem = OpenSua = OpenXoa = false;
    }

    // =========== BẢNG 1: THÔNG TIN MỨC LƯƠNG ===========
    Rectangle panelBang1 = {330, 305, 1450.0f, 225.0f};
    DrawRectangleRounded({panelBang1.x + 3, panelBang1.y + 4, panelBang1.width, panelBang1.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang1, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang1, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});

    string loaiNV = isTabFullTime ? "FullTime" : "PartTime";
    DrawTextEx(myFont, ("Thông tin mức lương (" + loaiNV + ")").c_str(), {360.0f, 315.0f}, 36, 1, BLACK);

    static Texture2D iconML;
    static bool isIconLoaded = false;
    if (!isIconLoaded)
    {
        iconML = LoadTexture("photo/anhto/vtri.png");
        isIconLoaded = true;
    }
    DrawTextureEx(iconML, {360.0f, 360.0f}, 0.0f, 0.5f, WHITE);

    // Ô nhập Mã vị trí
    Rectangle textMaVT = {870, 355, 300, 40};
    DrawTextEx(myFont, "Mã vị trí:", {680.0f, 355.0f}, 32, 1, BLACK);
    DrawRectangleRounded(textMaVT, 0.5f, 10, isMaVTActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textMaVT, 0.3f, 12, 3.0f, LIGHTGRAY);

    ViTriViecLam *vtFound = QL->get_DB()->FindVTVL(MaVTInput);
    if (vtFound != nullptr)
    {
        DrawTextEx(myFont, vtFound->get_TenVTVL().c_str(), {1190.0f, 360.0f}, 28, 1, DARKGREEN);
    }
    else if (letterCount_Ma > 0)
    {
        DrawTextEx(myFont, "(Chưa có vị trí)", {1190.0f, 360.0f}, 24, 1, MAROON);
    }

    // Ô nhập Bậc
    Rectangle textBac = {870, 410, 300, 40};
    DrawTextEx(myFont, "Bậc vị trí:", {680.0f, 410.0f}, 32, 1, BLACK);
    DrawRectangleRounded(textBac, 0.5f, 10, isBacActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textBac, 0.3f, 12, 3.0f, LIGHTGRAY);
    DrawTextEx(myFont, "(1, 2, 3...)", {1190.0f, 415.0f}, 24, 1, DARKGRAY);

    // Ô nhập Lương
    Rectangle textLuong = {870, 465, 300, 40};
    DrawTextEx(myFont, isTabFullTime ? "Lương tháng:" : "Lương/giờ:", {680.0f, 465.0f}, 32, 1, BLACK);
    DrawRectangleRounded(textLuong, 0.5f, 10, isLuongActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textLuong, 0.3f, 12, 3.0f, LIGHTGRAY);

    if (letterCount_Luong > 0)
    {
        long long valLuong = atoll(LuongInput);
        if (valLuong > 0)
        {
            string fmt = FormatSoTien(valLuong) + (isTabFullTime ? " đ/tháng" : " đ/h");
            DrawTextEx(myFont, fmt.c_str(), {1190.0f, 470.0f}, 28, 1, Color{40, 80, 200, 255});
        }
    }

    if (Clicked)
    {
        if (CheckCollisionPointRec(mousePos, textMaVT))
        {
            isMaVTActive = true;
            isBacActive = isLuongActive = false;
        }
        else if (CheckCollisionPointRec(mousePos, textBac))
        {
            isBacActive = true;
            isMaVTActive = isLuongActive = false;
        }
        else if (CheckCollisionPointRec(mousePos, textLuong))
        {
            isLuongActive = true;
            isMaVTActive = isBacActive = false;
        }
    }

    // =========== BẢNG 2: DANH SÁCH MỨC LƯƠNG ===========
    Rectangle panelBang2 = {330, 540, 1450.0f, 385.0f};
    DrawRectangleRounded({panelBang2.x + 3, panelBang2.y + 4, panelBang2.width, panelBang2.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang2, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang2, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});

    static float scrollY = 0.0f;
    float scrollSpeed = 30.0f;
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
    {
        scrollY += wheel * scrollSpeed;
    }
    if (scrollY > 0.0f)
        scrollY = 0.0f;

    int startX = 350;
    int startY = 560;
    int rowHeight = 60;
    int fontSize = 30;

    int colX[] = {350, 520, 850, 1060, 1370};

    float lech = OpenXoa ? 30.f : 0.f;
    DrawTextEx(myFont, "Mã Vị Trí", {colX[0] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Tên Vị Trí", {colX[1] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Bậc", {colX[2] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, isTabFullTime ? "Lương Tháng" : "Lương Theo Giờ", {colX[3] + lech, (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Số Nhân Viên", {colX[4] + lech, (float)startY}, fontSize, 1, DARKGRAY);

    DrawLine(startX, startY + 35, 1750, startY + 35, LIGHTGRAY);

    int listStartY = startY + 50;
    int listHeight = 310;

    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
    Rectangle listArea = {(float)startX, (float)listStartY, (float)(GetScreenWidth() - startX), (float)listHeight};
    bool canHover = CheckCollisionPointRec(mousePos, listArea);

    float currentY = listStartY + scrollY;

    Node<MucLuong> *current = QL->get_DB()->get_ListML()->get_header()->Next;
    int totalRows = 0;
    MucLuong *selML = nullptr;

    while (current != nullptr)
    {
        if (current->Data && current->Data->get_Loai_NV() == loaiNV)
        {
            MucLuong *ml = current->Data;
            string mvt = ml->get_Ma_VTVL();
            string tenVT = ml->get_VTVL() ? ml->get_VTVL()->get_TenVTVL() : "";
            int bac = ml->get_Bac_VTVL();
            long long luong = (long long)ml->get_Luong();
            int soNV = QL->DemNVThamChieuML(mvt, bac, loaiNV);
            string key = mvt + "_" + to_string(bac);

            Rectangle rowRect = {(float)startX, currentY - 5, (float)(GetScreenWidth() - startX - 20), 35};
            bool isHover = canHover && CheckCollisionPointRec(mousePos, rowRect);
            if (isHover && Clicked)
            {
                if (OpenXoa)
                {
                    if (dsXoaML.find(key) != dsXoaML.end())
                        dsXoaML.erase(key);
                    else
                        dsXoaML.insert(key);
                }
                else
                {
                    selectedMLKey = key;
                    needfill = true;
                }
            }

            bool isSelected = (key == selectedMLKey);

            if (OpenXoa)
            {
                if (dsXoaML.find(key) != dsXoaML.end())
                {
                    DrawRectangleRec(rowRect, Color{255, 230, 230, 255});
                }
                else if (isHover)
                {
                    DrawRectangleRec(rowRect, Color{245, 245, 250, 255});
                }

                Rectangle box = {(float)colX[0] + 2, currentY + 4, 20, 20};
                DrawRectangleRec(box, WHITE);
                DrawRectangleLinesEx(box, 2, DARKGRAY);
                if (dsXoaML.find(key) != dsXoaML.end())
                {
                    DrawRectangleRec({box.x + 4, box.y + 4, box.width - 8, box.height - 8}, DARKGREEN);
                }
            }
            else if (isSelected)
            {
                DrawRectangleRec(rowRect, isHover ? Color{180, 195, 230, 255} : Color{200, 210, 235, 255});
                selML = ml;
            }
            else if (isHover)
            {
                DrawRectangleRec(rowRect, Color{235, 235, 240, 255});
            }

            DrawTextEx(myFont, mvt.c_str(), {colX[0] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, tenVT.c_str(), {colX[1] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, to_string(bac).c_str(), {colX[2] + lech, currentY}, fontSize, 1, BLACK);

            string strLuong = FormatSoTien(luong) + (isTabFullTime ? " đ" : " đ/h");
            DrawTextEx(myFont, strLuong.c_str(), {colX[3] + lech, currentY}, fontSize, 1, BLACK);
            DrawTextEx(myFont, to_string(soNV).c_str(), {colX[4] + lech, currentY}, fontSize, 1, BLACK);
            DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));

            currentY += rowHeight;
            totalRows++;
        }
        current = current->Next;
    }

    float totalListHeight = totalRows * rowHeight;
    float maxScroll = listHeight - totalListHeight;
    if (totalListHeight > listHeight && scrollY < maxScroll)
    {
        scrollY = maxScroll;
    }
    else if (totalListHeight <= listHeight)
    {
        scrollY = 0.0f;
    }

    EndScissorMode();

    if (needfill && selML != nullptr)
    {
        strncpy(MaVTInput, selML->get_Ma_VTVL().c_str(), 49);
        letterCount_Ma = strlen(MaVTInput);
        MaVTInput[letterCount_Ma] = '\0';

        strncpy(BacInput, to_string(selML->get_Bac_VTVL()).c_str(), 49);
        letterCount_Bac = strlen(BacInput);
        BacInput[letterCount_Bac] = '\0';

        snprintf(LuongInput, sizeof(LuongInput), "%lld", (long long)selML->get_Luong());
        letterCount_Luong = strlen(LuongInput);

        needfill = false;
    }

    if (OpenThem)
    {
        Typing(isMaVTActive, MaVTInput, letterCount_Ma, mousePos, textMaVT);
        HieuUngGo(isMaVTActive, textMaVT, MaVTInput, 28);

        Typing(isBacActive, BacInput, letterCount_Bac, mousePos, textBac);
        HieuUngGo(isBacActive, textBac, BacInput, 28);

        Typing(isLuongActive, LuongInput, letterCount_Luong, mousePos, textLuong);
        HieuUngGo(isLuongActive, textLuong, LuongInput, 28);
    }
    else if (OpenSua && selML != nullptr)
    {
        DrawTextEx(myFont, MaVTInput, {textMaVT.x + 10, textMaVT.y + 6}, 28, 1, DARKGRAY);
        DrawTextEx(myFont, BacInput, {textBac.x + 10, textBac.y + 6}, 28, 1, DARKGRAY);

        Typing(isLuongActive, LuongInput, letterCount_Luong, mousePos, textLuong);
        HieuUngGo(isLuongActive, textLuong, LuongInput, 28);
    }
    else if (selML != nullptr || MaVTInput[0] != '\0')
    {
        DrawTextEx(myFont, MaVTInput, {textMaVT.x + 10, textMaVT.y + 6}, 28, 1, BLACK);
        DrawTextEx(myFont, BacInput, {textBac.x + 10, textBac.y + 6}, 28, 1, BLACK);
        DrawTextEx(myFont, LuongInput, {textLuong.x + 10, textLuong.y + 6}, 28, 1, BLACK);
    }

    // =========== CÁC NÚT ĐIỀU KHIỂN DƯỚI CÙNG ===========
    Rectangle btnThemMucLuong = {350, 940, 280, 50};
    Rectangle btnSuaMucLuong = {660, 940, 280, 50};
    Rectangle btnXoaMucLuong = {970, 940, 280, 50};

    Rectangle btnXacNhan = {350, 940, 230, 50};
    Rectangle btnHuy = {610, 940, 230, 50};

    Rectangle btnHuyXoa = {350, 940, 230, 50};
    Rectangle btnXacNhanXoa = {610, 940, 230, 50};

    if (Clicked)
    {
        if (OpenXoa)
        {
            if (CheckCollisionPointRec(mousePos, btnHuyXoa))
            {
                dsXoaML.clear();
                OpenXoa = false;
            }
            else if (CheckCollisionPointRec(mousePos, btnXacNhanXoa))
            {
                if (dsXoaML.empty())
                {
                    HienThongBao("Chưa chọn mức lương nào để xóa!", true);
                }
                else
                {
                    vector<MucLuong *> canXoa;
                    Node<MucLuong> *p = QL->get_DB()->get_ListML()->get_header()->Next;
                    while (p != nullptr)
                    {
                        if (p->Data && p->Data->get_Loai_NV() == loaiNV)
                        {
                            string key = p->Data->get_Ma_VTVL() + "_" + to_string(p->Data->get_Bac_VTVL());
                            if (dsXoaML.find(key) != dsXoaML.end())
                            {
                                canXoa.push_back(p->Data);
                            }
                        }
                        p = p->Next;
                    }

                    int dem = 0;
                    string loiCuoi = "";
                    for (MucLuong *ml : canXoa)
                    {
                        try
                        {
                            QL->XoaMucLuong(ml);
                            dem++;
                        }
                        catch (const string &loi)
                        {
                            loiCuoi = loi;
                        }
                    }

                    if (dem > 0)
                    {
                        QL->get_DB()->SaveFileML();
                        if (loiCuoi.empty())
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " mức lương thành công!", false);
                        }
                        else
                        {
                            HienThongBao("Đã xóa " + to_string(dem) + " mức lương. " + loiCuoi, true);
                        }
                    }
                    else
                    {
                        HienThongBao(loiCuoi.empty() ? "Xóa không thành công!" : loiCuoi, true);
                    }

                    dsXoaML.clear();
                    OpenXoa = false;
                    selectedMLKey = "";
                    MaVTInput[0] = BacInput[0] = LuongInput[0] = '\0';
                    letterCount_Ma = letterCount_Bac = letterCount_Luong = 0;
                }
            }
        }
        else if (OpenThem || OpenSua)
        {
            if (CheckCollisionPointRec(mousePos, btnXacNhan))
            {
                if (OpenThem)
                {
                    ViTriViecLam *vt = QL->get_DB()->FindVTVL(MaVTInput);
                    if (vt == nullptr)
                    {
                        HienThongBao("Mã vị trí không tồn tại trong hệ thống!", true);
                    }
                    else
                    {
                        int bac = atoi(BacInput);
                        double luong = atof(LuongInput);
                        if (bac <= 0)
                        {
                            HienThongBao("Bậc vị trí phải lớn hơn 0!", true);
                        }
                        else if (luong <= 0)
                        {
                            HienThongBao("Mức lương phải lớn hơn 0!", true);
                        }
                        else
                        {
                            MucLuong *mlNew = nullptr;
                            if (isTabFullTime)
                                mlNew = new MucLuong_FullTime(vt, bac, luong);
                            else
                                mlNew = new MucLuong_PartTime(vt, bac, luong);

                            try
                            {
                                QL->ThemMucLuong(mlNew);
                                QL->get_DB()->SaveFileML();
                                OpenThem = false;
                                MaVTInput[0] = BacInput[0] = LuongInput[0] = '\0';
                                letterCount_Ma = letterCount_Bac = letterCount_Luong = 0;
                                HienThongBao("Thêm mức lương mới thành công!", false);
                            }
                            catch (const string &loi)
                            {
                                delete mlNew;
                                HienThongBao(loi, true);
                            }
                        }
                    }
                }
                else if (OpenSua && selML != nullptr)
                {
                    double luong = atof(LuongInput);
                    if (luong <= 0)
                    {
                        HienThongBao("Mức lương phải lớn hơn 0!", true);
                    }
                    else
                    {
                        selML->set_Luong(luong);
                        QL->get_DB()->SaveFileML();
                        OpenSua = false;
                        MaVTInput[0] = BacInput[0] = LuongInput[0] = '\0';
                        letterCount_Ma = letterCount_Bac = letterCount_Luong = 0;
                        HienThongBao("Cập nhật mức lương thành công!", false);
                    }
                }
            }
            else if (CheckCollisionPointRec(mousePos, btnHuy))
            {
                OpenThem = OpenSua = false;
                MaVTInput[0] = BacInput[0] = LuongInput[0] = '\0';
                letterCount_Ma = letterCount_Bac = letterCount_Luong = 0;
            }
        }
        else
        {
            if (CheckCollisionPointRec(mousePos, btnThemMucLuong))
            {
                OpenThem = true;
                selectedMLKey = "";
                MaVTInput[0] = BacInput[0] = LuongInput[0] = '\0';
                letterCount_Ma = letterCount_Bac = letterCount_Luong = 0;
            }
            else if (CheckCollisionPointRec(mousePos, btnSuaMucLuong))
            {
                if (selML == nullptr)
                {
                    HienThongBao("Vui lòng chọn 1 mức lương trong danh sách để sửa!", true);
                }
                else
                {
                    OpenSua = true;
                }
            }
            else if (CheckCollisionPointRec(mousePos, btnXoaMucLuong))
            {
                OpenXoa = true;
                dsXoaML.clear();
            }
        }
    }

    if (OpenXoa)
    {
        DrawRectangleRounded(btnHuyXoa, 0.2f, 10, WHITE);
        DrawRectangleRoundedLinesEx(btnHuyXoa, 0.2f, 10, 1.5f, DARKGRAY);
        DrawTextEx(myFont, "Hủy xóa", {btnHuyXoa.x + 55, btnHuyXoa.y + 10}, 30, 1, BLACK);

        DrawRectangleRounded(btnXacNhanXoa, 0.2f, 10, RED);
        DrawTextEx(myFont, "Xác nhận xoá", {btnXacNhanXoa.x + 32, btnXacNhanXoa.y + 10}, 30, 1, WHITE);
    }
    else if (OpenSua || OpenThem)
    {
        DrawRectangleRounded(btnXacNhan, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Xác nhận", {btnXacNhan.x + 32, btnXacNhan.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnHuy, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Hủy", {btnHuy.x + 70, btnHuy.y + 10}, 30, 1, WHITE);
    }
    else
    {
        DrawRectangleRounded(btnThemMucLuong, 0.2f, 10, GREEN);
        DrawTextEx(myFont, "   Thêm Mức Lương", {btnThemMucLuong.x + 15, btnThemMucLuong.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnSuaMucLuong, 0.2f, 10, BLUE);
        DrawTextEx(myFont, "   Sửa Mức Lương", {btnSuaMucLuong.x + 20, btnSuaMucLuong.y + 10}, 30, 1, WHITE);

        DrawRectangleRounded(btnXoaMucLuong, 0.2f, 10, RED);
        DrawTextEx(myFont, "   Xóa Mức Lương", {btnXoaMucLuong.x + 20, btnXoaMucLuong.y + 10}, 30, 1, WHITE);
    }

    DrawTextEx(myFont, ("Tổng mức lương: " + to_string(totalRows)).c_str(), {1520, 960}, 25, 1, BLACK);
}
// Hàm format số có dấu chấm phân cách hàng nghìn (VD: 8450000 -> "8.450.000")

void Draw_Screen_FullTime(Vector2 mousePos, bool Clicked, QLNV *QL)
{
    Rectangle panelBang = {330, 345, 1450.0f, 640.0f};
    DrawRectangleRounded({panelBang.x + 3, panelBang.y + 4, panelBang.width, panelBang.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});

    string default_choice = "Tất cả";

    // =========== COMBO CHỨC VỤ ===========
    Rectangle comboChucVu1 = {350, 400, 250, 45};
    static bool isChucVuOpen1 = false;
    static int selectedChucVuIndex1 = 0;

    // =========== COMBO PHÒNG BAN ===========
    Rectangle comboPhong1 = {620, 400, 250, 45};
    static bool isPhongOpen1 = false;
    static int selectedPhongIndex1 = 0;

    // =========== Ô TÌM KIẾM ===========
    Rectangle textHoTen1 = {890, 400, 350, 45};
    static bool isHoTenActive1 = false;
    static char hoTenInput1[50] = "\0";
    static int letterCount1 = 0;

    // =========== COMBO SẮP XẾP LƯƠNG (CHỈ TĂNG HOẶC GIẢM) ===========
    Rectangle comboSort1 = {1290, 400, 260, 45};
    static bool isSortOpen1 = false;
    static int selectedSortIndex1 = 0;
    string dsSort[] = {"...", "Lương tăng dần", "Lương giảm dần"};
    int nSort = 2;

    // =========== TRẠNG THÁI CHỌN NHÂN VIÊN & CHI TIẾT ===========
    static string selectedMNV_FT = "";
    static bool isOpenChiTiet = false;
    bool openedThisFrame = false; // Cờ chống nháy khi vừa bấm mở modal
    static bool isEditingThuong = false;
    static bool isThuongActive = false;
    static char thuongInput[20] = "";
    static int thuongLetterCount = 0;
    static string lastSelectedMNV = "";
    if (lastSelectedMNV != selectedMNV_FT)
    {
        lastSelectedMNV = selectedMNV_FT;
        isEditingThuong = false;
        isThuongActive = false;
    }

    // =========== LẤY TIÊU CHÍ LỌC ===========
    static string tieuchi_CV = "";
    static string tieuchi_PB = "";

    if (selectedChucVuIndex1 != 0)
        tieuchi_CV = (*QL->get_DB()->get_ListCV())[selectedChucVuIndex1 - 1]->Data->get_MaChucVu();
    else
        tieuchi_CV = "";

    if (selectedPhongIndex1 != 0)
        tieuchi_PB = (*QL->get_DB()->get_ListPB())[selectedPhongIndex1 - 1]->Data->get_MaPhongBan();
    else
        tieuchi_PB = "";

    // =========== CẤU HÌNH BẢNG ===========
    static float scrollY = 0.0f;
    float scrollSpeed = 30.0f;
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
        scrollY += wheel * scrollSpeed;
    if (scrollY > 0.0f)
        scrollY = 0.0f;

    int startX = 350;
    int startY = 500;
    int rowHeight = 60;
    int fontSize = 30;

    float colX[] = {350.f, 470.f, 730.f, 890.f, 1100.f, 1350.f, 1450.f, 1600.f};

    // =========== HEADER ===========
    DrawTextEx(myFont, "Mã NV", {colX[0], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Họ tên", {colX[1], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Phòng ban", {colX[2], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Chức vụ", {colX[3], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Vị trí", {colX[4], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Hệ số", {colX[5], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Ngày công", {colX[6], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Tổng lương", {colX[7], (float)startY}, fontSize, 1, DARKGRAY);
    DrawLine(startX, startY + 30, 1780, startY + 30, LIGHTGRAY);

    // =========== THU THẬP VÀ SẮP XẾP DANH SÁCH FULLTIME ===========
    vector<FullTime *> listFT;
    Node<Employee> *cur = QL->get_ListEmployee()->get_header()->Next;
    while (cur != nullptr)
    {
        Employee *emp = cur->Data;
        FullTime *ft = dynamic_cast<FullTime *>(emp);
        if (ft != nullptr && CheckEmployee(emp, tieuchi_CV, tieuchi_PB, hoTenInput1))
        {
            listFT.push_back(ft);
        }
        cur = cur->Next;
    }

    // Sắp xếp lương theo yêu cầu (1: Tăng dần, 2: Giảm dần)
    if (selectedSortIndex1 == 1)
    {
        sort(listFT.begin(), listFT.end(), [QL](FullTime *a, FullTime *b)
             { return QL->TinhLuong(a) < QL->TinhLuong(b); });
    }
    else if (selectedSortIndex1 == 2)
    {
        sort(listFT.begin(), listFT.end(), [QL](FullTime *a, FullTime *b)
             { return QL->TinhLuong(a) > QL->TinhLuong(b); });
    }

    // =========== VẼ DANH SÁCH NHÂN VIÊN ===========
    int listStartY = startY + 35;
    int listHeight = GetScreenHeight() - listStartY - 120;

    Rectangle listArea = {(float)startX, (float)listStartY, (float)(GetScreenWidth() - startX), (float)listHeight};
    bool canHover = !isOpenChiTiet && !isChucVuOpen1 && !isPhongOpen1 && !isSortOpen1 && CheckCollisionPointRec(mousePos, listArea);

    float currentY = listStartY + scrollY;
    int totalRows = (int)listFT.size();
    Employee *selEmp = nullptr;
    if (!selectedMNV_FT.empty())
        selEmp = QL->TimTheoMa(selectedMNV_FT);

    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
    for (FullTime *ft : listFT)
    {
        Employee *emp = ft;
        string mnv = emp->get_mnv();
        string name = emp->get_name();
        string phong = emp->get_PhongBan() ? emp->get_PhongBan()->get_TenPhongBan() : "";
        string cv = emp->get_ChucVu() ? emp->get_ChucVu()->get_TenChucVu() : "";
        string vitri = emp->get_ViTri() ? emp->get_ViTri()->get_TenVTVL() : "";

        ostringstream oss;
        oss << ft->get_HeSo();
        double tongluong = QL->TinhLuong(ft);
        if (tongluong < 0)
            tongluong = 0;
        int ngaycong = ft->get_SoNgayCong();

        string strHeSo = oss.str();
        string strNgayCong = to_string(ngaycong);
        string strLuong = FormatSoTien((long long)tongluong);

        // ----- HOVER + CHỌN DÒNG -----
        Rectangle rowRect = {(float)startX, currentY - 5, (float)(GetScreenWidth() - startX - 20), 35};
        bool isHover = canHover && CheckCollisionPointRec(mousePos, rowRect);
        if (isHover && Clicked)
        {
            selectedMNV_FT = mnv;
            selEmp = emp;
        }
        bool isSelected = (mnv == selectedMNV_FT);

        if (isSelected)
        {
            DrawRectangleRec(rowRect, isHover ? Color{180, 195, 230, 255} : Color{200, 210, 235, 255});
            selEmp = emp;
        }
        else if (isHover)
        {
            DrawRectangleRec(rowRect, Color{235, 235, 240, 255});
        }

        DrawTextEx(myFont, mnv.c_str(), {colX[0], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, name.c_str(), {colX[1], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, phong.c_str(), {colX[2], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, cv.c_str(), {colX[3], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, vitri.c_str(), {colX[4], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, strHeSo.c_str(), {colX[5], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, (strNgayCong + "/26").c_str(), {colX[6], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, strLuong.c_str(), {colX[7], currentY}, fontSize, 1, BLACK);
        DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));
        currentY += rowHeight;
    }
    EndScissorMode();

    // =========== CHẶN SCROLL ===========
    float totalListHeight = totalRows * rowHeight;
    float maxScroll = listHeight - totalListHeight;
    if (totalListHeight > listHeight && scrollY < maxScroll)
        scrollY = maxScroll;
    else if (totalListHeight <= listHeight)
        scrollY = 0.0f;

    // =========== VẼ LABEL + COMBO CHỨC VỤ ===========
    DrawTextEx(myFont, "Chức Vụ", {350, 360}, 40, 1, BLACK);
    int nCV = QL->get_DB()->get_ListCV()->get_size();
    string dsCVFilter[nCV + 1];
    dsCVFilter[0] = default_choice;
    for (int i = 0; i < nCV; i++)
        dsCVFilter[i + 1] = (*QL->get_DB()->get_ListCV())[i]->Data->get_TenChucVu();

    VeDanhSachCombo(comboChucVu1, isChucVuOpen1, dsCVFilter, nCV + 1, mousePos, 35);
    if (selectedChucVuIndex1 == 0)
        DrawTextEx(myFont, default_choice.c_str(), {comboChucVu1.x + 15, comboChucVu1.y + 5}, 35, 1, DARKGRAY);
    else
        DrawTextEx(myFont, (*QL->get_DB()->get_ListCV())[selectedChucVuIndex1 - 1]->Data->get_TenChucVu().c_str(), {comboChucVu1.x + 15, comboChucVu1.y + 5}, 35, 1, DARKGRAY);

    if (Clicked)
        XuLyCombo(comboChucVu1, isChucVuOpen1, selectedChucVuIndex1, nCV + 1, mousePos, Clicked);

    // =========== VẼ LABEL + COMBO PHÒNG BAN ===========
    DrawTextEx(myFont, "Phòng Ban", {620, 360}, 40, 1, BLACK);
    int nPB = QL->get_DB()->get_ListPB()->get_size();
    string dsPBFilter[nPB + 1];
    dsPBFilter[0] = default_choice;
    for (int i = 0; i < nPB; i++)
        dsPBFilter[i + 1] = (*QL->get_DB()->get_ListPB())[i]->Data->get_TenPhongBan();

    VeDanhSachCombo(comboPhong1, isPhongOpen1, dsPBFilter, nPB + 1, mousePos, 35);
    if (selectedPhongIndex1 == 0)
        DrawTextEx(myFont, default_choice.c_str(), {comboPhong1.x + 15, comboPhong1.y + 5}, 35, 1, DARKGRAY);
    else
        DrawTextEx(myFont, (*QL->get_DB()->get_ListPB())[selectedPhongIndex1 - 1]->Data->get_TenPhongBan().c_str(), {comboPhong1.x + 15, comboPhong1.y + 5}, 35, 1, DARKGRAY);

    if (Clicked)
        XuLyCombo(comboPhong1, isPhongOpen1, selectedPhongIndex1, nPB + 1, mousePos, Clicked);

    // =========== VẼ Ô TÌM KIẾM ===========
    Typing(isHoTenActive1, hoTenInput1, letterCount1, mousePos, textHoTen1);
    DrawTextEx(myFont, "Tìm kiếm", {890, 360}, 40, 1, BLACK);
    DrawRectangleRounded(textHoTen1, 0.5f, 10, isHoTenActive1 ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textHoTen1, 0.3f, 12, 3.0f, LIGHTGRAY);
    DrawTextEx(myFont, hoTenInput1, {textHoTen1.x + 10, textHoTen1.y + 8}, 35, 1, BLACK);
    if (isHoTenActive1 && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(hoTenInput1, 35);
        DrawTextEx(myFont, "_", {textHoTen1.x + 12 + textWidth, textHoTen1.y + 8}, 35, 1, MAROON);
    }

    // =========== VẼ COMBO SẮP XẾP LƯƠNG ===========
    DrawTextEx(myFont, "Sắp xếp theo:", {1290, 360}, 40, 1, BLACK);
    VeDanhSachCombo(comboSort1, isSortOpen1, dsSort, nSort + 1, mousePos, 35);
    if (selectedSortIndex1 == 0)
        DrawTextEx(myFont, dsSort[0].c_str(), {comboSort1.x + 15, comboSort1.y + 5}, 35, 1, DARKGRAY);
    else
        DrawTextEx(myFont, dsSort[selectedSortIndex1].c_str(), {comboSort1.x + 15, comboSort1.y + 5}, 35, 1, DARKGRAY);

    if (Clicked && !isOpenChiTiet)
        XuLyCombo(comboSort1, isSortOpen1, selectedSortIndex1, nSort + 1, mousePos, Clicked);

    // =========== NÚT CHI TIẾT GÓC DƯỚI BÊN PHẢI KHI CHỌN NHÂN VIÊN ===========
    if (selEmp != nullptr && !selectedMNV_FT.empty())
    {
        Rectangle btnChiTiet = {1530, 940, 220, 48};
        bool hoverChiTiet = CheckCollisionPointRec(mousePos, btnChiTiet);
        DrawRectangleRounded(btnChiTiet, 0.25f, 8, hoverChiTiet ? Color{80, 50, 215, 255} : Color{98, 70, 234, 255});
        DrawRectangleRoundedLinesEx(btnChiTiet, 0.25f, 8, 1.5f, Color{80, 60, 220, 255});

        float tw = MeasureTextEx(tieude, "Chi tiết", 26, 1).x;
        DrawTextEx(tieude, "Chi tiết", {btnChiTiet.x + (btnChiTiet.width - tw) / 2.0f, btnChiTiet.y + 11}, 26, 1, WHITE);

        if (Clicked && !isOpenChiTiet && hoverChiTiet)
        {
            isOpenChiTiet = true;
            openedThisFrame = true; // Ngăn chặn sự kiện click này tự đóng modal ngay lập tức
        }
    }

    // =========== MODAL CHI TIẾT TÍNH LƯƠNG (GIỐNG HÌNH ẢNH) ===========
    if (isOpenChiTiet && selEmp != nullptr)
    {
        FullTime *ft = dynamic_cast<FullTime *>(selEmp);
        if (ft != nullptr)
        {
            float screenW = GetScreenWidth();
            float screenH = GetScreenHeight();

            // Overlay mờ che nền
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.45f));

            // Kích thước modal (được tăng to hơn rõ ràng, rộng rãi)
            float modalW = 580.0f;
            float modalH = 670.0f;
            Rectangle modal = {(screenW - modalW) / 2.0f, (screenH - modalH) / 2.0f, modalW, modalH};

            // Đổ bóng mờ nhẹ
            DrawRectangleRounded({modal.x + 4, modal.y + 6, modal.width, modal.height}, 0.04f, 10, Fade(BLACK, 0.16f));
            // Hộp trắng chính bo góc
            DrawRectangleRounded(modal, 0.04f, 10, WHITE);
            DrawRectangleRoundedLinesEx(modal, 0.04f, 10, 1.5f, Color{225, 220, 245, 255});

            // Vạch điểm nhấn màu tím góc trái tiêu đề
            DrawRectangleRounded({modal.x + 36, modal.y + 26, 7, 30}, 0.5f, 4, Color{110, 65, 240, 255});

            // Tiêu đề
            DrawTextEx(tieude, "Chi tiết tính lương", {modal.x + 54, modal.y + 24}, 30, 1, Color{50, 30, 140, 255});

            // Tên và mã nhân viên nhỏ bên dưới tiêu đề
            string infoNV = selEmp->get_name() + " (" + selEmp->get_mnv() + ")";
            DrawTextEx(myFont, infoNV.c_str(), {modal.x + 36, modal.y + 64}, 22, 1, Color{120, 130, 145, 255});

            // Nút đóng [X]
            Rectangle btnDong = {modal.x + modal.width - 54, modal.y + 22, 34, 34};
            bool hoverX = CheckCollisionPointRec(mousePos, btnDong);
            DrawRectangleRounded(btnDong, 0.3f, 6, hoverX ? Color{235, 240, 250, 255} : Color{245, 246, 250, 255});
            DrawLineEx({btnDong.x + 10, btnDong.y + 10}, {btnDong.x + 24, btnDong.y + 24}, 2.0f, DARKGRAY);
            DrawLineEx({btnDong.x + 24, btnDong.y + 10}, {btnDong.x + 10, btnDong.y + 24}, 2.0f, DARKGRAY);

            // Đường gạch ngang phân cách
            DrawLine(modal.x + 36, modal.y + 98, modal.x + modal.width - 36, modal.y + 98, Color{235, 240, 248, 255});

            // Lấy các chỉ số tính lương
            double lcb = QL->get_LuongCoBan(ft);
            if (lcb < 0)
                lcb = 0;
            double heso = ft->get_HeSo();
            int ngaycong = ft->get_SoNgayCong();
            double phucap = (ft->get_ChucVu() != nullptr) ? ft->get_ChucVu()->get_PhuCap() : 0.0;
            double thuong = ft->get_ThuongPhat();
            double tongluong = QL->TinhLuong(ft);
            if (tongluong < 0)
                tongluong = 0;
            double khautru = 0; // Khấu trừ
            double thuclinh = tongluong - khautru;

            float leftX = modal.x + 38;
            float rightX = modal.x + modal.width - 38;
            float rowY = modal.y + 118;
            float rowSpacing = 46;
            int labelSize = 24;
            int valSize = 24;
            Color labelColor = Color{45, 55, 72, 255};
            Color valColor = Color{25, 30, 45, 255};

            // 1. Lương cơ bản
            DrawTextEx(myFont, "Lương cơ bản", {leftX, rowY}, labelSize, 1, labelColor);
            string sLCB = FormatSoTien((long long)lcb) + " đ";
            float wLCB = MeasureTextEx(myFont, sLCB.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sLCB.c_str(), {rightX - wLCB, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 2. Hệ số lương
            DrawTextEx(myFont, "Hệ số lương", {leftX, rowY}, labelSize, 1, labelColor);
            ostringstream ossHS;
            ossHS << fixed << setprecision(1) << heso;
            string sHS = ossHS.str();
            float wHS = MeasureTextEx(myFont, sHS.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sHS.c_str(), {rightX - wHS, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 3. Ngày công thực tế
            DrawTextEx(myFont, "Ngày công thực tế", {leftX, rowY}, labelSize, 1, labelColor);
            string sNC = to_string(ngaycong) + " / 26";
            float wNC = MeasureTextEx(myFont, sNC.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sNC.c_str(), {rightX - wNC, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 4. Phụ cấp
            DrawTextEx(myFont, "Phụ cấp", {leftX, rowY}, labelSize, 1, labelColor);
            string sPC = FormatSoTien((long long)phucap) + " đ";
            float wPC = MeasureTextEx(myFont, sPC.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sPC.c_str(), {rightX - wPC, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 5. Thưởng (Có chức năng xem và chỉnh sửa trực tiếp)
            DrawTextEx(myFont, "Thưởng", {leftX, rowY}, labelSize, 1, labelColor);

            if (!isEditingThuong)
            {
                // Nút "Sửa" nhỏ bên cạnh chữ Thưởng
                Rectangle btnSuaThuong = {leftX + 85, rowY - 2, 54, 28};
                bool hoverSua = CheckCollisionPointRec(mousePos, btnSuaThuong);
                DrawRectangleRounded(btnSuaThuong, 0.3f, 6, hoverSua ? Color{225, 218, 255, 255} : Color{242, 238, 255, 255});
                DrawRectangleRoundedLinesEx(btnSuaThuong, 0.3f, 6, 1.2f, Color{160, 140, 230, 255});
                DrawTextEx(myFont, "Sửa", {btnSuaThuong.x + 12, btnSuaThuong.y + 3}, 18, 1, Color{98, 70, 234, 255});

                string sThuong = FormatSoTien((long long)thuong) + " đ";
                float wThuong = MeasureTextEx(myFont, sThuong.c_str(), valSize, 1).x;
                Rectangle recThuongVal = {rightX - wThuong - 4, rowY - 2, wThuong + 8, 30};
                bool hoverVal = CheckCollisionPointRec(mousePos, recThuongVal);

                DrawTextEx(myFont, sThuong.c_str(), {rightX - wThuong, rowY}, valSize, 1, hoverVal ? Color{98, 70, 234, 255} : valColor);

                if (Clicked && (hoverSua || hoverVal))
                {
                    isEditingThuong = true;
                    isThuongActive = true;
                    snprintf(thuongInput, sizeof(thuongInput), "%lld", (long long)thuong);
                    thuongLetterCount = (int)strlen(thuongInput);
                }
            }
            else
            {
                Rectangle boxThuong = {rightX - 250, rowY - 4, 130, 34};
                Rectangle btnLuuThuong = {rightX - 112, rowY - 4, 54, 34};
                Rectangle btnHuyThuong = {rightX - 52, rowY - 4, 48, 34};

                bool hoverLuu = CheckCollisionPointRec(mousePos, btnLuuThuong);
                bool hoverHuy = CheckCollisionPointRec(mousePos, btnHuyThuong);

                if (Clicked)
                {
                    if (CheckCollisionPointRec(mousePos, boxThuong))
                        isThuongActive = true;
                    else if (!hoverLuu && !hoverHuy)
                        isThuongActive = false;
                }

                if (isThuongActive)
                {
                    int key = GetCharPressed();
                    while (key > 0)
                    {
                        if (key >= '0' && key <= '9' && thuongLetterCount < 11)
                        {
                            thuongInput[thuongLetterCount++] = (char)key;
                            thuongInput[thuongLetterCount] = '\0';
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
                    {
                        if (thuongLetterCount > 0)
                        {
                            thuongLetterCount--;
                            thuongInput[thuongLetterCount] = '\0';
                        }
                    }
                }

                // Vẽ ô nhập số tiền thưởng
                DrawRectangleRounded(boxThuong, 0.25f, 6, isThuongActive ? WHITE : Color{245, 246, 250, 255});
                DrawRectangleRoundedLinesEx(boxThuong, 0.25f, 6, 1.5f, isThuongActive ? Color{98, 70, 234, 255} : LIGHTGRAY);
                DrawTextEx(myFont, thuongInput, {boxThuong.x + 8, boxThuong.y + 6}, 20, 1, BLACK);

                // Con trỏ nhấp nháy
                if (isThuongActive && ((int)(GetTime() * 2) % 2 == 0))
                {
                    float tw = MeasureTextEx(myFont, thuongInput, 20, 1).x;
                    DrawLineEx({boxThuong.x + 8 + tw + 2, boxThuong.y + 6}, {boxThuong.x + 8 + tw + 2, boxThuong.y + 28}, 1.5f, Color{98, 70, 234, 255});
                }

                // Nút Lưu
                DrawRectangleRounded(btnLuuThuong, 0.25f, 6, hoverLuu ? Color{80, 50, 215, 255} : Color{98, 70, 234, 255});
                DrawTextEx(tieude, "Lưu", {btnLuuThuong.x + 12, btnLuuThuong.y + 8}, 18, 1, WHITE);

                // Nút Hủy
                DrawRectangleRounded(btnHuyThuong, 0.25f, 6, hoverHuy ? Color{225, 230, 238, 255} : Color{240, 242, 246, 255});
                DrawRectangleRoundedLinesEx(btnHuyThuong, 0.25f, 6, 1.0f, Color{200, 205, 215, 255});
                DrawTextEx(myFont, "Hủy", {btnHuyThuong.x + 11, btnHuyThuong.y + 8}, 18, 1, DARKGRAY);

                // Xử lý Lưu: click Lưu hoặc nhấn Enter
                bool doSave = (Clicked && hoverLuu) || (isThuongActive && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)));
                if (doSave)
                {
                    int newT = 0;
                    if (strlen(thuongInput) > 0)
                    {
                        try
                        {
                            long long val = stoll(thuongInput);
                            if (val < 0)
                                val = 0;
                            if (val > 2000000000LL)
                                val = 2000000000LL;
                            newT = (int)val;
                        }
                        catch (...)
                        {
                            newT = 0;
                        }
                    }
                    ft->set_ThuongPhat(newT);
                    QL->get_DB()->SaveFileEmployee();
                    tongluong = QL->TinhLuong(ft);
                    if (tongluong < 0)
                        tongluong = 0;
                    thuclinh = tongluong - khautru;
                    HienThongBao("Cập nhật tiền thưởng thành công!", false);
                    isEditingThuong = false;
                    isThuongActive = false;
                }

                // Xử lý Hủy: click Hủy hoặc nhấn Escape
                if ((Clicked && hoverHuy) || IsKeyPressed(KEY_ESCAPE))
                {
                    isEditingThuong = false;
                    isThuongActive = false;
                }
            }
            rowY += rowSpacing + 10;

            // 6. Box Tổng thu nhập (Hộp xanh nhạt bo góc với icon máy tính)
            Rectangle boxTongThuNhap = {leftX, rowY, modal.width - 76, 58};
            DrawRectangleRounded(boxTongThuNhap, 0.25f, 8, Color{242, 246, 255, 255});
            DrawRectangleRoundedLinesEx(boxTongThuNhap, 0.25f, 8, 1.2f, Color{220, 230, 252, 255});

            // Vẽ icon máy tính nhỏ
            Rectangle iconCalc = {leftX + 16, rowY + 16, 22, 26};
            DrawRectangleRounded(iconCalc, 0.2f, 4, Color{50, 90, 220, 255});
            DrawRectangle(iconCalc.x + 3, iconCalc.y + 4, 16, 4, WHITE);
            DrawCircle(iconCalc.x + 6, iconCalc.y + 13, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 11, iconCalc.y + 13, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 16, iconCalc.y + 13, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 6, iconCalc.y + 20, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 11, iconCalc.y + 20, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 16, iconCalc.y + 20, 1.5f, WHITE);

            DrawTextEx(tieude, "Tổng thu nhập", {leftX + 50, rowY + 16}, 24, 1, Color{40, 75, 215, 255});
            string sTong = FormatSoTien((long long)tongluong) + " đ";
            float wTong = MeasureTextEx(tieude, sTong.c_str(), 26, 1).x;
            DrawTextEx(tieude, sTong.c_str(), {rightX - wTong - 16, rowY + 15}, 26, 1, Color{30, 60, 200, 255});
            rowY += 72;

            // 7. Khấu trừ (Icon tròn đỏ có dấu trừ)
            DrawCircle(leftX + 18, rowY + 13, 12, Color{245, 95, 85, 255});
            DrawLineEx({leftX + 11, rowY + 13}, {leftX + 25, rowY + 13}, 2.2f, WHITE);
            DrawTextEx(myFont, "Khấu trừ", {leftX + 44, rowY}, labelSize, 1, Color{100, 105, 115, 255});
            string sKT = FormatSoTien((long long)khautru) + " đ";
            float wKT = MeasureTextEx(myFont, sKT.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sKT.c_str(), {rightX - wKT, rowY}, valSize, 1, Color{100, 105, 115, 255});
            rowY += 52;

            // 8. Box Thực lĩnh (Hộp tím nhạt bo góc với icon checkmark tím)
            Rectangle boxThucLinh = {leftX, rowY, modal.width - 76, 62};
            DrawRectangleRounded(boxThucLinh, 0.25f, 8, Color{246, 242, 255, 255});
            DrawRectangleRoundedLinesEx(boxThucLinh, 0.25f, 8, 1.2f, Color{225, 215, 250, 255});

            // Icon tròn tím có dấu checkmark
            DrawCircle(leftX + 20, rowY + 31, 13, Color{98, 70, 234, 255});
            DrawLineEx({leftX + 14, rowY + 31}, {leftX + 19, rowY + 36}, 2.4f, WHITE);
            DrawLineEx({leftX + 19, rowY + 36}, {leftX + 27, rowY + 24}, 2.4f, WHITE);

            DrawTextEx(tieude, "Thực lĩnh", {leftX + 50, rowY + 18}, 26, 1, Color{98, 70, 234, 255});
            string sTL = FormatSoTien((long long)thuclinh) + " đ";
            float wTL = MeasureTextEx(tieude, sTL.c_str(), 28, 1).x;
            DrawTextEx(tieude, sTL.c_str(), {rightX - wTL - 16, rowY + 17}, 28, 1, Color{98, 70, 234, 255});

            // Xử lý đóng modal: click [X] hoặc click ra ngoài modal (chỉ khi không phải vừa mở ở frame này)
            if (Clicked && !openedThisFrame)
            {
                if (hoverX || !CheckCollisionPointRec(mousePos, modal))
                {
                    isOpenChiTiet = false;
                    isEditingThuong = false;
                    isThuongActive = false;
                }
            }
        }
    }
}
void Draw_Screen_PartTime(Vector2 mousePos, bool Clicked, QLNV *QL)
{
    Rectangle panelBang = {330, 345, 1450.0f, 640.0f};
    DrawRectangleRounded({panelBang.x + 3, panelBang.y + 4, panelBang.width, panelBang.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});

    string default_choice = "Tất cả";

    // =========== COMBO CHỨC VỤ ===========
    Rectangle comboChucVu2 = {350, 400, 250, 45};
    static bool isChucVuOpen2 = false;
    static int selectedChucVuIndex2 = 0;

    // =========== COMBO PHÒNG BAN ===========
    Rectangle comboPhong2 = {620, 400, 250, 45};
    static bool isPhongOpen2 = false;
    static int selectedPhongIndex2 = 0;

    // =========== Ô TÌM KIẾM ===========
    Rectangle textHoTen2 = {890, 400, 350, 45};
    static bool isHoTenActive2 = false;
    static char hoTenInput2[50] = "\0";
    static int letterCount2 = 0;

    // =========== COMBO SẮP XẾP LƯƠNG (CHỈ TĂNG HOẶC GIẢM) ===========
    Rectangle comboSort2 = {1290, 400, 260, 45};
    static bool isSortOpen2 = false;
    static int selectedSortIndex2 = 0;
    string dsSort[] = {"...", "Lương tăng dần", "Lương giảm dần"};
    int nSort = 2;

    // =========== TRẠNG THÁI CHỌN NHÂN VIÊN & CHI TIẾT ===========
    static string selectedMNV_PT = "";
    static bool isOpenChiTiet2 = false;
    bool openedThisFrame2 = false; // Cờ chống nháy khi vừa bấm mở modal
    static bool isEditingThuong2 = false;
    static bool isThuongActive2 = false;
    static char thuongInput2[20] = "";
    static int thuongLetterCount2 = 0;
    static string lastSelectedMNV2 = "";
    if (lastSelectedMNV2 != selectedMNV_PT)
    {
        lastSelectedMNV2 = selectedMNV_PT;
        isEditingThuong2 = false;
        isThuongActive2 = false;
    }

    // =========== LẤY TIÊU CHÍ LỌC ===========
    static string tieuchi_CV = "";
    static string tieuchi_PB = "";

    if (selectedChucVuIndex2 != 0)
        tieuchi_CV = (*QL->get_DB()->get_ListCV())[selectedChucVuIndex2 - 1]->Data->get_MaChucVu();
    else
        tieuchi_CV = "";

    if (selectedPhongIndex2 != 0)
        tieuchi_PB = (*QL->get_DB()->get_ListPB())[selectedPhongIndex2 - 1]->Data->get_MaPhongBan();
    else
        tieuchi_PB = "";

    // =========== CẤU HÌNH BẢNG ===========
    static float scrollY = 0.0f;
    float scrollSpeed = 30.0f;
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
        scrollY += wheel * scrollSpeed;
    if (scrollY > 0.0f)
        scrollY = 0.0f;

    int startX = 350;
    int startY = 500;
    int rowHeight = 60;
    int fontSize = 30;

    float colX[] = {350.f, 470.f, 730.f, 890.f, 1100.f, 1390.f, 1600.f};

    // =========== HEADER ===========
    DrawTextEx(myFont, "Mã NV", {colX[0], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Họ tên", {colX[1], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Phòng ban", {colX[2], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Chức vụ", {colX[3], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Vị trí", {colX[4], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Số giờ làm", {colX[5], (float)startY}, fontSize, 1, DARKGRAY);
    DrawTextEx(myFont, "Tổng lương", {colX[6], (float)startY}, fontSize, 1, DARKGRAY);
    DrawLine(startX, startY + 30, 1780, startY + 30, LIGHTGRAY);

    // =========== THU THẬP VÀ SẮP XẾP DANH SÁCH PARTTIME ===========
    vector<PartTime *> listPT;
    Node<Employee> *cur = QL->get_ListEmployee()->get_header()->Next;
    while (cur != nullptr)
    {
        Employee *emp = cur->Data;
        PartTime *pt = dynamic_cast<PartTime *>(emp);
        if (pt != nullptr && CheckEmployee(emp, tieuchi_CV, tieuchi_PB, hoTenInput2))
        {
            listPT.push_back(pt);
        }
        cur = cur->Next;
    }

    // Sắp xếp lương theo yêu cầu (1: Tăng dần, 2: Giảm dần)
    if (selectedSortIndex2 == 1)
    {
        sort(listPT.begin(), listPT.end(), [QL](PartTime *a, PartTime *b)
             { return QL->TinhLuong(a) < QL->TinhLuong(b); });
    }
    else if (selectedSortIndex2 == 2)
    {
        sort(listPT.begin(), listPT.end(), [QL](PartTime *a, PartTime *b)
             { return QL->TinhLuong(a) > QL->TinhLuong(b); });
    }

    // =========== VẼ DANH SÁCH NHÂN VIÊN ===========
    int listStartY = startY + 35;
    int listHeight = GetScreenHeight() - listStartY - 120;

    Rectangle listArea = {(float)startX, (float)listStartY, (float)(GetScreenWidth() - startX), (float)listHeight};
    bool canHover = !isOpenChiTiet2 && !isChucVuOpen2 && !isPhongOpen2 && !isSortOpen2 && CheckCollisionPointRec(mousePos, listArea);

    float currentY = listStartY + scrollY;
    int totalRows = (int)listPT.size();
    Employee *selEmp = nullptr;
    if (!selectedMNV_PT.empty())
        selEmp = QL->TimTheoMa(selectedMNV_PT);

    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
    for (PartTime *pt : listPT)
    {
        Employee *emp = pt;
        string mnv = emp->get_mnv();
        string name = emp->get_name();
        string phong = emp->get_PhongBan() ? emp->get_PhongBan()->get_TenPhongBan() : "";
        string cv = emp->get_ChucVu() ? emp->get_ChucVu()->get_TenChucVu() : "";
        string vitri = emp->get_ViTri() ? emp->get_ViTri()->get_TenVTVL() : "";

        double tongluong = QL->TinhLuong(pt);
        if (tongluong < 0)
            tongluong = 0;
        int timework = pt->get_TimeWork();

        string strBac = "Bậc " + to_string(pt->get_BacViTri());
        string strGio = to_string(timework) + "h";
        string strLuong = FormatSoTien((long long)tongluong);

        // ----- HOVER + CHỌN DÒNG -----
        Rectangle rowRect = {(float)startX, currentY - 5, (float)(GetScreenWidth() - startX - 20), 35};
        bool isHover = canHover && CheckCollisionPointRec(mousePos, rowRect);
        if (isHover && Clicked)
        {
            selectedMNV_PT = mnv;
            selEmp = emp;
        }
        bool isSelected = (mnv == selectedMNV_PT);

        if (isSelected)
        {
            DrawRectangleRec(rowRect, isHover ? Color{180, 195, 230, 255} : Color{200, 210, 235, 255});
            selEmp = emp;
        }
        else if (isHover)
        {
            DrawRectangleRec(rowRect, Color{235, 235, 240, 255});
        }

        DrawTextEx(myFont, mnv.c_str(), {colX[0], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, name.c_str(), {colX[1], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, phong.c_str(), {colX[2], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, cv.c_str(), {colX[3], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, vitri.c_str(), {colX[4], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, strGio.c_str(), {colX[5], currentY}, fontSize, 1, BLACK);
        DrawTextEx(myFont, strLuong.c_str(), {colX[6], currentY}, fontSize, 1, BLACK);
        DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));
        currentY += rowHeight;
    }
    EndScissorMode();

    // =========== CHẶN SCROLL ===========
    float totalListHeight = totalRows * rowHeight;
    float maxScroll = listHeight - totalListHeight;
    if (totalListHeight > listHeight && scrollY < maxScroll)
        scrollY = maxScroll;
    else if (totalListHeight <= listHeight)
        scrollY = 0.0f;

    // =========== VẼ LABEL + COMBO CHỨC VỤ ===========
    DrawTextEx(myFont, "Chức Vụ", {350, 360}, 40, 1, BLACK);
    int nCV = QL->get_DB()->get_ListCV()->get_size();
    string dsCVFilter[nCV + 1];
    dsCVFilter[0] = default_choice;
    for (int i = 0; i < nCV; i++)
        dsCVFilter[i + 1] = (*QL->get_DB()->get_ListCV())[i]->Data->get_TenChucVu();

    VeDanhSachCombo(comboChucVu2, isChucVuOpen2, dsCVFilter, nCV + 1, mousePos, 35);
    if (selectedChucVuIndex2 == 0)
        DrawTextEx(myFont, default_choice.c_str(), {comboChucVu2.x + 15, comboChucVu2.y + 5}, 35, 1, DARKGRAY);
    else
        DrawTextEx(myFont, (*QL->get_DB()->get_ListCV())[selectedChucVuIndex2 - 1]->Data->get_TenChucVu().c_str(), {comboChucVu2.x + 15, comboChucVu2.y + 5}, 35, 1, DARKGRAY);

    if (Clicked)
        XuLyCombo(comboChucVu2, isChucVuOpen2, selectedChucVuIndex2, nCV + 1, mousePos, Clicked);

    // =========== VẼ LABEL + COMBO PHÒNG BAN ===========
    DrawTextEx(myFont, "Phòng Ban", {620, 360}, 40, 1, BLACK);
    int nPB = QL->get_DB()->get_ListPB()->get_size();
    string dsPBFilter[nPB + 1];
    dsPBFilter[0] = default_choice;
    for (int i = 0; i < nPB; i++)
        dsPBFilter[i + 1] = (*QL->get_DB()->get_ListPB())[i]->Data->get_TenPhongBan();

    VeDanhSachCombo(comboPhong2, isPhongOpen2, dsPBFilter, nPB + 1, mousePos, 35);
    if (selectedPhongIndex2 == 0)
        DrawTextEx(myFont, default_choice.c_str(), {comboPhong2.x + 15, comboPhong2.y + 5}, 35, 1, DARKGRAY);
    else
        DrawTextEx(myFont, (*QL->get_DB()->get_ListPB())[selectedPhongIndex2 - 1]->Data->get_TenPhongBan().c_str(), {comboPhong2.x + 15, comboPhong2.y + 5}, 35, 1, DARKGRAY);

    if (Clicked)
        XuLyCombo(comboPhong2, isPhongOpen2, selectedPhongIndex2, nPB + 1, mousePos, Clicked);

    // =========== VẼ Ô TÌM KIẾM ===========
    Typing(isHoTenActive2, hoTenInput2, letterCount2, mousePos, textHoTen2);
    DrawTextEx(myFont, "Tìm kiếm", {890, 360}, 40, 1, BLACK);
    DrawRectangleRounded(textHoTen2, 0.5f, 10, isHoTenActive2 ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textHoTen2, 0.3f, 12, 3.0f, LIGHTGRAY);
    DrawTextEx(myFont, hoTenInput2, {textHoTen2.x + 10, textHoTen2.y + 8}, 35, 1, BLACK);
    if (isHoTenActive2 && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(hoTenInput2, 35);
        DrawTextEx(myFont, "_", {textHoTen2.x + 12 + textWidth, textHoTen2.y + 8}, 35, 1, MAROON);
    }

    // =========== VẼ COMBO SẮP XẾP LƯƠNG ===========
    DrawTextEx(myFont, "Sắp xếp theo:", {1290, 360}, 40, 1, BLACK);
    VeDanhSachCombo(comboSort2, isSortOpen2, dsSort, nSort + 1, mousePos, 35);
    if (selectedSortIndex2 == 0)
        DrawTextEx(myFont, dsSort[0].c_str(), {comboSort2.x + 15, comboSort2.y + 5}, 35, 1, DARKGRAY);
    else
        DrawTextEx(myFont, dsSort[selectedSortIndex2].c_str(), {comboSort2.x + 15, comboSort2.y + 5}, 35, 1, DARKGRAY);

    if (Clicked && !isOpenChiTiet2)
        XuLyCombo(comboSort2, isSortOpen2, selectedSortIndex2, nSort + 1, mousePos, Clicked);

    // =========== NÚT CHI TIẾT GÓC DƯỚI BÊN PHẢI KHI CHỌN NHÂN VIÊN ===========
    if (selEmp != nullptr && !selectedMNV_PT.empty())
    {
        Rectangle btnChiTiet = {1530, 940, 220, 48};
        bool hoverChiTiet = CheckCollisionPointRec(mousePos, btnChiTiet);
        DrawRectangleRounded(btnChiTiet, 0.25f, 8, hoverChiTiet ? Color{80, 50, 215, 255} : Color{98, 70, 234, 255});
        DrawRectangleRoundedLinesEx(btnChiTiet, 0.25f, 8, 1.5f, Color{80, 60, 220, 255});

        float tw = MeasureTextEx(tieude, "Chi tiết", 26, 1).x;
        DrawTextEx(tieude, "Chi tiết", {btnChiTiet.x + (btnChiTiet.width - tw) / 2.0f, btnChiTiet.y + 11}, 26, 1, WHITE);

        if (Clicked && !isOpenChiTiet2 && hoverChiTiet)
        {
            isOpenChiTiet2 = true;
            openedThisFrame2 = true;
        }
    }

    // =========== MODAL CHI TIẾT TÍNH LƯƠNG PARTTIME ===========
    if (isOpenChiTiet2 && selEmp != nullptr)
    {
        PartTime *pt = dynamic_cast<PartTime *>(selEmp);
        if (pt != nullptr)
        {
            float screenW = GetScreenWidth();
            float screenH = GetScreenHeight();

            // Overlay mờ che nền
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.45f));

            // Kích thước modal (được tăng to hơn rõ ràng, rộng rãi)
            float modalW = 580.0f;
            float modalH = 670.0f;
            Rectangle modal = {(screenW - modalW) / 2.0f, (screenH - modalH) / 2.0f, modalW, modalH};

            // Đổ bóng mờ nhẹ
            DrawRectangleRounded({modal.x + 4, modal.y + 6, modal.width, modal.height}, 0.04f, 10, Fade(BLACK, 0.16f));
            // Hộp trắng chính bo góc
            DrawRectangleRounded(modal, 0.04f, 10, WHITE);
            DrawRectangleRoundedLinesEx(modal, 0.04f, 10, 1.5f, Color{225, 220, 245, 255});

            // Vạch điểm nhấn màu tím góc trái tiêu đề
            DrawRectangleRounded({modal.x + 36, modal.y + 26, 7, 30}, 0.5f, 4, Color{110, 65, 240, 255});

            // Tiêu đề
            DrawTextEx(tieude, "Chi tiết tính lương", {modal.x + 54, modal.y + 24}, 30, 1, Color{50, 30, 140, 255});

            // Tên và mã nhân viên nhỏ bên dưới tiêu đề
            string infoNV = selEmp->get_name() + " (" + selEmp->get_mnv() + ") - PartTime";
            DrawTextEx(myFont, infoNV.c_str(), {modal.x + 36, modal.y + 64}, 22, 1, Color{120, 130, 145, 255});

            // Nút đóng [X]
            Rectangle btnDong = {modal.x + modal.width - 54, modal.y + 22, 34, 34};
            bool hoverX = CheckCollisionPointRec(mousePos, btnDong);
            DrawRectangleRounded(btnDong, 0.3f, 6, hoverX ? Color{235, 240, 250, 255} : Color{245, 246, 250, 255});
            DrawLineEx({btnDong.x + 10, btnDong.y + 10}, {btnDong.x + 24, btnDong.y + 24}, 2.0f, DARKGRAY);
            DrawLineEx({btnDong.x + 24, btnDong.y + 10}, {btnDong.x + 10, btnDong.y + 24}, 2.0f, DARKGRAY);

            // Đường gạch ngang phân cách
            DrawLine(modal.x + 36, modal.y + 98, modal.x + modal.width - 36, modal.y + 98, Color{235, 240, 248, 255});

            // Lấy các chỉ số tính lương PartTime
            double lcb = QL->get_LuongCoBan(pt);
            if (lcb < 0)
                lcb = 0;
            int bac = pt->get_BacViTri();
            int timework = pt->get_TimeWork();
            double phucap = (pt->get_ChucVu() != nullptr) ? pt->get_ChucVu()->get_PhuCap() : 0.0;
            double thuong = pt->get_ThuongPhat();
            double tongluong = QL->TinhLuong(pt);
            if (tongluong < 0)
                tongluong = 0;
            double khautru = 0; // Khấu trừ
            double thuclinh = tongluong - khautru;

            float leftX = modal.x + 38;
            float rightX = modal.x + modal.width - 38;
            float rowY = modal.y + 118;
            float rowSpacing = 46;
            int labelSize = 24;
            int valSize = 24;
            Color labelColor = Color{45, 55, 72, 255};
            Color valColor = Color{25, 30, 45, 255};

            // 1. Lương theo giờ
            DrawTextEx(myFont, "Lương theo giờ", {leftX, rowY}, labelSize, 1, labelColor);
            string sLCB = FormatSoTien((long long)lcb) + " đ/h";
            float wLCB = MeasureTextEx(myFont, sLCB.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sLCB.c_str(), {rightX - wLCB, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 2. Bậc vị trí
            DrawTextEx(myFont, "Bậc vị trí", {leftX, rowY}, labelSize, 1, labelColor);
            string sBac = "Bậc " + to_string(bac);
            float wBac = MeasureTextEx(myFont, sBac.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sBac.c_str(), {rightX - wBac, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 3. Số giờ làm việc thực tế
            DrawTextEx(myFont, "Số giờ làm việc", {leftX, rowY}, labelSize, 1, labelColor);
            string sGio = to_string(timework) + " giờ" + (timework > 80 ? (" (OT: " + to_string(timework - 80) + "h)") : " / 80h");
            float wGio = MeasureTextEx(myFont, sGio.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sGio.c_str(), {rightX - wGio, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 4. Phụ cấp
            DrawTextEx(myFont, "Phụ cấp", {leftX, rowY}, labelSize, 1, labelColor);
            string sPC = FormatSoTien((long long)phucap) + " đ";
            float wPC = MeasureTextEx(myFont, sPC.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sPC.c_str(), {rightX - wPC, rowY}, valSize, 1, valColor);
            rowY += rowSpacing;

            // 5. Thưởng (Có chức năng xem và chỉnh sửa trực tiếp)
            DrawTextEx(myFont, "Thưởng", {leftX, rowY}, labelSize, 1, labelColor);

            if (!isEditingThuong2)
            {
                // Nút "Sửa" nhỏ bên cạnh chữ Thưởng
                Rectangle btnSuaThuong = {leftX + 85, rowY - 2, 54, 28};
                bool hoverSua = CheckCollisionPointRec(mousePos, btnSuaThuong);
                DrawRectangleRounded(btnSuaThuong, 0.3f, 6, hoverSua ? Color{225, 218, 255, 255} : Color{242, 238, 255, 255});
                DrawRectangleRoundedLinesEx(btnSuaThuong, 0.3f, 6, 1.2f, Color{160, 140, 230, 255});
                DrawTextEx(myFont, "Sửa", {btnSuaThuong.x + 12, btnSuaThuong.y + 3}, 18, 1, Color{98, 70, 234, 255});

                string sThuong = FormatSoTien((long long)thuong) + " đ";
                float wThuong = MeasureTextEx(myFont, sThuong.c_str(), valSize, 1).x;
                Rectangle recThuongVal = {rightX - wThuong - 4, rowY - 2, wThuong + 8, 30};
                bool hoverVal = CheckCollisionPointRec(mousePos, recThuongVal);

                DrawTextEx(myFont, sThuong.c_str(), {rightX - wThuong, rowY}, valSize, 1, hoverVal ? Color{98, 70, 234, 255} : valColor);

                if (Clicked && (hoverSua || hoverVal))
                {
                    isEditingThuong2 = true;
                    isThuongActive2 = true;
                    snprintf(thuongInput2, sizeof(thuongInput2), "%lld", (long long)thuong);
                    thuongLetterCount2 = (int)strlen(thuongInput2);
                }
            }
            else
            {
                Rectangle boxThuong = {rightX - 250, rowY - 4, 130, 34};
                Rectangle btnLuuThuong = {rightX - 112, rowY - 4, 54, 34};
                Rectangle btnHuyThuong = {rightX - 52, rowY - 4, 48, 34};

                bool hoverLuu = CheckCollisionPointRec(mousePos, btnLuuThuong);
                bool hoverHuy = CheckCollisionPointRec(mousePos, btnHuyThuong);

                if (Clicked)
                {
                    if (CheckCollisionPointRec(mousePos, boxThuong))
                        isThuongActive2 = true;
                    else if (!hoverLuu && !hoverHuy)
                        isThuongActive2 = false;
                }

                if (isThuongActive2)
                {
                    int key = GetCharPressed();
                    while (key > 0)
                    {
                        if (key >= '0' && key <= '9' && thuongLetterCount2 < 11)
                        {
                            thuongInput2[thuongLetterCount2++] = (char)key;
                            thuongInput2[thuongLetterCount2] = '\0';
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
                    {
                        if (thuongLetterCount2 > 0)
                        {
                            thuongLetterCount2--;
                            thuongInput2[thuongLetterCount2] = '\0';
                        }
                    }
                }

                // Vẽ ô nhập số tiền thưởng
                DrawRectangleRounded(boxThuong, 0.25f, 6, isThuongActive2 ? WHITE : Color{245, 246, 250, 255});
                DrawRectangleRoundedLinesEx(boxThuong, 0.25f, 6, 1.5f, isThuongActive2 ? Color{98, 70, 234, 255} : LIGHTGRAY);
                DrawTextEx(myFont, thuongInput2, {boxThuong.x + 8, boxThuong.y + 6}, 20, 1, BLACK);

                // Con trỏ nhấp nháy
                if (isThuongActive2 && ((int)(GetTime() * 2) % 2 == 0))
                {
                    float tw = MeasureTextEx(myFont, thuongInput2, 20, 1).x;
                    DrawLineEx({boxThuong.x + 8 + tw + 2, boxThuong.y + 6}, {boxThuong.x + 8 + tw + 2, boxThuong.y + 28}, 1.5f, Color{98, 70, 234, 255});
                }

                // Nút Lưu
                DrawRectangleRounded(btnLuuThuong, 0.25f, 6, hoverLuu ? Color{80, 50, 215, 255} : Color{98, 70, 234, 255});
                DrawTextEx(tieude, "Lưu", {btnLuuThuong.x + 12, btnLuuThuong.y + 8}, 18, 1, WHITE);

                // Nút Hủy
                DrawRectangleRounded(btnHuyThuong, 0.25f, 6, hoverHuy ? Color{225, 230, 238, 255} : Color{240, 242, 246, 255});
                DrawRectangleRoundedLinesEx(btnHuyThuong, 0.25f, 6, 1.0f, Color{200, 205, 215, 255});
                DrawTextEx(myFont, "Hủy", {btnHuyThuong.x + 11, btnHuyThuong.y + 8}, 18, 1, DARKGRAY);

                // Xử lý Lưu: click Lưu hoặc nhấn Enter
                bool doSave = (Clicked && hoverLuu) || (isThuongActive2 && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)));
                if (doSave)
                {
                    int newT = 0;
                    if (strlen(thuongInput2) > 0)
                    {
                        try
                        {
                            long long val = stoll(thuongInput2);
                            if (val < 0)
                                val = 0;
                            if (val > 2000000000LL)
                                val = 2000000000LL;
                            newT = (int)val;
                        }
                        catch (...)
                        {
                            newT = 0;
                        }
                    }
                    pt->set_ThuongPhat(newT);
                    QL->get_DB()->SaveFileEmployee();
                    tongluong = QL->TinhLuong(pt);
                    if (tongluong < 0)
                        tongluong = 0;
                    thuclinh = tongluong - khautru;
                    HienThongBao("Cập nhật tiền thưởng thành công!", false);
                    isEditingThuong2 = false;
                    isThuongActive2 = false;
                }

                // Xử lý Hủy: click Hủy hoặc nhấn Escape
                if ((Clicked && hoverHuy) || IsKeyPressed(KEY_ESCAPE))
                {
                    isEditingThuong2 = false;
                    isThuongActive2 = false;
                }
            }
            rowY += rowSpacing + 10;

            // 6. Box Tổng thu nhập (Hộp xanh nhạt bo góc với icon máy tính)
            Rectangle boxTongThuNhap = {leftX, rowY, modal.width - 76, 58};
            DrawRectangleRounded(boxTongThuNhap, 0.25f, 8, Color{242, 246, 255, 255});
            DrawRectangleRoundedLinesEx(boxTongThuNhap, 0.25f, 8, 1.2f, Color{220, 230, 252, 255});

            // Vẽ icon máy tính nhỏ
            Rectangle iconCalc = {leftX + 16, rowY + 16, 22, 26};
            DrawRectangleRounded(iconCalc, 0.2f, 4, Color{50, 90, 220, 255});
            DrawRectangle(iconCalc.x + 3, iconCalc.y + 4, 16, 4, WHITE);
            DrawCircle(iconCalc.x + 6, iconCalc.y + 13, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 11, iconCalc.y + 13, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 16, iconCalc.y + 13, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 6, iconCalc.y + 20, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 11, iconCalc.y + 20, 1.5f, WHITE);
            DrawCircle(iconCalc.x + 16, iconCalc.y + 20, 1.5f, WHITE);

            DrawTextEx(tieude, "Tổng thu nhập", {leftX + 50, rowY + 16}, 24, 1, Color{40, 75, 215, 255});
            string sTong = FormatSoTien((long long)tongluong) + " đ";
            float wTong = MeasureTextEx(tieude, sTong.c_str(), 26, 1).x;
            DrawTextEx(tieude, sTong.c_str(), {rightX - wTong - 16, rowY + 15}, 26, 1, Color{30, 60, 200, 255});
            rowY += 72;

            // 7. Khấu trừ (Icon tròn đỏ có dấu trừ)
            DrawCircle(leftX + 18, rowY + 13, 12, Color{245, 95, 85, 255});
            DrawLineEx({leftX + 11, rowY + 13}, {leftX + 25, rowY + 13}, 2.2f, WHITE);
            DrawTextEx(myFont, "Khấu trừ", {leftX + 44, rowY}, labelSize, 1, Color{100, 105, 115, 255});
            string sKT = FormatSoTien((long long)khautru) + " đ";
            float wKT = MeasureTextEx(myFont, sKT.c_str(), valSize, 1).x;
            DrawTextEx(myFont, sKT.c_str(), {rightX - wKT, rowY}, valSize, 1, Color{100, 105, 115, 255});
            rowY += 52;

            // 8. Box Thực lĩnh (Hộp tím nhạt bo góc với icon checkmark tím)
            Rectangle boxThucLinh = {leftX, rowY, modal.width - 76, 62};
            DrawRectangleRounded(boxThucLinh, 0.25f, 8, Color{246, 242, 255, 255});
            DrawRectangleRoundedLinesEx(boxThucLinh, 0.25f, 8, 1.2f, Color{225, 215, 250, 255});

            // Icon tròn tím có dấu checkmark
            DrawCircle(leftX + 20, rowY + 31, 13, Color{98, 70, 234, 255});
            DrawLineEx({leftX + 14, rowY + 31}, {leftX + 19, rowY + 36}, 2.4f, WHITE);
            DrawLineEx({leftX + 19, rowY + 36}, {leftX + 27, rowY + 24}, 2.4f, WHITE);

            DrawTextEx(tieude, "Thực lĩnh", {leftX + 50, rowY + 18}, 26, 1, Color{98, 70, 234, 255});
            string sTL = FormatSoTien((long long)thuclinh) + " đ";
            float wTL = MeasureTextEx(tieude, sTL.c_str(), 28, 1).x;
            DrawTextEx(tieude, sTL.c_str(), {rightX - wTL - 16, rowY + 17}, 28, 1, Color{98, 70, 234, 255});

            // Xử lý đóng modal: click [X] hoặc click ra ngoài modal (chỉ khi không phải vừa mở ở frame này)
            if (Clicked && !openedThisFrame2)
            {
                if (hoverX || !CheckCollisionPointRec(mousePos, modal))
                {
                    isOpenChiTiet2 = false;
                    isEditingThuong2 = false;
                    isThuongActive2 = false;
                }
            }
        }
    }
}

void Draw_Screen_Salary(QLNV *QL)
{
    static Texture2D a1;
    static bool isLoaded = false;
    if (!isLoaded)
    {
        a1 = LoadTexture("photo/anhto/Asset-3.png");
        isLoaded = true;
    }
    Vector2 pa1 = {370.0f, 45.0f}; // Tọa độ (x, y) postion
    float ra1 = 0.0f;              // Góc xoay (0 độ) rotation
    float sa1 = 0.33f;
    DrawTextureEx(a1, pa1, ra1, sa1, WHITE);
    DrawTextEx(tieude, "Quản Lý Lương & Mức Lương", {pa1.x + 100, pa1.y}, 45, 1, WHITE);
    DrawTextEx(myFont, "Bảng lương nhân sự và định mức lương theo bậc của công ty", {pa1.x + 100, pa1.y + 50}, 30, 1, WHITE);

    static bool isOpenBL = true;
    static bool isOpenML = false;
    Vector2 mousePos = GetMousePosition();
    bool Clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float TabX = 330.0f;
    float TabY = 160.0f;
    float TabH = 90.0f;
    Rectangle TabBL = {TabX, TabY, 300.0f, TabH};
    Rectangle TabML = {TabX + 310.0f, TabY, 300.0f, TabH};
    bool hoverPB = CheckCollisionPointRec(mousePos, TabBL);
    bool hoverCV = CheckCollisionPointRec(mousePos, TabML);
    Color CTabBL = isOpenBL == 1 ? BLUE : WHITE;
    Color CTabVT = isOpenML == 1 ? BLUE : WHITE;
    if (isOpenBL == true && hoverCV)
        CTabVT = Fade(BLUE, 0.4);
    if (isOpenML == true && hoverPB)
        CTabBL = Fade(BLUE, 0.4);
    DrawRectangleRounded(TabBL, 0.3f, 10, CTabBL);
    DrawRectangleRoundedLinesEx(TabBL, 0.3f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Bảng lương", {TabBL.x + 70.0f, TabBL.y + 15.0f}, 40, 1, BLACK);

    DrawRectangleRounded(TabML, 0.3f, 10, CTabVT);
    DrawRectangleRoundedLinesEx(TabBL, 0.3f, 10, 1.5f, Color{225, 230, 240, 255});
    DrawTextEx(myFont, "Mức lương", {TabML.x + 90.0f, TabML.y + 15.0f}, 40, 1, BLACK);

    // Rectangle panelBang = {330, 230, 1450.0f, 690.0f};
    // DrawRectangleRounded({panelBang.x + 3, panelBang.y + 4, panelBang.width, panelBang.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    // DrawRectangleRounded(panelBang, 0.03f, 10, WHITE);
    // DrawRectangleRoundedLinesEx(panelBang, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});

    if (Clicked && CheckCollisionPointRec(mousePos, TabBL))
    {
        isOpenBL = true;
        isOpenML = false;
    }
    else if (Clicked && CheckCollisionPointRec(mousePos, TabML))
    {
        isOpenML = true;
        isOpenBL = false;
    }

    if (isOpenBL)
    {
        Draw_Screen_BangLuong(mousePos, Clicked, QL);
    }
    else if (isOpenML)
    {
        Draw_Screen_MucLuong(mousePos, Clicked, QL);
    }
}
/*
void Draw_Screen_Search(QLNV *QL)
{
    Vector2 mousePos = GetMousePosition();
    Rectangle Title = {300.0f, 0.0f, 1620.0f, 30.0f};
    DrawRectangle(Title.x, Title.y, Title.width, Title.height, GRAY);
    DrawTextEx(myFont,"Tim kiem thong tin", Title.x, Title.y + 5, 25, BLACK);

    Rectangle btnThongTin = {850.0f, 200.0f, 300.0f, 35.0f};
    DrawRectangleRounded(btnThongTin, 0.3f, 4, Fade(BLUE, 0.2f));
    DrawTextEx(myFont,"Thong Tin", btnThongTin.x - 200, btnThongTin.y + 7, 25, BLACK);
    static bool isTimKiemActive = false;
    static char TimKiemInput[50] = "\0";
    static int letterCount_TimKiem = 0;
    Typing(isTimKiemActive, TimKiemInput, letterCount_TimKiem, mousePos, btnThongTin);
    DrawTextEx(myFont,TimKiemInput, btnThongTin.x + 10, btnThongTin.y + 8, 20, BLACK);
    if (isTimKiemActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(TimKiemInput, 20);
        DrawTextEx(myFont,"_", btnThongTin.x + 14 + textWidth, btnThongTin.y + 11, 20, MAROON);
    }
    // 1. Định nghĩa các thông số dùng chung
    // ==========================================
    // TÙY CHỌN 1: MÃ NHÂN VIÊN
    // ==========================================
    int text1X = btnThongTin.x - 80;
    int text1Y = btnThongTin.y + 120;

    // Tính tọa độ tâm hình tròn dựa trên tọa độ chữ
    Vector2 circle1Center = {(float)(text1X - 25), (float)(text1Y + 25 / 2.0f)};

    // TÙY CHỌN 2: HỌ VÀ TÊN
    int text2X = btnThongTin.x + 200;
    int text2Y = btnThongTin.y + 120;

    // Tính tọa độ tâm hình tròn dựa trên tọa độ chữ
    Vector2 circle2Center = {(float)(text2X - 25), (float)(text2Y + 25 / 2.0f)};

    bool isHover1 = CheckCollisionPointCircle(mousePos, circle1Center, 10);
    bool isHover2 = CheckCollisionPointCircle(mousePos, circle2Center, 10);
    static int selectedRadio = 0;

    // KIỂM TRA CLICK CHUỘT
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (isHover1)
            selectedRadio = 1;
        else if (isHover2)
            selectedRadio = 2;
    }

    DrawCircleLines(circle1Center.x, circle1Center.y, 10, DARKGRAY);

    if (selectedRadio == 1)
    {
        // Trạng thái 1: Đã được click chọn -> Vẽ tâm đậm
        DrawCircle(circle1Center.x, circle1Center.y, 10 - 4, DARKGRAY);
    }
    else if (isHover1)
    {
        // Trạng thái 2: Chưa chọn, nhưng đang rê chuột vào -> Vẽ tâm màu nhạt mờ mờ
        DrawCircle(circle1Center.x, circle1Center.y, 10 - 4, LIGHTGRAY);
    }

    DrawTextEx(myFont,"Ma nhan vien", text1X, text1Y, 25, BLACK);

    // ==========================================
    // VẼ NÚT 2: HỌ VÀ TÊN
    // ==========================================
    DrawCircleLines(circle2Center.x, circle2Center.y, 10, DARKGRAY);

    if (selectedRadio == 2)
    {
        DrawCircle(circle2Center.x, circle2Center.y, 10 - 4, DARKGRAY);
    }
    else if (isHover2)
    {
        DrawCircle(circle2Center.x, circle2Center.y, 10 - 4, LIGHTGRAY);
    }

    DrawTextEx(myFont,"Ho va ten", text2X, text2Y, 25, BLACK);
}
**/