#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Abstract class Person
class Person {
protected:
    string firstName;
    string lastName;
    string date;

public:
    Person(string firstName, string lastName, string date)
        : firstName(firstName), lastName(lastName), date(date) {}

    string getFullName() const {
        return firstName + " " + lastName;
    }

    virtual ~Person() = default;
};

// Student class inheriting from Person
class Student : public Person {
private:
    string studentID;
    string email;
    bool isGraduated;

public:
    Student(string firstName, string lastName, string studentID, string email, string date)
        : Person(firstName, lastName, date), studentID(studentID), email(email), isGraduated(false) {}

    string getStudentID() const {
        return studentID;
    }

    bool getGraduationdate() const {
        return isGraduated;
    }

    void graduate() {
        isGraduated = true;
    }

    string getEmail() const {
        return email;
    }
};

// Faculty class
class Faculty {
private:
    string name;
    string field;
    vector<Student*> currentStudents;
    vector<Student*> graduatedStudents;

public:
    Faculty(string name, string field) : name(name), field(field) {}

    // Add a new student to the faculty
    void addStudent(Student* student) {
        currentStudents.push_back(student);
    }

    // Graduate a student from the faculty
    void graduateStudent(string studentID) {
        auto it = find_if(currentStudents.begin(), currentStudents.end(),
                          [&studentID](Student* student) { return student->getStudentID() == studentID; });

        if (it != currentStudents.end()) {
            (*it)->graduate();
            graduatedStudents.push_back(*it);
            currentStudents.erase(it);
        }
    }

    // Display all currently enrolled students
    void displayEnrolledStudents() const {
        cout << "Enrolled Students in " << name << ":" << endl;
        for (const auto& student : currentStudents) {
            cout << "- " << student->getFullName() << endl;
        }
    }

    // Display all graduates
    void displayGraduates() const {
        cout << "Graduated Students in " << name << ":" << endl;
        for (const auto& student : graduatedStudents) {
            cout << "- " << student->getFullName() << endl;
        }
    }

    // Check if a student belongs to this faculty
    bool hasStudent(string studentID) const {
        for (const auto& student : currentStudents) {
            if (student->getStudentID() == studentID) {
                return true;
            }
        }
        for (const auto& student : graduatedStudents) {
            if (student->getStudentID() == studentID) {
                return true;
            }
        }
        return false;
    }

    string getName() const {
        return name;
    }

    string getField() const {
        return field;
    }
};

// University class
class University {
private:
    string name;
    vector<Faculty> faculties;

public:
    University(string name) : name(name) {}

    // Create a new faculty
    void createFaculty(string facultyName, string field) {
        faculties.emplace_back(facultyName, field);
    }

    // Search for the faculty of a student by unique identifier (e.g., student ID)
    string searchFacultyByStudent(string studentID) const {
        for (const auto& faculty : faculties) {
            if (faculty.hasStudent(studentID)) {
                return faculty.getName();
            }
        }
        return "Student not found in any faculty.";
    }

    // Display all faculties
    void displayAllFaculties() const {
        cout << "Faculties in " << name << ":" << endl;
        for (const auto& faculty : faculties) {
            cout << "- " << faculty.getName() << endl;
        }
    }

    // Display all faculties in a specific field
    void displayFacultiesByField(string field) const {
        cout << "Faculties in the field of " << field << ":" << endl;
        for (const auto& faculty : faculties) {
            if (faculty.getField() == field) {
                cout << "- " << faculty.getName() << endl;
            }
        }
    }

    // Get reference to a specific faculty by name
    Faculty* getFacultyByName(string facultyName) {
        for (auto& faculty : faculties) {
            if (faculty.getName() == facultyName) {
                return &faculty;
            }
        }
        return nullptr;
    }
};

// Menu function to interact with the system
void showMenu(University& tum) {
    int choice;
    do {
        cout << "\n---- University Management System ----\n";
        cout << "1. Create Faculty\n";
        cout << "2. Add Student to Faculty\n";
        cout << "3. Graduate Student\n";
        cout << "4. Display Enrolled Students in Faculty\n";
        cout << "5. Display Graduated Students in Faculty\n";
        cout << "6. Search Faculty by Student ID\n";
        cout << "7. Display All Faculties\n";
        cout << "8. Display Faculties by Field\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string facultyName, field;
                cout << "Enter Faculty Name: ";
                cin.ignore();
                getline(cin, facultyName);
                cout << "Enter Faculty Field: ";
                getline(cin, field);
                tum.createFaculty(facultyName, field);
                cout << "Faculty created successfully.\n";
                break;
            }
            case 2: {
                string facultyName, firstName, lastName, studentID, email, date;
                cout << "Enter Faculty Name: ";
                cin.ignore();
                getline(cin, facultyName);
                Faculty* faculty = tum.getFacultyByName(facultyName);
                if (faculty) {
                    cout << "Enter Student's First Name: ";
                    getline(cin, firstName);
                    cout << "Enter Student's Last Name: ";
                    getline(cin, lastName);
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    cout << "Enter Student Email: ";
                    getline(cin, email);
                    cout << "Enter Student Date of Birth (DD-MM-YYYY): ";
                    getline(cin, date);
                    Student* newStudent = new Student(firstName, lastName, studentID, email, date);
                    faculty->addStudent(newStudent);
                    cout << "Student added successfully.\n";
                } else {
                    cout << "Faculty not found.\n";
                }
                break;
            }
            case 3: {
                string facultyName, studentID;
                cout << "Enter Faculty Name: ";
                cin.ignore();
                getline(cin, facultyName);
                Faculty* faculty = tum.getFacultyByName(facultyName);
                if (faculty) {
                    cout << "Enter Student ID: ";
                    getline(cin, studentID);
                    faculty->graduateStudent(studentID);
                    cout << "Student graduated successfully.\n";
                } else {
                    cout << "Faculty not found.\n";
                }
                break;
            }
            case 4: {
                string facultyName;
                cout << "Enter Faculty Name: ";
                cin.ignore();
                getline(cin, facultyName);
                Faculty* faculty = tum.getFacultyByName(facultyName);
                if (faculty) {
                    faculty->displayEnrolledStudents();
                } else {
                    cout << "Faculty not found.\n";
                }
                break;
            }
            case 5: {
                string facultyName;
                cout << "Enter Faculty Name: ";
                cin.ignore();
                getline(cin, facultyName);
                Faculty* faculty = tum.getFacultyByName(facultyName);
                if (faculty) {
                    faculty->displayGraduates();
                } else {
                    cout << "Faculty not found.\n";
                }
                break;
            }
            case 6: {
                string studentID;
                cout << "Enter Student ID: ";
                cin.ignore();
                getline(cin, studentID);
                cout << "Student belongs to: " << tum.searchFacultyByStudent(studentID) << endl;
                break;
            }
            case 7: {
                tum.displayAllFaculties();
                break;
            }
            case 8: {
                string field;
                cout << "Enter Field: ";
                cin.ignore();
                getline(cin, field);
                tum.displayFacultiesByField(field);
                break;
            }
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);
}

// Main function
int main() {
    // Create a university
    University tum("Technical University of Moldova");

    // Start the menu
    showMenu(tum);

    return 0;
}
