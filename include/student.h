#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>

class Faculty;

class Student {
    public:
        Student(std::string_view fullName, std::string_view groupNumber, std::weak_ptr<Faculty> faculty, unsigned int maxHoursPerWeek, unsigned int hours);

        std::string_view getFullName() const;
        std::string_view getGroupNumber() const;
        std::weak_ptr<Faculty> getFaculty() const;
        unsigned int getHours() const;
        unsigned int getMaxHoursPerWeek() const;

        void setGroupNumber(std::string_view newGroupNumber);
        void setHours(unsigned int newHours);
        void setFaculty(std::weak_ptr<Faculty> newFaculty);
        void setMaxHoursPerWeek(unsigned int newMaxHoursPerWeek);

        void printStudentInformation() const;
        
    private:
        std::string _fullName;
        std::string _groupNumber;
        std::weak_ptr<Faculty> _faculty;
        unsigned int _maxHoursPerWeek;
        unsigned int _hours;
};