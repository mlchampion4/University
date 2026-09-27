#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include <iostream>
#include "member.h"
#include "admin.h"
#include "faculty.h"
#include "exceptions.h"

Administrator::Administrator(int id, std::string_view fullName, std::weak_ptr<Faculty> faculty, std::string_view position, int managedPeople)
    : UniversityMember(id, fullName, faculty), _position(position), _managedPeople(managedPeople) {
        if (position.empty()) throw InvalidDataException("Должность не может быть пустой");
        if (managedPeople < 0) throw InvalidDataException("Количество подчинённых не может быть отрицательным");
    }

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
    if (newManagedPeople < 0) throw InvalidDataException("Количество подчинённых не может быть отрицательным");
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
    if (value < 0 || value > 10) throw InvalidDataException("Недопустимое количество новых подчинённых: " +
            std::to_string(value));

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