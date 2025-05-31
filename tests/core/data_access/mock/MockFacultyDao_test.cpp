#include <gtest/gtest.h>
#include "../../../src/core/data_access/mock/MockFacultyDao.h" // (➕) Sửa đường dẫn nếu cần
#include "../../../src/core/entities/Faculty.h"     // (➕)
#include "../../../src/common/ErrorType.h"         // (➕)

Faculty makeSampleFaculty(const std::string& id = "IT_Test", const std::string& name = "Test InfoTech") {
    return Faculty(id, name);
}

class MockFacultyDaoTest : public ::testing::Test { // (➕) Đổi tên Test Fixture
protected:
    MockFacultyDao dao; // Mỗi test case sẽ có instance DAO riêng, nhưng data là static

    void SetUp() override {
        MockFacultyDao::clearMockData(); // Xóa dữ liệu từ test case/suite trước
        // Không gọi initializeDefaultMockData() ở đây để mỗi test tự quyết định dữ liệu ban đầu
        // Hoặc nếu muốn bộ dữ liệu chung cho cả suite:
        // MockFacultyDao::initializeDefaultMockData(); 
    }

    void TearDown() override {
        MockFacultyDao::clearMockData(); // Dọn dẹp sau mỗi test case
    }
};

TEST_F(MockFacultyDaoTest, AddAndGetById_WhenEmpty) {
    //MockFacultyDao dao; // Không cần tạo lại, dùng this->dao hoặc dao trực tiếp
    Faculty f = makeSampleFaculty("F01", "Faculty One");
    auto addResult = dao.add(f);
    ASSERT_TRUE(addResult.has_value()) << addResult.error().message;

    auto getResult = dao.getById("F01");
    ASSERT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getId(), "F01");
    EXPECT_EQ(getResult->getName(), "Faculty One");
}

TEST_F(MockFacultyDaoTest, GetById_NotFound_WhenEmpty) {
    //MockFacultyDao dao;
    auto result = dao.getById("NON_EXIST");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFacultyDaoTest, Add_DuplicateId_ReturnsError) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F02", "Faculty Two"));
    Faculty duplicate("F02", "Another Faculty Two");
    auto result = dao.add(duplicate);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockFacultyDaoTest, Add_DuplicateName_ReturnsError) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F03", "Faculty Three"));
    Faculty duplicate("F04", "Faculty Three"); // ID khác, tên trùng
    auto result = dao.add(duplicate);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}


TEST_F(MockFacultyDaoTest, Exists_WhenPresent) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F05", "Faculty Five"));

    auto exist = dao.exists("F05");
    ASSERT_TRUE(exist.has_value());
    EXPECT_TRUE(exist.value());
}

TEST_F(MockFacultyDaoTest, Exists_WhenNotPresent) {
    //MockFacultyDao dao;
    auto notExist = dao.exists("XYZ");
    ASSERT_TRUE(notExist.has_value()); // exists luôn trả về expected<bool, Error> (true/false), không phải error nếu không tìm thấy
    EXPECT_FALSE(notExist.value());
}


TEST_F(MockFacultyDaoTest, UpdateFaculty_Success) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F06", "Faculty Six"));

    Faculty updated("F06", "New Name Six");
    auto updateResult = dao.update(updated);
    ASSERT_TRUE(updateResult.has_value()) << (updateResult.has_value() ? "" : updateResult.error().message) ;
    EXPECT_TRUE(updateResult.value());

    auto getResult = dao.getById("F06");
    ASSERT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getName(), "New Name Six");
}

TEST_F(MockFacultyDaoTest, UpdateFaculty_NameToExisting_ReturnsError) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F07A", "Faculty Seven A"));
    dao.add(makeSampleFaculty("F07B", "Faculty Seven B"));

    Faculty updated("F07B", "Faculty Seven A"); // Cố gắng đổi tên F07B thành tên của F07A
    auto updateResult = dao.update(updated);
    ASSERT_FALSE(updateResult.has_value());
    EXPECT_EQ(updateResult.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockFacultyDaoTest, RemoveFaculty_Success) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F08", "Faculty Eight"));

    auto removeResult = dao.remove("F08");
    ASSERT_TRUE(removeResult.has_value());
    EXPECT_TRUE(removeResult.value());

    auto getResult = dao.getById("F08");
    EXPECT_FALSE(getResult.has_value());
}

TEST_F(MockFacultyDaoTest, RemoveFaculty_NotFound_ReturnsError) {
    //MockFacultyDao dao;
    auto removeResult = dao.remove("F09_NON_EXISTENT");
    ASSERT_FALSE(removeResult.has_value());
    EXPECT_EQ(removeResult.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFacultyDaoTest, FindByName_Success) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F10", "InfoTech Unique"));
    dao.add(makeSampleFaculty("F11", "Electrical Unique"));

    auto found = dao.findByName("Electrical Unique");
    ASSERT_TRUE(found.has_value()) << (found.has_value() ? "" : found.error().message);
    EXPECT_EQ(found->getId(), "F11");
}

TEST_F(MockFacultyDaoTest, FindByName_NotFound) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("F12", "Another Faculty"));
    auto notFound = dao.findByName("Non Existent Name");
    ASSERT_FALSE(notFound.has_value());
    EXPECT_EQ(notFound.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockFacultyDaoTest, GetAll_WhenEmpty) {
    //MockFacultyDao dao;
    auto all = dao.getAll();
    ASSERT_TRUE(all.has_value());
    EXPECT_TRUE(all->empty());
}

TEST_F(MockFacultyDaoTest, GetAll_WithData) {
    //MockFacultyDao dao;
    dao.add(makeSampleFaculty("IT", "InfoTech"));
    dao.add(makeSampleFaculty("EE", "Electrical"));

    auto all = dao.getAll();
    ASSERT_TRUE(all.has_value());
    EXPECT_EQ(all->size(), 2);
}
