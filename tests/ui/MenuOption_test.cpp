#include "gtest/gtest.h"
#include "../src/ui/MenuOption.h"

TEST(MenuOptionTest, MainMenuOption) {
    ASSERT_EQ(static_cast<int>(MainMenuOption::FINANCE), 1);
    ASSERT_EQ(static_cast<int>(MainMenuOption::ADMIN), 2);
    ASSERT_EQ(static_cast<int>(MainMenuOption::LOGIN), 3);
    ASSERT_EQ(static_cast<int>(MainMenuOption::EXIT), 0);
}

TEST(MenuOptionTest, StudentMenuOption) {
    ASSERT_EQ(static_cast<int>(StudentMenuOption::VIEW_RESULT), 1);
    ASSERT_EQ(static_cast<int>(StudentMenuOption::VIEW_DETAILS), 2);
    ASSERT_EQ(static_cast<int>(StudentMenuOption::BACK), 0);
}

TEST(MenuOptionTest, FacultyMenuOption) {
    ASSERT_EQ(static_cast<int>(FacultyMenuOption::VIEW_DETAILS), 1);
    ASSERT_EQ(static_cast<int>(FacultyMenuOption::BACK), 0);
}

TEST(MenuOptionTest, AdminMainMenuOption) {
    ASSERT_EQ(static_cast<int>(AdminMainMenuOption::STUDENT_SECTION), 1);
    ASSERT_EQ(static_cast<int>(AdminMainMenuOption::FACULTY_SECTION), 2);
    ASSERT_EQ(static_cast<int>(AdminMainMenuOption::BACK), 0);
}

TEST(MenuOptionTest, AdminStudentMenuOption) {
    ASSERT_EQ(static_cast<int>(AdminStudentMenuOption::FIND_DISPLAY_STUDENT), 1);
    ASSERT_EQ(static_cast<int>(AdminStudentMenuOption::LIST_BY_DEPARTMENT), 2);
    ASSERT_EQ(static_cast<int>(AdminStudentMenuOption::BACK), 0);
}

TEST(MenuOptionTest, AdminFacultyMenuOption) {
    ASSERT_EQ(static_cast<int>(AdminFacultyMenuOption::FIND_DISPLAY_FACULTY), 1);
    ASSERT_EQ(static_cast<int>(AdminFacultyMenuOption::LIST_HODS), 2);
    ASSERT_EQ(static_cast<int>(AdminFacultyMenuOption::LIST_BY_DEPARTMENT), 3);
    ASSERT_EQ(static_cast<int>(AdminFacultyMenuOption::BACK), 0);
}

TEST(MenuOptionTest, FinanceStudentMenuOption) {
    ASSERT_EQ(static_cast<int>(FinanceStudentMenuOption::PAY_FEES), 1);
    ASSERT_EQ(static_cast<int>(FinanceStudentMenuOption::VIEW_RECEIPT), 2);
    ASSERT_EQ(static_cast<int>(FinanceStudentMenuOption::BACK), 0);
}

TEST(MenuOptionTest, FinanceFacultyMenuOption) {
    ASSERT_EQ(static_cast<int>(FinanceFacultyMenuOption::GENERATE_SALARY_CERT), 1);
    ASSERT_EQ(static_cast<int>(FinanceFacultyMenuOption::BACK), 0);
}