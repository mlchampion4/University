#include <string>
#include <iostream>
#include <vector>
#include <string_view>
#include "subject.h"

Subject::Subject(std::string_view subjectName, unsigned int hours, ControlType controlType)
    : _subjectName(subjectName), _hours(hours), _controlType(controlType) {}

std::string_view Subject::getSubjectName() const {
    return _subjectName;
}

unsigned int Subject::getHours() const {
    return _hours;
}

ControlType Subject::getControlType() const {
    return _controlType;
}

void Subject::setHours(unsigned int newHours) {
    _hours = newHours;
}

void Subject::setControlType(ControlType newControlType) {
    _controlType = newControlType;
}

void Subject::printSubjectInformation() const {
    std::cout << "Предмет: " << _subjectName << std::endl;
    std::cout << "Количество часов в неделю: " << _hours << std::endl;
    std::cout << "Форма контроля: " << _controlType << std::endl;
}