classDiagram
%% ===== Entity Layer =====
class User {
  -id: string
  -name: string
  -username: string
  -password: string
}


class Student {
  -studentId: string
  -gpa: float
  -facultyId: string
}


class Teacher {
  -teacherId: string
  -salary: double
  -facultyId: string
}


class AdminUser


class Faculty {
  -facultyId: string
  -name: string
}


class Course {
  -courseId: string
  -courseName: string
  -credit: int
}


class CourseResult {
  -studentId: string
  -courseId: string
  -score: float
}


class Enrollment {
  -studentId: string
  -courseId: string
}


class FeeRecord {
  -studentId: string
  -amount: double
  -date: string
}


class SalaryRecord {
  -teacherId: string
  -amount: double
  -month: string
}


%% ===== Inheritance =====
Student --|> User
Teacher --|> User
AdminUser --|> User


%% ===== Service Layer =====
class StudentService {
  +registerCourse()
  +viewFee()
  +viewResults()
}


class TeacherService {
  +submitGrade()
  +viewSalary()
}


class CourseService {
  +createCourse()
  +assignTeacher()
}


class ResultService {
  +updateScore()
  +calculateGPA()
}


class FinanceService {
  +getFeeRecords()
  +getSalaryRecords()
}


%% ===== DAO Interfaces =====
class IStudentDao
class ITeacherDao
class ICourseDao
class IResultDao
class IFeeRecordDao
class ISalaryRecordDao


%% ===== DAO Implementations (not shown in full) =====
class SqlStudentDao
class SqlTeacherDao
class SqlCourseDao
class SqlResultDao
class SqlFeeRecordDao
class SqlSalaryRecordDao


SqlStudentDao --|> IStudentDao
SqlTeacherDao --|> ITeacherDao
SqlCourseDao --|> ICourseDao
SqlResultDao --|> IResultDao
SqlFeeRecordDao --|> IFeeRecordDao
SqlSalaryRecordDao --|> ISalaryRecordDao


%% ===== Associations =====
Faculty "1" --> "many" Student
Faculty "1" --> "many" Teacher
Course "1" --> "many" CourseResult
Student "1" --> "many" FeeRecord
Student "1" --> "many" Enrollment
Course "1" --> "many" Enrollment
Teacher "1" --> "many" SalaryRecord


%% ===== Layer Connections (Service uses DAO) =====
StudentService --> IStudentDao
StudentService --> IFeeRecordDao
StudentService --> IResultDao
TeacherService --> ISalaryRecordDao
TeacherService --> IResultDao
CourseService --> ICourseDao
CourseService --> ITeacherDao
ResultService --> IResultDao
FinanceService --> IFeeRecordDao
FinanceService --> ISalaryRecordDao




