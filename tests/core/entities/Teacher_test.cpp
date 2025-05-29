#include <gtest/gtest.h>
#include "../src/core/entities/Teacher.h"
#include "../src/common/LoginStatus.h"
#include "../src/core/entities/User.h"

// Để test abstract class User, tạo subclass tạm nếu cần
class TestTeacher : public Teacher {
public:
    TestTeacher(const std::string& id,
                const std::string& firstName,
                const std::string& lastName,
                const std::string& facultyId,
                LoginStatus status = LoginStatus::ACTIVE)
        : Teacher(id, firstName, lastName, facultyId, status) {}

    std::string display() const override {
        return getFullName() + " - " + getFacultyId();
    }

    ValidationResult validateBasic() const override {
        // Giả lập trả về hợp lệ nếu facultyId không rỗng, năm kinh nghiệm >= 0
        ValidationResult res;
        if (getId().empty()) {
            res.isValid = false;
            res.addError(1, "ID không được để trống");
        }
        if (getFirstName().empty()) {
            res.isValid = false;
            res.addError(2, "FirstName không được để trống");
        }
        if (getLastName().empty()) {
            res.isValid = false;
            res.addError(3, "LastName không được để trống");
        }
        if (getFacultyId().empty()) {
            res.isValid = false;
            res.addError(4, "FacultyId không được để trống");
        }
        if (getExperienceYears() < 0) {
            res.isValid = false;
            res.addError(5, "ExperienceYears phải >= 0");
        }
        return res;
    }
};

TEST(TeacherTest, ConstructorAndGetters) {
    TestTeacher t("GV001", "Anh", "Tran", "CS");

    EXPECT_EQ(t.getId(), "GV001");
    EXPECT_EQ(t.getFirstName(), "Anh");
    EXPECT_EQ(t.getLastName(), "Tran");
    EXPECT_EQ(t.getFacultyId(), "CS");
    EXPECT_EQ(t.getQualification(), "");
    EXPECT_EQ(t.getSpecializationSubjects(), "");
    EXPECT_EQ(t.getDesignation(), "");
    EXPECT_EQ(t.getExperienceYears(), 0);
    EXPECT_EQ(t.getStatus(), LoginStatus::ACTIVE);
}

TEST(TeacherTest, SettersValid) {
    TestTeacher t("GV002", "Binh", "Le", "Math");

    EXPECT_TRUE(t.setFacultyId("Physics"));
    EXPECT_TRUE(t.setQualification("PhD"));
    EXPECT_TRUE(t.setSpecializationSubjects("Algebra, Geometry"));
    EXPECT_TRUE(t.setDesignation("Professor"));
    EXPECT_TRUE(t.setExperienceYears(10));

    EXPECT_EQ(t.getFacultyId(), "Physics");
    EXPECT_EQ(t.getQualification(), "PhD");
    EXPECT_EQ(t.getSpecializationSubjects(), "Algebra, Geometry");
    EXPECT_EQ(t.getDesignation(), "Professor");
    EXPECT_EQ(t.getExperienceYears(), 10);
}

TEST(TeacherTest, SettersInvalid) {
    TestTeacher t("GV003", "Cuong", "Pham", "Chem");

    EXPECT_FALSE(t.setFacultyId(""));   // Không cho phép rỗng
    EXPECT_FALSE(t.setExperienceYears(-1)); // Không cho phép âm
    EXPECT_TRUE(t.setQualification("MSc")); // Có thể set bình thường
    EXPECT_TRUE(t.setDesignation("Lecturer")); // Bình thường

    // Giá trị không thay đổi sau gọi set không hợp lệ
    EXPECT_EQ(t.getFacultyId(), "Chem");
    EXPECT_EQ(t.getExperienceYears(), 0);
    EXPECT_EQ(t.getQualification(), "MSc");
    EXPECT_EQ(t.getDesignation(), "Lecturer");
}

TEST(TeacherTest, DisplayIncludesBasicInfo) {
    TestTeacher t("GV004", "Dung", "Hoang", "Physics");
    t.setQualification("PhD");
    t.setSpecializationSubjects("Quantum Mechanics");
    t.setDesignation("HOD");
    t.setExperienceYears(15);

    std::string disp = t.display();

    EXPECT_NE(disp.find("Dung"), std::string::npos);
    EXPECT_NE(disp.find("Hoang"), std::string::npos);
    EXPECT_NE(disp.find("Physics"), std::string::npos);
}

TEST(TeacherTest, ValidateBasicReturnsValid) {
    TestTeacher t("GV005", "Hieu", "Nguyen", "Biology");
    t.setExperienceYears(5);

    ValidationResult res = t.validateBasic();

    EXPECT_TRUE(res.isValid);
    EXPECT_TRUE(res.errors.empty());
}

TEST(TeacherTest, ValidateBasicReturnsInvalid) {
    TestTeacher t1("", "Hanh", "Tran", "CS");
    TestTeacher t2("GV006", "", "Tran", "CS");
    TestTeacher t3("GV007", "Hanh", "", "CS");
    TestTeacher t4("GV008", "Hanh", "Tran", "");
    TestTeacher t5("GV009", "Hanh", "Tran", "CS");
    t5.setExperienceYears(-3);

    EXPECT_FALSE(t1.validateBasic().isValid);
    EXPECT_FALSE(t2.validateBasic().isValid);
    EXPECT_FALSE(t3.validateBasic().isValid);
    EXPECT_FALSE(t4.validateBasic().isValid);
    EXPECT_FALSE(t5.validateBasic().isValid);
}
