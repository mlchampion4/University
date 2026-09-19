#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "department.h"
#include "subject.h"
#include "teacher.h"

Teacher::Teacher(std::string_view fullName, std::weak_ptr<Faculty> faculty, std::weak_ptr<Department> department, std::shared_ptr<Subject> subject, int maxTeachingLoad)
    : UniversityMember(fullName, faculty), _department(department), _subject(subject), _maxTeachingLoad(maxTeachingLoad) {}

std::shared_ptr<Department> Teacher::getDepartment() const {
    return _department.lock();
}

std::shared_ptr<Subject> Teacher::getSubject() const {
    return _subject;
}

void Teacher::setDepartment(std::weak_ptr<Department> newDepartment) {
    _department = newDepartment;
}

void Teacher::setSubject(std::shared_ptr<Subject> newSubject) {
    _subject = newSubject;
}

int Teacher::getTeachingLoad() const {
    return _teachingLoad;
}

std::string Teacher::getType() const {
    return "Teacher";
}

void Teacher::printInformation(std::ostream& os) const {
    os << "ФИО Преподавателя: " << _fullName << "\n"
    << "Факультет: " << _faculty.lock()->getFacultyName() << "\n"
    << "Предмет: " << _subject->getSubjectName() << "\n"
    << "Учебная нагрузка: " << _teachingLoad << "\n";
}

double Teacher::calculateMetric() const {
    return 4 * _teachingLoad;
}

void Teacher::applyEffect(int value) {
    if (value < 0) {
        std::cout << "Нельзя уменьшать нагрузку!\n";
        return;
    }
    if (_teachingLoad + value > _maxTeachingLoad) {
        std::cout << "Поставить новые часы нельзя! Нагрузка слишком большая\n";
        return;
    } else {
        _teachingLoad += value;
        std::cout << "Преподавателю " << _fullName << " успешно добавлены часы " << value << '\n';
    }
}