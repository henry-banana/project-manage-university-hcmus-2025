#ifndef MENUOPTION_H
#define MENUOPTION_H

// Enum cho các lựa chọn ở Main Menu
enum class MainMenuOption {
    FINANCE = 1,
    ADMIN = 2,
    STUDENT_FACULTY_LOGIN = 3,
    EXIT = 0
};

// Enum cho Student Menu
enum class StudentMenuOption {
    VIEW_RESULT = 1,
    VIEW_DETAILS = 2,
    BACK = 0 // Hoặc EXIT_TO_MAIN
};

// Enum cho Faculty Menu
enum class FacultyMenuOption {
    VIEW_DETAILS = 1,
    BACK = 0
};

// Enum cho Admin Menu (Top level)
enum class AdminMainMenuOption {
    STUDENT_SECTION = 1,
    FACULTY_SECTION = 2,
    BACK = 0
};

// Enum cho Admin Student Sub-Menu
enum class AdminStudentMenuOption {
    FIND_DISPLAY_STUDENT = 1,
    LIST_BY_DEPARTMENT = 2,
    BACK = 0
};

// Enum cho Admin Faculty Sub-Menu
enum class AdminFacultyMenuOption {
    FIND_DISPLAY_FACULTY = 1,
    LIST_HODS = 2,
    LIST_BY_DEPARTMENT = 3,
    BACK = 0
};


// Enum cho Finance Menu (Student)
enum class FinanceStudentMenuOption {
    PAY_FEES = 1,
    VIEW_RECEIPT = 2,
    BACK = 0
};

// Enum cho Finance Menu (Faculty)
enum class FinanceFacultyMenuOption {
    GENERATE_SALARY_CERT = 1,
    BACK = 0
};

#endif // MENUOPTION_H