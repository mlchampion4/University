#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include <iostream>
#include "member.h"
#include "admin.h"
#include "faculty.h"

Administrator::Administrator(std::string_view fullName, std::weak_ptr<Faculty> faculty, std::string_view position, int managedPeople)
    : UniversityMember(fullName, faculty), _position(position), _managedPeople(managedPeople) {}

std::string Administrator::getPosition() const {
    return _position;
}

int Administrator::getManagedPeople() const {
    return _managedPeople;
}

void Administrator::setPosition(std::string_view newPosition) {
    _position = newPosition;
}

void Administrator::setManagedPeople(int newManagedPeople) {
    _managedPeople = newManagedPeople;
}

std::string Administrator::getType() const {
    return "Administrator";
}

void Administrator::printInformation(std::ostream& os) const {
    os << "ФИО Администратора: " << _fullName << "\n"
    << "Факультет: " << _faculty.lock()->getFacultyName() << "\n"
    << "Должность: " << _position << "\n"
    << "Количество подчинённых: " << _managedPeople << "\n";
}

double Administrator::calculateMetric() const {
    return _managedPeople * 1.5;
}

void Administrator::applyEffect(int value) {
    if (value < 0 || value > 5) {
        std::cout << "Недопустимое количество новых подчинённых, повторите попытку\n";
        return;
    }

    _managedPeople += value;
    std::cout << "Администратору " << _fullName << " успешно добавлены подчинённые " << value << '\n';
}