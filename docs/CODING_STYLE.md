# ✨ Coding Style Guide - University Management Project (C++)


## 1. 📦 Tên Class
- **PascalCase**
- Dùng danh từ, mô tả đối tượng

```cpp
class Student;
class CourseManager;
class AuthService;
```

---

## 2. 🔡 Tên Biến
- **snake_case**
- Ngắn gọn, rõ ràng

```cpp
std::string student_name;
int course_credit;
float avg_score;
```

---

## 3. 🚀 Tên Hàm
- **camelCase**
- Tên là động từ hoặc cụm động từ

```cpp
void calculateGPA();
bool isValidEmail();
void loadFromFile();
```

---

## 4. 🧱 Hằng Số
- **UPPER_CASE** (có thể dùng `constexpr` hoặc `#define`)

```cpp
constexpr int MAX_COURSES = 8;
#define DB_FILE_PATH "data/students.csv"
```

---

## 5. 📄 Tên File
- Theo tên class, dạng PascalCase, phân chia `.hpp` và `.cpp`

```text
Student.hpp
Student.cpp
ResultService.hpp
```

---

## 6. 🔧 Quy tắc dấu ngoặc `{}`
- **Allman Style**: Đặt `{` ở dòng mới

```cpp
void showInfo()
{
    std::cout << "Student info";
}
```

---

## 7. ✨ Indent & Spacing
- Indent: **4 spaces** (không dùng tab)
- Có khoảng trắng quanh toán tử

```cpp
if (gpa >= 3.5)
{
    total_score += score;
}
```

---

## 8. 💬 Comment
- Dùng `//` cho dòng ngắn, `/* ... */` cho block
- Comment giải thích logic, KHÔNG ghi điều hiển nhiên

```cpp
// Tính điểm trung bình GPA từ danh sách điểm
float calculateGPA(const std::vector<int>& scores);
```

---

## 9. 🧩 Namespace (tùy chọn)
- Dùng khi cần chia nhóm module lớn

```cpp
namespace uniman {
    class Student { ... };
}
```

---

## 10. 📌 Quy tắc Khác
| Nội dung | Quy tắc |
|----------|---------|
| Include guard | Dùng `#pragma once` |
| File `.h` chỉ chứa khai báo | `.cpp` chứa logic |
| Không viết logic trong `main.cpp` | Dùng để gọi UI/Service |
