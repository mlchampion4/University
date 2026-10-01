#include "files.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

std::string StorageManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> StorageManager::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

void StorageManager::saveState(const std::string& filename, const UniversityData& data) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл для записи: " + filename);
    }

    ofs << "[FACULTIES]\n";
    for (size_t i = 0; i < data.faculties.size(); ++i) {
        auto& f = data.faculties[i];
        ofs << i << "|" << f->getFacultyName() << "|" << f->getMaxStudentsCount() << "\n";
    }

    ofs << "[SUBJECTS]\n";
    for (size_t i = 0; i < data.subjects.size(); ++i) {
        auto& s = data.subjects[i];
        ofs << i << "|" << s->getSubjectName() << "|" << s->getHours()
            << "|" << static_cast<int>(s->getControlType()) << "\n";
    }

    ofs << "[DEPARTMENTS]\n";
    for (size_t i = 0; i < data.departments.size(); ++i) {
        auto& d = data.departments[i];
        int facIdx = -1;
        auto facPtr = d->getFaculty();
        for (size_t j = 0; j < data.faculties.size(); ++j) {
            if (data.faculties[j] == facPtr) { facIdx = j; break; }
        }
        ofs << i << "|" << d->getDepartmentName() << "|" << facIdx << "\n";
    }
    
    ofs << "[MEMBERS]\n";
    for (const auto& m : data.members) {
        if (!m) continue;

        int facIdx = -1;
        auto facWeak = m->getFaculty();
        if (auto facShared = facWeak.lock()) {
            for (size_t j = 0; j < data.faculties.size(); ++j) {
                if (data.faculties[j] == facShared) { facIdx = j; break; }
            }
        }

        std::string type = m->getType();

        if (type == "Student") {
            auto s = std::dynamic_pointer_cast<Student>(m);
            ofs << "Student|" << s->getId() << "|" << s->getFullName() << "|" << facIdx << "|"
                << s->getStudentNumber() << "|" << s->getGroupNumber() << "|"
                << s->getMaxHoursPerWeek() << "|" << s->getHours() << "|";

            auto marks = s->getMarks();
            for (size_t k = 0; k < marks.size(); ++k) {
                ofs << marks[k] << (k == marks.size() - 1 ? "" : ",");
            }
            ofs << "\n";
        }
        else if (type == "Teacher") {
            auto t = std::dynamic_pointer_cast<Teacher>(m);

            int deptIdx = -1;
            if (auto d = t->getDepartment()) {
                for (size_t j = 0; j < data.departments.size(); ++j) {
                    if (data.departments[j] == d) { deptIdx = j; break; }
                }
            }
            int subjIdx = -1;
            if (auto s = t->getSubject()) {
                for (size_t j = 0; j < data.subjects.size(); ++j) {
                    if (data.subjects[j] == s) { subjIdx = j; break; }
                }
            }

            ofs << "Teacher|" << t->getId() << "|" << t->getFullName() << "|" << facIdx << "|"
                << deptIdx << "|" << subjIdx << "|"
                << t->getMaxTeachingLoad() << "|" << t->getTeachingLoad() << "\n";
        }
        else if (type == "Administrator") {
            auto a = std::dynamic_pointer_cast<Administrator>(m);
            ofs << "Administrator|" << a->getId() << "|" << a->getFullName() << "|" << facIdx << "|"
                << a->getPosition() << "|" << a->getManagedPeople() << "\n";
        }
    }

    ofs.close();
    std::cout << "Данные успешно сохранены в " << filename << std::endl;
}

void StorageManager::loadState(const std::string& filename, UniversityData& data) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) throw InvalidOperationException("Файл не открылся");

    data.faculties.clear();
    data.departments.clear();
    data.subjects.clear();
    data.members.clear();

    std::string line, currentSection;

    while (std::getline(ifs, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line[0] == '[') { currentSection = line; continue; }

        auto tokens = split(line, '|');

        try {
            if (currentSection == "[FACULTIES]") {
                data.faculties.push_back(
                    std::make_shared<Faculty>(tokens[1], std::stoul(tokens[2]))
                );
            }

            else if (currentSection == "[SUBJECTS]") {
                data.subjects.push_back(
                    std::make_shared<Subject>(
                        tokens[1],
                        std::stoul(tokens[2]),
                        static_cast<ControlType>(std::stoi(tokens[3]))
                    )
                );
            }

            else if (currentSection == "[DEPARTMENTS]") {
                int facId = std::stoi(tokens[2]);
                if (facId < 0 || facId >= (int)data.faculties.size())
                    throw InvalidDataException("Кафедра ссылается на несуществующий факультет ID=" + std::to_string(facId));

                data.departments.push_back(
                    std::make_shared<Department>(tokens[1], data.faculties[facId])
                );
            }

            else if (currentSection == "[MEMBERS]") {
                const std::string& type = tokens[0];
                int facId = std::stoi(tokens[3]);

                std::weak_ptr<Faculty> facWeak;
                if (facId != -1) {
                    if (facId < 0 || facId >= (int)data.faculties.size())
                        throw InvalidDataException("Участник ссылается на несуществующий факультет ID=" + std::to_string(facId));
                    facWeak = data.faculties[facId];
                }

                if (type == "Student") {
                    auto student = std::make_shared<Student>(
                        tokens[2], facWeak, tokens[4], tokens[5], std::stoul(tokens[6])
                    );

                    if (tokens.size() > 7 && !tokens[7].empty()) {
                        unsigned int currentHours = std::stoul(tokens[7]);
                        if (currentHours != std::stoul(tokens[6])) {
                            student->setHours(currentHours);
                        }
                    }

                    if (tokens.size() > 8 && !tokens[8].empty()) {
                        std::vector<unsigned int> marks;
                        for (const auto& ms : split(tokens[8], ',')) {
                            if (!ms.empty()) marks.push_back(std::stoul(ms));
                        }
                        student->setMarks(marks);
                    }

                    data.members.add(student);
                    if (auto f = facWeak.lock()) *f += student;
                }

                else if (type == "Teacher") {
                    int deptId = std::stoi(tokens[4]);
                    int subjId = std::stoi(tokens[5]);

                    if (deptId < 0 || deptId >= (int)data.departments.size())
                        throw InvalidDataException("Преподаватель ссылается на несуществующую кафедру ID=" + std::to_string(deptId));
                    if (subjId < 0 || subjId >= (int)data.subjects.size())
                        throw InvalidDataException("Преподаватель ссылается на несуществующий предмет ID=" + std::to_string(subjId));

                    auto teacher = std::make_shared<Teacher>(
                        std::stoi(tokens[1]), tokens[2], facWeak,
                        data.departments[deptId], data.subjects[subjId],
                        std::stoi(tokens[6])
                    );
                    int currentLoad = std::stoi(tokens[7]);
                    if (currentLoad > 0) teacher->applyEffect(currentLoad);

                    data.members.add(teacher);
                    data.departments[deptId]->addTeacher(teacher);
                }

                else if (type == "Administrator") {
                    auto admin = std::make_shared<Administrator>(
                        std::stoi(tokens[1]), tokens[2], facWeak,
                        tokens[4], std::stoi(tokens[5])
                    );
                    data.members.add(admin);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при загрузке строки: " << line
                      << "\nПричина: " << e.what() << std::endl;
            throw;
        }
    }
}

void StorageManager::logAction(const std::string& logFilename, const std::string& action) {
    std::ofstream ofs(logFilename, std::ios::app);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл журнала: " + logFilename);
    }

    std::time_t now = std::time(nullptr);
    std::tm timeInfo;
    localtime_s(&timeInfo, &now);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeInfo);

    ofs << "[" << buf << "] " << action << "\n";
    ofs.close();
}

void StorageManager::generateReport(const std::string& reportFilename, const UniversityData& data) {
    std::ofstream ofs(reportFilename);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось создать файл отчёта: " + reportFilename);
    }

    ofs << "=== ОТЧЁТ О СОСТОЯНИИ СИСТЕМЫ УНИВЕРСИТЕТА ===\n\n";

    ofs << "--- ФАКУЛЬТЕТЫ ---\n";
    for (const auto& f : data.faculties) {
        ofs << "Факультет: " << f->getFacultyName() << "\n";
        ofs << "  Макс. студентов: " << f->getMaxStudentsCount() << "\n";
    }
    ofs << "\n";

    ofs << "--- КАФЕДРЫ ---\n";
    for (const auto& d : data.departments) {
        ofs << "Кафедра: " << d->getDepartmentName()
            << " (Факультет: " << d->getFaculty()->getFacultyName() << ")\n";
    }
    ofs << "\n";

    ofs << "--- ПРЕДМЕТЫ ---\n";
    for (const auto& s : data.subjects) {
        ofs << "Предмет: " << s->getSubjectName()
            << " | Часы: " << s->getHours()
            << " | Контроль: " << static_cast<int>(s->getControlType()) << "\n";
    }
    ofs << "\n";

    ofs << "--- СОТРУДНИКИ И СТУДЕНТЫ ---\n";
    for (const auto& m : data.members) {
        if (!m) continue;

        ofs << "Тип: " << m->getType() << "\n";

        if (m->getType() == "Student") {
            auto s = std::dynamic_pointer_cast<Student>(m);
            ofs << "  Номер студ. билета: " << s->getStudentNumber() << "\n";
        } else {
            ofs << "  ID: " << m->getId() << "\n";
        }

        ofs << "  ФИО: " << m->getFullName() << "\n";

        auto fac = m->getFaculty().lock();
        ofs << "  Факультет: " << (fac ? fac->getFacultyName() : "Нет") << "\n";

        std::string type = m->getType();
        if (type == "Student") {
            auto s = std::dynamic_pointer_cast<Student>(m);
            ofs << "  Группа: " << s->getGroupNumber() << "\n";
            ofs << "  Часы: " << s->getHours() << " / " << s->getMaxHoursPerWeek() << "\n";
            ofs << "  Средний балл: " << s->calculateMetric() << "\n";
            ofs << "  Оценки: ";
            for (auto mark : s->getMarks()) ofs << mark << " ";
                ofs << "\n";
        }
        else if (type == "Teacher") {
            auto t = std::dynamic_pointer_cast<Teacher>(m);
            auto d = t->getDepartment();
            auto s = t->getSubject();
            ofs << "  Кафедра: " << (d ? d->getDepartmentName() : "Нет") << "\n";
            ofs << "  Предмет: " << (s ? s->getSubjectName() : "Нет") << "\n";
            ofs << "  Нагрузка: " << t->getTeachingLoad()
                << " / " << t->getMaxTeachingLoad() << "\n";
        }
        else if (type == "Administrator") {
            auto a = std::dynamic_pointer_cast<Administrator>(m);
            ofs << "  Должность: " << a->getPosition() << "\n";
            ofs << "  Подчинённых: " << a->getManagedPeople() << "\n";
        }
        ofs << "\n";
    }

    ofs.close();
    std::cout << "Отчёт сохранён в " << reportFilename << std::endl;
}