#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

struct Student {
    int id;
    string name;
    string department;
    double gpa;
};

const string FILE_NAME = "students.txt";

vector<Student> loadStudents() {
    vector<Student> students;
    ifstream file(FILE_NAME);
    string line;

    while (getline(file, line)) {
        stringstream row(line);
        string idText, name, department, gpaText;

        if (getline(row, idText, '|') &&
            getline(row, name, '|') &&
            getline(row, department, '|') &&
            getline(row, gpaText)) {
            try {
                Student s;
                s.id = stoi(idText);
                s.name = name;
                s.department = department;
                s.gpa = stod(gpaText);
                students.push_back(s);
            }
            catch (...) {
                // Skip a malformed record.
            }
        }
    }
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream file(FILE_NAME);
    for (const Student& s : students) {
        file << s.id << '|' << s.name << '|' << s.department << '|' << s.gpa << '\n';
    }
}

bool idExists(const vector<Student>& students, int id) {
    for (const Student& s : students) {
        if (s.id == id) return true;
    }
    return false;
}

void addStudent(vector<Student>& students) {
    Student s{};

    cout << "Student ID: ";
    if (!(cin >> s.id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid ID.\\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (idExists(students, s.id)) {
        cout << "That ID already exists.\\n";
        return;
    }

    cout << "Student name: ";
    getline(cin, s.name);
    cout << "Department: ";
    getline(cin, s.department);
    cout << "GPA (0 to 4): ";

    if (!(cin >> s.gpa) || s.gpa < 0 || s.gpa > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid GPA. Enter a value from 0 to 4.\\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    students.push_back(s);
    saveStudents(students);
    cout << "Student added successfully.\\n";
}

void listStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students saved yet.\\n";
        return;
    }

    cout << "\\n--- Student List ---\\n";
    for (const Student& s : students) {
        cout << "ID: " << s.id
            << " | Name: " << s.name
            << " | Department: " << s.department
            << " | GPA: " << s.gpa << '\n';
    }
}

void searchStudent(const vector<Student>& students) {
    int id;
    cout << "Enter student ID to search: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid ID.\\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (const Student& s : students) {
        if (s.id == id) {
            cout << "Found: " << s.name << " | " << s.department
                << " | GPA: " << s.gpa << '\n';
            return;
        }
    }
    cout << "Student not found.\\n";
}

void deleteStudent(vector<Student>& students) {
    int id;
    cout << "Enter student ID to delete: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid ID.\\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->id == id) {
            students.erase(it);
            saveStudents(students);
            cout << "Student deleted successfully.\\n";
            return;
        }
    }
    cout << "Student not found.\\n";
}

int main() {
    vector<Student> students = loadStudents();
    int choice = 0;

    do {
        cout << "\\n=== Student Management System (C++) ===\\n"
            << "1. Add student\\n"
            << "2. List students\\n"
            << "3. Search student by ID\\n"
            << "4. Delete student by ID\\n"
            << "5. Exit\\n"
            << "Choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number from 1 to 5.\\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: addStudent(students); break;
        case 2: listStudents(students); break;
        case 3: searchStudent(students); break;
        case 4: deleteStudent(students); break;
        case 5: cout << "Goodbye!\\n"; break;
        default: cout << "Please choose a number from 1 to 5.\\n";
        }
    } while (choice != 5);

    return 0;
}
