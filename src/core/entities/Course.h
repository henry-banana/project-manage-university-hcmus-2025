#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
private:
    std::string _id; // Ví dụ: "CE142"
    std::string _name; // Ví dụ: "OOPC"
    int _credits;

protected:

public:
    Course(std::string id, std::string name, int credits);

    const std::string& getId() const { return _id; }
    const std::string& getName() const { return _name; }
    int getCredits() const { return _credits; }
};

#endif // COURSE_H