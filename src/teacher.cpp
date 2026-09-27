#include <string>
#include <vector>
#include <iostream>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "department.h"
#include "subject.h"
#include "teacher.h"
#include "exceptions.h"

Teacher::Teacher(int id, std::string_view fullName, std::weak_ptr<Faculty> faculty, std::weak_ptr<Department> department, std::shared_ptr<Subject> subject, int maxTeachingLoad)
    : UniversityMember(id, fullName, faculty), _department(department), _subject(subject), _maxTeachingLoad(maxTeachingLoad) {
        if (!subject) throw InvalidDataException("Преподаватель не может быть без предмета");
        if (maxTeachingLoad <= 0) throw InvalidDataException("Макс.нагрузка не должна быть отрицательной");
    }

std::shared_ptr<Department> Teacher::getDepartment() const {
    return _department.lock();
}

std::shared_ptr<Subject> Teacher::getSubject() const {
    return _subject;
}

void Teacher::setDepartment(std::weak_ptr<Department> newDepartment) {
    if (newDepartment.expired() && !newDepartment.lock()) throw ObjectNotFoundException("Нельзя привязать преподавателя к несуществующей кафедре");
    _department = newDepartment;
}

void Teacher::setSubject(std::shared_ptr<Subject> newSubject) {
    if (!newSubject) throw InvalidDataException("Нельзя поставить несуществующий предмет");
    _subject = newSubject;
}

int Teacher::getTeachingLoad() const {
    return _teachingLoad;
}

int Teacher::getMaxTeachingLoad() const {
    return _maxTeachingLoad;
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
    if (value < 0) throw InvalidDataException("Нельзя уменьшать нагрузку (значение " +
            std::to_string(value) + ")");

    if (_teachingLoad + value > _maxTeachingLoad) throw LimitExceededException("Нагрузка " +
            std::to_string(_teachingLoad + value) +
            " превышает максимум " + std::to_string(_maxTeachingLoad));
            
    _teachingLoad += value;
    std::cout << "Преподавателю " << _fullName << " успешно добавлены часы " << value << '\n';
}

void Teacher::readFrom(std::istream& is) {
    std::cout << "Введите полное имя преподавателя: ";
    std::getline(is >> std::ws, _fullName);
    std::cout << "Введите макс. нагрузку: ";
    is >> _maxTeachingLoad;
}

bool Teacher::equals(const UniversityMember& other) const {
    auto* t = dynamic_cast<const Teacher*>(&other);
    if (!t) return false;

    return _id == t->_id;
}

std::string Teacher::getMetricName() const {
    return "Метрика нагрузки";
}