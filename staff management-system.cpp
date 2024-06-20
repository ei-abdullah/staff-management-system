#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> // For setting width and alignment

using namespace std;

class Person
{
protected:
    string name;
    int age;

public:
    Person() : name(""), age(0) {}

    Person(string name, int age) : name(name), age(age) {}

    virtual void display()
    {
        cout << left << setw(15) << "Name:" << name << endl
             << left << setw(15) << "Age:" << age << endl;
    }

    virtual void save(ofstream &out)
    {
        out << name << endl
            << age << endl;
    }

    virtual void load(ifstream &in)
    {
        getline(in, name);
        in >> age;
        in.ignore(); // Ignore the newline character after the integer
    }
};

class Staff : public Person
{
protected:
    string position;
    int staffID;

public:
    Staff() : Person(), position(""), staffID(0) {}

    Staff(string name, int age, string position, int staffID) : Person(name, age), position(position), staffID(staffID) {}

    void display()
    {
        Person::display();
        cout << left << setw(15) << "Staff ID:" << staffID << endl
             << left << setw(15) << "Position:" << position << endl;
    }

    void save(ofstream &out)
    {
        Person::save(out);
        out << staffID << endl
            << position << endl;
    }

    void load(ifstream &in)
    {
        Person::load(in);
        in >> staffID;
        in.ignore();
        getline(in, position);
    }

    int getStaffID()
    {
        return staffID;
    }
};

class FullTimeStaff : public Staff
{
private:
    double salary;

public:
    FullTimeStaff() : Staff(), salary(0.0) {}

    FullTimeStaff(string name, int age, string position, int staffID, double salary) : Staff(name, age, position, staffID), salary(salary) {}

    void display()
    {
        Staff::display();
        cout << left << setw(15) << "Salary:" << salary << endl;
    }

    void save(ofstream &out)
    {
        Staff::save(out);
        out << salary << endl;
    }

    void load(ifstream &in)
    {
        Staff::load(in);
        in >> salary;
        in.ignore();
    }
};

class PartTimeStaff : public Staff
{
private:
    double hourlyRate;
    int hoursWorked;

public:
    PartTimeStaff() : Staff(), hourlyRate(0.0), hoursWorked(0) {}

    PartTimeStaff(string name, int age, string position, int staffID, double hourlyRate, int hoursWorked) : Staff(name, age, position, staffID), hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}

    void display()
    {
        Staff::display();
        cout << left << setw(15) << "Hourly Rate:" << hourlyRate << endl
             << left << setw(15) << "Hours Worked:" << hoursWorked << endl;
    }

    void save(ofstream &out)
    {
        Staff::save(out);
        out << hourlyRate << endl
            << hoursWorked << endl;
    }

    void load(ifstream &in)
    {
        Staff::load(in);
        in >> hourlyRate >> hoursWorked;
        in.ignore();
    }
};

void addStaff(Staff *staff[], int &count)
{
    int type;
    cout << "Create Staff" << endl
         << "1. Full-time Staff" << endl
         << "2. Part-time Staff" << endl;
    cin >> type;

    string name;
    int age;
    string position;
    int staffID;

    cout << "Enter the name: ";
    cin >> ws;
    getline(cin, name);
    cout << "Enter the age of " << name << ": ";
    cin >> age;
    cout << "Enter StaffID of " << name << ": ";
    cin >> staffID;
    cout << "Enter position of " << name << ": ";
    cin >> ws;
    getline(cin, position);

    if (type == 1)
    {
        double salary;
        cout << "Enter the salary of " << name << ": ";
        cin >> salary;
        staff[count++] = new FullTimeStaff(name, age, position, staffID, salary);
    }
    else if (type == 2)
    {
        double hourlyRate;
        int hoursWorked;
        cout << "Enter the hourly rate of " << name << ": ";
        cin >> hourlyRate;
        cout << "Enter the amount of hours " << name << " has worked: ";
        cin >> hoursWorked;
        staff[count++] = new PartTimeStaff(name, age, position, staffID, hourlyRate, hoursWorked);
    }
    else
    {
        cout << "Invalid Type!" << endl;
    }
}

void displayStaff(Staff *staff[], int count)
{
    for (int i = 0; i < count; i++)
    {
        cout << "---------------------------------------------" << endl;
        staff[i]->display();
    }
    cout << "---------------------------------------------" << endl;
}

void saveToFile(Staff *staff[], int count)
{
    ofstream outFile("staff_data.txt");
    if (outFile.is_open())
    {
        outFile << count << endl;
        for (int i = 0; i < count; i++)
        {
            FullTimeStaff *fullTime = dynamic_cast<FullTimeStaff *>(staff[i]);
            PartTimeStaff *partTime = dynamic_cast<PartTimeStaff *>(staff[i]);
            if (fullTime != nullptr)
            {
                outFile << "FullTime" << endl;
            }
            else if (partTime != nullptr)
            {
                outFile << "PartTime" << endl;
            }
            staff[i]->save(outFile);
        }
        outFile.close();
        cout << "Data Saved Successfully" << endl;
    }
    else
    {
        cout << "Unable to open file for writing." << endl;
    }
}

void loadFromFile(Staff *staff[], int &count)
{
    ifstream inFile("staff_data.txt");
    if (inFile.is_open())
    {
        inFile >> count;
        inFile.ignore();
        cout << "Count loaded: " << count << endl;

        if (count <= 0 || count > 10)
        {
            cout << "Invalid count value loaded. Exiting load process." << endl;
            inFile.close();
            return;
        }

        string type;
        for (int i = 0; i < count; i++)
        {
            getline(inFile, type);
            if (type == "FullTime")
            {
                staff[i] = new FullTimeStaff();
            }
            else if (type == "PartTime")
            {
                staff[i] = new PartTimeStaff();
            }
            else
            {
                cout << "Unknown staff type found in file. Exiting load process." << endl;
                inFile.close();
                return;
            }
            staff[i]->load(inFile);
        }
        inFile.close();
        cout << "Data Loaded Successfully" << endl;
    }
    else
    {
        cout << "Unable to open file for reading." << endl;
    }
}

void searchStaff(Staff *staff[], int count)
{
    int ID;
    cout << "Enter staffID to search: ";
    cin >> ID;
    for (int i = 0; i < count; i++)
    {
        if (staff[i]->getStaffID() == ID)
        {
            cout << "---------------------------------------------" << endl;
            staff[i]->display();
            cout << "---------------------------------------------" << endl;
            return;
        }
    }
    cout << "Staff with ID " << ID << " not found." << endl;
}

void clearFile()
{
    ofstream ofs("staff_data.txt", ios::out);
    if (!ofs)
    {
        cout << "Unable to open file for writing";
    }

    ofs.close();
    cout << "File has been cleared" << endl;
}

int main()
{
    Staff *staff[10];
    int count = 0;
    cout << endl
         << "\tStaff Management System"
         << endl;

    int choice;
    do
    {
        cout << endl
             << "1. Add Staff" << endl
             << "2. Display Staff" << endl
             << "3. Save to File" << endl
             << "4. Load from File" << endl
             << "5. Search Staff" << endl
             << "6. Clear File Data" << endl
             << "7. Exit" << endl
             << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addStaff(staff, count);
            break;
        case 2:
            displayStaff(staff, count);
            break;
        case 3:
            saveToFile(staff, count);
            break;
        case 4:
            loadFromFile(staff, count);
            displayStaff(staff, count); // Automatically display after loading
            break;
        case 5:
            searchStaff(staff, count);
            break;
        case 6:
            clearFile();
            break;
        case 7:
            cout << "Exiting...." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl
                 << "Please try again!" << endl;
            break;
        }
    } while (choice != 7);

    for (int i = 0; i < count; i++)
    {
        delete staff[i];
    }

    return 0;
}
