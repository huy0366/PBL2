#include "raylib.h"
#include <ctime>
#include "QLNV.h"
#include <bits/stdc++.h>
using namespace std;
Font myFont, tieude;
void DrawSidebar(int &currentScreen);
void DrawMainContent(int &currentScreen, Texture2D &BackGround, QLNV *QL);
void Draw_Screen_Home(Texture2D &BackGround);
void Draw_Screen_Employee(QLNV *QL);
// void Draw_Screen_Deparment(QLNV *QL);
bool DrawCalendar(Vector2 pos, int &selDay, int &month, int &year, Rectangle &Calendar, bool &isOpenCalendar);
// void Draw_Screen_Salary(QLNV *QL); // 2 cái duyệt cho nhau
// void Draw_Screen_Search(QLNV *QL);
void HieuUngGo(bool &active, Rectangle &btn, char input[]);
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

// 1. Gọi hàm này mỗi khi muốn bật thông báo
void HienThongBao(const string &noiDung, bool laLoi = true)
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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
        // Draw_Screen_Deparment(QL);
    }
    else if (currentScreen == 3)
    {
        // Draw_Screen_Salary(QL);
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

void HieuUngGo(bool &active, Rectangle &btn, char input[])
{
    if (active && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(input, 20);
        DrawTextEx(myFont, "_", {btn.x + 12 + textWidth, btn.y + 8}, 20, 1, MAROON);
    }
    DrawTextEx(myFont, input, {btn.x + 10, btn.y + 8}, 20, 1, BLACK);
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
void Draw_Screen_Employee(QLNV *QL)
{
    Rectangle panelBang = {330, 160, 1450.0f, 835.0f};
    DrawRectangleRounded({panelBang.x + 3, panelBang.y + 4, panelBang.width, panelBang.height}, 0.03f, 10, Fade(BLACK, 0.08f));
    DrawRectangleRounded(panelBang, 0.03f, 10, WHITE);
    DrawRectangleRoundedLinesEx(panelBang, 0.03f, 10, 1.5f, Color{225, 230, 240, 255});
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
    DrawTextEx(tieude, "Quản Lý Nhân Sự", {pa1.x + 100, pa1.y}, 45, 1, WHITE);
    DrawTextEx(myFont, "Danh sách và thông tin nhân sự của công ty", {pa1.x + 100, pa1.y + 50}, 30, 1, WHITE);
    Vector2 mousePos = GetMousePosition();
    bool Clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
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

    Rectangle comboChucVu = {350, 220, 250, 45};
    static bool isChucVuOpen = false;
    static int selectedChucVuIndex = 0;
    string default_choice = "Tất cả";

    Rectangle comboPhong = {620, 220, 250, 45};
    static bool isPhongOpen = false;
    static int selectedPhongIndex = 0;

    Rectangle textHoTen = {890, 220, 350, 45};
    // Các biến cho ô nhập liệu Họ Tên
    static bool isHoTenActive = false; // Trạng thái: ô có đang được click vào không
    static char hoTenInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount = 0;

    Rectangle comboSortTT = {1560, 220, 200, 45}; // thứ tự (a->z, z->a, tuổi ...)
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
    float startY = 290.f; // Lề trên
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
    DrawTextEx(myFont, "Chức Vụ", {350, 170}, 40, 1, BLACK);

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
    DrawTextEx(myFont, "Phòng Ban", {620, 175}, 40, 1, BLACK);

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
    DrawTextEx(myFont, "Tìm kiếm", {890, 170}, 40, 1, BLACK);
    // Vẽ khung trắng bo góc cho ô nhập liệu, nếu đang active thì viền xám nhẹ
    DrawRectangleRounded(textHoTen, 0.5f, 10, isHoTenActive ? LIGHTGRAY : WHITE);
    DrawRectangleRoundedLinesEx(textHoTen, 0.3f, 12, 3.0f, RED);
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

    Rectangle comboSort = {1290, 220, 250, 45};
    static bool isSortOpen = false;
    static int selectedSortIndex = 0;
    string dsSort[] = {"...", "Mã NV", "Họ và Tên", "Ngày Sinh"};
    int nSort = 3;

    DrawTextEx(myFont, "Sắp xếp theo:", {1290, 170}, 40, 1, BLACK);
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

                    if (selLoai == 0)
                        nv = new FullTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac);
                    else
                        nv = new PartTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac);

                    if (clickThem)
                    {
                        QL->ThemEmployee(nv);
                        HienThongBao("Thêm nhân viên mới thành công!", false); // false = Màu xanh
                    }
                    else
                    {
                        QL->SuaEmployee(nv);
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
}
/**
void Draw_Screen_Deparment(QLNV *QL)
{
    // Vẽ tiêu đề
    Vector2 mousePos = GetMousePosition();
    Rectangle Title = {300.0f, 0.0f, 1620.0f, 60.0f};

    Rectangle textMaPhongBan = {900, 200, 300, 40};
    // Các biến cho ô nhập liệu MaPhongBan
    static bool isMaPhongBanActive = false; // Trạng thái: ô có đang được click vào không
    static char MaPhongBanInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Ma = 0;
    Typing(isMaPhongBanActive, MaPhongBanInput, letterCount_Ma, mousePos, textMaPhongBan);

    DrawText("Ma Phong Ban", 730, 210, 20, BLACK);
    DrawRectangle(Title.x, Title.y, Title.width, Title.height, GRAY);
    DrawText("Quan ly phong ban", 300.0f, 15.0f, 25, BLACK);
    DrawRectangleRounded(textMaPhongBan, 0.5f, 10, isMaPhongBanActive ? LIGHTGRAY : WHITE);
    DrawText(MaPhongBanInput, textMaPhongBan.x + 10, textMaPhongBan.y + 8, 20, BLACK);
    // Nhấp nháy con trỏ chuột mô phỏng (dấu gạch dưới)
    if (isMaPhongBanActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(MaPhongBanInput, 20);
        DrawText("_", textMaPhongBan.x + 12 + textWidth, textMaPhongBan.y + 8, 20, MAROON);
    }

    Rectangle textTenPhongBan = {900, 285, 300, 40};
    // Các biến cho ô nhập liệu MaPhongBan
    static bool isTenPhongBanActive = false; // Trạng thái: ô có đang được click vào không
    static char TenPhongBanInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Ten = 0;

    Typing(isTenPhongBanActive, TenPhongBanInput, letterCount_Ten, mousePos, textTenPhongBan);

    DrawText("Ten Phong Ban", 730, 300, 20, BLACK);
    DrawRectangleRounded(textTenPhongBan, 0.5f, 10, isTenPhongBanActive ? LIGHTGRAY : WHITE);
    DrawText(TenPhongBanInput, textTenPhongBan.x + 10, textTenPhongBan.y + 8, 20, BLACK);
    // Nhấp nháy con trỏ chuột mô phỏng (dấu gạch dưới)
    if (isTenPhongBanActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(MaPhongBanInput, 20);
        DrawText("_", textTenPhongBan.x + 12 + textWidth, textTenPhongBan.y + 8, 20, MAROON);
    }

    DrawText("Ngay thanh lap", 730, 370, 20, BLACK);
    Rectangle textNgayThanhLap = {900, 360, 300, 40};
    static bool isOpenCalendar = false;

    string textCalendar = to_string(day) + '/' + to_string(month) + '/' + to_string(year);
    if (day < 10)
    {
        textCalendar = '0' + textCalendar;
    }
    if (month + 1 < 10)
    {
        textCalendar.insert(3, "0");
    }
    DrawRectangleRounded(textNgayThanhLap, 0.5f, 10, isOpenCalendar ? LIGHTGRAY : WHITE);
    DrawText(textCalendar.c_str(), textNgayThanhLap.x + 5, textNgayThanhLap.y + 10, 25, BLACK);

    // Vẽ nút chức năng để ở trên để không bị che
    Rectangle btnThemPhongBan = {450, 500, 200, 40};
    DrawRectangleRounded(btnThemPhongBan, 0.2f, 10, BLUE);
    DrawText("Them Phong Ban", btnThemPhongBan.x + 15, btnThemPhongBan.y + 10, 20, BLACK);
    Rectangle btnSuaPhongBan = {1050, 500, 200, 40};
    DrawRectangleRounded(btnSuaPhongBan, 0.2f, 10, BLUE);
    DrawText("Sua Phong Ban", btnSuaPhongBan.x + 20, btnSuaPhongBan.y + 10, 20, BLACK);
    Rectangle btnLamMoi = {1650, 500, 200, 40};
    DrawRectangleRounded(btnLamMoi, 0.2f, 10, BLUE);
    DrawText("Lam Moi", btnLamMoi.x + 65, btnLamMoi.y + 10, 20, BLACK);
}

void Draw_Screen_Salary(QLNV *QL)
{
    Vector2 mousePos = GetMousePosition();
    Rectangle Title = {300.0f, 0.0f, 1620.0f, 30.0f};
    DrawRectangle(Title.x, Title.y, Title.width, Title.height, GRAY);
    DrawText("Thong tin bang luong", Title.x, Title.y, 25, BLACK);

    Rectangle btnBacLuong = {550.0f, 200.0f, 200.0f, 40.f};
    DrawRectangleRounded(btnBacLuong, 0.3f, 4, Fade(BLUE, 0.2f));
    DrawText("Bac luong", btnBacLuong.x - 120, btnBacLuong.y + 10, 20, BLACK);

    Rectangle btnPhuCapKhac = {950.0f, 200.0f, 250.0f, 40.f};
    DrawRectangleRounded(btnPhuCapKhac, 0.3f, 4, Fade(BLUE, 0.2f));
    DrawText("Phu cap khac", btnPhuCapKhac.x - 150, btnPhuCapKhac.y + 10, 20, BLACK);
    static bool isPhuCapKhacActive = false;  // Trạng thái: ô có đang được click vào không
    static char PhucCapKhacInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_PhucCapKhac = 0;
    Typing(isPhuCapKhacActive, PhucCapKhacInput, letterCount_PhucCapKhac, mousePos, btnPhuCapKhac);
    DrawText(PhucCapKhacInput, btnPhuCapKhac.x + 10, btnPhuCapKhac.y + 8, 20, BLACK);
    if (isPhuCapKhacActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(PhucCapKhacInput, 20);
        DrawText("_", btnPhuCapKhac.x + 14 + textWidth, btnPhuCapKhac.y + 11, 20, MAROON);
    }

    Rectangle btnThuong = {550.0f, 300.0f, 250.0f, 40.f};
    DrawRectangleRounded(btnThuong, 0.3f, 4, Fade(BLUE, 0.2f));
    DrawText("Thuong", btnThuong.x - 100, btnThuong.y + 10, 20, BLACK);
    static bool isThuongActive = false; // Trạng thái: ô có đang được click vào không
    static char ThuongInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_Thuong = 0;
    Typing(isThuongActive, ThuongInput, letterCount_Thuong, mousePos, btnThuong);
    DrawText(ThuongInput, btnThuong.x + 10, btnThuong.y + 8, 20, BLACK);
    if (isThuongActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(ThuongInput, 20);
        DrawText("_", btnThuong.x + 14 + textWidth, btnThuong.y + 11, 20, MAROON);
    }

    Rectangle btnKhauTru = {950.0f, 300.0f, 250.0f, 40.f};
    DrawRectangleRounded(btnKhauTru, 0.3f, 4, Fade(BLUE, 0.2f));
    DrawText("Khau tru", btnKhauTru.x - 100, btnKhauTru.y + 10, 20, BLACK);
    static bool isKhauTruActive = false; // Trạng thái: ô có đang được click vào không
    static char KhauTruInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount_KhauTru = 0;
    Typing(isKhauTruActive, KhauTruInput, letterCount_KhauTru, mousePos, btnKhauTru);
    DrawText(KhauTruInput, btnKhauTru.x + 10, btnKhauTru.y + 8, 20, BLACK);
    if (isKhauTruActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(KhauTruInput, 20);
        DrawText("_", btnKhauTru.x + 14 + textWidth, btnKhauTru.y + 11, 20, MAROON);
    }
}

void Draw_Screen_Search(QLNV *QL)
{
    Vector2 mousePos = GetMousePosition();
    Rectangle Title = {300.0f, 0.0f, 1620.0f, 30.0f};
    DrawRectangle(Title.x, Title.y, Title.width, Title.height, GRAY);
    DrawText("Tim kiem thong tin", Title.x, Title.y + 5, 25, BLACK);

    Rectangle btnThongTin = {850.0f, 200.0f, 300.0f, 35.0f};
    DrawRectangleRounded(btnThongTin, 0.3f, 4, Fade(BLUE, 0.2f));
    DrawText("Thong Tin", btnThongTin.x - 200, btnThongTin.y + 7, 25, BLACK);
    static bool isTimKiemActive = false;
    static char TimKiemInput[50] = "\0";
    static int letterCount_TimKiem = 0;
    Typing(isTimKiemActive, TimKiemInput, letterCount_TimKiem, mousePos, btnThongTin);
    DrawText(TimKiemInput, btnThongTin.x + 10, btnThongTin.y + 8, 20, BLACK);
    if (isTimKiemActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(TimKiemInput, 20);
        DrawText("_", btnThongTin.x + 14 + textWidth, btnThongTin.y + 11, 20, MAROON);
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

    DrawText("Ma nhan vien", text1X, text1Y, 25, BLACK);

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

    DrawText("Ho va ten", text2X, text2Y, 25, BLACK);
}
**/