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
├── CMakeLists.txt                     # Build system
├── conanfile.txt                      # (nếu dùng Conan)
├── README.md                          # Hướng dẫn chạy
│
├── src/
│   ├── main.cpp                      // Điểm vào chính, khởi tạo và chạy ứng dụng.
│   │
│   ├── common/                       // Các định nghĩa, enum dùng chung.
│   │   ├── AppConfig.h/.cpp          // Struct/class để lưu cấu hình ứng dụng (đường dẫn file, loại DB).
│   │   ├── ErrorType.h               // Định nghĩa struct Error { int code; std::string message; }.
│   │   ├── UserRole.h                // enum class UserRole { ADMIN, STUDENT, TEACHER, PENDING_STUDENT };
│   │   ├── OperationResult.h         // Typedef std::expected<T, Error> cho các thao tác.
│   │   │
    ├── core/
    │   ├── entities/                 // Các đối tượng nghiệp vụ (Domain Objects).
    │   │   ├── IEntity.h             // Interface cơ sở: virtual std::string getId() const = 0; virtual std::string toString() const = 0;
    │   │   ├── Birthday.h/.cpp       // Lớp quản lý ngày sinh.
    │   │   ├── User.h/.cpp           // Lớp cơ sở trừu tượng, kế thừa IEntity.
    │   │   │                         // (id, firstName, lastName, birthday, address, citizenId, email, phoneNumber, role, passwordHash, salt)
    │   │   ├── Student.h/.cpp        // Kế thừa User. (facultyId)
    │   │   ├── Teacher.h/.cpp        // Kế thừa User. (facultyId, qualification, specializationSubjects, designation, experienceYears)
    │   │   ├── Faculty.h/.cpp        // Khoa/Bộ môn, kế thừa IEntity. (id, name)
    │   │   ├── Course.h/.cpp         // Môn học, kế thừa IEntity. (id, name, credits, facultyId)
    │   │   ├── CourseResult.h/.cpp   // Kết quả 1 môn của 1 SV. (studentId, courseId, marks, grade) - không kế thừa IEntity.
    │   │   ├── FeeRecord.h/.cpp      // Học phí, kế thừa IEntity (ID là studentId). (studentId, totalFee, paidFee)
    │   │   └── SalaryRecord.h/.cpp   // Lương, kế thừa IEntity (ID là teacherId). (teacherId, basicMonthlyPay)

    │   ├── data_access/              // Lớp Truy cập Dữ liệu (DAO/Repository Pattern).
    │   │   ├── interface/
    │   │   │   ├── IDao.h            // Template Interface: IDao<TEntity, TId>
    │   │   │   │                     // (getById, getAll, add, update, remove) - trả về OperationResult<T> hoặc OperationResult<bool>
    │   │   │   ├── IStudentDao.h     // Kế thừa IDao<Student, std::string>, thêm findByFacultyId, findByEmail.
    │   │   │   ├── ITeacherDao.h     // Kế thừa IDao<Teacher, std::string>, thêm findByFacultyId, findByDesignation.
    │   │   │   ├── IFacultyDao.h     // Kế thừa IDao<Faculty, std::string>, thêm findByName.
    │   │   │   ├── ICourseDao.h      // Kế thừa IDao<Course, std::string>, thêm findByFacultyId.
    │   │   │   ├── IEnrollmentDao.h  // Quản lý bản ghi đăng ký học (studentId, courseId) - không cần kế thừa IDao nếu chỉ có add/remove/find.
    │   │   │   │                     // (addEnrollment, removeEnrollment, findCoursesByStudent, findStudentsByCourse, isEnrolled)
    │   │   │   ├── ICourseResultDao.h// Quản lý CourseResult.
    │   │   │   │                     // (find, findByStudent, findByCourse, addOrUpdate, remove)
    │   │   │   ├── IFeeRecordDao.h   // Kế thừa IDao<FeeRecord, std::string> (ID là studentId).
    │   │   │   ├── ISalaryRecordDao.h// Kế thừa IDao<SalaryRecord, std::string> (ID là teacherId).
    │   │   │   └── ILoginDao.h       // Quản lý thông tin đăng nhập (userId, passwordHash, salt, role).
    │   │   │                         // (findCredentialsById, addUserCredentials, updatePassword, removeUserCredentials, getUserRole)
    │   │   ├── impl_csv/             // Triển khai DAO cho CSV.
    │   │   │   ├── CsvStudentDao.h/.cpp
    │   │   │   ├── CsvTeacherDao.h/.cpp
    │   │   │   ├── CsvFacultyDao.h/.cpp
    │   │   │   ├── CsvCourseDao.h/.cpp
    │   │   │   ├── CsvEnrollmentDao.h/.cpp
    │   │   │   ├── CsvCourseResultDao.h/.cpp
    │   │   │   ├── CsvFeeRecordDao.h/.cpp
    │   │   │   ├── CsvSalaryRecordDao.h/.cpp
    │   │   │   └── CsvLoginDao.h/.cpp
    │   │   ├── impl_sql/             // Triển khai DAO cho SQL (sẽ làm sau).
    │   │   │   └── ...
    │   │   ├── mock/              // (➕) Thư mục cho Mock DAOs
    │   │   │   │   ├── MockStudentDao.h/.cpp
    │   │   │   │   ├── MockTeacherDao.h/.cpp
    │   │   │   │   └── ...
    │   │   └── DaoFactory.h/.cpp     // Factory để tạo DAO instances dựa trên AppConfig.
    │   │
    │   ├── services/                 // Lớp Dịch vụ (Logic Nghiệp vụ).
    │   │   ├── interface/
    │   │   │   ├── IAuthService.h
    │   │   │   ├── IStudentService.h
    │   │   │   ├── ITeacherService.h
    │   │   │   ├── IFacultyService.h
    │   │   │   ├── ICourseService.h
    │   │   │   ├── IEnrollmentService.h
    │   │   │   ├── IResultService.h
    │   │   │   ├── IFinanceService.h
    │   │   │   └── IAdminService.h      // (Có thể bao gồm cả việc duyệt đăng ký SV mới).
    │   │   ├── impl/
    │   │   │   ├── AuthService.h/.cpp   // (login, logout, registerStudent, changePassword, getCurrentUser)
    │   │   │   ├── StudentService.h/.cpp// (getDetails, getAll, updateDetails, /* viewEnrolledCourses - bỏ sang EnrollmentService */, /* viewResultReport - bỏ sang ResultService */)
    │   │   │   ├── TeacherService.h/.cpp// (getDetails, getAll, updateDetails, /* enterMarks - bỏ sang ResultService */)
    │   │   │   ├── FacultyService.h/.cpp// (getById, getAll, add, update, remove - admin only)
    │   │   │   ├── CourseService.h/.cpp // (getById, getAll, getByFaculty, add, update, remove - admin only)
    │   │   │   ├── EnrollmentService.h/.cpp // (enroll, drop, getEnrolledCourses, getEnrolledStudents)
    │   │   │   ├── ResultService.h/.cpp   // (enterMarks, getStudentResults, getCourseResults, generateReport, calculateCGPA/SGPA)
    │   │   │   ├── FinanceService.h/.cpp  // (getStudentFee, makePayment, setTotalFee, getTeacherSalary, setBasicSalary, generateFeeReceipt, generateSalaryCert)
    │   │   │   └── AdminService.h/.cpp    // (approveStudentRegistration, manageUsers (add/update/remove student/teacher thông qua services tương ứng), manageFaculties, manageCourses)

    │   ├── validators/               // Lớp Kiểm tra Dữ liệu Đầu vào.
    │   │   ├── interface/
    │   │   │   └── IValidator.h      // template<typename T> { virtual ValidationResult validate(const T& data) const; } // ValidationResult là struct chứa bool isValid và vector<Error>
    │   │   ├── impl/
    │   │   │   ├── StudentValidator.h/.cpp
    │   │   │   ├── TeacherValidator.h/.cpp
    │   │   │   ├── CourseValidator.h/.cpp
    │   │   │   └── GeneralInputValidator.h/.cpp // (validateIdFormat, email, phone, non-empty string, numeric range)

    │   └── database_adapter/         // (Nếu dùng SQL) Giao tiếp với CSDL cụ thể.
    │       ├── interface/
    │       │   └── IDatabaseAdapter.h// (connect, disconnect, executeQuery, executeUpdate)
    │       ├── impl_mysql/
    │       │   ├── MySqlAdapter.h/.cpp
    │       │   └── MySqlConnectionManager.h/.cpp // Quản lý kết nối MySQL.

    ├── ui/                           // Giao diện Người dùng.
    │   ├── ConsoleUI.h/.cpp          // Lớp chính quản lý UI Console.
    │   ├── MenuOption.h              // Enums cho các lựa chọn menu.
    │   ├── InputPrompter.h/.cpp      // Lớp helper để lấy input (string, int, y/n) an toàn.
    │   └── TableDisplayer.h/.cpp     // Lớp helper để hiển thị dữ liệu dạng bảng.

    ├── utils/                        // Các tiện ích.
    │   ├── Logger.h/.cpp             // Ghi log.
    │   ├── FileHandler.h/.cpp        // Đọc/ghi file CSV cơ bản.
    │   ├── PasswordUtils.h/.cpp      // Hash/verify password, generate salt, check complexity.
    │   ├── StringUtils.h/.cpp        // trim, toLower, split, join.
    │   └── ConfigLoader.h/.cpp       // Đọc file cấu hình app_config.ini.

    ├── config/                       // File cấu hình.
    │   └── app_config.ini

    ├── data/                         // Thư mục chứa file CSV (khi datasource_type=CSV).
    │   ├── students.csv
    │   ├── teachers.csv
    │   ├── faculties.csv
    │   ├── courses.csv
    │   ├── enrollments.csv
    │   ├── results.csv
    │   ├── fees.csv
    │   ├── salaries.csv
    │   └── logins.csv

    ├── tests/                        // (Quan trọng) Unit Tests.
    │   ├── services/
    │   │   └── StudentServiceTest.cpp
    │   ├── data_access/
    │   │   └── CsvStudentDaoTest.cpp
    │   └── validators/
    │       └── StudentValidatorTest.cpp

    ├── CMakeLists.txt                // File build cho CMake.
    └── README.md
│
├── data/                             # Data files
│   ├── students.csv
│   ├── teachers.csv
│   ├── faculties.csv
│   ├── courses.csv
│   ├── logins.csv
│   └── other data files...
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
