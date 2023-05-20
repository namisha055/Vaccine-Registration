#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class User
{
	public:
    string name;
    int age;
    string address;
    const string vaccineType[2] = {"covishield", "covaxin"};
    string status;
};

class Admin
{
	public:
    string username;
    string password;
};

void registerUser()
{
    User user;
    cout << "Please enter your name: ";
    getline(cin, user.name);
    cout << "Please enter your age: ";
    cin >> user.age;
    if (user.age < 18)
    {
        cout << "\nYou are not eligible!\n\n";
        return;
    }
    cin.ignore();
    cout << "Please enter your address: ";
    getline(cin, user.address);
    cout << "Please enter the type of vaccine you want \n1. Covishield\n2. Covaxin\n Enter your choice: ";
    int choice;
    cin >> choice;
    if (choice != 1 && choice != 2)
    {
        cout << "\n Please choose valid vaccine type!\n\n";
        return;
    }

    user.status = "Pending"; 

    ofstream outFile("users.txt", ios::app);
    outFile << user.name << "," << user.age << "," << user.address << "," << user.vaccineType[choice-1] << "," << user.status << endl;
    outFile.close();

    cout << "Registration successful. Your status is currently pending.\n";
}

void checkStatus(string name)
{
    string line;
    ifstream inFile("users.txt");
    while (getline(inFile, line))
    {
        char *str = new char[line.length() + 1];
        strcpy(str, line.c_str());
        char *token = strtok(str, ",");
        if (token == name)
        {
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            string status = strtok(NULL, ",");
            cout << "Your registration status is: " << status << endl;
            return;
        }
    }
    cout << "No record found for this name.\n";
}

bool adminLogin(Admin admin)
{
    string username, password;
    cout << "Please enter username: ";
    cin >> username;
    cout << "Please enter password: ";
    cin >> password;
    if (admin.username == username && admin.password == password)
    {
        return true;
    }
    return false;
}

void updateUserStatus()
{
    string name;
    cout << "Please enter user name: ";
    getline(cin, name);
    string line;
    ifstream inFile("users.txt");
    ofstream outFile("temp.txt", ios::app);
    bool found = false;
    while (getline(inFile, line))
    {
        char *str = new char[line.length() + 1];
        strcpy(str, line.c_str());
        char *token = strtok(str, ",");
        if (token == name)
        {
            found = true;
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
           token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            string status;
            cout << "Enter new status (Approved/Rejected): ";
            cin >> status;
            outFile << name << "," << token << "," << token << "," << token << "," << status << endl;
            cout << "Status updated successfully.\n";
        }
        else
        {
            outFile << line << endl;
        }
    }
    inFile.close();
    outFile.close();
    if (!found)
    {
        cout << "No record found for this name.\n";
        return;
    }
    remove("users.txt");
    rename("temp.txt", "users.txt");
}

int main()
{
    int choice;
    bool adminLoggedIn = false;
    Admin admin;
    admin.username = "namisha";
    admin.password = "123";
    while (true)
    {
        cout << "Please select an option:\n";
        cout << "1. Register for vaccine (Only for 18 and above years old!!! )\n";
        cout << "2. Check registration status\n";
        cout << "3. Admin login\n";
        cout << "4. Exit\n";
        cin >> choice;
        cin.ignore();
        switch (choice)
        {
        case 1:
            registerUser();
            break;
        case 2:
        {
            string name;
            cout << "Please enter your name: ";
            getline(cin, name);
            checkStatus(name);
            break;
        }
        case 3:
        {
            if (adminLoggedIn)
            {
                cout << "You are already logged in as admin.\n";
            }
            else
            {
                if (adminLogin(admin))
                {
                    adminLoggedIn = true;
                    cout << "Admin login successful.\n";
                }
                else
                {
                    cout << "Invalid username or password.\n";
                }
            }
            break;
        }
        case 4:
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        if (adminLoggedIn)
        {
            cout << "\nAdmin menu:\n";
            cout << "1. Update user status\n";
            cout << "2. Logout\n";
            cin >> choice;
            cin.ignore();
            switch (choice)
            {
            case 1:
                updateUserStatus();
                break;
            case 2:
                adminLoggedIn = false;
                cout << "Admin logout successful.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
}
