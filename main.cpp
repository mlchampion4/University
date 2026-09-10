// main.cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <locale>
#include "faculty.h"
#include "department.h"
#include "student.h"
#include "teacher.h"
#include "subject.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "========== ТЕСТ 1: Создание факультета ==========" << std::endl;
    auto faculty = std::make_shared<Faculty>("ФИТиП", 100);
    std::cout << "Факультет: " << faculty->getFacultyName() << std::endl;
    std::cout << "Макс. студентов: " << faculty->getMaxStudentsCount() << std::endl;

    std::cout << "\n========== ТЕСТ 2: Создание кафедр ==========" << std::endl;
    auto dept1 = std::make_shared<Department>("Кафедра ВТ", faculty);
    auto dept2 = std::make_shared<Department>("Кафедра ИВТ", faculty);
    faculty->addDepartment(dept1);
    faculty->addDepartment(dept2);
    std::cout << "Кафедры добавлены: " << dept1->getDepartmentName()
              << ", " << dept2->getDepartmentName() << std::endl;

    std::cout << "\n========== ТЕСТ 3: Создание предметов ==========" << std::endl;
    auto subj1 = std::make_shared<Subject>("Программирование", 4, EXAM);
    auto subj2 = std::make_shared<Subject>("Базы данных", 3, GRADEDCREDIT);
    auto subj3 = std::make_shared<Subject>("Философия", 2, CREDIT);
    subj1->printSubjectInformation();
    std::cout << "---" << std::endl;
    subj2->printSubjectInformation();

    std::cout << "\n========== ТЕСТ 4: Создание преподавателей ==========" << std::endl;
    auto teacher1 = std::make_shared<Teacher>("Иванов Иван Иванович", faculty, dept1, subj1);
    auto teacher2 = std::make_shared<Teacher>("Петров Пётр Петрович", faculty, dept1, subj2);
    auto teacher3 = std::make_shared<Teacher>("Сидоров Сидор Сидорович", faculty, dept2, subj3);
    dept1->addTeacher(teacher1);
    dept1->addTeacher(teacher2);
    dept2->addTeacher(teacher3);

    teacher1->printTeacherInformation();
    std::cout << "---" << std::endl;
    teacher2->printTeacherInformation();

    std::cout << "\n========== ТЕСТ 5: Создание студентов ==========" << std::endl;
    auto s1 = std::make_shared<Student>("Алексеев Алексей Алексеевич", "ИУ7-11Б", faculty, 40);
    auto s2 = std::make_shared<Student>("Борисов Борис Борисович", "ИУ7-11Б", faculty, 40);
    auto s3 = std::make_shared<Student>("Викторов Виктор Викторович", "ИУ7-11Б", faculty, 40);
    faculty->addStudent(s1);
    faculty->addStudent(s2);
    faculty->addStudent(s3);

    s1->printStudentInformation();

    std::cout << "\n========== ТЕСТ 6: Печать информации о факультете ==========" << std::endl;
    faculty->printFacultyInformafion();

    std::cout << "\n========== ТЕСТ 7: Добавление лекции ==========" << std::endl;
    std::vector<std::shared_ptr<Student>> group = {s1, s2, s3};
    faculty->addLecture(group, dept1, "Иванов Иван Иванович");
    std::cout << "---" << std::endl;
    faculty->addLecture(group, dept1, "Петров Пётр Петрович");
    std::cout << "---" << std::endl;
    // Ошибка: нет такого преподавателя
    faculty->addLecture(group, dept1, "Несуществующий Преподаватель");

    std::cout << "\n========== ТЕСТ 8: Изменение данных ==========" << std::endl;
    faculty->setFacultyName("ПИиКТ");
    std::cout << "Новое имя факультета: " << faculty->getFacultyName() << std::endl;

    dept1->setDepartmentName("Кафедра ПМИ");
    std::cout << "Новое имя кафедры: " << dept1->getDepartmentName() << std::endl;

    subj1->setHours(5);
    std::cout << "Новое количество часов: " << subj1->getHours() << std::endl;

    s1->setGroupNumber("ИУ7-12Б");
    std::cout << "Новая группа студента: " << s1->getGroupNumber() << std::endl;

    std::cout << "\n========== ТЕСТ 9: Печать кафедры ==========" << std::endl;
    dept1->printDepartmentInformation();
    
    std::cout << "\nВсе тесты завершены." << std::endl;
    return 0;
}