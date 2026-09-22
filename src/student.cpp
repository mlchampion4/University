#include <string>
#include <iostream>
#include <vector>
#include <string_view>
#include <memory>
#include "faculty.h"
#include "student.h"
#include "member.h"

Student::Student(std::string_view fullName, std::weak_ptr<Faculty> faculty, std::string_view studentNumber, std::string_view groupNumber, unsigned int maxHoursPerWeek)
    : UniversityMember(fullName, faculty), _studentNumber(studentNumber), _groupNumber(groupNumber), _maxHoursPerWeek(maxHoursPerWeek), _hours(maxHoursPerWeek) {}

std::string Student::getStudentNumber() const {
    return _studentNumber;
}

unsigned int Student::getHours() const {
    return _hours;
}

std::string_view Student::getGroupNumber() const {
    return _groupNumber;
}

unsigned int Student::getMaxHoursPerWeek() const {
    return _maxHoursPerWeek;
}

std::vector<unsigned int> Student::getMarks() const {
    return _marks;
}

void Student::setGroupNumber(std::string_view newGroupNumber) {
    _groupNumber = newGroupNumber;
}

void Student::setStudentNumber(std::string_view newStudentNumber) {
    _studentNumber = newStudentNumber;
}

void Student::setHours(unsigned int newHours) {
    _hours = newHours;
}

void Student::setMaxHoursPerWeek(unsigned int newMaxHoursPerWeek) {
    _maxHoursPerWeek = newMaxHoursPerWeek;
}

void Student::setMarks(std::vector<unsigned int> newMarks) {
    _marks = newMarks;
}

std::string Student::getType() const {
    return "Student";
}

void Student::printInformation(std::ostream& os) const {
    os << "ФИО Студента: " << _fullName << "\n" << "Номер студенческого билета: "
     << _studentNumber << "\n" << "Факультет: " << _faculty.lock()->getFacultyName() << "\n"
     << "Группа: " << _groupNumber << "\n" << "Максимум часов в неделю: "
     << _maxHoursPerWeek << std::endl;
     os << "Оценки: \n";
     for (const auto& s: _marks)
        os << s << " ";
}

double Student::calculateMetric() const {
    if (_marks.empty()) return 0;
    unsigned int sum = 0;
    for (unsigned int m : _marks) sum += m;
    return static_cast<double>(sum) / _marks.size();
}

void Student::applyEffect(int value) {
    if (value < 0 || value > 10) {
        std::cout << "Введенное число не является оценкой, повторите попытку\n";
        return;
    }

    _marks.push_back(static_cast<unsigned int>(value));
    std::cout << "Cтуденту " << _fullName << " успешно выставлена оценка " << value << '\n';
}

void Student::readFrom(std::istream& is) {
    std::cout << "Введите полное имя студента: ";
    std::getline(is >> std::ws, _fullName);
    std::cout << "Введите номер студ. билета: ";
    std::getline(is, _studentNumber);
    std::cout << "Введите номер группы: ";
    std::getline(is, _groupNumber);
    std::cout << "Введите максимальное число часов: ";
    is >> _maxHoursPerWeek;
}

bool Student::equals(const UniversityMember& other) const {
    auto* s = dynamic_cast<const Student*>(&other);
    if (!s) return false;

    return _studentNumber == s->_studentNumber;
}

std::string Student::getMetricName() const {
    return "Средний балл";
}