# **Proposal Đồ Án: Tái Cấu Trúc Hệ Thống Quản Lý Đại Học Theo Hướng Đối Tượng**

## **1. Giới thiệu**

Đồ án này tập trung vào việc tái cấu trúc mã nguồn **University Management System** từ [CodeWithC](https://www.codewithc.com/university-management-system-project-c/) theo hướng đối tượng (OOP), áp dụng các nguyên tắc SOLID, Design Pattern, Dependency Injection, và kiến trúc phần mềm (Layered Architecture).

### **Mục tiêu:**

- **Viết lại mã nguồn theo OOP** (kế thừa, đóng gói, đa hình, trừu tượng).
- **Áp dụng SOLID, Design Pattern** để tăng tính mở rộng và bảo trì.
- **Triển khai Dependency Injection** để giảm coupling.
- **Xây dựng tài liệu kiến trúc, class diagram, coding convention**.
- **Quản lý mã nguồn bằng Git (Feature Branch Workflow)**.
- **Viết Unit Test (Google Test/Catch2) và tạo test report**.

---

## **2. Lộ Trình & Kế Hoạch (8 Tuần)**

### **Tuần 1: Phân tích & Thiết kế**

- **Phân tích mã nguồn hiện có** (chức năng, cấu trúc, hạn chế).
- **Thiết kế Class Diagram** (sử dụng UML) và kiến trúc phần mềm (Layered Architecture).
- **Lập kế hoạch Git (Feature Branch Workflow)**:
    - **`main`** → nhánh chính ổn định.
    - **`feature/*`** → phát triển từng tính năng.
    - **`refactor/*`** → tái cấu trúc.
- **Tài liệu hóa Coding Convention** (Google C++ Style Guide cải tiến).

### **Tuần 2-3: Tái Cấu Trúc OOP**

- **Chia lớp theo nguyên tắc SRP (Single Responsibility)**:
    - **`Student`**, **`Course`**, **`Professor`**, **`Department`** → các lớp độc lập.
    - **`UniversityDB`** → quản lý dữ liệu (Repository Pattern).
- **Áp dụng Inheritance & Polymorphism**:
    - **`User`** (base class) → **`Student`**, **`Professor`** (kế thừa).
- **Sử dụng Composition thay vì Inheritance** khi cần (Ví dụ: **`Course`** chứa **`Professor`**).

### **Tuần 4: Áp Dụng SOLID & Design Pattern**

- **SOLID**:
    - **Dependency Inversion**: Sử dụng Interface (**`IStorage`**) để **`UniversityDB`** phụ thuộc vào abstraction.
    - **Open/Closed**: Mở rộng bằng cách thêm lớp mới (không sửa lớp cũ).
- **Design Patterns**:
    - **Factory Method**: Tạo đối tượng **`User`** (Student/Professor).
    - **Observer**: Thông báo khi có thay đổi dữ liệu (ví dụ: điểm mới).
    - **Singleton**: Đảm bảo **`UniversityDB`** chỉ có 1 instance.

### **Tuần 5: Dependency Injection & Kiến Trúc**

- **Dependency Injection (DI)**:
    - Inject **`IStorage`** vào **`UniversityDB`** (dùng thư viện [Boost.DI](https://boost-ext.github.io/di/) hoặc tự viết).
- **Kiến trúc 3 lớp**:
    - **Presentation Layer**: Giao diện console/GUI.
    - **Business Logic Layer**: Xử lý nghiệp vụ (quản lý sinh viên, khóa học).
    - **Data Access Layer**: Lưu trữ (file/CSV, có thể mở rộng sang SQL).

### **Tuần 6: Unit Test & Chất Lượng Mã**

- **Viết Unit Test (Google Test)**:
    - Test các nghiệp vụ chính (thêm sinh viên, tính điểm).
    - Đạt coverage > 70%.
- **Test Plan & Report**:
    - Liệt kê test cases (input, expected output).
    - Báo cáo coverage và bug (nếu có).

### **Tuần 7: Hoàn Thiện Tài Liệu**

- **Class Diagram** (PlantUML hoặc Lucidchart).
- **Tài liệu mô tả lớp/hàm** (Doxygen).
- **Hướng dẫn cài đặt & chạy** (README.md).

### **Tuần 8: Demo & Báo Cáo Cuối Kỳ**

- **Demo chức năng**:
    - Thêm/xóa sinh viên, đăng ký khóa học.
    - Tính điểm trung bình.
- **Báo cáo đánh giá**:
    - So sánh trước/sau khi refactor.
    - Giải thích các Design Pattern đã dùng

---


## **3. Phân Công Công Việc (Nhóm 2 Người)**

| **Thành Viên** | **Công Việc Chính** |
| --- | --- |
| **Nhóm Trưởng** | Thiết kế kiến trúc, SOLID, DI, Git workflow, Design Pattern, |
| **Thành Viên 2** | Tái cấu trúc OOP, Unit Test, Tài liệu hóa |
---

## **4. Công Cụ & Công Nghệ**

- **Ngôn ngữ**: C++17 (modern OOP features).
- **Build System**: CMake.
- **Testing**: Google Test.
- **Version Control**: Git (GitHub/GitLab).
- **Documentation**: Doxygen, Markdown.
- **UML**: PlantUML hoặc Lucidchart.
---

## **5. Đánh Giá Rủi Ro & Giải Pháp**

- **Rủi ro**: Khó debug khi áp dụng DI.
**→ Giải pháp**: Sử dụng `logging (spdlog)` để theo dõi luồng dữ liệu.
- **Rủi ro**: Unit Test không cover hết.
**→ Giải pháp**: Ưu tiên `test nghiệp vụ chính` trước.
---
## **6. Một Số Quy Chuẩn**

### **1. Cấu Trúc Thư Mục (Modern C++ Project Layout)**

```makefile
university-management/
├── CMakeLists.txt                  # File cấu hình build
├── README.md                       # Tài liệu dự án
│
├── src/                            # Thư mục mã nguồn
│   ├── main.cpp                    # Điểm vào chính của ứng dụng
│   │
│   ├── common/                     # Các định nghĩa và tiện ích dùng chung
│   │   ├── AppConfig.h/.cpp        # Cấu trúc lưu cấu hình ứng dụng
│   │   ├── ErrorType.h             # Định nghĩa cấu trúc lỗi
│   │   ├── UserRole.h              # Enum các vai trò người dùng
│   │   └── OperationResult.h       # Wrapper kết quả các thao tác
│   │
│   ├── core/                       # Logic nghiệp vụ cốt lõi
│   │   ├── entities/               # Các đối tượng nghiệp vụ
│   │   │   ├── IEntity.h           # Interface cơ sở cho các entity
│   │   │   ├── Birthday.h/.cpp     # Lớp quản lý ngày sinh
│   │   │   ├── User.h/.cpp         # Lớp cơ sở trừu tượng cho người dùng
│   │   │   ├── Student.h/.cpp      # Lớp sinh viên (kế thừa User)
│   │   │   ├── Teacher.h/.cpp      # Lớp giảng viên (kế thừa User)
│   │   │   ├── Faculty.h/.cpp      # Lớp khoa/bộ môn
│   │   │   ├── Course.h/.cpp       # Lớp môn học
│   │   │   ├── CourseResult.h/.cpp # Lớp kết quả môn học của sinh viên
│   │   │   ├── FeeRecord.h/.cpp    # Lớp quản lý học phí sinh viên
│   │   │   └── SalaryRecord.h/.cpp # Lớp quản lý lương giảng viên
│   │   │
│   │   ├── data_access/            # Tầng truy cập dữ liệu
│   │   │   ├── interface/          # Các interface DAO
│   │   │   │   ├── IDao.h          # Interface chung cho DAO
│   │   │   │   ├── IStudentDao.h   # Interface DAO sinh viên
│   │   │   │   ├── ITeacherDao.h   # Interface DAO giảng viên
│   │   │   │   ├── IFacultyDao.h   # Interface DAO khoa/bộ môn
│   │   │   │   ├── ICourseDao.h    # Interface DAO môn học
│   │   │   │   ├── IEnrollmentDao.h # Interface DAO đăng ký học
│   │   │   │   ├── ICourseResultDao.h # Interface DAO kết quả môn học
│   │   │   │   ├── IFeeRecordDao.h # Interface DAO học phí
│   │   │   │   ├── ISalaryRecordDao.h # Interface DAO lương
│   │   │   │   └── ILoginDao.h     # Interface DAO đăng nhập
│   │   │   │
│   │   │   ├── impl_csv/           # Triển khai DAO với CSV
│   │   │   │   ├── CsvStudentDao.h/.cpp
│   │   │   │   ├── CsvTeacherDao.h/.cpp
│   │   │   │   ├── CsvFacultyDao.h/.cpp
│   │   │   │   ├── CsvCourseDao.h/.cpp
│   │   │   │   ├── CsvEnrollmentDao.h/.cpp
│   │   │   │   ├── CsvCourseResultDao.h/.cpp
│   │   │   │   ├── CsvFeeRecordDao.h/.cpp
│   │   │   │   ├── CsvSalaryRecordDao.h/.cpp
│   │   │   │   └── CsvLoginDao.h/.cpp
│   │   │   │
│   │   │   ├── impl_sql/           # Triển khai DAO với SQL (phát triển sau)
│   │   │   │   └── ...
│   │   │   │
│   │   │   ├── mock/               # Mock DAO để phục vụ kiểm thử
│   │   │   │   ├── MockStudentDao.h/.cpp
│   │   │   │   ├── MockTeacherDao.h/.cpp
│   │   │   │   └── ...
│   │   │   │
│   │   │   ├── DaoFactory.h/.cpp   # Factory tạo các DAO
│   │   │
│   │   ├── services/               # Các dịch vụ nghiệp vụ
│   │   │   ├── interface/          # Interface dịch vụ
│   │   │   │   ├── IAuthService.h  # Interface xác thực người dùng
│   │   │   │   ├── IStudentService.h # Interface quản lý sinh viên
│   │   │   │   ├── ITeacherService.h # Interface quản lý giảng viên
│   │   │   │   ├── IFacultyService.h # Interface quản lý khoa
│   │   │   │   ├── ICourseService.h # Interface quản lý môn học
│   │   │   │   ├── IEnrollmentService.h # Interface đăng ký học
│   │   │   │   ├── IResultService.h # Interface quản lý kết quả học tập
│   │   │   │   ├── IFinanceService.h # Interface quản lý tài chính
│   │   │   │   └── IAdminService.h # Interface quản trị hệ thống
│   │   │   │
│   │   │   └── impl/              # Triển khai dịch vụ
│   │   │       ├── AuthService.h/.cpp # Dịch vụ xác thực
│   │   │       ├── StudentService.h/.cpp # Dịch vụ sinh viên
│   │   │       ├── TeacherService.h/.cpp # Dịch vụ giảng viên
│   │   │       ├── FacultyService.h/.cpp # Dịch vụ khoa
│   │   │       ├── CourseService.h/.cpp # Dịch vụ môn học
│   │   │       ├── EnrollmentService.h/.cpp # Dịch vụ đăng ký học
│   │   │       ├── ResultService.h/.cpp # Dịch vụ kết quả học tập
│   │   │       ├── FinanceService.h/.cpp # Dịch vụ tài chính
│   │   │       └── AdminService.h/.cpp # Dịch vụ quản trị
│   │   │
│   │   ├── validators/             # Kiểm tra dữ liệu đầu vào
│   │   │   ├── interface/
│   │   │   │   └── IValidator.h    # Interface validator
│   │   │   │
│   │   │   └── impl/
│   │   │       ├── StudentValidator.h/.cpp # Validator sinh viên
│   │   │       ├── TeacherValidator.h/.cpp # Validator giảng viên
│   │   │       ├── CourseValidator.h/.cpp  # Validator môn học
│   │   │       └── GeneralInputValidator.h/.cpp # Validator chung
│   │   │
│   │   └── database_adapter/       # Kết nối cơ sở dữ liệu (cho SQL)
│   │       ├── interface/
│   │       │   └── IDatabaseAdapter.h # Interface adapter CSDL
│   │       │
│   │       └── impl_mysql/
│   │           ├── MySqlAdapter.h/.cpp # Adapter MySQL
│   │           └── MySqlConnectionManager.h/.cpp # Quản lý kết nối
│   │
│   ├── ui/                         # Giao diện người dùng
│   │   ├── ConsoleUI.h/.cpp        # Giao diện console
│   │   ├── MenuOption.h            # Enum các lựa chọn menu
│   │   ├── InputPrompter.h/.cpp    # Lớp hỗ trợ nhập liệu
│   │   └── TableDisplayer.h/.cpp   # Lớp hiển thị dữ liệu dạng bảng
│   │
│   ├── utils/                      # Các tiện ích
│   │   ├── Logger.h/.cpp           # Ghi log
│   │   ├── FileHandler.h/.cpp      # Xử lý file
│   │   ├── PasswordUtils.h/.cpp    # Tiện ích mật khẩu
│   │   ├── StringUtils.h/.cpp      # Xử lý chuỗi
│   │   └── ConfigLoader.h/.cpp     # Đọc cấu hình
│   │
│   └── config/                     # File cấu hình
│       └── app_config.ini
│
├── data/                           # Dữ liệu lưu trữ
│   ├── students.csv                # Dữ liệu sinh viên
│   ├── teachers.csv                # Dữ liệu giảng viên
│   ├── faculties.csv               # Dữ liệu khoa
│   ├── courses.csv                 # Dữ liệu môn học
│   ├── enrollments.csv             # Dữ liệu đăng ký học
│   ├── results.csv                 # Dữ liệu kết quả học tập
│   ├── fees.csv                    # Dữ liệu học phí
│   ├── salaries.csv                # Dữ liệu lương
│   └── logins.csv                  # Dữ liệu đăng nhập
│
├── tests/                          # Kiểm thử đơn vị
│   ├── services/                   # Test các dịch vụ
│   │   └── StudentServiceTest.cpp  # Test dịch vụ sinh viên
│   ├── data_access/                # Test các DAO
│   │   └── CsvStudentDaoTest.cpp   # Test DAO sinh viên CSV
│   └── validators/                 # Test các validator
│       └── StudentValidatorTest.cpp # Test validator sinh viên
│
├── docs/
│   ├── architecture/
│   │   ├── layered_architecture.md
│   │   └── patterns.md
│   ├── diagrams/
│   │   ├── class_diagram.puml
│   │   └── sequence_diagram.png
│   ├── api/                          # Doxygen
│   └── CODING_STYLE.md
│
└── .github/
    └── workflows/
        ├── build.yml
        └── coverage.yml

```

---

### **2. Git Workflow (Feature Branch + CI/CD)**

#### **Quy Trình**

1. **Nhánh chính:**
    - **`main`**: Phiên bản ổn định (protected branch).
    - **`develop`**: Tích hợp tính năng (staging).

2. **Nhánh phát triển**:
    
    ```
    git checkout -b feature/enrollment-service  # Tạo nhánh mới
    git push origin feature/enrollment-service
    ```
    
    - Quy ước đặt tên:
        - **`feature/*`**: Tính năng mới (ví dụ: **`feature/grading-system`**)
        - **`refactor/*`**: Tái cấu trúc (ví dụ: **`refactor/di-container`**)
        - **`fix/*`**: Sửa lỗi.
3. **Code Review:**
    - Tạo **Pull Request** từ **`feature/*`** vào **`develop`**.
    - Yêu cầu ít nhất 1 reviewer.
    - Kích hoạt CI/CD (build + test).
4. **Merge & Release**:
    - Khi **`develop`** ổn định → merge vào **`main`** và tag version (**`v1.0.0`**).

#### **Công Cụ Hỗ Trợ**

- **Pre-commit Hooks**: Tự động format code (clang-format).
- **Github Actions**: Chạy unit test trên mỗi PR.

---

## **7. Chủ Đề Nâng Cao (Advanced Topics)**

### **a. Performance Optimization**

- **Cache Layer**: Dùng **Proxy Pattern** để cache dữ liệu sinh viên.
- **Multithreading**: Xử lý batch import dữ liệu (ví dụ: **`std::async`**).

### **b. Extensibility**

- **Plugin System**: Dùng **Abstract Factory** để hỗ trợ thêm định dạng lưu trữ (SQLite, MongoDB).
- **Event-Driven Architecture**: Sử dụng **Observer Pattern** để gửi thông báo khi có điểm mới.

### **c. Security**

- **Authentication**: Áp dụng **Strategy Pattern** cho đăng nhập (local/OAuth2).
- **Data Encryption**: Mã hóa dữ liệu nhạy cảm (ví dụ: điểm).

### **d. Internationalization (i18n)**

- **Factory Method** để tạo UI đa ngôn ngữ (tiếng Anh/Việt).
---

### *Ví Dụ Cải Tiến Cụ Thể*

#### **Bài Toán: Quản Lý Đăng Ký Môn Học**

**Code Cũ**:

```cpp
void registerCourse(int studentId, int courseId) {
    // Logic trộn lẫn UI + data access
}
```

**Code Mới (Áp dụng SOLID + DI)**:

```cpp
class EnrollmentService {
private:
    IStudentRepository& studentRepo;  // Dependency Injection
    ICourseRepository& courseRepo;
public:
    void enroll(int studentId, int courseId) {
        // Business logic thuần túy
    }
};

// Usage:
auto service = EnrollmentService(csvStudentRepo, sqlCourseRepo);
service.enroll(123, 456);
```

#### **So Sánh Hiệu Quả**

| **Tiêu Chí** | **Code Cũ** | **Code Mới** |
| --- | --- | --- |
| **Testability** | Khó test | Dễ test (mock DI) |
| **Extensibility** | Khó thêm định dạng mới | Dễ dàng (chỉ cần triển khai **`IRepository`**) |
| **Coupling** | High (phụ thuộc trực tiếp vào file CSV) | Low (phụ thuộc abstraction) |

---

## **8. Tài Liệu Tham Khảo**

- **Design Patterns**: *Head First Design Patterns* (O'Reilly).
- **SOLID**: *Clean Code* (Robert C. Martin).
- **C++ Modern**: *Effective Modern C++* (Scott Meyers).
- **Tools**:
    - Google Test: https://github.com/google/googletest
    - PlantUML: https://plantuml.com/
    - Doxygen: https://www.doxygen.nl/
