#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include <iostream>
#include "member.h"
#include "admin.h"
#include "faculty.h"

Administrator::Administrator(int id, std::string_view fullName, std::weak_ptr<Faculty> faculty, std::string_view position, int managedPeople)
    : UniversityMember(id, fullName, faculty), _position(position), _managedPeople(managedPeople) {}

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
    if (value < 0 || value > 10) {
        std::cout << "Недопустимое количество новых подчинённых, повторите попытку\n";
        return;
    }

    _managedPeople += value;
    std::cout << "Администратору " << _fullName << " успешно добавлены подчинённые " << value << '\n';
}

void Administrator::readFrom(std::istream& is) {
    std::cout << "Введите полное имя администратора: ";
    std::getline(is >> std::ws, _fullName);
    std::cout << "Введите должность: ";
    std::getline(is, _position);
    std::cout << "Кодичество подчинённых: ";
    is >> _managedPeople;
}

bool Administrator::equals(const UniversityMember& other) const {
    auto* a = dynamic_cast<const Administrator*>(&other);
    if (!a) return false;

    return _id == a->_id;
}

std::string Administrator::getMetricName() const {
    return "Влиятельность";
}