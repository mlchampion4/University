#include "files.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

std::string StorageManager::trim(const std::string& str) {
    const size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) return "";
    const size_t last = str.find_last_not_of(" \t\n\r");
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

std::string StorageManager::join(const std::vector<unsigned int>& values, char sep) {
    std::ostringstream oss;
    for (size_t i = 0; i < values.size(); ++i) {
        oss << values[i];
        if (i + 1 < values.size()) oss << sep;
    }
    return oss.str();
}

template <typename T>
static int indexOfPtr(const std::vector<std::shared_ptr<T>>& vec,
                      const std::shared_ptr<T>& target)
{
    if (!target) return -1;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) return static_cast<int>(i);
    }
    return -1;
}

int StorageManager::parseIndexOrThrow(const std::string& token, int limit,
                                      const std::string& entity)
{
    int idx = std::stoi(token);
    if (idx < 0 || idx >= limit) {
        throw RelationException(entity + " ссылается на несуществующий ID=" + token);
    }
    return idx;
}

std::shared_ptr<Faculty> StorageManager::resolveFaculty(const UniversityData& data, int idx) {
    return data.faculties[parseIndexOrThrow(std::to_string(idx),
                                            (int)data.faculties.size(), "Факультет")];
}

std::weak_ptr<Faculty> StorageManager::resolveFacultyWeak(const UniversityData& data, int idx) {
    if (idx == -1) return {};
    return resolveFaculty(data, idx);
}

std::shared_ptr<Department> StorageManager::resolveDepartment(const UniversityData& data, int idx) {
    return data.departments[parseIndexOrThrow(std::to_string(idx),
                                              (int)data.departments.size(), "Кафедра")];
}

std::shared_ptr<Subject> StorageManager::resolveSubject(const UniversityData& data, int idx) {
    return data.subjects[parseIndexOrThrow(std::to_string(idx),
                                           (int)data.subjects.size(), "Предмет")];
}

void StorageManager::saveState(const std::string& filename, const UniversityData& data) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось открыть файл для записи: " + filename);
    }

    saveFaculties(ofs, data);
    saveSubjects(ofs, data);
    saveDepartments(ofs, data);
    saveMembers(ofs, data);

    ofs.close();
    std::cout << "Данные успешно сохранены в " << filename << std::endl;
}

void StorageManager::saveFaculties(std::ofstream& ofs, const UniversityData& data) {
    ofs << SEC_FACULTIES << "\n";
    for (size_t i = 0; i < data.faculties.size(); ++i) {
        const auto& f = data.faculties[i];
        ofs << i << "|" << f->getFacultyName() << "|" << f->getMaxStudentsCount() << "\n";
    }
}

void StorageManager::saveSubjects(std::ofstream& ofs, const UniversityData& data) {
    ofs << SEC_SUBJECTS << "\n";
    for (size_t i = 0; i < data.subjects.size(); ++i) {
        const auto& s = data.subjects[i];
        ofs << i << "|" << s->getSubjectName() << "|" << s->getHours()
            << "|" << static_cast<int>(s->getControlType()) << "\n";
    }
}

void StorageManager::saveDepartments(std::ofstream& ofs, const UniversityData& data) {
    ofs << SEC_DEPARTMENTS << "\n";
    for (size_t i = 0; i < data.departments.size(); ++i) {
        const auto& d = data.departments[i];
        const int facIdx = indexOfPtr(data.faculties, d->getFaculty());
        ofs << i << "|" << d->getDepartmentName() << "|" << facIdx << "\n";
    }
}

void StorageManager::saveMembers(std::ofstream& ofs, const UniversityData& data) {
    ofs << SEC_MEMBERS << "\n";
    for (const auto& m : data.members) {
        if (!m) continue;

        int facIdx = -1;
        if (auto facShared = m->getFaculty().lock()) {
            facIdx = indexOfPtr(data.faculties, facShared);
        }

        const std::string type = m->getType();

        if (type == TYPE_STUDENT) {
            saveStudent(ofs, std::dynamic_pointer_cast<Student>(m), facIdx);
        } else if (type == TYPE_TEACHER) {
            saveTeacher(ofs, std::dynamic_pointer_cast<Teacher>(m), facIdx, data);
        } else if (type == TYPE_ADMIN) {
            saveAdministrator(ofs, std::dynamic_pointer_cast<Administrator>(m), facIdx);
        }
    }
}

void StorageManager::saveStudent(std::ofstream& ofs, const std::shared_ptr<Student>& s, int facIdx) {
    ofs << TYPE_STUDENT << "|" << s->getId() << "|" << s->getFullName() << "|" << facIdx << "|"
        << s->getStudentNumber() << "|" << s->getGroupNumber() << "|"
        << s->getMaxHoursPerWeek() << "|" << s->getHours() << "|"
        << join(s->getMarks(), ',') << "\n";
}

void StorageManager::saveTeacher(std::ofstream& ofs, const std::shared_ptr<Teacher>& t,
                                 int facIdx, const UniversityData& data)
{
    int deptIdx = -1;
    if (auto d = t->getDepartment()) {
        deptIdx = indexOfPtr(data.departments, d);
    }
    int subjIdx = -1;
    if (auto s = t->getSubject()) {
        subjIdx = indexOfPtr(data.subjects, s);
    }

    ofs << TYPE_TEACHER << "|" << t->getId() << "|" << t->getFullName() << "|" << facIdx << "|"
        << deptIdx << "|" << subjIdx << "|"
        << t->getMaxTeachingLoad() << "|" << t->getTeachingLoad() << "\n";
}

void StorageManager::saveAdministrator(std::ofstream& ofs,
                                       const std::shared_ptr<Administrator>& a, int facIdx)
{
    ofs << TYPE_ADMIN << "|" << a->getId() << "|" << a->getFullName() << "|" << facIdx << "|"
        << a->getPosition() << "|" << a->getManagedPeople() << "\n";
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

        const auto tokens = split(line, '|');

        try {
            if      (currentSection == SEC_FACULTIES)   loadFacultyLine(data, tokens);
            else if (currentSection == SEC_SUBJECTS)    loadSubjectLine(data, tokens);
            else if (currentSection == SEC_DEPARTMENTS) loadDepartmentLine(data, tokens);
            else if (currentSection == SEC_MEMBERS)     loadMemberLine(data, tokens);
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при загрузке строки: " << line
                      << "\nПричина: " << e.what() << std::endl;
            throw;
        }
    }
}

void StorageManager::loadFacultyLine(UniversityData& data, const std::vector<std::string>& t) {
    data.faculties.push_back(std::make_shared<Faculty>(t[1], std::stoul(t[2])));
}

void StorageManager::loadSubjectLine(UniversityData& data, const std::vector<std::string>& t) {
    data.subjects.push_back(std::make_shared<Subject>(
        t[1], std::stoul(t[2]), static_cast<ControlType>(std::stoi(t[3]))
    ));
}

void StorageManager::loadDepartmentLine(UniversityData& data, const std::vector<std::string>& t) {
    auto fac = resolveFaculty(data, std::stoi(t[2]));
    data.departments.push_back(std::make_shared<Department>(t[1], fac));
}

void StorageManager::loadMemberLine(UniversityData& data, const std::vector<std::string>& t) {
    const std::string& type = t[0];
    if      (type == TYPE_STUDENT) loadStudent(data, t);
    else if (type == TYPE_TEACHER) loadTeacher(data, t);
    else if (type == TYPE_ADMIN)   loadAdministrator(data, t);
}

void StorageManager::loadStudent(UniversityData& data, const std::vector<std::string>& t) {
    const int facId = std::stoi(t[3]);
    auto facWeak = resolveFacultyWeak(data, facId);

    auto student = std::make_shared<Student>(
        t[2], facWeak, t[4], t[5], std::stoul(t[6])
    );

    if (t.size() > 7 && !t[7].empty()) {
        const unsigned int currentHours = std::stoul(t[7]);
        if (currentHours != std::stoul(t[6])) {
            student->setHours(currentHours);
        }
    }

    if (t.size() > 8 && !t[8].empty()) {
        std::vector<unsigned int> marks;
        for (const auto& ms : split(t[8], ',')) {
            if (!ms.empty()) marks.push_back(std::stoul(ms));
        }
        student->setMarks(marks);
    }

    data.members.add(student);
    if (auto f = facWeak.lock()) *f += student;
}

void StorageManager::loadTeacher(UniversityData& data, const std::vector<std::string>& t) {
    const int facId  = std::stoi(t[3]);
    const int deptId = std::stoi(t[4]);
    const int subjId = std::stoi(t[5]);

    auto facWeak = resolveFacultyWeak(data, facId);
    auto dept    = resolveDepartment(data, deptId);
    auto subj    = resolveSubject(data, subjId);

    auto teacher = std::make_shared<Teacher>(
        std::stoi(t[1]), t[2], facWeak, dept, subj, std::stoi(t[6])
    );

    const int currentLoad = std::stoi(t[7]);
    if (currentLoad > 0) teacher->applyEffect(currentLoad);

    data.members.add(teacher);
    dept->addTeacher(teacher);
}

void StorageManager::loadAdministrator(UniversityData& data, const std::vector<std::string>& t) {
    const int facId = std::stoi(t[3]);
    auto facWeak = resolveFacultyWeak(data, facId);

    auto admin = std::make_shared<Administrator>(
        std::stoi(t[1]), t[2], facWeak, t[4], std::stoi(t[5])
    );
    data.members.add(admin);
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
}

void StorageManager::generateReport(const std::string& reportFilename, const UniversityData& data) {
    std::ofstream ofs(reportFilename);
    if (!ofs.is_open()) {
        throw InvalidOperationException("Не удалось создать файл отчёта: " + reportFilename);
    }

    ofs << "=== ОТЧЁТ О СОСТОЯНИИ СИСТЕМЫ УНИВЕРСИТЕТА ===\n\n";

    reportFaculties(ofs, data);
    reportDepartments(ofs, data);
    reportSubjects(ofs, data);
    reportMembers(ofs, data);

    ofs.close();
    std::cout << "Отчёт сохранён в " << reportFilename << std::endl;
}

void StorageManager::reportFaculties(std::ofstream& ofs, const UniversityData& data) {
    ofs << "--- ФАКУЛЬТЕТЫ ---\n";
    for (const auto& f : data.faculties) {
        ofs << "Факультет: " << f->getFacultyName() << "\n";
        ofs << "  Макс. студентов: " << f->getMaxStudentsCount() << "\n";
    }
    ofs << "\n";
}

void StorageManager::reportDepartments(std::ofstream& ofs, const UniversityData& data) {
    ofs << "--- КАФЕДРЫ ---\n";
    for (const auto& d : data.departments) {
        ofs << "Кафедра: " << d->getDepartmentName()
            << " (Факультет: " << d->getFaculty()->getFacultyName() << ")\n";
    }
    ofs << "\n";
}

void StorageManager::reportSubjects(std::ofstream& ofs, const UniversityData& data) {
    ofs << "--- ПРЕДМЕТЫ ---\n";
    for (const auto& s : data.subjects) {
        ofs << "Предмет: " << s->getSubjectName()
            << " | Часы: " << s->getHours()
            << " | Контроль: " << static_cast<int>(s->getControlType()) << "\n";
    }
    ofs << "\n";
}

void StorageManager::reportMembers(std::ofstream& ofs, const UniversityData& data) {
    ofs << "--- СОТРУДНИКИ И СТУДЕНТЫ ---\n";
    for (const auto& m : data.members) {
        if (!m) continue;

        const std::string type = m->getType();
        if      (type == TYPE_STUDENT) reportStudent(ofs, std::dynamic_pointer_cast<Student>(m));
        else if (type == TYPE_TEACHER) reportTeacher(ofs, std::dynamic_pointer_cast<Teacher>(m));
        else if (type == TYPE_ADMIN)   reportAdministrator(ofs, std::dynamic_pointer_cast<Administrator>(m));
    }
}

void StorageManager::reportStudent(std::ofstream& ofs, const std::shared_ptr<Student>& s) {
    ofs << "Тип: " << s->getType() << "\n";
    ofs << "  Номер студ. билета: " << s->getStudentNumber() << "\n";
    ofs << "  ФИО: " << s->getFullName() << "\n";

    auto fac = s->getFaculty().lock();
    ofs << "  Факультет: " << (fac ? fac->getFacultyName() : "Нет") << "\n";

    ofs << "  Группа: " << s->getGroupNumber() << "\n";
    ofs << "  Часы: " << s->getHours() << " / " << s->getMaxHoursPerWeek() << "\n";
    ofs << "  Средний балл: " << s->calculateMetric() << "\n";
    ofs << "  Оценки: ";
    for (auto mark : s->getMarks()) ofs << mark << " ";
    ofs << "\n\n";
}

void StorageManager::reportTeacher(std::ofstream& ofs, const std::shared_ptr<Teacher>& t) {
    ofs << "Тип: " << t->getType() << "\n";
    ofs << "  ID: " << t->getId() << "\n";
    ofs << "  ФИО: " << t->getFullName() << "\n";

    auto fac = t->getFaculty().lock();
    ofs << "  Факультет: " << (fac ? fac->getFacultyName() : "Нет") << "\n";

    auto d = t->getDepartment();
    auto s = t->getSubject();
    ofs << "  Кафедра: " << (d ? d->getDepartmentName() : "Нет") << "\n";
    ofs << "  Предмет: " << (s ? s->getSubjectName() : "Нет") << "\n";
    ofs << "  Нагрузка: " << t->getTeachingLoad()
        << " / " << t->getMaxTeachingLoad() << "\n\n";
}

void StorageManager::reportAdministrator(std::ofstream& ofs, const std::shared_ptr<Administrator>& a) {
    ofs << "Тип: " << a->getType() << "\n";
    ofs << "  ID: " << a->getId() << "\n";
    ofs << "  ФИО: " << a->getFullName() << "\n";

    auto fac = a->getFaculty().lock();
    ofs << "  Факультет: " << (fac ? fac->getFacultyName() : "Нет") << "\n";

    ofs << "  Должность: " << a->getPosition() << "\n";
    ofs << "  Подчинённых: " << a->getManagedPeople() << "\n\n";
}