// --- START OF NEW FILE tests/TableDisplayer_test.cpp ---
#include "gtest/gtest.h"
#include "../src/ui/view_helpers/TableDisplayer.h"
#include <sstream>

TEST(TableDisplayerTest, DisplayEmptyTable) {
    TableDisplayer displayer;
    std::vector<std::string> headers = {"ID", "Name"};
    std::vector<std::vector<std::string>> rows = {};
    std::vector<int> widths = {5, 20}; // ID:3+2, Name:18+2

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    displayer.displayTable(headers, rows, widths);

    std::cout.rdbuf(oldCout);

    std::string expectedOutput =
        "+-----+--------------------+\n"
        "| ID  | Name                 |\n" // setw(4) cho ID, setw(19) cho Name
        "+-----+--------------------+\n"
        "|  No data available         |\n" // setw(26) tổng (5+20+1)
        "+-----+--------------------+\n";
    EXPECT_EQ(ss.str(), expectedOutput);
}

TEST(TableDisplayerTest, DisplayTableWithData) {
    TableDisplayer displayer;
    std::vector<std::string> headers = {"ID", "Name", "Role"};
    std::vector<std::vector<std::string>> rows = {
        {"1", "Alice", "Admin"},
        {"2", "Bob", "User"}
    };
    std::vector<int> widths = {4, 10, 10}; // ID:2+2, Name:8+2, Role:8+2

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    displayer.displayTable(headers, rows, widths);

    std::cout.rdbuf(oldCout);

    std::string expectedOutput =
        "+----+----------+----------+\n"
        "| ID | Name     | Role     |\n" // setw(3), setw(9), setw(9)
        "+----+----------+----------+\n"
        "| 1  | Alice    | Admin    |\n"
        "| 2  | Bob      | User     |\n"
        "+----+----------+----------+\n";
    EXPECT_EQ(ss.str(), expectedOutput);
}

TEST(TableDisplayerTest, DisplayTableAutoWidth) {
    TableDisplayer displayer;
    std::vector<std::string> headers = {"Key", "Value"};
    std::vector<std::vector<std::string>> rows = {
        {"Lang", "C++"},
        {"Year", "2023"}
    };
    // Không truyền widths, để tự tính

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    displayer.displayTable(headers, rows);

    std::cout.rdbuf(oldCout);

    // Độ rộng tự tính: Key (4+2=6), Value (4+2=6)
    std::string expectedOutput =
        "+------+------+\n"
        "| Key  | Value|\n" // setw(5), setw(5)
        "+------+------+\n"
        "| Lang | C++  |\n"
        "| Year | 2023 |\n"
        "+------+------+\n";
    EXPECT_EQ(ss.str(), expectedOutput);
}
// --- END OF NEW FILE tests/TableDisplayer_test.cpp ---