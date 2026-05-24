#include <iostream>
#include <vector>
using namespace std;

// ================= BASE CLASS =================
class Account {
protected:
    int accNo;
    string name;
    int pin;
    string nid;
    double balance;
    vector<string> history;

public:
    static int nextAccNo;

    Account() {}

    Account(string n, int p, string id) {
        accNo = nextAccNo++;
        name = n;
        pin = p;
        nid = id;
        balance = 0;
    }

    int getAccNo() { return accNo; }
    int getPin() { return pin; }
    string getNID() { return nid; }
    double getBalance() { return balance; }

    virtual void showMenu() = 0;

    void deposit(double amount) {
        balance += amount;
        history.push_back("Deposit: " + to_string(amount));
        cout << "Deposit Successful\n";
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            history.push_back("Withdraw: " + to_string(amount));
            cout << "Withdraw Successful\n";
        } else {
            cout << "Insufficient Balance\n";
        }
    }

    void transfer(Account &to, double amount) {
        if (amount <= balance) {
            balance -= amount;
            to.balance += amount;

            history.push_back("Transfer to " + to_string(to.accNo));
            to.history.push_back("Received from " + to_string(accNo));

            cout << "Transfer Successful\n";
        } else {
            cout << "Insufficient Balance\n";
        }
    }

    void showDetails() {
        cout << "\nAccount Details\n";
        cout << "Acc No: " << accNo << endl;
        cout << "Name: " << name << endl;
        cout << "Balance: " << balance << endl;
    }

    void showHistory() {
        cout << "\nTransaction History\n";
        for (string h : history)
            cout << h << endl;
    }
};

int Account::nextAccNo = 25204001;


// ================= USER CLASS =================
class User : public Account {
public:
    User(string n, int p, string id) : Account(n, p, id) {}

    void showMenu() {
        cout << "\nUser Menu\n";
        cout << "1. Withdraw\n";
        cout << "2. Transfer\n";
        cout << "3. Balance\n";
        cout << "4. Details\n";
        cout << "5. History\n";
        cout << "6. Logout\n";
        cout << "Choice: ";
    }
};


// ================= ADMIN CLASS =================
class Admin {
private:
    string username;
    string password;

public:
    Admin(string u, string p) {
        username = u;
        password = p;
    }

    string getUsername() { return username; }

    bool login(string u, string p) {
        return (u == username && p == password);
    }
};


// ================= BANK CLASS =================
class Bank {
private:
    vector<User> users;
    vector<Admin> admins;

public:
    Bank() {
        admins.push_back(Admin("admin", "1234"));
    }

    bool adminLogin() {
        string u, p;

        cout << "Username: ";
        cin >> u;

        cout << "Password: ";
        cin >> p;

        for (auto &a : admins) {
            if (a.login(u, p)) {
                return true;
            }
        }

        cout << "Invalid Admin Login\n";
        return false;
    }

    void createAdmin() {
        string u, p;

        cout << "New Admin Username: ";
        cin >> u;

        for (auto &a : admins) {
            if (a.getUsername() == u) {
                cout << "Username already exists\n";
                return;
            }
        }

        cout << "Password: ";
        cin >> p;

        admins.push_back(Admin(u, p));
        cout << "New Admin Created\n";
    }

    bool nidExists(string nid) {
        for (auto &u : users)
            if (u.getNID() == nid)
                return true;
        return false;
    }

    void registerUser() {
        string name, nid;
        int pin;

        cin.ignore();
        cout << "Name: ";
        getline(cin, name);

        cout << "NID: ";
        cin >> nid;

        if (nidExists(nid)) {
            cout << "Account already exists with this NID\n";
            return;
        }

        cout << "PIN: ";
        cin >> pin;

        User u(name, pin, nid);
        cout << "Account Created Acc No: " << u.getAccNo() << endl;

        users.push_back(u);
    }

    void adminCreateAccount() {
        string name, nid;
        int pin;

        cin.ignore();
        cout << "Name: ";
        getline(cin, name);

        cout << "NID: ";
        cin >> nid;

        if (nidExists(nid)) {
            cout << "NID already exists\n";
            return;
        }

        cout << "PIN: ";
        cin >> pin;

        User u(name, pin, nid);
        users.push_back(u);

        cout << "Account Created by Admin\n";
    }

    User* userLogin() {
        int accNo, pin;

        cout << "Account No: ";
        cin >> accNo;

        cout << "PIN: ";
        cin >> pin;

        for (auto &u : users) {
            if (u.getAccNo() == accNo && u.getPin() == pin)
                return &u;
        }

        cout << "Invalid Login\n";
        return nullptr;
    }

    User* findUser(int accNo) {
        for (auto &u : users)
            if (u.getAccNo() == accNo)
                return &u;
        return nullptr;
    }

    void adminDeposit() {
        int accNo;
        double amount;

        cout << "Account No: ";
        cin >> accNo;

        User* u = findUser(accNo);

        if (u) {
            cout << "Amount: ";
            cin >> amount;
            u->deposit(amount);
        } else {
            cout << "Account Not Found\n";
        }
    }
};


// ================= MAIN =================
int main() {
    Bank bank;

    while (true) {
        int choice;

        cout << "\nBank System\n";
        cout << "1. Admin Login\n";
        cout << "2. User Login\n";
        cout << "3. Registration\n";
        cout << "4. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            if (bank.adminLogin()) {
                int op;
                while (true) {
                    cout << "\nAdmin Menu\n";
                    cout << "1. Create Account\n";
                    cout << "2. Deposit\n";
                    cout << "3. Create Admin\n";
                    cout << "4. Logout\n";
                    cout << "Choice: ";
                    cin >> op;

                    if (op == 1) bank.adminCreateAccount();
                    else if (op == 2) bank.adminDeposit();
                    else if (op == 3) bank.createAdmin();
                    else break;
                }
            }
        }

        else if (choice == 2) {
            User* user = bank.userLogin();

            if (user) {
                while (true) {
                    user->showMenu();
                    int op;
                    cin >> op;

                    if (op == 1) {
                        double amt;
                        cout << "Amount: ";
                        cin >> amt;
                        user->withdraw(amt);
                    }

                    else if (op == 2) {
                        int to;
                        double amt;

                        cout << "Transfer To: ";
                        cin >> to;
                        cout << "Amount: ";
                        cin >> amt;

                        User* rec = bank.findUser(to);

                        if (rec)
                            user->transfer(*rec, amt);
                        else
                            cout << "Account Not Found\n";
                    }

                    else if (op == 3) {
                        cout << "Balance: " << user->getBalance() << endl;
                    }

                    else if (op == 4) {
                        user->showDetails();
                    }

                    else if (op == 5) {
                        user->showHistory();
                    }

                    else break;
                }
            }
        }

        else if (choice == 3) {
            bank.registerUser();
        }

        else if (choice == 4) {
            cout << "Goodbye\n";
            break;
        }
    }

    return 0;
}