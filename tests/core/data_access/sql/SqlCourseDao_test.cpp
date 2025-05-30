// // File: test_SqlCourseDao.cpp
// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "../../src/core/data_access/sql/SqlCourseDao.h"
// #include "../../src/core/entities/Course.h"
// #include "../../src/common/ErrorType.h"
// #include "../../src/core/database_adapter/sql/SQLiteAdapter.h"
// #include "../../src/core/parsing/impl_sql_parser/CourseSqlParser.h"

// using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnRefOfCopy;
// using ::testing::Throw;
// using ::testing::_;
// using ::testing::NiceMock;


// class SqlCourseDaoTest : public ::testing::Test {
// protected:
//     std::shared_ptr<NiceMock<SQLiteAdapter>> dbAdapter;
//     std::shared_ptr<NiceMock<CourseSqlParser>> parser;
//     std::unique_ptr<SqlCourseDao> dao;

//     void SetUp() override {
//         dbAdapter = std::make_shared<NiceMock<SQLiteAdapter>>();
//         parser = std::make_shared<NiceMock<CourseSqlParser>>();
//         dao = std::make_unique<SqlCourseDao>(dbAdapter, parser);
//     }
// };

// TEST_F(SqlCourseDaoTest, GetById_ReturnsCourse_WhenFound) {
//     std::string id = "CS101";
//     DbQueryResultRow row;
//     Course expectedCourse{"CS101", "Intro to CS",4, "F001"};

//     EXPECT_CALL(*dbAdapter, querySingle(_)).WillOnce(Return(row));
//     EXPECT_CALL(*parser, parse(row)).WillOnce(Return(expectedCourse));

//     auto result = dao->getById(id);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getId(), "CS101");
// }

// TEST_F(SqlCourseDaoTest, GetById_ReturnsError_WhenNotFound) {
//     EXPECT_CALL(*dbAdapter, querySingle(_)).WillOnce(Return(Error{"Not found"}));

//     auto result = dao->getById("INVALID_ID");
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().message(), "Not found");
// }

// TEST_F(SqlCourseDaoTest, GetAll_ReturnsCourses) {
//     DbQueryResult result;
//     std::vector<DbQueryResultRow> rows = {DbQueryResultRow{}, DbQueryResultRow{}};
//     Course course1{"CS101", "Intro to CS", "F001"};
//     Course course2{"CS102", "Data Structures", "F001"};

//     result.rows = rows;

//     EXPECT_CALL(*dbAdapter, query(_)).WillOnce(Return(result));
//     EXPECT_CALL(*parser, parse(_)).WillOnce(Return(course1)).WillOnce(Return(course2));

//     auto resultExp = dao->getAll();
//     ASSERT_TRUE(resultExp.has_value());
//     EXPECT_EQ(resultExp->size(), 2);
// }

// TEST_F(SqlCourseDaoTest, Add_ReturnsInsertedCourse) {
//     Course course{"CS103", "Algorithms", "F002"};

//     EXPECT_CALL(*dbAdapter, insert(_)).WillOnce(Return(true));
//     EXPECT_CALL(*parser, serialize(course)).WillOnce(Return(DbQuery{}));

//     auto result = dao->add(course);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_EQ(result->getId(), "CS103");
// }

// TEST_F(SqlCourseDaoTest, Add_ReturnsErrorOnFailure) {
//     Course course{"CS103", "Algorithms", "F002"};

//     EXPECT_CALL(*dbAdapter, insert(_)).WillOnce(Return(Error{"Insert failed"}));
//     EXPECT_CALL(*parser, serialize(course)).WillOnce(Return(DbQuery{}));

//     auto result = dao->add(course);
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().message(), "Insert failed");
// }

// TEST_F(SqlCourseDaoTest, Update_Success) {
//     Course course{"CS101", "Intro to CS", "F001"};

//     EXPECT_CALL(*dbAdapter, update(_)).WillOnce(Return(true));
//     EXPECT_CALL(*parser, serialize(course)).WillOnce(Return(DbQuery{}));

//     auto result = dao->update(course);
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(SqlCourseDaoTest, Update_Failure) {
//     Course course{"CS101", "Intro to CS", "F001"};

//     EXPECT_CALL(*dbAdapter, update(_)).WillOnce(Return(Error{"Update failed"}));
//     EXPECT_CALL(*parser, serialize(course)).WillOnce(Return(DbQuery{}));

//     auto result = dao->update(course);
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().message(), "Update failed");
// }

// TEST_F(SqlCourseDaoTest, Remove_Success) {
//     EXPECT_CALL(*dbAdapter, remove(_)).WillOnce(Return(true));

//     auto result = dao->remove("CS101");
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(SqlCourseDaoTest, Remove_Failure) {
//     EXPECT_CALL(*dbAdapter, remove(_)).WillOnce(Return(Error{"Delete failed"}));

//     auto result = dao->remove("CS101");
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().message(), "Delete failed");
// }

// TEST_F(SqlCourseDaoTest, Exists_ReturnsTrue) {
//     EXPECT_CALL(*dbAdapter, exists(_)).WillOnce(Return(true));

//     auto result = dao->exists("CS101");
//     ASSERT_TRUE(result.has_value());
//     EXPECT_TRUE(result.value());
// }

// TEST_F(SqlCourseDaoTest, Exists_ReturnsFalse) {
//     EXPECT_CALL(*dbAdapter, exists(_)).WillOnce(Return(false));

//     auto result = dao->exists("CS101");
//     ASSERT_TRUE(result.has_value());
//     EXPECT_FALSE(result.value());
// }

// TEST_F(SqlCourseDaoTest, Exists_ReturnsError) {
//     EXPECT_CALL(*dbAdapter, exists(_)).WillOnce(Return(Error{"Exist check failed"}));

//     auto result = dao->exists("CS101");
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().message(), "Exist check failed");
// }

// TEST_F(SqlCourseDaoTest, FindByFacultyId_ReturnsCourses) {
//     DbQueryResult result;
//     result.rows = {DbQueryResultRow{}, DbQueryResultRow{}};
//     Course c1{"CS101", "Intro", "F001"}, c2{"CS102", "Adv", "F001"};

//     EXPECT_CALL(*dbAdapter, query(_)).WillOnce(Return(result));
//     EXPECT_CALL(*parser, parse(_)).WillOnce(Return(c1)).WillOnce(Return(c2));

//     auto resultExp = dao->findByFacultyId("F001");
//     ASSERT_TRUE(resultExp.has_value());
//     EXPECT_EQ(resultExp->size(), 2);
//     EXPECT_EQ(resultExp->at(0).getId(), "CS101");
// }

// TEST_F(SqlCourseDaoTest, FindByFacultyId_ReturnsError) {
//     EXPECT_CALL(*dbAdapter, query(_)).WillOnce(Return(Error{"Query failed"}));

//     auto result = dao->findByFacultyId("F001");
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().message(), "Query failed");
// }