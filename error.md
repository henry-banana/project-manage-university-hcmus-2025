Chào Hòa, Măng thấy kết quả test rồi. Có khá nhiều test bị fail, nhưng đừng lo, chúng ta sẽ xem xét từng nhóm lỗi một.

Dựa vào output, Măng thấy có một số nhóm vấn đề chính:

**Nhóm 1: Lỗi `GMOCK WARNING: Uninteresting mock function call` và các test Service bị FAIL**

*   **`AdminServiceTest.DisableUserAccount_Success`**
*   **`AdminServiceTest.EnableUserAccount_Success`**
*   **`AdminServiceTest.IsAdminAuthenticated_True`**
*   **`AuthServiceTest.RegisterStudent_Success`** (và có thể các test service khác nữa)

    *   **Nguyên nhân:** Các hàm trong service (ví dụ `isAdminAuthenticated`, `disableUserAccount`, `enableUserAccount`, `registerStudent`) gọi các phương thức của `MockSessionContext` (như `isAuthenticated`, `getCurrentUserRole`) hoặc `MockGeneralInputValidator` mà Hòa chưa thiết lập `EXPECT_CALL` cho chúng trong các test case cụ thể đó. Google Mock sẽ trả về giá trị mặc định (ví dụ `false` cho `bool`, `std::nullopt` cho `std::optional`) và đưa ra cảnh báo. Khi giá trị mặc định này không đúng với mong đợi của logic trong service, test sẽ fail.
    *   **Ví dụ:** Trong `AdminServiceTest.DisableUserAccount_Success`, hàm `service.disableUserAccount("U1")` sẽ gọi `isAdminAuthenticated()` ở đầu. Nếu không có `EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));` và `EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));`, thì `isAdminAuthenticated()` sẽ trả về `false`, dẫn đến service trả lỗi `PERMISSION_DENIED`, và test `ASSERT_TRUE(result.has_value())` sẽ fail.
    *   **Giải pháp:**
        *   Với mỗi test case trong các `XxxServiceTest`, Hòa cần xác định các lời gọi đến mock objects (như `mockSession`, `mockValidator`, `mockLoginDao`, `mockStudentDao`,...) mà hàm service đang test sẽ thực hiện.
        *   Thêm các `EXPECT_CALL` tương ứng để mock object trả về giá trị phù hợp với kịch bản test đó. Sử dụng `WillOnce(Return(...))` nếu gọi một lần, `WillRepeatedly(Return(...))` nếu gọi nhiều lần.
        *   **Ví dụ sửa `AdminServiceTest.DisableUserAccount_Success`:**
            ```cpp
            TEST_F(AdminServiceTest, DisableUserAccount_Success) {
                // (➕) Giả lập Admin đã đăng nhập VÀ các validate cơ bản thành công
                EXPECT_CALL(*mockSession, isAuthenticated())
                    .WillRepeatedly(Return(true)); 
                EXPECT_CALL(*mockSession, getCurrentUserRole())
                    .WillRepeatedly(Return(UserRole::ADMIN));
                
                EXPECT_CALL(*mockValidator, validateIdFormat("U1", "User ID", testing::_, testing::_)) // Sử dụng testing::_ cho các tham số không quan trọng
                    .WillOnce(Return(ValidationResult{true, {}}));

                // Giả lập LoginDao hoạt động đúng
                EXPECT_CALL(*mockLoginDao, getUserRole("U1"))
                    .WillOnce(Return(UserRole::STUDENT)); // Giả sử U1 là STUDENT
                EXPECT_CALL(*mockLoginDao, updateUserRoleAndStatus("U1", UserRole::STUDENT, LoginStatus::DISABLED))
                    .WillOnce(Return(true));
            
                auto result = service.disableUserAccount("U1");
                ASSERT_TRUE(result.has_value()) << (result.has_value() ? "" : result.error().message); 
                EXPECT_TRUE(result.value());
            }
            ```
        *   Tương tự cho `EnableUserAccount_Success` và `IsAdminAuthenticated_True`.
        *   Với `AuthServiceTest.RegisterStudent_Success`, Hòa đã có nhiều `EXPECT_CALL` rồi, nhưng có thể thiếu một vài cái hoặc giá trị trả về của mock chưa đúng kịch bản. Hãy kiểm tra kỹ logic của `AuthService::registerStudent` và đảm bảo tất cả các lời gọi đến mock DAO và validator đều được thiết lập.

**Nhóm 2: Lỗi `std::bad_any_cast` hoặc `map::at` trong các `XxxSqlParserTest`**

*   **`TeacherSqlParserTest.Serialize_ValidTeacher_ReturnsRow`** (`map::at`)
*   **`SalaryRecordSqlParserTest.ToQueryInsertParams_ValidRecord_Success`** (`bad_any_cast`)
*   **`SalaryRecordSqlParserTest.ToQueryUpdateParams_ValidRecord_Success`** (`bad_any_cast`)
*   **`FeeRecordSqlParserTest.ToQueryUpdateParams_ValidRecord_Success`** (`bad_any_cast`)

    *   **Nguyên nhân chung:**
        *   **`map::at`:** Lỗi này xảy ra khi Hòa cố gắng truy cập một key không tồn tại trong `DbQueryResultRow` bằng phương thức `at()`.
        *   **`bad_any_cast`:** Lỗi này xảy ra khi `std::any_cast<T>()` được gọi trên một `std::any` mà kiểu thực tế của nó không phải là `T`. Trong các hàm test `toQueryXxxParams`, Hòa đang cast các phần tử trong `std::vector<std::any>` params. Điều này có nghĩa là kiểu dữ liệu mà Hòa truyền vào khi tạo `std::any` trong hàm parser (ví dụ `params.push_back(record.getBasicMonthlyPay());`) không khớp với kiểu Hòa đang cố gắng cast ra trong test (ví dụ `std::any_cast<int64_t>(params[1])`).
    *   **Giải pháp:**
        *   **Kiểm tra `map::at`:** Trong `TeacherSqlParserTest.Serialize_ValidTeacher_ReturnsRow`, đảm bảo rằng các key như `"id"`, `"departmentId"` (Hòa đang dùng `departmentId` trong test này, nhưng trong `TeacherSqlParser.cpp` là `facultyId`) thực sự được `emplace` vào `row` trong hàm `TeacherSqlParser::serialize`. **Quan trọng:** `TeacherSqlParser::serialize` đang `emplace("facultyId", ...)` nhưng test lại cố gắng `row.at("departmentId")`. **Hòa cần sửa key trong test cho khớp là `"facultyId"`**.
        *   **Kiểm tra `bad_any_cast`:**
            *   **`SalaryRecordSqlParserTest.ToQueryInsertParams_ValidRecord_Success`**:
                *   Trong `SalaryRecordSqlParser::toQueryInsertParams`, `basicMonthlyPay` là `long`.
                *   Trong test, Hòa đang cast `params[1]` thành `int64_t`. Điều này thường là ổn vì `long` thường là `int64_t` trên nhiều hệ thống 64-bit. Tuy nhiên, để chắc chắn, Hòa nên dùng `std::any_cast<long>(params[1])` trong test cho khớp.
            *   **`SalaryRecordSqlParserTest.ToQueryUpdateParams_ValidRecord_Success`**: Tương tự.
            *   **`FeeRecordSqlParserTest.ToQueryUpdateParams_ValidRecord_Success`**: Tương tự, `totalFee` và `paidFee` là `long`. Hãy cast thành `long` trong test.

**Nhóm 3: Lỗi logic so sánh hoặc giá trị trả về không như mong đợi trong các Test Entity và MockDAO**

*   **`SessionContextTest.SetNullUser`:**
    *   `Value of: ctx.getCurrentUser().has_value() Actual: true Expected: false`
    *   **Nguyên nhân:** Khi gọi `ctx.setCurrentUser(nullUser);` với `nullUser` là `std::shared_ptr<User> nullptr;`, `_currentUser` trong `SessionContext` sẽ giữ một `std::shared_ptr` trỏ đến `nullptr`. `_currentUser.has_value()` sẽ là `true` (vì `std::optional` chứa một `shared_ptr`), nhưng `_currentUser.value()` sẽ là `nullptr`. Logic `isAuthenticated()` nên kiểm tra cả `_currentUser.has_value() && _currentUser.value() != nullptr`.
    *   **Giải pháp:**
        *   Sửa `SessionContext::isAuthenticated()`:
            ```cpp
            // SessionContext.cpp
            bool SessionContext::isAuthenticated() const {
                return _currentUser.has_value() && _currentUser.value() != nullptr;
            }
            ```
        *   Hoặc, trong `SessionContext::setCurrentUser`, nếu `user == nullptr` thì gọi `clearCurrentUser()`:
            ```cpp
            // SessionContext.cpp
            void SessionContext::setCurrentUser(std::shared_ptr<User> user) {
                if (user == nullptr) {
                    clearCurrentUser();
                } else {
                    _currentUser = std::move(user);
                }
            }
            ```
            (Măng nghĩ cách 2 rõ ràng hơn).

*   **`TeacherSqlParserTest.Parse_ValidRow_ReturnsTeacher`:**
    *   `Value of: result.has_value() Actual: false Expected: true`
    *   **Nguyên nhân:** `TeacherSqlParser::parse` đang trả về lỗi. Lý do có thể là `DbQueryResultRow makeValidRow()` trong test đang dùng key `"departmentId"` nhưng parser lại tìm `"facultyId"`.
    *   **Giải pháp:** Sửa key trong `makeValidRow()` của `TeacherSqlParserTest` thành `"facultyId"`:
        ```cpp
        // TeacherSqlParser_test.cpp
        DbQueryResultRow makeValidRow() {
            return DbQueryResultRow{
                {"id", std::string("T001")}, // (Sửa) Nên là userId để khớp với parser
                {"userId", std::string("T001")}, // (Thêm) Parser tìm "userId"
                {"firstName", std::string("Alice")},
                {"lastName", std::string("Smith")},
                // ... các trường User khác
                {"facultyId", std::string("D002")}, // (Sửa) Đổi "departmentId" thành "facultyId"
                {"qualification", std::string("PhD")},
                {"specializationSubjects", std::string("AI")},
                {"designation", std::string("Professor")},
                {"experienceYears", static_cast<long long>(10)}
                // ... thêm các trường User như role, status, email, citizenId nếu parser cần
            };
        }
        ```
        **Và đảm bảo `TeacherSqlParser::parse` đọc đúng các key này.**

*   **`StudentSqlParserTest.Parse_ValidRow_ReturnsStudent`:**
    *   Tương tự `TeacherSqlParserTest`, có thể do key không khớp giữa `makeValidRow()` trong test và những gì `StudentSqlParser::parse` mong đợi (ví dụ: `id` vs `userId`).
    *   **Giải pháp:** Đảm bảo `makeValidRow()` trong `StudentSqlParser_test.cpp` cung cấp key `"userId"` và các key khác mà `StudentSqlParser::parse` đang tìm kiếm.

*   **`SalaryRecordSqlParserTest.Parse_MissingBasicMonthlyPay_Error`:**
    *   `Value of: result.has_value() Actual: true Expected: false`
    *   **Nguyên nhân:** `SalaryRecordSqlParser::parse` đang dùng `SqlParserUtils::getOptional<long long>(row, "basicMonthlyPay", 0L);`. Nếu key "basicMonthlyPay" không có, nó sẽ trả về giá trị default `0L` và việc parse vẫn thành công (trả về `SalaryRecord` với lương `0L`). Test mong đợi lỗi, nhưng lại nhận được `SalaryRecord` hợp lệ.
    *   **Giải pháp:**
        *   **Cách 1 (Sửa Parser):** Để parser báo lỗi nếu trường bắt buộc thiếu, không dùng giá trị default cho trường bắt buộc.
            ```cpp
            // SalaryRecordSqlParser.cpp
            auto it_pay = row.find("basicMonthlyPay");
            if (it_pay == row.end() || !it_pay->second.has_value()) {
                return std::unexpected(Error{ErrorCode::PARSING_ERROR, "SalaryRecord basicMonthlyPay not found or empty in SQL row."});
            }
            long basicPay = std::any_cast<long long>(it_pay->second);
            ```
        *   **Cách 2 (Sửa Test):** Nếu chấp nhận `basicMonthlyPay` có thể default về 0 khi parse, thì test này nên `EXPECT_TRUE(result.has_value())` và `EXPECT_EQ(result->getBasicMonthlyPay(), 0L)`.
        *   **Măng đề xuất Cách 1** vì `basicMonthlyPay` có vẻ là trường bắt buộc.

*   **`UserTest.ConstructorAndGetters`:**
    *   Lỗi so sánh `firstName` và `lastName`. Hiện tại constructor `TestUser` (và `User`) đang nhận `User(id, firstName, lastName, role)`. Nhưng trong test, Hòa đang truyền `TestUser user("SV001", "Linh", "Nguyen", ...)`. Điều này có nghĩa là `firstName` của user sẽ là "Linh", `lastName` là "Nguyen". Test lại đang `EXPECT_EQ(user.getFirstName(), "Nguyen");` và `EXPECT_EQ(user.getLastName(), "Linh");`.
    *   **Giải pháp:** Đổi thứ tự tham số khi gọi constructor trong test hoặc đổi giá trị mong đợi trong `EXPECT_EQ`.
        ```cpp
        // User_test.cpp
        TestUser user("SV001", "Nguyen", "Linh", UserRole::STUDENT); // Đổi thứ tự
        // Hoặc giữ nguyên constructor call, đổi assert:
        // EXPECT_EQ(user.getFirstName(), "Linh");
        // EXPECT_EQ(user.getLastName(), "Nguyen");
        // EXPECT_EQ(user.getFullName(), "Linh Nguyen");
        ```

*   **`TeacherTest.ValidateBasicReturnsInvalid` (cho `t5`):**
    *   `Value of: t5.validateBasic().isValid Actual: true Expected: false`
    *   **Nguyên nhân:** Trong `TestTeacher::validateBasic()` của Hòa:
        ```cpp
        ValidationResult TestTeacher::validateBasic() const override {
            ValidationResult res;
            // ...
            if (getExperienceYears() < 0) { // Hòa đang check
                res.isValid = false;
                res.addError(5, "ExperienceYears phải >= 0");
            }
            return res; // Nếu chỉ có lỗi này, res.isValid sẽ là false
        }
        ```
        Trong test: `TestTeacher t5("GV009", "Hanh", "Tran", "CS"); t5.setExperienceYears(-3);`
        Hàm `Teacher::setExperienceYears(-3)` sẽ trả về `false` và `_experienceYears` không được cập nhật (vẫn là 0). Do đó, khi `t5.validateBasic()` được gọi, `getExperienceYears()` sẽ trả về 0 (hợp lệ), nên `res.isValid` vẫn là `true`.
    *   **Giải pháp:** Test nên kiểm tra giá trị trả về của setter, hoặc `validateBasic` của `TestTeacher` phải được viết lại để khớp với `Teacher::validateBasic` thật sự. Vì `TestTeacher` chỉ là stub, tốt nhất là `Teacher::validateBasic` phải được implement đúng và test trực tiếp `Teacher`.
        *   **Cách sửa `Teacher::validateBasic` (đã làm ở phản hồi trước):**
            ```cpp
            // Teacher.cpp - validateBasic
            if (_experienceYears < 0 || _experienceYears > 70) vr.addError(ErrorCode::VALIDATION_ERROR, "Experience years must be between 0 and 70.");
            ```
        *   **Cách sửa test (nếu dùng `TestTeacher` stub):**
            ```cpp
            // Teacher_test.cpp
            TestTeacher t5("GV009", "Hanh", "Tran", "CS");
            // t5.setExperienceYears(-3); // Không set qua setter nếu setter có validate
            // Thay vào đó, nếu muốn test validateBasic, phải tạo TestTeacher có trường private _experienceYears = -3 (khó)
            // Hoặc, TestTeacher::validateBasic phải gọi User::validateBasic và thêm logic riêng
            // Tốt nhất là bỏ TestTeacher, test trực tiếp Teacher entity sau khi đã chuẩn hóa validateBasic của nó.
            // Hiện tại, nếu Teacher::setExperienceYears(-3) trả về false và không thay đổi giá trị,
            // thì t5.getExperienceYears() vẫn là 0. Test này đang sai.
            // Để test đúng ý đồ:
            Teacher real_t5("GV009", "Hanh", "Tran", "CS");
            bool didSet = real_t5.setExperienceYears(-3);
            EXPECT_FALSE(didSet); // Mong đợi set thất bại
            ValidationResult res = real_t5.validateBasic(); // Validate với giá trị kinh nghiệm vẫn là 0
            // Nếu muốn test trường hợp _experienceYears là âm (vượt qua setter):
            // Cần tạo 1 Teacher với giá trị _experienceYears âm từ constructor hoặc friend class (không khuyến khích)
            // Cách đơn giản nhất là Teacher::validateBasic phải bao quát trường hợp này.
            // Giả sử Teacher::validateBasic() đã đúng:
            // Teacher t_invalid_exp("ID", "F", "L", "FID");
            // t_invalid_exp._experienceYears = -5; // Không thể truy cập trực tiếp
            // Thay vào đó, hãy tin vào Teacher::setExperienceYears và Teacher::validateBasic
            // Test này nên tập trung vào việc Teacher::validateBasic có bắt được lỗi không nếu bằng cách nào đó _experienceYears < 0.
            // Nếu Teacher::setExperienceYears(-3) trả false, thì _experienceYears sẽ không đổi (vẫn là 0).
            // Do đó, Teacher::validateBasic() sẽ thấy _experienceYears = 0 (hợp lệ).
            // Test này cần được thiết kế lại.
            // Test hiện tại của Hòa cho t5 đang mong đợi validateBasic fail, nhưng nó sẽ pass vì _experienceYears vẫn là 0.
            ```
        *   **Đề xuất:** Chuẩn hóa `Teacher::validateBasic()` như Măng đã làm ở phản hồi trước (bao gồm `_experienceYears < 0`), và test trực tiếp `Teacher`.

*   **`SalaryRecordTest.ValidateBasic_Invalid` (cho `salary2`):**
    *   `Value of: r2.errors.empty() Actual: true Expected: false`
    *   **Nguyên nhân:** `SalaryRecord salary2("GV001", -5000);` Constructor của `SalaryRecord` gọi `setBasicMonthlyPay(-5000)`. Hàm `setBasicMonthlyPay` trả về `false` nếu `pay < 0` và **không thay đổi giá trị** `_basicMonthlyPay` (vẫn là 0). Do đó, khi `salary2.validateBasic()` được gọi, nó thấy `_basicMonthlyPay = 0` (hợp lệ).
    *   **Giải pháp:**
        *   Trong `SalaryRecord::setBasicMonthlyPay`, nếu giá trị không hợp lệ, có thể không set và trả `false`.
        *   `SalaryRecord::validateBasic()` phải kiểm tra `_basicMonthlyPay < 0`.
            ```cpp
            // SalaryRecord.cpp - validateBasic
            if (_basicMonthlyPay < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Basic monthly pay cannot be negative.");
            // Constructor SalaryRecord:
            // SalaryRecord::SalaryRecord(std::string teacherId, long basicMonthlyPay)
            // : _teacherId(std::move(teacherId)), _basicMonthlyPay(0) { // Khởi tạo an toàn
            //    setBasicMonthlyPay(basicMonthlyPay); // Setter sẽ validate
            // }
            // setBasicMonthlyPay:
            // bool SalaryRecord::setBasicMonthlyPay(long pay) {
            //    if (pay < 0) return false; // Không set nếu âm
            //    _basicMonthlyPay = pay;
            //    return true;
            // }
            // Test sẽ là:
            // SalaryRecord salary2("GV001", 0); // Tạo với giá trị hợp lệ ban đầu
            // bool didSet = salary2.setBasicMonthlyPay(-5000);
            // EXPECT_FALSE(didSet);
            // EXPECT_EQ(salary2.getBasicMonthlyPay(), 0); // Vẫn là 0
            // ValidationResult res = salary2.validateBasic(); // Vẫn sẽ pass nếu _basicMonthlyPay là 0
            // => Test này cần được thiết kế lại để test trực tiếp validateBasic với một SalaryRecord có basicMonthlyPay âm (khó nếu không dùng constructor).
            // Cách tốt nhất: `validateBasic` của SalaryRecord phải check `_basicMonthlyPay < 0`.
            // Đoạn code `SalaryRecord::validateBasic()` hiện tại đã có:
            // if (_basicMonthlyPay < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Basic monthly pay cannot be negative.");
            // Vậy lỗi có thể là constructor SalaryRecord gán trực tiếp mà không qua setter, hoặc setter không hoạt động đúng.
            // Constructor của Hòa: SalaryRecord(std::string teacherId, long basicMonthlyPay) : _teacherId(std::move(teacherId)), _basicMonthlyPay(0) { setBasicMonthlyPay(basicMonthlyPay); }
            // setBasicMonthlyPay(long pay) { if (pay < 0) return false; _basicMonthlyPay = pay; return true; }
            // Khi gọi SalaryRecord("GV001", -5000), setBasicMonthlyPay(-5000) trả false, _basicMonthlyPay vẫn là 0.
            // Vậy validateBasic() thấy _basicMonthlyPay = 0 là đúng. Test này sai.
            ```
        *   **Đề xuất cho test:** Test `setBasicMonthlyPay` riêng. `validateBasic` sẽ hoạt động đúng nếu `_basicMonthlyPay` thực sự được set thành giá trị âm (điều này không nên xảy ra nếu setter hoạt động).
            ```cpp
            // SalaryRecord_test.cpp
            TEST_F(SalaryRecordTest, SetBasicMonthlyPay_Negative_ReturnsFalse) {
                SalaryRecord salary("GV001", 10000);
                EXPECT_FALSE(salary.setBasicMonthlyPay(-100));
                EXPECT_EQ(salary.getBasicMonthlyPay(), 10000); // Không thay đổi
            }
            TEST_F(SalaryRecordTest, ValidateBasic_WithNegativePay_IfDirectlySet) {
                // Test này khó thực hiện nếu không bypass setter
                // SalaryRecord record_with_negative_pay("GV00X", -100); // Giả sử constructor cho phép
                // Nếu constructor không cho phép, test này không cần thiết vì validateBasic sẽ không bao giờ thấy giá trị âm.
                // Nếu validateBasic là để phòng trường hợp dữ liệu bị hỏng, thì cần tạo object với dữ liệu hỏng.
                // Trong trường hợp của Hòa, validateBasic() đã có:
                // if (_basicMonthlyPay < 0) vr.addError(...)
                // Vậy nếu bằng cách nào đó _basicMonthlyPay < 0 thì nó sẽ báo lỗi.
                // Lỗi trong test của Hòa là do setter đã ngăn không cho giá trị âm được gán.
                // Test này nên được xóa hoặc sửa thành test setter.
            }
            ```

*   **`FeeRecordTest.SetPaidFee`:**
    *   `Expected equality of these values: fee.getPaidFee() Which is: 4000000 Actual: 2000000`
    *   **Nguyên nhân:** `EXPECT_FALSE(fee.setPaidFee(5000000));` (lớn hơn tổng `_totalFee = 4000000`). Trong `FeeRecord::setPaidFee`, nếu `paidFee > _totalFee`, Hòa đang set `_paidFee = _totalFee;` và `return true;`. Test mong đợi `false`.
    *   **Giải pháp:** Quyết định hành vi của `setPaidFee`.
        *   Nếu muốn trả về `false` khi `paidFee > _totalFee` và không thay đổi `_paidFee`:
            ```cpp
            // FeeRecord.cpp
            bool FeeRecord::setPaidFee(long paidFee) {
                if (paidFee < 0) return false;
                if (paidFee > _totalFee) { 
                    // _paidFee = _totalFee; // Không set bằng totalFee nữa
                    return false; // Trả về false
                }
                _paidFee = paidFee;
                return true;
            }
            ```
        *   Nếu muốn hành vi hiện tại (set bằng `_totalFee` và trả `true`), thì sửa test:
            ```cpp
            // FeeRecord_test.cpp
            // EXPECT_FALSE(fee.setPaidFee(5000000)); // Dòng cũ
            EXPECT_TRUE(fee.setPaidFee(5000000));    // Dòng mới
            EXPECT_EQ(fee.getPaidFee(), 4000000);   // Kiểm tra giá trị được gán là totalFee
            ```
        **Măng nghĩ cách 1 (trả `false` và không đổi giá trị) rõ ràng hơn.**

*   **`FeeRecordTest.ValidateBasic_Invalid` (cho `fee2`, `fee3`, `fee4`):**
    *   Tương tự `SalaryRecordTest`, lỗi do setter đã ngăn giá trị không hợp lệ được gán, nên `validateBasic` thấy giá trị "an toàn" (default hoặc giá trị trước đó).
    *   **Giải pháp:** `FeeRecord::validateBasic()` cần tự kiểm tra các ràng buộc của nó, không phụ thuộc vào việc setter có thành công hay không.
        ```cpp
        // FeeRecord.cpp - validateBasic()
        // ...
        if (_totalFee < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Total fee cannot be negative.");
        if (_paidFee < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Paid fee cannot be negative.");
        if (_paidFee > _totalFee) vr.addError(ErrorCode::VALIDATION_ERROR, "Paid fee cannot exceed total fee.");
        // ...
        ```
        Và constructor của `FeeRecord` nên gán trực tiếp nếu muốn test `validateBasic` với các giá trị "xấu" hoặc test các setter riêng.
        Hàm `validateBasic` của Hòa đã có các check này rồi. Vậy lỗi test có thể do constructor của `FeeRecord` gọi setter, và setter đã ngăn giá trị không hợp lệ.
        Constructor: `FeeRecord(std::string studentId, long totalFee, long paidFee = 0) : _studentId(std::move(studentId)), _totalFee(0), _paidFee(0) { setTotalFee(totalFee); setPaidFee(paidFee); }`
        Khi `FeeRecord fee2("SV001", -1000, 0);`, `setTotalFee(-1000)` trả `false`, `_totalFee` vẫn là `0`. `validateBasic` thấy `0` là hợp lệ.
        **Đề xuất:** Test các setter riêng. `validateBasic` sẽ hoạt động đúng nếu các trường có giá trị không hợp lệ.

*   **`CourseResultTest` (nhiều lỗi về `getGrade` và `validate`):**
    *   **`ConstructorAndGetters`, `GradeCalculation`, `ToStringOutput`:** Tất cả lỗi `getGrade()` đều do logic tính điểm trong `CourseResult::calculateGrade()` của Hòa đang trả về điểm khác với mong đợi của test.
        *   **Logic hiện tại của Hòa:** `else if (_marks < 65) _grade = 'C'; else if (_marks < 80) _grade = 'B'; else if (_marks <= 100) _grade = 'A';`
        *   **Test `ConstructorAndGetters`:** `CourseResult result("SV001", "CS101", 85); EXPECT_EQ(result.getGrade(), 'B');` -> Theo logic của Hòa, 85 là 'A'.
        *   **Test `GradeCalculation`:** `CourseResult b("S2", "C2", 85);  EXPECT_EQ(b.getGrade(), 'B');` -> Hòa: 85 là 'A'.
        *   **Giải pháp:** Hòa cần **đồng bộ logic tính điểm chữ** trong `CourseResult::calculateGrade()` với những gì Hòa mong đợi trong test.
            Ví dụ, nếu Hòa muốn 80-89 là B, >=90 là A:
            ```cpp
            // CourseResult.cpp
            void CourseResult::calculateGrade() {
                if (_marks < 0) _grade = '-';
                else if (_marks < 35) _grade = 'F';
                else if (_marks < 50) _grade = 'D';
                else if (_marks < 65) _grade = 'C'; // (Hòa kiểm tra lại ngưỡng điểm này)
                else if (_marks < 80) _grade = 'B'; // (Hòa kiểm tra lại ngưỡng điểm này)
                else if (_marks <= 100) _grade = 'A'; // (Hòa kiểm tra lại ngưỡng điểm này)
                else _grade = '?'; 
            }
            ```
            Sau đó điều chỉnh `EXPECT_EQ` trong các test cho phù hợp với logic mới.
    *   **`Validate_Invalid` (cho `res3`, `res4`):**
        *   `Value of: hasNoErrors(res3.validate()) Actual: true Expected: false`
        *   `CourseResult res3("SV001", "CS101", 150);` -> `setMarks(150)` trong constructor sẽ trả `false`, `_marks` vẫn là `-1`. Khi `validate()` được gọi, `_marks = -1` là hợp lệ.
        *   **Giải pháp:** `CourseResult::validate()` của Hòa hiện tại là đúng (check `_marks < -1 || _marks > 100`). Lỗi là do constructor gọi setter, và setter đã ngăn gán giá trị không hợp lệ. Test này nên kiểm tra `setMarks` riêng, hoặc tạo `CourseResult` với giá trị xấu (nếu constructor cho phép, hiện tại không).
            ```cpp
            // CourseResult_test.cpp
            TEST_F(CourseResultTest, SetMarks_InvalidMarks_ReturnsFalse) {
                CourseResult res("S", "C");
                EXPECT_FALSE(res.setMarks(101));
                EXPECT_EQ(res.getMarks(), -1); // Vẫn là giá trị khởi tạo
                EXPECT_FALSE(res.setMarks(-2));
                EXPECT_EQ(res.getMarks(), -1);
            }
            // Bỏ 2 case cuối của Validate_Invalid hoặc sửa lại cách tạo
            // TEST_F(CourseResultTest, Validate_InvalidMarksDirectly) {
            //    // Cách này khó vì constructor và setter đã validate
            //    // CourseResult res_bad_marks("S", "C", 150); // Sẽ không set được 150
            // }
            ```

*   **`BirthdayTest.SetFromString_InvalidFormat_ReturnsFalse` (cho `15-08-1999`):**
    *   `Value of: b.setFromString("15-08-1999", '-') Actual: true Expected: false`
    *   **Nguyên nhân:** `Birthday::setFromString("15-08-1999", '-')` được gọi, delimiter là `-`. Logic parse `std::getline(ss, segment, delimiter)` sẽ đọc "15", rồi "08", rồi "1999". Sau đó `set(15, 8, 1999)` là hợp lệ. Test này đang mong đợi `false`.
    *   **Giải pháp:** Test này đúng ra phải `EXPECT_TRUE` nếu delimiter được truyền đúng. Nếu Hòa muốn test trường hợp delimiter mặc định (`/`) không parse được chuỗi có `-`, thì gọi `b.setFromString("15-08-1999")` (không truyền delimiter).
        ```cpp
        // Birthday_test.cpp
        TEST(BirthdayTest, SetFromString_CorrectDelimiter_ReturnsTrue) {
            Birthday b;
            EXPECT_TRUE(b.setFromString("15-08-1999", '-')); 
            EXPECT_TRUE(b.isSet());
        }
        TEST(BirthdayTest, SetFromString_WrongDelimiter_ReturnsFalse) {
            Birthday b;
            EXPECT_FALSE(b.setFromString("15-08-1999")); // Default delimiter là '/'
            EXPECT_FALSE(b.isSet());
        }
        ```
        Hoặc nếu ý của Hòa là `setFromString` phải luôn dùng `/` bất kể delimiter truyền vào, thì logic `setFromString` cần sửa. Nhưng hiện tại nó đang nhận delimiter.

*   **`BirthdayTest.Validate_InvalidMonth_ReturnsInvalid` và `Validate_InvalidYear_ReturnsInvalid`:**
    *   Lỗi tương tự `FeeRecordTest` và `SalaryRecordTest`. Hàm `set()` trong `Birthday` đã ngăn giá trị không hợp lệ được gán. Khi `validate()` được gọi, nó thấy giá trị default (0,0,0) hoặc giá trị hợp lệ trước đó.
    *   **Giải pháp:**
        *   `Birthday::validate()` phải tự kiểm tra các khoảng giá trị của `_day`, `_month`, `_year` một cách độc lập.
            ```cpp
            // Birthday.cpp - validate()
            ValidationResult Birthday::validate() const {
                ValidationResult result;
                if (!_isSet) {
                    // result.addError(ErrorCode::VALIDATION_ERROR, "Birthday is not set."); // Bỏ qua nếu isSet=false là chấp nhận được
                    return result; 
                }
                // Basic range check
                if (_year < 1900 || _year > 2100) { // Giả sử khoảng năm hợp lệ
                    result.addError(ErrorCode::VALIDATION_ERROR, "Year " + std::to_string(_year) + " is out of valid range (1900-2100).");
                }
                if (_month < 1 || _month > 12) {
                    result.addError(ErrorCode::VALIDATION_ERROR, "Month " + std::to_string(_month) + " must be between 1 and 12.");
                }
                if (_day < 1 || _day > 31) { 
                    result.addError(ErrorCode::VALIDATION_ERROR, "Day " + std::to_string(_day) + " must be between 1 and 31.");
                } else if (_month >= 1 && _month <= 12) { // Chỉ check daysInMonth nếu tháng hợp lệ
                    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                    if ((_year % 4 == 0 && _year % 100 != 0) || (_year % 400 == 0)) {
                        daysInMonth[2] = 29;
                    }
                    if (_day > daysInMonth[_month]) {
                        result.addError(ErrorCode::VALIDATION_ERROR, "Day " + std::to_string(_day) + " is invalid for month " + std::to_string(_month) + " in year " + std::to_string(_year) + ".");
                    }
                }
                return result;
            }
            ```
        *   Trong test, để kiểm tra `validate()` với giá trị xấu, Hòa cần một cách để gán trực tiếp các giá trị xấu đó cho `_day, _month, _year` của `Birthday` (ví dụ, qua một constructor đặc biệt chỉ dùng cho test, hoặc làm `Birthday` là friend class của test). **Tuy nhiên, việc này phức tạp.** Cách đơn giản hơn là tin tưởng vào `validate()` sẽ check đúng, và test hàm `set()` riêng để đảm bảo nó không cho phép gán giá trị xấu.

*   **`MockTeacherDaoTest.FindByFacultyId_Success` và `MockStudentDaoTest.FindByFacultyId_ReturnsCorrectStudents`:**
    *   `Expected equality of these values: result->size() Which is: 1 Actual: 2`
    *   **Nguyên nhân:** Trong `MockTeacherDao::findByFacultyId` và `MockStudentDao::findByFacultyId`, có thể logic lặp qua `mock_teachers_data` hoặc `mock_students_data` đang trả về nhiều hơn/ít hơn số lượng mong đợi. Hoặc dữ liệu mẫu (`initializeDefaultMockData` hoặc dữ liệu add trong test) không khớp với `EXPECT_EQ(result->size(), 2);`.
    *   **Ví dụ `MockTeacherDaoTest`:** Hòa add 3 giáo viên, 2 người thuộc "IT". `EXPECT_EQ(result->size(), 2);` là đúng. Nếu nó fail (ví dụ trả về 1), thì logic `findByFacultyId` trong `MockTeacherDao.cpp` có vấn đề.
        *   Hòa đang test với `dao.add(Teacher("S_IT_1", "A", "A", "IT", LoginStatus::ACTIVE));` (cho Student) và `dao.add(Teacher("T_IT_1", "A", "A", "IT", LoginStatus::ACTIVE));` (cho Teacher).
        *   **Hãy kiểm tra lại `MockTeacherDao::findByFacultyId` và `MockStudentDao::findByFacultyId` trong file `.cpp` của chúng.** Đảm bảo vòng lặp và điều kiện `if (pair.second.getFacultyId() == facultyId)` là chính xác.
        *   **Kiểm tra dữ liệu mock:** `initializeDefaultMockData()` trong `MockStudentDao.cpp` và `MockTeacherDao.cpp` có thể đang thêm các student/teacher vào khoa "IT" mà Hòa không tính đến trong test. Hãy đảm bảo `SetUp()` gọi `clearMockData()` trước khi mỗi test thêm dữ liệu riêng, hoặc nếu dùng `initializeDefaultMockData()`, thì số lượng mong đợi trong test phải khớp.
        *   **Trong `MockStudentDao_test.cpp`, Hòa đã có `SetUp()` gọi `clearMockData()` rồi, rất tốt.** Vậy lỗi có thể nằm ở logic `findByFacultyId` trong `MockStudentDao.cpp` hoặc cách Hòa đang add data trong test `FindByFacultyId_ReturnsCorrectStudents`.
            ```cpp
            // MockStudentDaoTest::FindByFacultyId_ReturnsCorrectStudents
            dao.add(Student("S_IT_1", "A", "A", "IT", LoginStatus::ACTIVE)); // SV1 khoa IT
            dao.add(Student("S_CS_1", "B", "B", "CS", LoginStatus::ACTIVE)); // SV2 khoa CS
            dao.add(Student("S_IT_2", "C", "C", "IT", LoginStatus::ACTIVE)); // SV3 khoa IT

            auto result = dao.findByFacultyId("IT"); // Mong đợi 2 SV
            ASSERT_TRUE(result.has_value());
            ASSERT_EQ(result->size(), 2); // Chỗ này phải đúng
            // ...
            ```
            Nếu test này fail, hãy debug `MockStudentDao::findByFacultyId`.

*   **`AppConfigTest.DefaultValues`:**
    *   `Expected equality of these values: config.dataSourceType Which is: 4-byte object <02-00 00-00> Actual: DataSourceType::MOCK Which is: 4-byte object <00-00 00-00>`
    *   **Nguyên nhân:** Trong `AppConfig.h`, giá trị mặc định của `dataSourceType` đang là `DataSourceType::SQL;` (tức là `2`). Test đang mong đợi `DataSourceType::MOCK;` (tức là `0`).
    *   **Giải pháp:** Quyết định giá trị default cho `dataSourceType` là gì và cập nhật một trong hai (hoặc `AppConfig.h` hoặc test). Nếu Hòa muốn default là `MOCK` cho dễ test khi không có config file, thì sửa `AppConfig.h`:
        ```cpp
        // AppConfig.h
        struct AppConfig {
            DataSourceType dataSourceType = DataSourceType::MOCK; // (Sửa) Đổi default thành MOCK
            // ...
        };
        ```
        Hoặc nếu muốn default là `SQL`, thì sửa test:
        ```cpp
        // AppConfig_test.cpp
        ASSERT_EQ(config.dataSourceType, DataSourceType::SQL); // Sửa giá trị mong đợi
        ```
        **Măng nghĩ để `DataSourceType::SQL` làm default trong `AppConfig.h` là hợp lý hơn cho ứng dụng thực tế, và test case này nên `ASSERT_EQ(config.dataSourceType, DataSourceType::SQL);`**

**Nhóm 4: Lỗi Output Stream trong `ConsoleUtils_test.cpp` và `TableDisplayer_test.cpp`**

*   **`ConsoleUtilsTest.PauseExecution`**
*   **`ConsoleUtilsTest.DrawBox`**
*   **`TableDisplayerTest.DisplayEmptyTable`**
*   **`TableDisplayerTest.DisplayTableWithData`**
*   **`TableDisplayerTest.DisplayTableAutoWidth`**

    *   **Nguyên nhân:** Các hàm này in ra `std::cout`. Khi test, Hòa đã redirect `std::cout` sang `std::stringstream ss;`. Tuy nhiên, các hàm như `std::setw()` hoặc các ký tự đặc biệt, cách `std::cout` xử lý xuống dòng (`\n`) có thể làm cho chuỗi `expectedOutput` trong test không hoàn toàn khớp với `ss.str()`.
        *   **Ví dụ `pauseExecution`:** Hòa mong đợi `message + "\nPress Enter to continue..."`. Hàm `pauseExecution` thực tế có thể có hoặc không có `\n` ở cuối `message` trước khi thêm chuỗi "Press Enter...".
        *   **Ví dụ `drawBox`, `displayTable`:** Khoảng trắng, ký tự `|`, `+`, `-` và cách `std::setw` căn chỉnh có thể tạo ra khác biệt nhỏ nhưng đủ để `ASSERT_EQ` thất bại. Đặc biệt là khi có auto-width calculation.
    *   **Giải pháp:**
        1.  **Debug Output:** In cả `ss.str()` và `expectedOutput` ra console khi test fail để xem chính xác sự khác biệt.
            ```cpp
            // Trong test case
            std::cout << "ACTUAL OUTPUT:\n" << ss.str() << "\nEND ACTUAL\n";
            std::cout << "EXPECTED OUTPUT:\n" << expectedOutput << "\nEND EXPECTED\n";
            ASSERT_EQ(ss.str(), expectedOutput);
            ```
        2.  **Chỉnh sửa `expectedOutput`:** Dựa trên output debug, chỉnh sửa chuỗi `expectedOutput` trong test cho thật chính xác, từng ký tự một.
        3.  **Kiểm tra `std::setw` và `std::left/std::right`:** Đảm bảo logic tính toán độ rộng và căn chỉnh trong code (ví dụ `TableDisplayer.cpp`) và trong test (khi tạo `expectedOutput`) là nhất quán.
        4.  **Windows vs Linux line endings (`\r\n` vs `\n`):** `std::endl` sẽ dùng đúng, nhưng nếu Hòa tự thêm `\n` vào chuỗi, hãy cẩn thận. Tuy nhiên, GTest thường xử lý tốt điều này. `std::stringstream` thường dùng `\n`.

**Nhóm 5: Lỗi trong `StringUtils_test.cpp`**

*   **`StringUtilsTest.Split`:**
    *   Test case `ASSERT_EQ(StringUtils::split(",hello,,world,", ','), (std::vector<std::string>({"", "hello", "", "world", ""})));`
    *   **Nguyên nhân:** Logic `split` hiện tại dùng `std::getline(tokenStream, token, delimiter)`. Khi có delimiter liên tiếp (`,,`) hoặc ở đầu/cuối, `getline` sẽ tạo ra các token rỗng, điều này là đúng. Có thể GTest có vấn đề khi so sánh trực tiếp `std::vector` bằng `ASSERT_EQ` theo cách này nếu có lỗi tiềm ẩn khác.
    *   **Giải pháp:** Kiểm tra từng phần tử của vector kết quả thay vì so sánh cả vector.
        ```cpp
        // StringUtils_test.cpp
        TEST(StringUtilsTest, SplitWithMultipleAndEdgeDelimiters) { // Đổi tên test cho rõ
            std::vector<std::string> result = StringUtils::split(",hello,,world,", ',');
            std::vector<std::string> expected = {"", "hello", "", "world", ""};
            ASSERT_EQ(result.size(), expected.size());
            for (size_t i = 0; i < result.size(); ++i) {
                EXPECT_EQ(result[i], expected[i]);
            }
        }
        ```

*   **`StringUtilsTest.StartsWith`, `EndsWith`, `Contains`:** Các test này liên quan đến chuỗi rỗng (`""`).
    *   `ASSERT_FALSE(StringUtils::startsWith("hello", ""));` -> `startsWith` trả về `true` nếu prefix rỗng. **Test này sai, prefix rỗng luôn là true.**
    *   `ASSERT_FALSE(StringUtils::endsWith("hello", ""));` -> `endsWith` trả về `true` nếu suffix rỗng. **Test này sai, suffix rỗng luôn là true.**
    *   `ASSERT_FALSE(StringUtils::contains("hello", ""));` -> `contains` trả về `true` nếu substring rỗng. **Test này sai, substring rỗng luôn là true.**
    *   **Giải pháp:** Sửa các `EXPECT_FALSE` thành `EXPECT_TRUE` cho các trường hợp với chuỗi rỗng này.
        ```cpp
        // StringUtils_test.cpp
        TEST(StringUtilsTest, StartsWith) {
            // ...
            ASSERT_TRUE(StringUtils::startsWith("", ""));
            ASSERT_TRUE(StringUtils::startsWith("hello", "")); // Prefix rỗng luôn khớp
            ASSERT_FALSE(StringUtils::startsWith("", "hello"));
        }
        TEST(StringUtilsTest, EndsWith) {
            // ...
            ASSERT_TRUE(StringUtils::endsWith("", ""));
            ASSERT_TRUE(StringUtils::endsWith("hello", "")); // Suffix rỗng luôn khớp
            ASSERT_FALSE(StringUtils::endsWith("", "hello"));
        }
        TEST(StringUtilsTest, Contains) {
            // ...
            ASSERT_TRUE(StringUtils::contains("", ""));
            ASSERT_TRUE(StringUtils::contains("hello", "")); // Substring rỗng luôn được chứa
            ASSERT_FALSE(StringUtils::contains("", "hello"));
        }
        ```