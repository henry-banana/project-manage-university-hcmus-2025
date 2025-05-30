#include "gtest/gtest.h"
#include "../src/common/ErrorType.h"

TEST(ErrorTest, Constructor) {
    Error error(404, "Not Found");
    ASSERT_EQ(error.code, 404);
    ASSERT_EQ(error.message, "Not Found");
}

TEST(ErrorCodeTest, Validation) {
    ASSERT_EQ(ErrorCode::VALIDATION_ERROR, 1001);
}

TEST(ErrorCodeTest, NotFound) {
    ASSERT_EQ(ErrorCode::NOT_FOUND, 1002);
}

TEST(ErrorCodeTest, AlreadyExists) {
    ASSERT_EQ(ErrorCode::ALREADY_EXISTS, 1003);
}

TEST(ErrorCodeTest, DataAccessError) {
    ASSERT_EQ(ErrorCode::DATA_ACCESS_ERROR, 1004);
}

TEST(ErrorCodeTest, AuthenticationFailed) {
    ASSERT_EQ(ErrorCode::AUTHENTICATION_FAILED, 1005);
}

TEST(ErrorCodeTest, PermissionDenied) {
    ASSERT_EQ(ErrorCode::PERMISSION_DENIED, 1006);
}

TEST(ErrorCodeTest, OperationFailed) {
    ASSERT_EQ(ErrorCode::OPERATION_FAILED, 1007);
}

TEST(ErrorCodeTest, ParsingError) {
    ASSERT_EQ(ErrorCode::PARSING_ERROR, 1008);
}

TEST(ErrorCodeTest, InvalidStudentData) {
    ASSERT_EQ(ErrorCode::INVALID_STUDENT_DATA, 1101);
}

TEST(ErrorCodeTest, InvalidTeacherData) {
    ASSERT_EQ(ErrorCode::INVALID_TEACHER_DATA, 1102);
}

TEST(ErrorCodeTest, InvalidCourseData) {
    ASSERT_EQ(ErrorCode::INVALID_COURSE_DATA, 1103);
}

TEST(ErrorCodeTest, InvalidEnrollmentData) {
    ASSERT_EQ(ErrorCode::INVALID_ENROLLMENT_DATA, 1104);
}

TEST(ErrorCodeTest, InvalidResultData) {
    ASSERT_EQ(ErrorCode::INVALID_RESULT_DATA, 1105);
}

TEST(ErrorCodeTest, InvalidFeeData) {
    ASSERT_EQ(ErrorCode::INVALID_FEE_DATA, 1106);
}

TEST(ErrorCodeTest, InvalidSalaryData) {
    ASSERT_EQ(ErrorCode::INVALID_SALARY_DATA, 1107);
}

TEST(ErrorCodeTest, CourseEnrollmentClosed) {
    ASSERT_EQ(ErrorCode::COURSE_ENROLLMENT_CLOSED, 1201);
}

TEST(ErrorCodeTest, CoursePrerequisiteNotMet) {
    ASSERT_EQ(ErrorCode::COURSE_PREREQUISITE_NOT_MET, 1202);
}

TEST(ErrorCodeTest, CourseCapacityReached) {
    ASSERT_EQ(ErrorCode::COURSE_CAPACITY_REACHED, 1203);
}

TEST(ErrorCodeTest, StudentAlreadyEnrolled) {
    ASSERT_EQ(ErrorCode::STUDENT_ALREADY_ENROLLED, 1204);
}

TEST(ErrorCodeTest, FeePaymentRequired) {
    ASSERT_EQ(ErrorCode::FEE_PAYMENT_REQUIRED, 1205);
}

TEST(ErrorCodeTest, GradingPeriodClosed) {
    ASSERT_EQ(ErrorCode::GRADING_PERIOD_CLOSED, 1206);
}

TEST(ErrorCodeTest, FileNotFound) {
    ASSERT_EQ(ErrorCode::FILE_NOT_FOUND, 1301);
}

TEST(ErrorCodeTest, FileAccessDenied) {
    ASSERT_EQ(ErrorCode::FILE_ACCESS_DENIED, 1302);
}

TEST(ErrorCodeTest, FileFormatError) {
    ASSERT_EQ(ErrorCode::FILE_FORMAT_ERROR, 1303);
}

TEST(ErrorCodeTest, FileWriteError) {
    ASSERT_EQ(ErrorCode::FILE_WRITE_ERROR, 1304);
}

TEST(ErrorCodeTest, CsvParsingError) {
    ASSERT_EQ(ErrorCode::CSV_PARSING_ERROR, 1305);
}

TEST(ErrorCodeTest, DbConnectionError) {
    ASSERT_EQ(ErrorCode::DB_CONNECTION_ERROR, 1401);
}

TEST(ErrorCodeTest, DbQueryError) {
    ASSERT_EQ(ErrorCode::DB_QUERY_ERROR, 1402);
}

TEST(ErrorCodeTest, DbTransactionError) {
    ASSERT_EQ(ErrorCode::DB_TRANSACTION_ERROR, 1403);
}

TEST(ErrorCodeTest, DbConstraintError) {
    ASSERT_EQ(ErrorCode::DB_CONSTRAINT_ERROR, 1404);
}

TEST(ErrorCodeTest, DbForeignKeyError) {
    ASSERT_EQ(ErrorCode::DB_FOREIGN_KEY_ERROR, 1405);
}

TEST(ErrorCodeTest, TransactionBeginFailed) {
    ASSERT_EQ(ErrorCode::TRANSACTION_BEGIN_FAILED, 1501);
}

TEST(ErrorCodeTest, TransactionCommitFailed) {
    ASSERT_EQ(ErrorCode::TRANSACTION_COMMIT_FAILED, 1502);
}

TEST(ErrorCodeTest, TransactionRollbackFailed) {
    ASSERT_EQ(ErrorCode::TRANSACTION_ROLLBACK_FAILED, 1503);
}

TEST(ErrorCodeTest, TransactionNestedError) {
    ASSERT_EQ(ErrorCode::TRANSACTION_NESTED_ERROR, 1504);
}

TEST(ErrorCodeTest, ConstraintViolation) {
    ASSERT_EQ(ErrorCode::CONSTRAINT_VIOLATION, 1601);
}