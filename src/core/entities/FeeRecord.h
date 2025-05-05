#ifndef FEE_RECORD_H
#define FEE_RECORD_H

#include <string>

class FeeRecord {
private:
    std::string _studentId;
    int _totalFee;
    int _paidFee; // Số tiền đã thanh toán

protected:

public:
    FeeRecord(); // Constructor mặc định
    FeeRecord(std::string studentId, int totalFee, int paidFee); 
    FeeRecord(const FeeRecord& other); // Copy constructor
    FeeRecord& operator=(const FeeRecord& other); // Toán tử gán sao chép
    ~FeeRecord() = default; // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa

    const std::string& studentId() const;
    int totalFee() const;
    int paidFee() const; // Số tiền đã thanh toán;
    bool isFullyPaid() const; // Kiểm tra xem đã thanh toán đủ chưa

    void setStudentId(const std::string& studentId);
    void setTotalFee(int totalFee);
    void setPaidFee(int paidFee); // Cập nhật số tiền đã thanh toán

    bool makePayment(int amount); // Thanh toán một khoản tiền
    void display() const; // Hiển thị thông tin hóa đơn
};

#endif // FEE_RECORD_H