#include "raylib.h"
#include <ctime>
#include "QLNV.h"
#include <bits/stdc++.h>
using namespace std;

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

enum Screen
{
    SCREEN_HOME,       // Trang Chủ
    SCREEN_EMPLOYEE,   // Nhân viên
    SCREEN_DEPARTMENT, // Bộ phận
    SCREEN_SALARY,     // Bảng lương
    SCREEN_Search,     // Hệ thống
    Exit,              // Thoát hệ thống
};
Font myFont;
int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1792, 1008, "PBL2");
    SetTargetFPS(60);
    // lấy ảnh
    Texture2D BackGround = LoadTexture("BackGround.png");
    myFont = LoadFontEx("font/times.ttf", 32, 0, 0);
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

        EndDrawing();
    }
    UnloadTexture(BackGround);
    CloseWindow();
}

void DrawSidebar(int &currentScreen)
{
    const char *menu[] = {"HOME",
                          "EMPLOYEE",
                          "DEPARTMENT",
                          "SALARY",
                          "SEARCH",
                          "Exit"};
    int startX = 30; // Vị trí X bắt đầu lề trái
    int startY = 80; // Vị trí Y của mục đầu tiên
    int spacing = 50;

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
            DrawTextEx(myFont, menu[i], {(startX + 10), currentY}, 20, 1, BLACK);
        }
        else
        {
            DrawTextEx(myFont, menu[i], {(startX + 10), currentY}, 20, 1, BLACK);
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
        DrawText("_", btn.x + 12 + textWidth, btn.y + 8, 20, MAROON);
    }
    DrawText(input, btn.x + 10, btn.y + 8, 20, BLACK);
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
        DrawText(TextFormat("Nam %d", year), Calendar.x + 85, Calendar.y + 50, 18, BLACK);
        // Biến này giúp lùi/tiến danh sách năm mà không làm thay đổi biến year gốc cho đến khi user thực sự click chọn
        static int yearOffset = 0;
        // ---------------- MÀN HÌNH CHỌN NĂM ----------------
        // Nút lùi 12 năm (<<)
        Rectangle btnPrevYear = {Calendar.x + 10, Calendar.y + 50, 30, 22};
        DrawRectangleRec(btnPrevYear, WHITE);
        DrawText("<<", btnPrevYear.x + 8, btnPrevYear.y + 3, 16, BLACK);
        if (clicked && CheckCollisionPointRec(mousepos, btnPrevYear))
        {
            yearOffset -= 12;
        }

        // Nút tiến 12 năm (>>)
        Rectangle btnNextYear = {Calendar.x + 210, Calendar.y + 50, 30, 22};
        DrawRectangleRec(btnNextYear, WHITE);
        DrawText(">>", btnNextYear.x + 8, btnNextYear.y + 3, 16, BLACK);
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
            DrawText(TextFormat("%d", displayYear), cellYear.x + 5, cellYear.y + 8, 16, BLACK);

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
        DrawText(TextFormat("Thang %d / %d", month, year), Calendar.x + 45, Calendar.y + 50, 18, BLACK);
        Rectangle btnPrev = {Calendar.x + 10, Calendar.y + 50, 30, 22};
        DrawRectangleRec(btnPrev, WHITE);
        // Nút lùi tháng(<)
        DrawText("<", btnPrev.x + 10, btnPrev.y + 3, 16, BLACK);
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
        DrawText(">", btnNext.x + 10, btnNext.y + 3, 16, BLACK);
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
            DrawText(days[i], Calendar.x + 12 + i * 34, Calendar.y + 80, 14, DARKGRAY);
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
            DrawText(TextFormat("%d", d), cell.x + (d < 10 ? 10 : 6), cell.y + 4, 14, BLACK);

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
bool XuLyCombo(Rectangle combo, bool &isOpen, int &selected, int count,
               Vector2 mousePos, bool Clicked, bool choPhepMo)
{
    if (!Clicked)
        return false;

    if (isOpen)
    {
        for (int i = 0; i < count; i++)
        {
            Rectangle itemRect = {combo.x, combo.y + 30 + (i * 30), combo.width, 30};
            if (CheckCollisionPointRec(mousePos, itemRect))
            {
                selected = i;
                isOpen = false;
                return true;
            }
        }
        isOpen = false; // click ra ngoài -> đóng
        return true;
    }
    else if (choPhepMo && CheckCollisionPointRec(mousePos, combo))
    {
        isOpen = true;
        return true;
    }
    return false;
}

// Vẽ ô + chữ đã chọn + tam giác (giống phần "Vẽ box" + "Vẽ tam giác")
void VeCombo(Rectangle combo, int selected, string items[], int count)
{
    DrawRectangleRounded(combo, 0.2f, 10, WHITE);
    if (selected >= 0 && selected < count)
    {
        DrawText(items[selected].c_str(), combo.x + 5, combo.y + 5, 18, DARKGRAY);
    }
    Vector2 p1 = {combo.x + combo.width - 15, combo.y + 12};
    Vector2 p2 = {combo.x + combo.width - 5, combo.y + 12};
    Vector2 p3 = {combo.x + combo.width - 10, combo.y + 20};
    DrawTriangle(p1, p3, p2, DARKGRAY);
}

// Vẽ bảng xổ xuống (giống đoạn if (isChucVuOpen) {...}), gọi CUỐI CÙNG
void VeDanhSachCombo(Rectangle combo, bool isOpen, string items[], int count, Vector2 mousePos)
{
    if (!isOpen)
        return;
    for (int i = 0; i < count; i++)
    {
        Rectangle itemRect = {combo.x, combo.y + 31 + (i * 30), combo.width, 30};
        bool isHover = CheckCollisionPointRec(mousePos, itemRect);
        DrawRectangleRec(itemRect, isHover ? LIGHTGRAY : RAYWHITE);
        DrawRectangleLinesEx(itemRect, 1, GRAY);
        DrawText(items[i].c_str(), itemRect.x + 10, itemRect.y + 6, 16, BLACK);
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

void Draw_Screen_Employee(QLNV *QL)
{
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

    Rectangle comboChucVu = {300, 90, 150, 30};
    static bool isChucVuOpen = false;
    static int selectedChucVuIndex = 0;
    string default_choice = "Tat ca";

    if (Clicked)
    {
        // Nếu danh sách đang MỞ -> Ưu tiên kiểm tra người dùng có click chọn mục nào không
        if (isChucVuOpen)
        {
            bool clickedInsideList = false;
            for (int i = 0; i <= QL->get_DB()->get_ListCV()->get_size(); i++)
            {
                Rectangle itemRect = {comboChucVu.x, comboChucVu.y + 30 + (i * 30), comboChucVu.width, 30};
                if (CheckCollisionPointRec(mousePos, itemRect))
                {
                    selectedChucVuIndex = i; // Cập nhật mục được chọn
                    isChucVuOpen = false;    // Đóng dropdown lại
                    clickedInsideList = true;
                    break;
                }
            }

            // Nếu click ra ngoài bảng danh sách thì đóng dropdown lại
            if (!clickedInsideList && !CheckCollisionPointRec(mousePos, comboChucVu))
            {
                isChucVuOpen = false;
            }
        }
        // Nếu danh sách đang ĐÓNG -> Click vào ô thì MỞ ra
        else if (CheckCollisionPointRec(mousePos, comboChucVu))
        {
            isChucVuOpen = true;
        }
    }

    Rectangle comboPhong = {550, 90, 150, 30};
    static bool isPhongOpen = false;
    static int selectedPhongIndex = 0;

    if (Clicked)
    {
        // Nếu danh sách đang MỞ -> Ưu tiên kiểm tra người dùng có click chọn mục nào không
        if (isPhongOpen)
        {
            bool clickedInsideList = false;
            for (int i = 0; i <= QL->get_DB()->get_ListPB()->get_size(); i++)
            {
                Rectangle itemRect = {comboPhong.x, comboPhong.y + 30 + (i * 30), comboPhong.width, 30};
                if (CheckCollisionPointRec(mousePos, itemRect))
                {
                    selectedPhongIndex = i; // Cập nhật mục được chọn
                    isPhongOpen = false;    // Đóng dropdown lại
                    clickedInsideList = true;
                    break;
                }
            }

            // Nếu click ra ngoài bảng danh sách thì đóng dropdown lại
            if (!clickedInsideList && !CheckCollisionPointRec(mousePos, comboPhong))
            {
                isPhongOpen = false;
            }
        }
        // Nếu danh sách đang ĐÓNG -> Click vào ô thì MỞ ra
        else if (CheckCollisionPointRec(mousePos, comboPhong))
        {
            isPhongOpen = true;
        }
    }
    Rectangle comboSort = {1200, 90, 130, 30};
    static bool isSortOpen = false;
    static int selectedSortIndex = 0;
    string listSort[] = {"Ma NV", "Ho Ten", "Ngay Sinh"};
    int sizeSort = 3;
    if (Clicked)
    {
        // Nếu danh sách đang MỞ -> Ưu tiên kiểm tra người dùng có click chọn mục nào không
        if (isSortOpen)
        {
            bool clickedInsideList = false;
            for (int i = 0; i <= sizeSort; i++)
            {
                Rectangle itemRect = {comboSort.x, comboSort.y + 30 + (i * 30), comboSort.width, 30};
                if (CheckCollisionPointRec(mousePos, itemRect))
                {
                    selectedSortIndex = i; // Cập nhật mục được chọn
                    isSortOpen = false;    // Đóng dropdown lại
                    clickedInsideList = true;
                    break;
                }
            }

            // Nếu click ra ngoài bảng danh sách thì đóng dropdown lại
            if (!clickedInsideList && !CheckCollisionPointRec(mousePos, comboSort))
            {
                isSortOpen = false;
            }
        }
        // Nếu danh sách đang ĐÓNG -> Click vào ô thì MỞ ra
        else if (CheckCollisionPointRec(mousePos, comboSort))
        {
            isSortOpen = true;
        }
    }
    Rectangle comboSortTT = {1400, 90, 100, 30}; // thứ tự (a->z, z->a, tuổi ...)
    static bool isSortTTOpen = false;
    static int selectedSortTTIndex = 0;
    string listSortTT[] = {"tăng dần", "giảm dần"};
    int sizeSortTT = 2;
    if (Clicked)
    {
        // Nếu danh sách đang MỞ -> Ưu tiên kiểm tra người dùng có click chọn mục nào không
        if (isSortTTOpen)
        {
            bool clickedInsideList = false;
            for (int i = 0; i <= sizeSort; i++)
            {
                Rectangle itemRect = {comboSortTT.x, comboSortTT.y + 30 + (i * 30), comboSortTT.width, 30};
                if (CheckCollisionPointRec(mousePos, itemRect))
                {
                    selectedSortTTIndex = i; // Cập nhật mục được chọn
                    isSortTTOpen = false;    // Đóng dropdown lại
                    clickedInsideList = true;
                    break;
                }
            }

            // Nếu click ra ngoài bảng danh sách thì đóng dropdown lại
            if (!clickedInsideList && !CheckCollisionPointRec(mousePos, comboSortTT))
            {
                isSortTTOpen = false;
            }
        }
        // Nếu danh sách đang ĐÓNG -> Click vào ô thì MỞ ra
        else if (CheckCollisionPointRec(mousePos, comboSortTT))
        {
            isSortTTOpen = true;
        }
    }

    Rectangle textHoTen = {800, 90, 200, 35};
    Rectangle btnLoc = {1000, 90, 40, 35};
    // Các biến cho ô nhập liệu Họ Tên
    static bool isHoTenActive = false; // Trạng thái: ô có đang được click vào không
    static char hoTenInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
    static int letterCount = 0;

    Typing(isHoTenActive, hoTenInput, letterCount, mousePos, textHoTen);

    if (CheckCollisionPointRec(mousePos, btnLoc))
    {
        // TODO: Viết code lọc danh sách nhân viên ở đây
    }

    DrawText("Chuc Vu", 210, 95, 20, BLACK);
    // Vẽ đường gạch dưới
    DrawLine(comboChucVu.x, comboChucVu.y + 30, comboChucVu.x + comboChucVu.width, comboChucVu.y + 30, GRAY);
    // Vẽ box
    if (selectedChucVuIndex == 0)
    {
        DrawText(default_choice.c_str(), comboChucVu.x + 5, comboChucVu.y + 5, 18, DARKGRAY);
    }
    else
    {
        DrawText((*QL->get_DB()->get_ListCV())[selectedChucVuIndex - 1]->Data->get_MaChucVu().c_str(), comboChucVu.x + 5, comboChucVu.y + 5, 18, DARKGRAY);
    }

    // Vẽ tam giác
    Vector2 p1 = {comboChucVu.x + comboChucVu.width - 15, comboChucVu.y + 12}; // Góc trên-trái
    Vector2 p2 = {comboChucVu.x + comboChucVu.width - 5, comboChucVu.y + 12};  // Góc trên-phải
    Vector2 p3 = {comboChucVu.x + comboChucVu.width - 10, comboChucVu.y + 20}; // Đỉnh dưới
    DrawTriangle(p1, p3, p2, DARKGRAY);

    // 4. VẼ BẢNG DANH SÁCH XỔ XUỐNG (Chỉ vẽ khi isBoPhanOpen == true)
    // LƯU Ý: Đoạn này phải đặt ở cuối phần vẽ UI để nó nổi lên trên cùng
    // ==========================================
    if (isChucVuOpen)
    {
        for (int i = 0; i <= QL->get_DB()->get_ListCV()->get_size(); i++)
        {
            Rectangle itemRect = {comboChucVu.x, comboChucVu.y + 31 + (i * 30), comboChucVu.width, 30};
            bool isHover = CheckCollisionPointRec(mousePos, itemRect);

            // Đổi màu nền khi rê chuột vào (Hover)
            DrawRectangleRec(itemRect, isHover ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLinesEx(itemRect, 1, GRAY); // Vẽ viền mỏng

            // In tên mục
            if (i == 0)
            {
                DrawText(default_choice.c_str(), itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
            else
            {
                DrawText((*QL->get_DB()->get_ListCV())[i - 1]->Data->get_MaChucVu().c_str(), itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
        }
    }

    // 2. Cụm "Phòng"
    DrawText("Phong", 480, 95, 20, BLACK);
    // Vẽ đường gạch dưới
    DrawLine(comboPhong.x, comboPhong.y + 30, comboPhong.x + comboPhong.width, comboPhong.y + 30, GRAY);
    // Vẽ box
    if (selectedPhongIndex == 0)
    {
        DrawText(default_choice.c_str(), comboPhong.x + 5, comboPhong.y + 5, 18, DARKGRAY);
    }
    else
    {
        DrawText((*QL->get_DB()->get_ListPB())[selectedPhongIndex - 1]->Data->get_MaPhongBan().c_str(), comboPhong.x + 5, comboPhong.y + 5, 18, DARKGRAY);
    }
    p1 = {comboPhong.x + comboPhong.width - 15, comboPhong.y + 12}; // Góc trên-trái
    p2 = {comboPhong.x + comboPhong.width - 5, comboPhong.y + 12};  // Góc trên-phải
    p3 = {comboPhong.x + comboPhong.width - 10, comboPhong.y + 20}; // Đỉnh dưới
    DrawTriangle(p1, p3, p2, DARKGRAY);

    // 4. VẼ BẢNG DANH SÁCH XỔ XUỐNG (Chỉ vẽ khi isBoPhanOpen == true)
    // LƯU Ý: Đoạn này phải đặt ở cuối phần vẽ UI để nó nổi lên trên cùng
    // ==========================================
    if (isPhongOpen)
    {
        for (int i = 0; i < QL->get_DB()->get_ListPB()->get_size(); i++)
        {
            Rectangle itemRect = {comboPhong.x, comboPhong.y + 31 + (i * 30), comboPhong.width, 30};
            bool isHover = CheckCollisionPointRec(mousePos, itemRect);

            // Đổi màu nền khi rê chuột vào (Hover)
            DrawRectangleRec(itemRect, isHover ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLinesEx(itemRect, 1, GRAY); // Vẽ viền mỏng

            // In tên mục
            if (i == 0)
            {
                DrawText(default_choice.c_str(), itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
            else
            {
                DrawText((*QL->get_DB()->get_ListPB())[i - 1]->Data->get_MaPhongBan().c_str(), itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
        }
    }

    DrawText("Sap Xep", 1100, 95, 20, BLACK);
    // Vẽ đường gạch dưới
    DrawLine(comboSort.x, comboSort.y + 30, comboSort.x + comboSort.width, comboSort.y + 30, GRAY);
    // Vẽ box
    if (selectedSortIndex == 0)
    {
        DrawText("...", comboSort.x + 5, comboSort.y + 5, 18, DARKGRAY);
    }
    else
    {
        DrawText(listSort[selectedSortIndex - 1].c_str(), comboSort.x + 5, comboSort.y + 5, 18, DARKGRAY);
    }

    // Vẽ tam giác
    p1 = {comboSort.x + comboSort.width - 15, comboSort.y + 12}; // Góc trên-trái
    p2 = {comboSort.x + comboSort.width - 5, comboSort.y + 12};  // Góc trên-phải
    p3 = {comboSort.x + comboSort.width - 10, comboSort.y + 20}; // Đỉnh dưới
    DrawTriangle(p1, p3, p2, DARKGRAY);

    // 4. VẼ BẢNG DANH SÁCH XỔ XUỐNG (Chỉ vẽ khi isBoPhanOpen == true)
    // LƯU Ý: Đoạn này phải đặt ở cuối phần vẽ UI để nó nổi lên trên cùng
    // ==========================================
    if (isSortOpen)
    {
        for (int i = 0; i <= sizeSort; i++)
        {
            Rectangle itemRect = {comboSort.x, comboSort.y + 31 + (i * 30), comboSort.width, 30};
            bool isHover = CheckCollisionPointRec(mousePos, itemRect);

            // Đổi màu nền khi rê chuột vào (Hover)
            DrawRectangleRec(itemRect, isHover ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLinesEx(itemRect, 1, GRAY); // Vẽ viền mỏng

            // In tên mục
            if (i == 0)
            {
                DrawText("...", itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
            else
            {
                DrawText(listSort[i - 1].c_str(), itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
        }
    }
    if (selectedSortIndex == 0)
    {
        DrawText("...", comboSort.x + 5, comboSort.y + 5, 18, DARKGRAY);
    }
    else
    {
        DrawText(listSort[selectedSortIndex - 1].c_str(), comboSort.x + 5, comboSort.y + 5, 18, DARKGRAY);
    }

    // Vẽ tam giác
    p1 = {comboSortTT.x + comboSortTT.width - 15, comboSortTT.y + 12}; // Góc trên-trái
    p2 = {comboSortTT.x + comboSortTT.width - 5, comboSortTT.y + 12};  // Góc trên-phải
    p3 = {comboSortTT.x + comboSortTT.width - 10, comboSortTT.y + 20}; // Đỉnh dưới
    DrawTriangle(p1, p3, p2, DARKGRAY);

    // 4. VẼ BẢNG DANH SÁCH XỔ XUỐNG (Chỉ vẽ khi isBoPhanOpen == true)
    // LƯU Ý: Đoạn này phải đặt ở cuối phần vẽ UI để nó nổi lên trên cùng
    // ==========================================
    if (isSortTTOpen)
    {
        for (int i = 0; i <= sizeSortTT; i++)
        {
            Rectangle itemRect = {comboSortTT.x, comboSortTT.y + 31 + (i * 30), comboSortTT.width, 30};
            bool isHover = CheckCollisionPointRec(mousePos, itemRect);

            // Đổi màu nền khi rê chuột vào (Hover)
            DrawRectangleRec(itemRect, isHover ? LIGHTGRAY : RAYWHITE);
            DrawRectangleLinesEx(itemRect, 1, GRAY); // Vẽ viền mỏng

            // In tên mục
            if (i == 0)
            {
                DrawText("...", itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
            else
            {
                DrawText(listSortTT[i - 1].c_str(), itemRect.x + 10, itemRect.y + 6, 16, BLACK);
            }
        }
    }
    if (selectedSortTTIndex == 0)
    {
        DrawText("...", comboSortTT.x + 5, comboSortTT.y + 5, 18, DARKGRAY);
    }
    else
    {
        DrawText(listSortTT[selectedSortTTIndex - 1].c_str(), comboSortTT.x + 5, comboSortTT.y + 5, 18, DARKGRAY);
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
    }

    // 3. Cụm "Họ Tên"
    DrawText("Ho Ten", 770, 95, 20, BLACK);
    // Vẽ khung trắng bo góc cho ô nhập liệu, nếu đang active thì viền xám nhẹ
    DrawRectangleRounded(textHoTen, 0.5f, 10, isHoTenActive ? LIGHTGRAY : WHITE);
    // In chữ người dùng đã gõ ra màn hình
    DrawText(hoTenInput, textHoTen.x + 10, textHoTen.y + 8, 20, BLACK);

    // Nhấp nháy con trỏ chuột mô phỏng (dấu gạch dưới)
    if (isHoTenActive && ((int)(GetTime() * 2) % 2 == 0))
    {
        int textWidth = MeasureText(hoTenInput, 20);
        DrawText("_", textHoTen.x + 12 + textWidth, textHoTen.y + 8, 20, MAROON);
    }

    // 4. Nút bấm Lọc (Nút gradient/màu nổi)
    // Do Raylib không hỗ trợ sẵn gradient cho hình bo góc, ta dùng màu tím đậm tạm thời
    DrawRectangleRounded(btnLoc, 0.4f, 10, PURPLE);
    // Vẽ icon phễu hoặc chữ (ở đây vẽ chữ tạm)
    DrawText("L", btnLoc.x + 15, btnLoc.y + 8, 20, WHITE);

    // In Nhân Viên

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
    int startX = 250;   // Lề trái
    int startY = 200;   // Lề trên
    int rowHeight = 40; // Chiều cao mỗi dòng
    int fontSize = 20;  // Kích thước chữ

    // Tọa độ X của từng cột (Bạn cần tinh chỉnh các số này cho vừa với màn hình)
    int colX[] = {250, 370, 600, 750, 860, 1000, 1200, 1460};

    // 1. VẼ TIÊU ĐỀ CỘT (Header)
    int lech = isOpenDelete ? 30 : 0;
    DrawText("Ma NV", colX[0] + lech, startY, fontSize, DARKGRAY);
    DrawText("Ho ten", colX[1] + lech, startY, fontSize, DARKGRAY);
    DrawText("Ngay sinh", colX[2] + lech, startY, fontSize, DARKGRAY);
    DrawText("Gioi tinh", colX[3] + lech, startY, fontSize, DARKGRAY);
    DrawText("Ma phong", colX[4] + lech, startY, fontSize, DARKGRAY);
    DrawText("Chuc vu", colX[5] + lech, startY, fontSize, DARKGRAY);
    DrawText("Vi tri", colX[6] + lech, startY, fontSize, DARKGRAY);
    DrawText("Bac", colX[7] + lech, startY, fontSize, DARKGRAY);
    // ... (Thêm các tiêu đề khác tương tự)

    // Kẻ đường gạch ngang dưới tiêu đề
    DrawLine(startX, startY + 30, 1900, startY + 30, LIGHTGRAY);

    // 4. KHU VỰC VẼ DANH SÁCH BỊ CẮT (Scissor Mode)
    int listStartY = startY + 35;                          // Điểm bắt đầu vẽ dữ liệu (ngay dưới đường kẻ)
    int listHeight = GetScreenHeight() - listStartY - 200; // Chiều cao khu vực hiển thị dữ liệu

    // Bật chế độ Scissor: Bất cứ thứ gì vẽ bên ngoài hình chữ nhật này sẽ bị ẩn đi
    BeginScissorMode(startX, listStartY, GetScreenWidth() - startX, listHeight);
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
    while (current != nullptr)
    {
        if (CheckEmployee(current->Data, tieuchi1, tieuchi2, hoTenInput))
        {
            string mnv = current->Data->get_mnv();
            string name = current->Data->get_name();
            string Ngaysinh = current->Data->get_NgaySinh().ToString();
            string mpb = current->Data->get_MaPhongBan();
            string sex = current->Data->get_sex();
            string chucvu = current->Data->get_ChucVu()->get_MaChucVu();
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
            DrawText(mnv.c_str(), colX[0] + lech + lech, currentY, fontSize, BLACK);
            DrawText(name.c_str(), colX[1] + lech, currentY, fontSize, BLACK);
            DrawText(Ngaysinh.c_str(), colX[2] + lech, currentY, fontSize, BLACK);
            DrawText(sex.c_str(), colX[3] + lech, currentY, fontSize, BLACK);
            DrawText(mpb.c_str(), colX[4] + lech, currentY, fontSize, BLACK);
            DrawText(chucvu.c_str(), colX[5] + lech, currentY, fontSize, BLACK);
            DrawText(vitri.c_str(), colX[6] + lech, currentY, fontSize, BLACK);
            DrawText(bac.c_str(), colX[7] + lech, currentY, fontSize, BLACK);
            DrawLine(startX, currentY + 30, GetScreenWidth() - 20, currentY + 30, Fade(LIGHTGRAY, 0.5f));
            currentY += rowHeight;
            totalRows++;
        }
        current = current->Next;
        idx++;
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
    if (!isOpenDelete)
    {
        Rectangle btnThemNhanSu = {350, 900, 200, 40};
        DrawRectangleRounded(btnThemNhanSu, 0.2f, 10, BLUE);
        DrawText("Them Nhan Su", btnThemNhanSu.x + 32, btnThemNhanSu.y + 10, 20, BLACK);

        Rectangle btnXoaNhanSu = {750, 900, 200, 40};
        DrawRectangleRounded(btnXoaNhanSu, 0.2f, 10, BLUE);
        DrawText("Xoa Nhan Su", btnXoaNhanSu.x + 32, btnXoaNhanSu.y + 10, 20, BLACK);

        Rectangle btnSuaNhanSu = {1150, 900, 200, 40};
        DrawRectangleRounded(btnSuaNhanSu, 0.2f, 10, BLUE);
        DrawText("Sua Nhan Su", btnSuaNhanSu.x + 32, btnSuaNhanSu.y + 10, 20, BLACK);
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
        Rectangle Table = {500.0f, 150.0f, 800.0f, 800.0f};
        DrawRectangleRounded(Table, 0.2f, 10, {223, 228, 232, 255});
        DrawText("Thong tin nhan vien", Table.x + 270, Table.y + 20, 25, BLACK);

        Rectangle btnMaNhanVien = {Table.x + 150, Table.y + 65, 220, 30};
        static bool isMaNhanVienActive = false; // Trạng thái: ô có đang được click vào không
        static char MaNhanVienInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
        static int letterCountMaNhanVien = 0;
        DrawRectangleRounded(btnMaNhanVien, 0.2f, 10, WHITE);
        DrawText("Ma nhan vien", btnMaNhanVien.x - 140, btnMaNhanVien.y + 5, 20, BLACK);
        if (clickSua)
        {
            isMaNhanVienActive = false;
            DrawText(MaNhanVienInput, btnMaNhanVien.x + 10, btnMaNhanVien.y + 5, 20, DARKGRAY);
        }
        else
        {
            Typing(isMaNhanVienActive, MaNhanVienInput, letterCountMaNhanVien, mousePos, btnMaNhanVien);
            HieuUngGo(isMaNhanVienActive, btnMaNhanVien, MaNhanVienInput);
        }
        Rectangle btnChucVu2 = {btnMaNhanVien.x + 380, btnMaNhanVien.y, 220, 30};
        static bool openCV = false;
        int nCV = QL->get_DB()->get_ListCV()->get_size();
        string dsCV[50];
        static int selCV = -1;
        for (int i = 0; i < nCV && i < 50; i++)
        {
            dsCV[i] = (*QL->get_DB()->get_ListCV())[i]->Data->get_MaChucVu();
        }

        Rectangle btnPhong = {btnMaNhanVien.x, btnMaNhanVien.y + 60, 220, 30};
        static bool openPB = false;
        int nPB = QL->get_DB()->get_ListPB()->get_size();
        string dsPB[50];
        static int selPB = -1;
        for (int i = 0; i < nPB && i < 50; i++)
        {
            dsPB[i] = (*QL->get_DB()->get_ListPB())[i]->Data->get_MaPhongBan();
        }

        Rectangle btnLoaiNV = {btnPhong.x + 380, btnPhong.y, 220, 30};
        static bool openLoai = false;
        string dsLoai[2] = {"FullTime", "PartTime"};
        static int selLoai = -1;

        Rectangle btnHovaTen = {btnPhong.x, btnPhong.y + 50, 220, 30};
        DrawRectangleRounded(btnHovaTen, 0.2f, 10, WHITE);
        static bool isHovaTenActive = false; // Trạng thái: ô có đang được click vào không
        static char HovaTenInput[50] = "\0"; // Chuỗi lưu ký tự người dùng gõ
        static int letterCountHovaTen = 0;
        DrawText("Ho va ten", btnHovaTen.x - 100, btnHovaTen.y + 3, 20, BLACK);
        Typing(isHovaTenActive, HovaTenInput, letterCountHovaTen, mousePos, btnHovaTen);
        HieuUngGo(isHovaTenActive, btnHovaTen, HovaTenInput);

        Rectangle btnGioiTinh = {btnHovaTen.x + 380, btnHovaTen.y, 100, 30};
        static bool openGT = false;
        string dsGT[2] = {"Nam", "Nu"};
        static int selGT = -1;

        Rectangle btnNgaySinh = {btnHovaTen.x, btnHovaTen.y + 50, 180, 30};
        DrawText("Ngay sinh", btnNgaySinh.x - 100, btnNgaySinh.y + 5, 20, BLACK);
        static bool isOpenCalendarNS = false;
        static bool changeNS = false;
        static int dayNS = day;
        static int monthNS = month;
        static int yearNS = year;
        string textNgaySinh = "";
        if (changeNS)
        {
            string strDay = (dayNS < 10 ? "0" : "") + to_string(dayNS);
            string strMonth = (monthNS + 1 < 10 ? "0" : "") + to_string(monthNS + 1);
            string strYear = to_string(yearNS);

            textNgaySinh = strDay + "/" + strMonth + "/" + strYear;
        }
        DrawRectangleRounded(btnNgaySinh, 0.5f, 10, isOpenCalendarNS ? LIGHTGRAY : WHITE);
        DrawText(textNgaySinh.c_str(), btnNgaySinh.x + 5, btnNgaySinh.y + 10, 25, BLACK);

        Rectangle btnNgayVaoLam = {btnNgaySinh.x + 375, btnNgaySinh.y, 180, 30};
        DrawText("Ngay vao lam", btnNgayVaoLam.x - 100, btnNgayVaoLam.y + 5, 20, BLACK);
        static bool isOpenCalendarNVL = false;
        static int dayNVL = day;
        static int monthNVL = month;
        static int yearNVL = year;
        string textNgayVaoLam = "";

        string strDay = (dayNVL < 10 ? "0" : "") + to_string(dayNVL);
        string strMonth = (monthNVL + 1 < 10 ? "0" : "") + to_string(monthNVL + 1);
        string strYear = to_string(yearNVL);

        textNgayVaoLam = strDay + "/" + strMonth + "/" + strYear;

        DrawRectangleRounded(btnNgayVaoLam, 0.5f, 10, isOpenCalendarNVL ? LIGHTGRAY : WHITE);
        DrawText(textNgayVaoLam.c_str(), btnNgayVaoLam.x + 5, btnNgayVaoLam.y + 10, 25, BLACK);

        Rectangle btnViTriViecLam = {btnNgaySinh.x, btnNgaySinh.y + 50, 200, 30};
        static bool openVT = false;
        string dsVT[50];
        int nVT = QL->get_DB()->get_ListVTVL()->get_size();
        static int selVT = -1;
        for (int i = 0; i < nVT && i < 50; i++)
        {
            dsVT[i] = (*QL->get_DB()->get_ListVTVL())[i]->Data->get_TenVTVL();
        }

        Rectangle btnBacLuong = {btnViTriViecLam.x + 370, btnViTriViecLam.y, 200, 30};
        static bool openBac = false;
        int nBac = 5;
        string dsBac[10];
        static int selBac = -1;
        for (int i = 0; i < nBac; i++)
        {
            dsBac[i] = to_string(i + 1);
        }

        // ----- ĐIỀN SẴN THÔNG TIN KHI BẤM SỬA -----
        if (needFill)
        {
            needFill = false;
            Employee *emp = selEmp;
            if (emp != nullptr)
            {
                // Ô nhập chữ
                strncpy(MaNhanVienInput, emp->get_mnv().c_str(), 49);
                MaNhanVienInput[49] = '\0';
                letterCountMaNhanVien = strlen(MaNhanVienInput);

                strncpy(HovaTenInput, emp->get_name().c_str(), 49);
                HovaTenInput[49] = '\0';
                letterCountHovaTen = strlen(HovaTenInput);

                // Các combo
                selCV = TimIndex(dsCV, nCV, emp->get_ChucVu()->get_MaChucVu());
                selPB = TimIndex(dsPB, nPB, emp->get_MaPhongBan());
                selGT = TimIndex(dsGT, 2, emp->get_sex());
                selVT = TimIndex(dsVT, nVT, emp->get_ViTri()->get_TenVTVL());
                selBac = emp->get_BacViTri() - 1;
                selLoai = (dynamic_cast<FullTime *>(emp) != nullptr) ? 0 : 1;

                // Ngày: tách từ chuỗi dd/mm/yyyy (tháng của calendar đếm từ 0 nên trừ 1)
                int d, m, y;
                if (sscanf(emp->get_NgaySinh().ToString().c_str(), "%d/%d/%d", &d, &m, &y) == 3)
                {
                    dayNS = d;
                    monthNS = m - 1;
                    yearNS = y;
                    changeNS = true; // để ô Ngày sinh hiện chữ
                }
                if (sscanf(emp->get_NgayLamViec().ToString().c_str(), "%d/%d/%d", &d, &m, &y) == 3)
                {
                    dayNVL = d;
                    monthNVL = m - 1;
                    yearNVL = y;
                }
            }
        }

        // vẽ
        DrawText("Chuc vu", btnChucVu2.x - 100, btnChucVu2.y + 5, 20, BLACK);
        VeCombo(btnChucVu2, selCV, dsCV, nCV);

        DrawText("Phong ban", btnPhong.x - 120, btnPhong.y + 5, 20, BLACK);
        VeCombo(btnPhong, selPB, dsPB, nPB);

        DrawText("Loai nhan vien", btnLoaiNV.x - 150, btnLoaiNV.y + 5, 20, BLACK);
        VeCombo(btnLoaiNV, selLoai, dsLoai, 2);

        DrawText("Gioi Tinh", btnGioiTinh.x - 100, btnGioiTinh.y + 5, 20, BLACK);
        VeCombo(btnGioiTinh, selGT, dsGT, 2);

        DrawText("Vi tri viec lam", btnViTriViecLam.x - 150, btnViTriViecLam.y + 5, 20, BLACK);
        VeCombo(btnViTriViecLam, selVT, dsVT, nVT);

        DrawText("Bac luong", btnBacLuong.x - 110, btnBacLuong.y + 5, 20, BLACK);
        VeCombo(btnBacLuong, selBac, dsBac, nBac);
        // Chỉ cho 1 bảng xổ xuống
        bool anyOpen = openCV || openPB || openLoai || openGT || openVT || openBac;
        bool choPhepMo = !anyOpen;
        bool comboClicked = false;

        comboClicked |= XuLyCombo(btnChucVu2, openCV, selCV, nCV, mousePos, Clicked, choPhepMo);
        comboClicked |= XuLyCombo(btnPhong, openPB, selPB, nPB, mousePos, Clicked, choPhepMo);
        comboClicked |= XuLyCombo(btnLoaiNV, openLoai, selLoai, 2, mousePos, Clicked, choPhepMo);
        comboClicked |= XuLyCombo(btnGioiTinh, openGT, selGT, 2, mousePos, Clicked, choPhepMo);
        comboClicked |= XuLyCombo(btnViTriViecLam, openVT, selVT, nVT, mousePos, Clicked, choPhepMo);
        comboClicked |= XuLyCombo(btnBacLuong, openBac, selBac, nBac, mousePos, Clicked, choPhepMo);

        Rectangle btnHuy = {Table.x + 200, Table.y + 600, 100, 50};
        DrawRectangleRounded(btnHuy, 0.2f, 10, RED);
        DrawText("Huy", btnHuy.x + 30, btnHuy.y + 15, 20, BLACK);
        if (CheckCollisionPointRec(mousePos, btnHuy) && Clicked && !comboClicked)
        {
            isOpenTable = false;
            clickSua = false;
            clickThem = false;
            selCV = selPB = selLoai = selGT = selVT = selBac = -1;
            openCV = openPB = openLoai = openGT = openVT = openBac = false;
        }

        Rectangle btnXacNhan = {btnHuy.x + 300, btnHuy.y, 100, 50};
        DrawRectangleRounded(btnXacNhan, 0.2f, 10, BLUE);
        if (clickSua)
        {
            DrawText("Sua", btnXacNhan.x + 25, btnXacNhan.y + 15, 20, BLACK);
        }
        else
        {
            DrawText("Them", btnXacNhan.x + 25, btnXacNhan.y + 15, 20, BLACK);
        }

        if (CheckCollisionPointRec(mousePos, btnXacNhan) && Clicked && !comboClicked)
        { // check xem điền đủ thông tin chưa
            bool hopLe = strlen(MaNhanVienInput) > 0 && strlen(HovaTenInput) > 0 && changeNS &&
                         selCV != -1 && selPB != -1 && selLoai != -1 &&
                         selGT != -1 && selVT != -1 && selBac != -1;

            if (hopLe && clickThem)
            {
                string mnv = MaNhanVienInput;
                string ten = HovaTenInput;
                string gioiTinh = dsGT[selGT];

                ChucVu *cv = (*QL->get_DB()->get_ListCV())[selCV]->Data;
                PhongBan *pb = (*QL->get_DB()->get_ListPB())[selPB]->Data;
                ViTriViecLam *vt = (*QL->get_DB()->get_ListVTVL())[selVT]->Data;

                Date ngaySinh(dayNS, monthNS + 1, yearNS);
                Date ngayVaoLam(dayNVL, monthNVL + 1, yearNVL);
                int bac = selBac + 1;

                Employee *nv = nullptr;
                if (selLoai == 0)
                {
                    nv = new FullTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac);
                }
                else
                {
                    nv = new PartTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac);
                }
                QL->ThemEmployee(nv);

                // reset form
                isOpenTable = false;
                clickThem = false;
                selCV = selPB = selLoai = selGT = selVT = selBac = -1;
                MaNhanVienInput[0] = '\0';
                letterCountMaNhanVien = 0;
                HovaTenInput[0] = '\0';
                letterCountHovaTen = 0;
                changeNS = false;
                isOpenCalendarNS = false;
                isOpenCalendarNVL = false;
            }
            if (hopLe && clickSua)
            {
                string mnv = MaNhanVienInput;
                string ten = HovaTenInput;
                string gioiTinh = dsGT[selGT];

                ChucVu *cv = (*QL->get_DB()->get_ListCV())[selCV]->Data;
                PhongBan *pb = (*QL->get_DB()->get_ListPB())[selPB]->Data;
                ViTriViecLam *vt = (*QL->get_DB()->get_ListVTVL())[selVT]->Data;

                Date ngaySinh(dayNS, monthNS + 1, yearNS);
                Date ngayVaoLam(dayNVL, monthNVL + 1, yearNVL);
                int bac = selBac + 1;

                Employee *nv = nullptr;
                if (selLoai == 0)
                {
                    nv = new FullTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac);
                }
                else
                {
                    nv = new PartTime(mnv, ten, ngaySinh, gioiTinh, cv, vt, pb, ngayVaoLam, bac);
                }
                QL->SuaEmployee(nv);
                isOpenTable = false;
                clickSua = false;
                selCV = selPB = selLoai = selGT = selVT = selBac = -1;
                MaNhanVienInput[0] = '\0';
                letterCountMaNhanVien = 0;
                HovaTenInput[0] = '\0';
                letterCountHovaTen = 0;
                changeNS = false;
                isOpenCalendarNS = false;
                isOpenCalendarNVL = false;
            }
        }
        VeDanhSachCombo(btnChucVu2, openCV, dsCV, nCV, mousePos);
        VeDanhSachCombo(btnPhong, openPB, dsPB, nPB, mousePos);
        VeDanhSachCombo(btnLoaiNV, openLoai, dsLoai, 2, mousePos);
        VeDanhSachCombo(btnGioiTinh, openGT, dsGT, 2, mousePos);
        if (Clicked && !comboClicked && CheckCollisionPointRec(mousePos, btnNgaySinh))
        {
            isOpenCalendarNS = true;
        }
        if (isOpenCalendarNS)
        {
            changeNS = DrawCalendar(mousePos, dayNS, monthNS, yearNS, btnNgaySinh, isOpenCalendarNS);
        }

        if (Clicked && !comboClicked && CheckCollisionPointRec(mousePos, btnNgayVaoLam))
        {
            isOpenCalendarNVL = true;
        }
        if (isOpenCalendarNVL)
        {
            DrawCalendar(mousePos, dayNVL, monthNVL, yearNVL, btnNgayVaoLam, isOpenCalendarNVL);
        }
        VeDanhSachCombo(btnViTriViecLam, openVT, dsVT, nVT, mousePos);
        VeDanhSachCombo(btnBacLuong, openBac, dsBac, nBac, mousePos);
    }
    if (isOpenDelete)
    {
        Rectangle btnHuyXoa = {350, 900, 200, 40};
        DrawRectangleRounded(btnHuyXoa, 0.2f, 10, BLUE);
        DrawText("Huy xoa", btnHuyXoa.x + 32, btnHuyXoa.y + 10, 20, BLACK);

        Rectangle btnXacNhanXoa = {600, 900, 200, 40};
        DrawRectangleRounded(btnXacNhanXoa, 0.2f, 10, BLUE);
        DrawText("Xac nhan xoa", btnXacNhanXoa.x + 32, btnXacNhanXoa.y + 10, 20, BLACK);

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