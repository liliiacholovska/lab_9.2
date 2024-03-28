#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <string>

using namespace std;

enum Specialization { CS, IT, ME, PHI, TECH };

string specializationNames[] = { "Computer Science", "Information Technology", "Mathematics and Economics", "Physics and Informatics", "Technology" };

union Grade {
    int programming;
    int numerical_methods;
    int pedagogy;
};

struct Student {
    string surname;
    int course;
    Specialization specialization;
    int physics_grade;
    int math_grade;
    Grade additional_grade;
};

void CreateStudents(Student* students, const int N);
void SortStudents(Student* students, const int N);
int* IndexSort(const Student* students, const int N);
void PrintIndexSorted(const Student* students, const int* indexArray, const int N);
void SortForBinarySearch(Student* students, const int N);
int CalculateSpecializedGrade(const Student& student);
bool BinarySearch(const Student* students, const int N, const string& searchSurname, const int searchCourse, const int searchGrade);
void PrintStudents(const Student* students, const int N);

int main() {
    int MenuItem;
    int N;
    cout << "Enter the number of students: ";
    cin >> N;

    Student* students = new Student[N];

    do {
        cout << "Choose an action:" << endl;
        cout << "[1] - Enter information about students" << endl;
        cout << "[2] - Display information about students" << endl;
        cout << "[3] - Sort information physically" << endl;
        cout << "[4] - Sort information index-wise" << endl;
        cout << "[5] - Search for a student (binary)" << endl;
        cout << "[0] - Exit the program" << endl;
        cout << "Enter your choice: ";
        cin >> MenuItem;
        cout << endl;

        switch (MenuItem) {
        case 1:
            CreateStudents(students, N);
            break;
        case 2:
            PrintStudents(students, N);
            break;
        case 3:
            SortStudents(students, N);
            cout << "Information sorted physically." << endl;
            break;
        case 4: {
            int* indexArray = IndexSort(students, N);
            PrintIndexSorted(students, indexArray, N);
            delete[] indexArray;
            break;
        }
        case 5: {
            string searchSurname;
            int searchCourse = 2;
            int searchGrade = 5;

            cout << "Enter student's surname to search: ";
            cin >> searchSurname;
            cout << "Enter student's course to search: ";
            cin >> searchCourse;
            cout << "Enter student's grade from the third subject to search: ";
            cin >> searchGrade;

            SortForBinarySearch(students, N);
            
            bool found = BinarySearch(students, N, searchSurname, searchCourse, searchGrade);
            if (found) {
                cout << "Student found!" << endl;
            }
            else {
                cout << "Student not found!" << endl;
            }
            break;
        }
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (MenuItem != 0);

    delete[] students;
    return 0;
}

void CreateStudents(Student* students, const int N) {
    for (int i = 0; i < N; ++i) {
        cout << "Student #" << i + 1 << ":\n";
        cout << "Surname: ";
        cin >> students[i].surname;
        cout << "Course: ";
        cin >> students[i].course;
        while (cin.fail()) {
            cout << "Invalid input. Please enter a number for course: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> students[i].course;
        }
        cout << "Specialization (0 - CS, 1 - IT, 2 - ME, 3 - PHI, 4 - TECH): ";
        int specialization;
        cin >> specialization;
        while (cin.fail() || specialization < 0 || specialization > 4) {
            cout << "Invalid input. Please enter a number between 0 and 4 for specialization: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> specialization;
        }
        students[i].specialization = static_cast<Specialization>(specialization);
        cout << "Grade in Physics: ";
        cin >> students[i].physics_grade;
        while (cin.fail()) {
            cout << "Invalid input. Please enter a number for physics grade: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> students[i].physics_grade;
        }
        cout << "Grade in Mathematics: ";
        cin >> students[i].math_grade;
        while (cin.fail()) {
            cout << "Invalid input. Please enter a number for math grade: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> students[i].math_grade;
        }

        if (students[i].specialization == CS) {
            cout << "Programming grade: ";
            cin >> students[i].additional_grade.programming;
        }
        else if (students[i].specialization == IT) {
            cout << "Numerical methods grade: ";
            cin >> students[i].additional_grade.numerical_methods;
        }
        else { 
            cout << "Pedagogy grade: ";
            cin >> students[i].additional_grade.pedagogy;
        }
    }
}

double CalculateAverageGrade(const Student& student) {
    double sum = student.physics_grade + student.math_grade;
    switch (student.specialization) {
    case CS:
        sum += student.additional_grade.programming;
        break;
    case IT:
        sum += student.additional_grade.numerical_methods;
        break;
    default:
        sum += student.additional_grade.pedagogy;
        break;
    }
    return sum / 3.0;
}

void SortStudents(Student* students, const int N) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            int gradeJ = 0, gradeJPlus1 = 0;
            switch (students[j].specialization) {
            case CS: gradeJ = students[j].additional_grade.programming; break;
            case IT: gradeJ = students[j].additional_grade.numerical_methods; break;
            default: gradeJ = students[j].additional_grade.pedagogy; break;
            }
            switch (students[j + 1].specialization) {
            case CS: gradeJPlus1 = students[j + 1].additional_grade.programming; break;
            case IT: gradeJPlus1 = students[j + 1].additional_grade.numerical_methods; break;
            default: gradeJPlus1 = students[j + 1].additional_grade.pedagogy; break;
            }

            if (gradeJ < gradeJPlus1 ||
                (gradeJ == gradeJPlus1 && students[j].course < students[j + 1].course) ||
                (gradeJ == gradeJPlus1 && students[j].course == students[j + 1].course && students[j].surname.compare(students[j + 1].surname) < 0)) {
                swap(students[j], students[j + 1]);
            }
        }
    }
}

int* IndexSort(const Student* students, const int N) {
    int* indexArray = new int[N];
    for (int i = 0; i < N; i++) {
        indexArray[i] = i;
    }

    sort(indexArray, indexArray + N, [&](const int& a, const int& b) -> bool {
        int gradeA, gradeB;

        switch (students[a].specialization) {
        case CS: gradeA = students[a].additional_grade.programming; break;
        case IT: gradeA = students[a].additional_grade.numerical_methods; break;
        default: gradeA = students[a].additional_grade.pedagogy; break;
        }

        switch (students[b].specialization) {
        case CS: gradeB = students[b].additional_grade.programming; break;
        case IT: gradeB = students[b].additional_grade.numerical_methods; break;
        default: gradeB = students[b].additional_grade.pedagogy; break;
        }

        if (gradeA != gradeB) return gradeA > gradeB;

        if (students[a].course != students[b].course) return students[a].course > students[b].course;

        return students[a].surname < students[b].surname;
        });

    return indexArray;
}

void PrintIndexSorted(const Student* students, const int* indexArray, const int N) {
    cout << "==========================================================================================================" << endl;
    cout << "|  # |   Surname      |  Course |      Specialization      |  Physics |  Mathematics |  Additional Grade |" << endl;
    cout << "----------------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < N; i++) {
        cout << "| " << setw(2) << right << i + 1 << " ";
        cout << "| " << setw(15) << left << students[indexArray[i]].surname;
        cout << "| " << setw(8) << right << students[indexArray[i]].course;
        cout << "| " << setw(25) << left << specializationNames[students[indexArray[i]].specialization];
        cout << "| " << setw(9) << right << students[indexArray[i]].physics_grade;
        cout << "| " << setw(13) << right << students[indexArray[i]].math_grade;

        switch (students[indexArray[i]].specialization) {
        case CS:
            cout << "| " << setw(18) << right << students[indexArray[i]].additional_grade.programming << "|" << endl;
            break;
        case IT:
            cout << "| " << setw(18) << right << students[indexArray[i]].additional_grade.numerical_methods << "|" << endl;
            break;
        default:
            cout << "| " << setw(18) << right << students[indexArray[i]].additional_grade.pedagogy << "|" << endl;
            break;
        }
    }
    cout << "==========================================================================================================" << endl << endl;
}

void SortForBinarySearch(Student* students, const int N) {
    sort(students, students + N, [](const Student& a, const Student& b) {
        if (a.surname != b.surname) return a.surname < b.surname;
        if (a.course != b.course) return a.course < b.course;
        int gradeA = CalculateSpecializedGrade(a);
        int gradeB = CalculateSpecializedGrade(b);
        return gradeA < gradeB;
        });
}

int CalculateSpecializedGrade(const Student& student) {
    switch (student.specialization) {
    case CS:
        return student.additional_grade.programming;
    case IT:
        return student.additional_grade.numerical_methods;
    default:
        return student.additional_grade.pedagogy;
    }
}

bool BinarySearch(const Student* students, const int N, const string& searchSurname, const int searchCourse, const int searchGrade) {
    int low = 0, high = N - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        const Student& midStudent = students[mid];
        int midGrade = CalculateSpecializedGrade(midStudent);

        if (midStudent.surname == searchSurname && midStudent.course == searchCourse && midGrade == searchGrade) {
            return true;
        }
        else if (midStudent.surname < searchSurname || (midStudent.surname == searchSurname && (midStudent.course < searchCourse || (midStudent.course == searchCourse && midGrade < searchGrade)))) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return false;
}

void PrintStudents(const Student* students, const int N) {
    cout << "========================================================================================================" << endl;
    cout << "| # | Surname       | Course | Specialization               | Physics | Mathematics | Additional Grade |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < N; ++i) {
        cout << "| " << setw(1) << right << i + 1 << " | ";
        cout << setw(13) << left << students[i].surname << " | ";
        cout << setw(6) << right << students[i].course << " | ";

        switch (students[i].specialization) {
        case CS:
            cout << setw(28) << left << "Computer Science" << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.programming << " |" << endl;
            break;
        case IT:
            cout << setw(28) << left << "Informatics" << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.numerical_methods << " |" << endl;
            break;
        case ME:
            cout << setw(28) << left << "Math and Economics" << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.pedagogy << " |" << endl;
            break;
        case PHI:
            cout << setw(28) << left << "Physics and Informatics" << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.numerical_methods << " |" << endl;
            break;
        case TECH:
            cout << setw(28) << left << "Technology" << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.pedagogy << " |" << endl;
            break;
        }
    }
    cout << "========================================================================================================" << endl << endl;
}