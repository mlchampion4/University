#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <string_view>

enum ControlType {
    EXAM,
    CREDIT,
    GRADEDCREDIT
};

class Subject {
    public:
        Subject(std::string_view subjectName, unsigned int hours, ControlType controlType);

        std::string_view getSubjectName() const;
        unsigned int getHours() const;
        ControlType getControlType() const;

        void setHours(unsigned int newHours);
        void setControlType(ControlType newControlType);

        void printSubjectInformation() const;

    private:
        std::string _subjectName;
        unsigned int _hours;
        ControlType _controlType;
};