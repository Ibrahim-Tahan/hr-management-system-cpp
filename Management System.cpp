#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Employee {
    int id;
    string FirstName;
    string LastName;
    int workingHour;
    double costPerHour;

    Employee* next;
    Employee* prev;

    double calcYearlySal() const {
        return workingHour * costPerHour * 52;
    }
};

struct hrDept {
    Employee* head = nullptr;
    Employee* tail = nullptr;
    int totalEmp = 0;


    bool uniqueID(int empId) const {
        Employee* current = head;
        while (current) {
            if (current->id == empId) {
                return false; 
            }
            current = current->next;
        }
        return true; 
    }


    void addEmp() {
        Employee* newNode = new Employee;

        cout << "Enter first name: ";
        cin >> newNode->FirstName;

        cout << "Enter last name: ";
        cin >> newNode->LastName;

        
        do
        {
            cout << "Enter ID: ";
            cin >> newNode->id;
        
            if (!uniqueID(newNode->id)) {
                cout << "Employee with ID " << newNode->id << " already exists. Please enter a unique ID.\n";
            }
        } while (!uniqueID(newNode->id));


        do {
            cout << "Enter working hours per week: ";
            cin >> newNode->workingHour;

            if (newNode->workingHour < 0) {
                cout << "Working hours per week cannot be negative. \nPlease enter a valid value.\n";
            }
        } while (newNode->workingHour < 0);

        do {
            cout << "Enter cost per hour: ";
            cin >> newNode->costPerHour;

            if (newNode->costPerHour < 0) {
                cout << "Cost per hour cannot be negative. \nPlease enter a valid value.\n";
            }
        } while (newNode->costPerHour < 0);

        newNode->next = nullptr;
        newNode->prev = nullptr;

        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        totalEmp++;
        cout << "Employee added successfully.\n";

        writeToFile("employee_data.csv");
    }

    void writeToFile(const string& filename) const {
        ofstream outFile(filename); 

        if (!outFile.is_open()) {
            cout << "Error: Could not open the file for writing.\n";
            return;
        }

        Employee* current = head;
        while (current) {
            outFile << current->id << "," << current->FirstName << "," << current->LastName
                << "," << current->workingHour << "," << current->costPerHour << "\n";
            current = current->next;
        }

        outFile.close();
        cout << "Employee information written to " << filename << " successfully.\n";
    }


    void DeleteEmp() {
        int empId;
        Employee* current = head;
        cout << "Enter the ID:";
        cin >> empId;
        while (current) {
            if (current->id == empId) {
                if (current == head) {
                    head = current->next;
                    if (head) head->prev = nullptr;
                }
                else if (current == tail) {
                    tail = current->prev;
                    if (tail) tail->next = nullptr;
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                delete current;
                totalEmp--;
                cout << "Employee with ID " << empId << " deleted successfully.\n";

                
                writeToFile("employee_data.csv");
                return;
            }
            current = current->next;
        }
        cout << "Employee with ID " << empId << " not found.\n";
    }

    void UpdatePerson() {
        int id, newHours;
        double newCost;
        Employee* current = head;
        cout << "ID: "; cin >> id;

        do {
            cout << "New Hour: ";
            cin >> newHours;

            if (newHours < 0) {
                cout << "Error: New working hours cannot be negative. Please enter a valid value.\n";
            }
        } while (newHours < 0);

        
        do {
            cout << "New Cost: ";
            cin >> newCost;

            if (newCost < 0) {
                cout << "Error: New cost per hour cannot be negative. Please enter a valid value.\n";
            }
        } while (newCost < 0);

        while (current) {
            if (current->id == id) {
                current->workingHour = newHours;
                current->costPerHour = newCost;
                cout << "Employee with ID " << id << " updated successfully.\n";

              
                writeToFile("employee_data.csv");
                return;
            }
            current = current->next;
        }
        cout << "Employee with ID " << id << " not found.\n";
    }
    void SearchEmp() {
        int empId;
        Employee* current = head;
        cout << "Insert the employee ID:";
        cin >> empId;
        while (current) {
            if (current->id == empId) {
                cout << "Employee found!\n";
                cout << "ID: " << current->id << ", Name: " << current->FirstName << " "
                    << current->LastName << ", Salary: $" << current->workingHour * current->costPerHour * 52 << "\n";
                return;
            }
            current = current->next;
        }
        cout << "Employee with ID " << empId << " not found.\n";
    }

    void ReportList() {
        cout << "Total Emp: \n" << totalEmp << endl;
        Employee* current = head;
        while (current) {
            cout << "ID: " << current->id << ", Name: " << current->FirstName << " "
                << current->LastName << ", Salary: $" << current->calcYearlySal() << "\n";
            current = current->next;
        }
    }

    void fetchDataFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cout << "Could not open file\n" << endl;
            return;
        }

        Employee* newNode;
        while (inFile.good()) {
            newNode = new Employee;
            inFile >> newNode->id;
            inFile.ignore();
            getline(inFile, newNode->FirstName, ',');
            getline(inFile, newNode->LastName, ',');
            inFile >> newNode->workingHour;
            inFile.ignore(); 
            inFile >> newNode->costPerHour;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n'); 

            if (newNode->id < 0) {
                delete newNode;
                continue;
            }

            newNode->next = nullptr;
            newNode->prev = nullptr;

            if (!head) {
                head = tail = newNode;
            }
            else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            totalEmp++;
        }
        inFile.close();
        cout << "Old records fetched from " << filename << " successfully.\n";
    }



};

int main() {
    hrDept hr;
    hr.fetchDataFromFile("employee_data.csv");
    int selection;

    cout << "Company TalentHub\n Human Resource Management Program" << endl;
    do {
        cout << "\n\n1- View all employees" << endl;
        cout << "2- Add new employee" << endl;
        cout << "3- Edit or Update employee" << endl;
        cout << "4- Delete employee" << endl;
        cout << "5- Search for employee" << endl;
        cout << "6- Quit" << endl;

        cout << "Num of Employees: " << hr.totalEmp;
        cout << "\nSelection: "; cin >> selection;

        switch (selection) {
        case 1:
            cout << "\nHere are all the employees: " << endl;
            hr.ReportList();
            break;

        case 2:
            cout << "\nWhat is the Emp to insert: ";
            hr.addEmp();
            break;

        case 3:
            cout << "\nEdit Or Update employee" << endl;
            hr.UpdatePerson();
            break;

        case 4:
            cout << "\nDelete employee" << endl;
            hr.DeleteEmp();
            break;

        case 5:
            cout << "\nSearch employee" << endl;
            hr.SearchEmp();
            break;

        case 6:
            cout << "\nGoodbye!";
        }
    } while (selection != 6);

    return 0;
}