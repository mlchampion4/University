#pragma once
#include <stdexcept>
#include <string>

class UniversityException : public std::runtime_error {
public:
    explicit UniversityException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidDataException : public UniversityException {
public:
    explicit InvalidDataException(const std::string& message)
        : UniversityException("Некорректные данные: " + message) {}
};

class ObjectNotFoundException : public UniversityException {
public:
    explicit ObjectNotFoundException(const std::string& message)
        : UniversityException("Объект не найден: " + message) {}
};

class DuplicateIdException : public UniversityException {
public:
    explicit DuplicateIdException(const std::string& message)
        : UniversityException("Дублирование ID: " + message) {}
};

class LimitExceededException : public UniversityException {
public:
    explicit LimitExceededException(const std::string& message)
        : UniversityException("Превышено ограничение: " + message) {}
};

class InvalidOperationException : public UniversityException {
public:
    explicit InvalidOperationException(const std::string& message)
        : UniversityException("Недопустимая операция: " + message) {}
};

class OutOfRangeException : public UniversityException {
public:
    explicit OutOfRangeException(const std::string& message)
        : UniversityException("Выход за пределы диапазона: " + message) {}
};

class RelationException : public UniversityException {
public:
    explicit RelationException(const std::string& message)
        : UniversityException("Нарушение связи: " + message) {}
};