#include <string>
#include <algorithm>
#include <vector>
#include <string_view>
#include <memory>
#include <iostream>
#include "department.h"
#include "student.h"
#include "faculty.h"
#include "teacher.h"
#include "subject.h"
#include "member.h"
#include "exceptions.h"

Faculty::Faculty(std::string_view facultyName, unsigned int maxStudentsCount)
    : _facultyName(facultyName), _maxStudentsCount(maxStudentsCount) {
        if (facultyName.empty()) throw InvalidDataException("Название факультета не может быть пустым");
        if (maxStudentsCount == 0) throw InvalidDataException("Максимум студентов должен быть больше нуля");
    }

std::string_view Faculty::getFacultyName() const {
    return _facultyName;
}

unsigned int Faculty::getMaxStudentsCount() const {
    return _maxStudentsCount;
}

void Faculty::setFacultyName(std::string_view newFacultyName) {
    if (newFacultyName.empty()) throw InvalidDataException("Название факультета не может быть пустым");
    _facultyName = newFacultyName;
}

void Faculty::setMaxStudentsCount(unsigned int newMaxStudentsCount) {
    if (newMaxStudentsCount == 0) throw InvalidDataException("Максимум студентов должен быть больше нуля");
    if (newMaxStudentsCount < _students.size()) throw LimitExceededException("Новый лимит (" +
            std::to_string(newMaxStudentsCount) +
            ") меньше текущего числа студентов (" +
            std::to_string(_students.size()) + ")");
    _maxStudentsCount = newMaxStudentsCount;
}

int findTeacher(std::shared_ptr<Department> department, std::string_view teacherName) {
    if (!department) return -1;

    const auto& staff = department->getTeachingStaff();

    for (int i = 0; i < int(staff.size()); i++) {
        if (staff[i]->getFullName() == teacherName) {
            return i;
        }
    }
    
    return -1;
}

void Faculty::addLecture(std::vector<std::shared_ptr<Student>> group, std::shared_ptr<Department> department, std::string_view teacherName) {
    if (group.empty()) throw InvalidDataException("Группа студентов пуста");
    if (!department) throw InvalidDataException("Кафедра не может быть пустой");

    int teacherIdx = findTeacher(department, teacherName);

    if (teacherIdx < 0) {
        throw ObjectNotFoundException("Преподаватель " + std::string(teacherName) +
            " не найден на кафедре");
    }

    auto teacher = department->getTeachingStaff()[teacherIdx];
    auto subject = teacher->getSubject();
    if (!subject) {
        throw ObjectNotFoundException("У преподавателя нет предмета");
    }

    if (group[0]->getHours() < subject->getHours()) {
        throw LimitExceededException("У группы недостаточно свободных часов: " +
            std::to_string(group[0]->getHours()) + " < " +
            std::to_string(subject->getHours()));
    }
    for (auto& s : group) {
        s->setHours(s->getHours() - subject->getHours());
    }
    std::cout << "Занятие по предмету " << subject->getSubjectName()
              << " с преподавателем " << teacher->getFullName()
              << " успешно установлено. Свободных часов: "
              << group[0]->getHours() << std::endl;
}

void Faculty::printFacultyInformafion() const {
    std::cout << "Название факультета: " << _facultyName << std::endl;
    std::cout << "Список кафедр: " << std::endl;
    for (int i = 0; i < int(_departments.size()); i++) {
        std::cout << _departments[i]->getDepartmentName() << std::endl;
    }
    std::cout << "Максимальное число студентов: " << _maxStudentsCount << std::endl;
    std::cout << "Список студентов: " << std::endl;
    for (int i = 0; i < int(_students.size()); i++) {
        std::cout << _students[i]->getFullName() << ", номер студ. билета: " << _students[i]->getStudentNumber() << " группа " << _students[i]->getGroupNumber() << std::endl;
    }
}

std::shared_ptr<Faculty> Faculty::operator+=(const std::shared_ptr<Student> student) {
    if (!student) throw InvalidDataException("Нельзя добавить пустого студента");
    if (int(_students.size()) + 1 > int(_maxStudentsCount)) {
        throw LimitExceededException("Факультет полон: " +
            std::to_string(_students.size()) + "/" +
            std::to_string(_maxStudentsCount));
    }
    for (const auto& s : _students) {
        if (s && s->getStudentNumber() == student->getStudentNumber()) throw DuplicateIdException("Студент с номером " +
                student->getStudentNumber() + " уже зачислен");
    }
    _students.push_back(student);
    return student->getFaculty().lock();
}

std::shared_ptr<Faculty> Faculty::operator-=(const std::shared_ptr<Student> student) {
    if (!student) {
        throw InvalidDataException("Нельзя удалить путого студента");
    }
    auto it = std::remove_if(_students.begin(), _students.end(),
        [&student](const std::shared_ptr<Student>& s) {
            return s && *s == *student;
        });
    if (it == _students.end()) {
        throw ObjectNotFoundException("Студент " + student->getFullName() +
            " не найден на факультете");
    }
    _students.erase(it, _students.end());
    return student->getFaculty().lock();
}

std::shared_ptr<Faculty> Faculty::operator+=(const std::shared_ptr<Department> department) {
    if (!department) {
        throw InvalidDataException("Нельзя добавить пустую кафедру");
    }
    for (const auto& d : _departments) {
        if (d && d->getDepartmentName() == department->getDepartmentName()) {
            throw DuplicateIdException("Кафедра с названием " +
                std::string(department->getDepartmentName()) + " уже существует");
        }
    }
    _departments.push_back(department);
    return department->getFaculty();
}

std::shared_ptr<Faculty> Faculty::operator-=(const std::shared_ptr<Department> department) {
    if (!department) {
        throw InvalidDataException("Нельзя удалить пустую кафедру");
    }
    if (!department->getTeachingStaff().empty()) {
        throw RelationException("Нельзя удалить кафедру " +
            std::string(department->getDepartmentName()) +
            ", к ней привязаны преподаватели");
    }
    auto it = std::find(_departments.begin(), _departments.end(), department);
    if (it == _departments.end()) {
        throw ObjectNotFoundException("Кафедра " +
            std::string(department->getDepartmentName()) + " не найдена");
    }
    _departments.erase(it);
    return department->getFaculty();
}