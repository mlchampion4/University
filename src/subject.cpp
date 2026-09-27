#include <string>
#include <iostream>
#include <vector>
#include <string_view>
#include "subject.h"
#include "exceptions.h"

Subject::Subject(std::string_view subjectName, unsigned int hours, ControlType controlType)
    : _subjectName(subjectName), _hours(hours), _controlType(controlType) {
        if (subjectName.empty()) throw InvalidDataException("Название предмета не может быть пустым");
        if (hours == 0) throw InvalidDataException("Количество часов должно быть больше нуля");
    }

std::ostream& operator<<(std::ostream& os, ControlType ct) {
    switch(ct) {
        case EXAM: return os << "Экзамен";
        case CREDIT: return os << "Зачёт";
        case GRADEDCREDIT: return os << "Дифф. зачёт";
        default: return os << "Ошибка";
    }
}

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
    if (newHours == 0) throw InvalidDataException("Количество часов не может быть нулевым");
    _hours = newHours;
}

void Subject::setControlType(ControlType newControlType) {
    if (newControlType != CREDIT && newControlType != GRADEDCREDIT && newControlType != EXAM)
        throw InvalidOperationException("Недопустимый тип контроля");
    _controlType = newControlType;
}

void Subject::printSubjectInformation() const {
    std::cout << "Предмет: " << _subjectName << std::endl;
    std::cout << "Количество часов в неделю: " << _hours << std::endl;
    std::cout << "Форма контроля: " << _controlType << std::endl;
}