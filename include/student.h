#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>

class Faculty;

class Student {
    public:
        Student(std::string_view fullName,
                std::string_view studentNumber,
                std::string_view groupNumber,
                std::weak_ptr<Faculty> faculty,
                unsigned int maxHoursPerWeek);

        Student() {}

        std::string_view getFullName() const;
        std::string getStudentNumber() const;
        std::string_view getGroupNumber() const;
        std::weak_ptr<Faculty> getFaculty() const;
        unsigned int getHours() const;
        unsigned int getMaxHoursPerWeek() const;
        std::vector<unsigned int> getMarks() const;

        void setGroupNumber(std::string_view newGroupNumber);
        void setStudentNumber(std::string_view newStudentNumber);
        void setHours(unsigned int newHours);
        void setFaculty(std::weak_ptr<Faculty> newFaculty);
        void setMaxHoursPerWeek(unsigned int newMaxHoursPerWeek);
        void setMarks(std::vector<unsigned int> newMarks);

        unsigned int calculateAvgMark(std::vector<unsigned int> marks) const;

        friend std::ostream& operator<<(std::ostream& os, const Student& student);
        friend std::istream& operator>>(std::istream& is, Student& student);

        bool operator==(const Student otherStudent) const;
        bool operator!=(const Student otherStudent) const;
        bool operator>(const Student otherStudent) const;
        bool operator<(const Student otherStudent) const;
        bool operator>=(const Student otherStudent) const;
        bool operator<=(const Student otherStudent) const;
        
    private:
        std::string _fullName;
        std::string _studentNumber;
        std::string _groupNumber;
        std::weak_ptr<Faculty> _faculty;
        unsigned int _maxHoursPerWeek;
        unsigned int _hours;
        std::vector<unsigned int> _marks;
};