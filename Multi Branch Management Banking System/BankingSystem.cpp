#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstring>
#include <limits>
#include <iomanip>
#include <sstream>

using namespace std;
//Forward delcarations
class Customer; 
extern vector<Customer> allCustomers; //Global customer List
void exportToCSV();
void exportToTest();
void saveSystemConfig();
//Stores a single transaction log
struct TransactionRecord {
    char description[100];//what happened
    double amount;//how much
    char date[20];//when
};

struct staffRecord {
    char staffID[20]; //T001 or M001
    char fullName[50];//The name of who ever it is
    char passwordHash[64]; //encrypted password 
    char branchCode[10];//"018105"
    bool isManager; //true = manager
};
//Prints a double as currency with 2 demical places
void printMoney(double amount) {
    cout << fixed << setprecision(2) << amount;
}
//Coverts a string to all lowercase (when searching the name)
string toLower(string s) {
    for (char& c : s) c = tolower(c);
    return s;
}
//Shifts each character forward by 1
string encryptStr(string raw) {
    for (int i = 0; i < (int)raw.length(); i++) raw[i]++;
    return raw;
}

//Returns current date/time as a string
string currentDateTime() {
    time_t now = time(0);
    char buf[20];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", localtime(&now));
    return string(buf);
}
//Returns current date in the format mentioned in the assignment
string currentDate() {
    time_t now = time(0);
    char buf[12];
    strftime(buf, sizeof(buf), "%d/%m/%Y", localtime(&now));
    return string(buf);
}

// Safe integer input with validation
int safeIntInput() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    return val;
}

class Branch {
private: 
    char branchName [50]; //E.g Sandton
    char branchCode[10];//e.g 018105
    char branchAddress[100];//Physical Address

public:
    double lastReportBalance;//Stores balance from last run
    //Initialises branch with name, code, address
    Branch(string name, string code, string address) {
        strncpy(this->branchName, name.c_str(), 49);
        branchName[49] = '\0';
        strncpy(this->branchCode, code.c_str(), 9);
        branchCode[9] = '\0';
        strncpy(branchAddress, address.c_str(), 99);
        branchAddress[99] = '\0';
        lastReportBalance = 0.0;
    }//Appends this branchs data to the .dat file
    void saveBranchBinary() {
        ofstream outFile("branch.dat", ios::binary | ios::app);
        if(outFile) {
            outFile.write(reinterpret_cast<char*>(this), sizeof(*this));
        }
    }
    double calBalance(const vector<Customer>& customers);
    string getCode() { return branchCode; }
    string getName() { return branchName; }
    string getAddress() { return branchAddress; }
};
//Global Network
vector<Branch> bankNetwork;
//Parents + children class
class Account{
protected: //The kids class can see it, but not others
    string accountNumber;
    double balance;

public:
    Account(string accNum, double initialDep) : accountNumber(accNum), balance(initialDep) {}
    virtual ~Account() {}//Virtual destruvtor ensures correct cleanup of derived classes
    //Pure virtual functions
    virtual string getAccountType() = 0;
    virtual double calculateInterest() = 0;
    virtual double getMinimumDeposit() = 0;
    //Standard deposit for all accounts
    void deposit(double amount) { balance += amount; }
        //Default withdrawal can be overridden
    virtual bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }
    double getBalance() { return balance; }
    string getAccNum() { return accountNumber; }
};
//Savings Account
class SavingsAccount : public Account {
public:
    SavingsAccount(string accNum, double dep) : Account(accNum, dep) {}
    string getAccountType() override { return "Savings"; }
    double calculateInterest() override { return balance * 0.0425; } //Interest rate
    double getMinimumDeposit() override { return 100.0; } //minimum deposit
};
//Cheque Account
class ChequeAccount : public Account {
public:
    ChequeAccount(string accNum, double dep) : Account(accNum, dep) {}
    string getAccountType() override { return "Cheque"; }
    double calculateInterest() override { return 0.0; } //Interest rate
    double getMinimumDeposit() override { return 100.0; }//Minimum deposit
};
//Student Account
class StudentAccount : public Account {
public:
    StudentAccount(string accNum, double dep) : Account(accNum, dep) {}
    string getAccountType() override { return "Student (Zero Fees)"; }
    double calculateInterest() override { return balance * 0.0475; }//Interest rate
    double getMinimumDeposit() override { return 50.0; }//Minimum Deposit
};
//Fixed Deposit Account
class FixedDepositAccount : public Account {
public:
    FixedDepositAccount(string accNum, double dep) : Account(accNum, dep) {}
    string getAccountType() override { return "Fixed Deposit"; }
    double calculateInterest() override { return balance * 0.0805; }//Interest Rate
    double getMinimumDeposit() override { return 1000.0; }//minimum deposit
    //Override withdraw to allow a 10% early penatly
    bool withdraw(double amount) override {
        cout << "Early withdrawal penalty of 10% applies to Fixed Deposit!" << endl;
        double penalty = amount * 0.10;
        double total = amount + penalty;
        if (total > balance) {
            cout << "Insufficient funds including penatly." << endl;
            return false;
        }
        balance -= total;
        return true;
    }
};
//Customers Class
class Customer{
private://Personal Details
    string name;
    string idNumber;
    string contactNumber;
    string email;
    string address;
    string dateOfBirth;
    string branchCode;
    Account* myAccount;
    int pinHash;    //Generating 5-digit PIN + Storing encrypted version
    int loginAttempts;  //Track Failures
    bool isLocked;  //Handle Lockout
    vector<TransactionRecord> history; //Stores a list of all actions
    //Simple PIN encryption formula
    int encryptPin(int rawPin) { return (rawPin * 3) + 7; }

public:
    //Frees the dynamically allocated Account objects to prevent memory leaks
    ~Customer() {
        delete myAccount;
        myAccount = nullptr;
    }
    //Does all with the personal details
    Customer(string n, string id, string contact, string email, string address,
        string dob, string bCode, Account* acc)
    : name(n), idNumber(id), contactNumber(contact), email(email), address(address),
    dateOfBirth(dob), branchCode(bCode), myAccount(acc), loginAttempts(0), isLocked(false) {
        int rawPin; bool isUnique; int attempts = 0;
        //Formula to generate a PIN
        do {
            isUnique = true;
            rawPin = (rand() % 90000) + 10000;
            for (Customer& c : allCustomers) {
                if (c.getPin() == rawPin) {
                    isUnique = false;
                    break;
                }
            }//Prevents infinite loop if system is at max capacity
            attempts++;
            if (attempts > 100000) {
                throw runtime_error("Could not generate a unqiue PIN. Too many customers.");
            }
        } while (!isUnique);
        //Encrypt it immediately for storage
        pinHash = encryptPin(rawPin);
        //Only show the raw PIN once
        cout << "Account created! Your 5-digit PIN is: " << rawPin << endl;
    }//Constructor LOADER - restoring from files
    Customer(string n, string id, string contact, string email, string address,
        string dob, string bCode, Account* acc, int savedHash, bool savedLocked)
    : name(n), idNumber(id), contactNumber(contact), email(email), address(address),
    dateOfBirth(dob), branchCode(bCode), myAccount(acc), pinHash(savedHash), loginAttempts(0), isLocked(savedLocked) {}
    //Deep copy constructor - require because customer owns a raw pointer
    Customer(const Customer& other) : name(other.name), idNumber(other.idNumber), contactNumber(other.contactNumber),
    email(other.email), address(other.address), dateOfBirth(other.dateOfBirth),
    branchCode(other.branchCode), pinHash(other.pinHash), loginAttempts(other.loginAttempts), 
    isLocked(other.isLocked), history(other.history) {
        //create a brand new account of the correct type
        string type = other.myAccount->getAccountType();
        double bal = other.myAccount->getBalance();
        string accNum = other.myAccount->getAccNum();
        if (type.find("Savings") != string::npos) myAccount = new SavingsAccount(accNum, bal);
        else if (type.find("Cheque") != string::npos) myAccount = new ChequeAccount(accNum, bal);
        else if (type.find("Fixed") != string::npos) myAccount = new FixedDepositAccount(accNum, bal);
        else                                        myAccount = new StudentAccount(accNum, bal);
    }//Same things as the copy constructor - needed for safe vector operation
    Customer& operator=(const Customer& other) {
        if (this == &other) return *this; //Self-assignment guard
        delete myAccount; //Free existing accounts before replacing
        name = other.name; idNumber = other.idNumber; contactNumber = (other.contactNumber);
        email = (other.email); address = (other.address); pinHash = other.pinHash; 
        dateOfBirth = (other.dateOfBirth); loginAttempts = other.loginAttempts;
        branchCode = (other.branchCode); isLocked = other.isLocked;
        history = other.history;
        string type = other.myAccount->getAccountType();
        double bal = other.myAccount->getBalance();
        string accNum = other.myAccount->getAccNum();
        if (type.find("Savings") != string::npos) myAccount = new SavingsAccount(accNum, bal);
        else if (type.find("Cheque") != string::npos) myAccount = new ChequeAccount(accNum, bal);
        else if (type.find("Fixed") != string::npos) myAccount = new FixedDepositAccount(accNum, bal);
        else                                        myAccount = new StudentAccount(accNum, bal);
        return *this;
    }
    
    //Customer Authentication Logic
    bool verifyCustomerLogin(string enteredAcc, int enteredPin) {
        if (isLocked) {
            cout << "This account is locked due to 3 failed attempts." << endl;
                return false;
        }
        if (enteredAcc == myAccount->getAccNum() && encryptPin(enteredPin) == pinHash) {
                loginAttempts = 0;
                return true;
        }//Faild attempts
        loginAttempts++;
        cout << "Invaild Pin. Attempt " << loginAttempts << " of 3." << endl;
        if (loginAttempts >= 3 ) { 
            isLocked = true;
            cout << "Account LOCKED for security." << endl;
        }
        return false;
    }
    //Use try-catch to vaildate amount before depositing
    void deposit(double amount) {
        try{
            if (amount <= 0) throw string("Deposit amount must be greater than zero.");
            myAccount->deposit(amount);
            logTransaction("Deposited: R" + to_string(amount), amount);
            cout << "Deposit successful. New Balance: R"; printMoney(getBalance()); cout << endl;
        }catch (string error) {
            cout << "\a" << error << endl;
        }
        
    }
//Validates amount ans checks balance before withdrawing
    bool withdraw(double amount) {
        try {
            if (amount <= 0) throw string("Withdrawal amount must be positive.");
            if (amount > myAccount->getBalance()) throw string("Insufficient funds in account.");
            //perform math
            myAccount->withdraw(amount);//May apply penatly
            logTransaction("Withdrew: R" + to_string(amount), amount);
            cout << "Withdrawal successful. New Balance: R"; printMoney(getBalance()); cout << endl;
            return true;
        }catch (string error) {
            cout << "\a" << error << endl; // \a makes a beep sound
            return false;
        }            
    }
//Logs the transaction on both accounts and more.
     bool transfer(double amount, Customer& recipient) {
        try{
            if (amount <= 0) throw string("Transfer amount must be positive.");
            if (amount > myAccount->getBalance()) throw string("Insufficinent funds for transfer.");
            myAccount->withdraw(amount);
            recipient.myAccount->deposit(amount);
            logTransaction("Transfer Out to " + recipient.getAccountNumber(), amount);
            recipient.logTransaction("Transfer In from " + getAccountNumber(), amount);
            return true;
        }catch (string error) { cout << "\a" << error << endl; return false;}
    }
//Requires the current PIN, then validates and sets the new one
    void changePin() {
        cout << "\n----- Change PIN -----" << endl;
        int oldPin;
        cout << "Enter current PIN: "; oldPin = safeIntInput();
        if (encryptPin(oldPin) != pinHash) {
            cout << "Incorrect current PIN." << endl;
            return;
        } 
        int p1, p2;
        cout << "Do not make the PIN easy for people to guess it!" << endl;
        cout << "Enter new 5-digit PIN: ";
        p1 = safeIntInput();
        cout << "Confirm new PIN: ";
        p2 = safeIntInput();
        if (p1 != p2) { cout << "PINs do not match." << endl; return;}
        //Uniqueness check
        if (p1 < 10000 || p1 > 99999) {
            cout << "PIN must be exactly 5 digits." << endl; return; } 
        for (Customer& c : allCustomers)
        if (c.getAccountNumber() != getAccountNumber() && c.getPin() == p1) {
            cout << "PIN already in use. Choose a different PIN." << endl;
            return;
        }
        pinHash = encryptPin(p1);
        cout << "PIN changed successfully!" << endl;
    }
    //Prints the full transaction history with formatted columns
    void printStatement() {
        cout << "\n========== Account Statement ===========" << endl;
        cout << "Account: " << getAccountNumber() << endl;
        cout << "Name: " << name << endl;
        cout << "Type: " << myAccount->getAccountType() << endl;
        cout << "Balance: "; printMoney(getBalance()); cout << endl;
        cout << "-------------------------------------------" << endl;
        if(history.empty()) { //Columns Headers
            cout << "No transaction on record." << endl;
            return;
        }
        cout << left << setw(45) << "Description" << setw(12) << "Amount" << "Date" << endl;
        cout << "--------------------------------------------" << endl;
        for (const auto & tr : history) {
            cout << left << setw(45) << tr.description << ": R" << setw(11) << fixed << setprecision(2) << tr.amount << tr.date << endl;
        }
        cout << "---------------------------------------------" << endl;  
    }
    //Full Customer record display for teller/manager use
    void displayInfo() {
        cout << "\n====== Customer Record ======" << endl;
        cout << "Name: " << name << endl;
        cout << "SA ID: " << idNumber << endl;
        cout << "Contact: " << contactNumber << endl;
        cout << "Email: " << email << endl;
        cout << "Date of Birth: " << dateOfBirth << endl;
        cout << "Address: " << address << endl;
        cout << "Account Number: " << getAccountNumber() << endl;
        cout << "Account Type: " << myAccount->getAccountType() << endl;
        cout << "Branch: " << branchCode << endl;
        cout << "Balance: R"; printMoney(getBalance()); cout << endl;
        cout << "Status: " << (isLocked ? "LOCKED" : "ACTIVE") << endl; 
    }
    //Searches by name even in the lower case
    Customer* findTransferRecipient(const string& senderAcc) {
        string searchName;
        cout << "Enter Recipient Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, searchName);
        string lowerSearch = toLower(searchName);
        //Linear search through all customers
        vector<int> matches;
        for(int i = 0; i < (int)allCustomers.size(); i++)
        if (toLower(allCustomers[i].getName()).find(lowerSearch) != string::npos && allCustomers[i].getAccountNumber() != senderAcc)
        matches.push_back(i);
        if(matches.empty()) { cout << "No matching customers found." << endl; return nullptr; }
        if (matches.size() == 1) { //Exactly one match
            Customer& c = allCustomers[matches[0]];
            cout << "Recipient: " << c.getName() << " | Account: " << c.getAccountNumber() << endl;
            return &c;
        }
        //Displays all and confirms by ccount number
        cout << "\nMultiple matches found." << endl;
        for (int i = 0; i < (int)matches.size(); i++)
        cout << i+1 << ". " << allCustomers[matches[i]].getName() << " | "
        << allCustomers[matches[i]].getAccountNumber() << endl;
        cout << "Enter Account Number to confirm: ";
        string accInput; cin >> accInput;
        for (int idx : matches)
            if (allCustomers[idx].getAccountNumber() == accInput) return &allCustomers[idx];
            cout <<"Not found in list." << endl;
            return nullptr;
    }
    //Saves the Customers
    void saveCustomerBinary() {
        ofstream outFile("customer_single.dat", ios::binary | ios::app);
        if (outFile.is_open()) {
            saveBinary(outFile);
            outFile.close();
        }
    } 
    //Customers do not need a teller or manager
    //They can help themselves out with this function.
    void customerSession() {
        int subChoice = 0;
        while (subChoice != 5) {
            cout << "\n----- Customer Self Service Session: " << name << " -----" << endl;
            cout << "1. View Balance" << endl;
            cout << "2. Deposit" << endl;
            cout << "3. Withdraw" << endl;
            cout << "4. Fund Transfer" << endl;
            cout << "5. View Statement" << endl;
            cout << "6. Change PIN" << endl;
            cout << "7. Logout" << endl;
            cout << "What do you choose?: ";
            subChoice = safeIntInput();

             if (subChoice == 1) {
                cout << "Current Balance: R"; printMoney(getBalance()); cout << endl;
            } else if (subChoice == 2) {
                double dep;
                cout << "Enter Deposit: R"; cin >> dep;
                deposit(dep);
            } else if (subChoice == 3) {
                double wth;
                cout << "How much do you want withdraw: "; cin >> wth;
                withdraw(wth);
            } else if (subChoice == 4) {
                Customer* r = findTransferRecipient(getAccountNumber());
                if (r) {
                    double amount; cout << "Amount: R"; cin >> amount;
                    if (transfer(amount, *r)) cout << "Transfer Successful!" << endl;
                }
            } else if (subChoice == 5) {
                printStatement();
            } else if (subChoice == 6) {
                changePin();
            } else if (subChoice == 7) {
                cout << "Session ended. Thank you for using Standard Bank." << endl;
                break;
            }
        } 
    }   
    //Adds to in-memory history and appends to transactions.dat
   void logTransaction(string desc, double amt) {
        TransactionRecord tr;
        strncpy(tr.description, desc.c_str(), 99); tr.description[99] = '\0';
        tr.amount = amt;
        strncpy(tr.date, currentDateTime().c_str(), 19); tr.date[19] = '\0';
        history.push_back(tr);
        //Append to Binary transaction log file
        ofstream out("transactions.dat", ios::binary | ios::app);
        if (out) out.write(reinterpret_cast<char*>(&tr), sizeof(tr));
    }
    //Writes all customer field to an open binary file stream.
    void saveBinary(ofstream& out) {//Lambda to write as a fixed-size char buffer
        auto writeStr = [&](const string& s, int maxLen) {
            vector<char> buf(maxLen, '\0');
            strncpy(buf.data(),s.c_str(), maxLen - 1);
            out.write(buf.data(), maxLen);
        };
        writeStr(name, 50);
        writeStr(idNumber, 14);
        writeStr(contactNumber, 11);
        writeStr(email, 60);
        writeStr(address, 100);
        writeStr(dateOfBirth, 11);
        writeStr(branchCode, 10);
        out.write(reinterpret_cast<char*>(&pinHash), sizeof(pinHash));
        out.write(reinterpret_cast<char*>(&isLocked), sizeof(isLocked));
        double bal = myAccount->getBalance();
        out.write(reinterpret_cast<char*>(&bal), sizeof(bal));
        writeStr(myAccount->getAccountType(), 30);
        writeStr(myAccount->getAccNum(), 30);
    }
    //these are the GETTERS
    string getAccountNumber() const { return myAccount->getAccNum(); }
    double getBalance() const { return myAccount->getBalance(); }
    string getName() const { return name; }
    string getBranchCode() const { return branchCode; }
    string getIDNumber() const { return idNumber; }
    string getEmail() const { return email; }
    string getDOB() const { return dateOfBirth; }
    string getAccountType() const { return myAccount->getAccountType(); }
    //Decrypts the stored PIN hash back to raw PIN.
    int getPin() { return (pinHash - 7) / 3; } 
    Account* getAccountPointer() { return myAccount; }
    //Unlocks a locked account and resets failed attempt counter
    void setLock(bool status) { isLocked = status; loginAttempts = 0; }
    bool getLocked() const { return isLocked; } 
    const vector<TransactionRecord>& getHistory() const { return history; }
 
};
//Defined here after customers because it references customer.
double Branch::calBalance(const vector<Customer>& allCustomers){
    double total = 0;
    for (const auto& c : allCustomers)  //Find customers string contains this branch code
        if(c.getAccountNumber().find(branchCode) != string::npos) 
            total += c.getBalance();
            return total;    
};

vector<Customer> allCustomers;//Global customer list
//Appends a single StaffRecord to tellers.dat
void saveStaffRecord(const staffRecord& sr) {
    ofstream out ("teller.dat", ios::binary | ios::app);
    if (out) out.write(reinterpret_cast<const char*>(&sr), sizeof(staffRecord));
}
//Reads all staffRecord from tellers.dat into a vector
vector<staffRecord> loadAllStaff() {
    vector<staffRecord> staff;
    ifstream in("tellers.dat", ios::binary);
    if (!in) return staff;
    staffRecord sr;
    while (in.read(reinterpret_cast<char*>(&sr), sizeof(staffRecord)))
    {
        staff.push_back(sr);
    } return staff;
}
//Overwrites tellers.dat with an updated vector
void rewriteStaffFile(const vector<staffRecord>& staff) {
    ofstream out ("tellers.dat", ios::binary | ios::trunc);
    for (const auto& sr : staff)
        out.write(reinterpret_cast<const char*>(&sr), sizeof(staffRecord));
}
//seeds tellers.dat with 3 branch managers and 10 tellers on first run.
void seedStaffIfEmpty() {
    ifstream check ("tellers.dat", ios::binary);
    if (check.good()) {
        check.seekg(0, ios::end);
        if (check.tellg() > 0) return;
    } 
    check.close();

    cout << "Seeding staff credentials..." << endl;
    //Seed Data and there names and such
    struct SeedData { const char* id; const char* name; const char* pass; const char* branch; bool mgr; };
    SeedData seed[] = {
        //Branch Managers
        {"M001", "Sipho Dlamini", "Manager001!", "018105", true},
        {"M002", "Priya Naidoo", "Manager002!", "055034", true},
        {"M003", "Anke van der Berg","Manager003!", "025309", true},
        // Tellers
        {"T001", "Lerato Mokoena", "Teller001!", "018105", false},
        {"T002", "James Sithole", "Teller002!", "018105", false},
        {"T003", "Fatima Osman", "Teller003!", "018105", false},
        {"T004", "Thabo Khumalo", "Teller004!", "018105", false},
        {"T005", "Rudo Mhlanga", "Teller005!", "055034", false},
        {"T006", "Nomsa Zulu", "Teller006!", "055034", false},
        {"T007", "Deon Pretorius", "Teller007!", "055034", false},
        {"T008", "Ayasha Pillay", "Teller008!", "025309", false},
        {"T009", "Kagiso Sithole", "Teller009!", "025309", false},
        {"T010", "Mia Fourie", "Teller010!", "025309", false}
    };
    
    for (auto& s : seed) {
        staffRecord sr;
        strncpy(sr.staffID, s.id, 19); sr.staffID[19] = '\0';
        strncpy(sr.fullName, s.name, 49); sr.fullName[49] = '\0';
        strncpy(sr.branchCode, s.branch, 9); sr.branchCode[9] = '\0';
        string h = encryptStr(string(s.pass));//Encrypt before storing
        strncpy(sr.passwordHash, h.c_str(), 63); sr.passwordHash[63] = '\0';
        sr.isManager = s.mgr;
        saveStaffRecord(sr);
    }
    cout << "3 managers + 10 tellers." << endl;
}
//Writes all customers to customers.dat in binary format
void saveAllData() {
    ofstream out("customer.dat", ios::binary);
    if (!out) return;
    size_t count = allCustomers.size();
    out.write(reinterpret_cast<char*>(&count), sizeof (count));
    for (auto& c : allCustomers) c.saveBinary(out);
}
//Reads all customers from customers.dat.
void loadAllData() {
    ifstream in("customers.dat", ios::binary);
    if (!in) {
        cout << "Main data missing. Attempting backup recovery..." << endl;
        in.open("customers_backup.dat", ios::binary);
    }
    if (!in) { cout << "No customer data. Starting fresh." << endl; return; }

    size_t count;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (!in) return;

    for (size_t i = 0; i < count; ++i) {
        char n[50], id[14], cont[11], email[60], address[100], dob[11], bCode[10];
        char t[30], an[30];
        int ph; bool lk; double bl;

        in.read(n, 50); in.read(id, 14); in.read(cont, 11);
        in.read(email, 60); in.read(address,100); in.read(dob,  11);
        in.read(bCode, 10);
        in.read(reinterpret_cast<char*>(&ph), sizeof(ph));
        in.read(reinterpret_cast<char*>(&lk), sizeof(lk));
        in.read(reinterpret_cast<char*>(&bl), sizeof(bl));
        in.read(t, 30); in.read(an, 30);
        if (!in) break;
        //Recreate the correct Account subtype based on store typr string
        Account* acc = nullptr;
        string tp(t);
        if      (tp.find("Savings") != string::npos) acc = new SavingsAccount(an, bl);
        else if (tp.find("Cheque")  != string::npos) acc = new ChequeAccount(an, bl);
        else if (tp.find("Fixed")   != string::npos) acc = new FixedDepositAccount(an, bl);
        else  acc = new StudentAccount(an, bl);
        //Use loader constructor
        allCustomers.push_back(Customer(
            string(n), string(id), string(cont), string(email),
            string(address), string(dob), string(bCode), acc, ph, lk));
    }
    cout << "Loaded " << allCustomers.size() << " customer(s)." << endl;
}
//Creates a customers_backup.dat as a copy of customers.dat
void performBackup() {
    ifstream src("customers.dat", ios::binary);
    ofstream dst("customers_backup.dat", ios::binary);
    if (src && dst) { dst << src.rdbuf(); cout << "Backup saved to customers_backup.dat" << endl; }
}
//Exports all customers to customer_export.csv
void exportToCSV() {
    ofstream f("customer_export.csv");
    if (!f) { cout << "Could not create CSV file." << endl; return; }
    f << "Name,ID Number,Contact,Email,DOB,Account Number,Account Type,Branch,Balance\n";
    for (Customer& c : allCustomers)
        f << c.getName() << ","
          << c.getIDNumber() << ","
          << c.getEmail() << ","
          << c.getDOB() << ","
          << c.getAccountNumber() << ","
          << c.getAccountType() << ","
          << c.getBranchCode() << ","
          << fixed << setprecision(2) << c.getBalance() << "\n";
    cout << "Exported to customer_export.csv" << endl;
}
//Exports all customers to customers_export.txt in a readable format
void exportToText() {
    ofstream f("customer_export.txt");
    if (!f) { cout << "Could not create text file." << endl; return; }
    f << "====================================================\n";
    f << "     STANDARD BANK — CUSTOMER ACCOUNT SUMMARY\n";
    f << "     Generated: " << currentDateTime() << "\n";
    f << "====================================================\n\n";
    for (Customer& c : allCustomers) {
        f << "Name: " << c.getName() << "\n";
        f << "ID Number: " << c.getIDNumber() << "\n";
        f << "Email: " << c.getEmail() << "\n";
        f << "Date of Birth: " << c.getDOB() << "\n";
        f << "Account No: " << c.getAccountNumber() << "\n";
        f << "Account Type: " << c.getAccountType() << "\n";
        f << "Branch: " << c.getBranchCode() << "\n";
        f << "Balance: R" << fixed << setprecision(2) << c.getBalance() << "\n";
        f << "Status: " << (c.getLocked() ? "LOCKED" : "Active") << "\n";
        f << "----------------------------------------------------\n";
    }
    cout << "Exported to customer_export.txt" << endl;
}
//System_Config.txt file
void saveSystemConfig(const string& lastUser = "") {
    ofstream f("system_config.txt");
    if (!f) return;
    f << "# Standard Bank System Configuration\n";
    f << "System_Name : Standard Bank Multi-Branch System\n";
    f << "Version: 1.0\n";
    f << "Currency: ZAR (South African Rand)\n";
    f << "Branches: " << bankNetwork.size() << "\n";
    f << "Total_Customers : " << allCustomers.size() << "\n";
    f << "Last_User: " << lastUser << "\n";
    f << "Last_Updated: " << currentDateTime() << "\n";
    f << "Backup_File: customers_backup.dat\n";
    f << "Min_Deposit_Savings: R100.00\n";
    f << "Min_Deposit_Cheque: R100.00\n";
    f << "Min_Deposit_FixedDeposit: R1000.00\n";
    f << "Min_Deposit_Student: R50.00\n";
    f << "Interest_Savings: 4.25%\n";
    f << "Interest_FixedDeposit: 8.05%\n";
    f << "Interest_Student: 4.75%\n";
    f << "Max_Login_Attempts: 3\n";
}

//Daily Transaction Report
void reportDailyTransactions(const string& branchCode) {
    string today = currentDate();
    cout << "\n========== Daily Transaction Report ==========\n";
    cout << "Branch: " << branchCode << "\n";
    cout << "Date: " << today      << "\n";
    cout << "----------------------------------------------\n";
    int count = 0;
    double totalIn = 0, totalOut = 0;
    for (Customer& c : allCustomers) {
        if (c.getBranchCode() != branchCode) continue;
        for (const auto& tr : c.getHistory()) {
            if (string(tr.date).find(today) != string::npos) {
                cout << c.getName() << " | " << tr.description
                     << " | R" << fixed << setprecision(2) << tr.amount
                     << " | " << tr.date << "\n";
                     //Classify as incoming or outgoing
                if (string(tr.description).find("Deposit") != string::npos ||
                    string(tr.description).find("Transfer In") != string::npos)
                    totalIn += tr.amount;
                else
                    totalOut += tr.amount;
                count++;
            }
        }
    }
    if (count == 0) cout << "No transactions recorded today.\n";
    cout << "----------------------------------------------\n";
    cout << "Total In: R" << fixed << setprecision(2) << totalIn  << "\n";
    cout << "Total Out: R" << fixed << setprecision(2) << totalOut << "\n";
    cout << "Net: R" << fixed << setprecision(2) << (totalIn - totalOut) << "\n";
    cout << "==============================================\n";

    //Also write to text file
    ofstream f("daily_transaction_report.txt", ios::app);
    if (f) {
        f << "\n========== Daily Transaction Report ==========\n";
        f << "Branch: " << branchCode << "\n";
        f << "Date: " << today << "\n";
        f << "Total In: R"  << fixed << setprecision(2) << totalIn  << "\n";
        f << "Total Out: R" << fixed << setprecision(2) << totalOut << "\n";
        f << "Net: R"       << fixed << setprecision(2) << (totalIn - totalOut) << "\n";
    }
}

//Customer Account Summary
void reportCustomerSummary(const string& branchCode) {
    cout << "\n========== Customer Account Summary ==========\n";
    cout << "Branch: " << branchCode << "\n";
    cout << "----------------------------------------------\n";
    int total = 0; double vault = 0;
    int savings = 0, cheque = 0, fixed = 0, student = 0, locked = 0;

    for (Customer& c : allCustomers) {
        if (c.getBranchCode() != branchCode) continue;
        total++;
        vault += c.getBalance();
        string tp = c.getAccountType();
        if (tp.find("Savings") != string::npos) savings++;
        else if (tp.find("Cheque")  != string::npos) cheque++;
        else if (tp.find("Fixed")   != string::npos) fixed++;
        else student++;
        if (c.getLocked()) locked++;
    }

    cout << "Total Customers: " << total   << "\n";
    cout << "Total Vault: R" << fixed << setprecision(2) << vault << "\n";
    cout << "Savings Accounts: " << savings << "\n";
    cout << "Cheque Accounts: " << cheque  << "\n";
    cout << "Fixed Deposit: " << fixed   << "\n";
    cout << "Student Accounts: " << student << "\n";
    cout << "Locked Accounts: " << locked  << "\n";
    cout << "----------------------------------------------\n";
    cout << "Interest Forecast (yearly):\n";
    for (Customer& c : allCustomers) {
        if (c.getBranchCode() != branchCode) continue;
        double interest = c.getAccountPointer()->calculateInterest();
        if (interest > 0) {
            cout << "  " << c.getName() << ": R"
                 << fixed << setprecision(2) << interest << "\n";
        }
    }
    cout << "==============================================\n";
}

//Branch Performance Report
void reportBranchPerformance() {
    cout << "\n========== Branch Performance Report ==========\n";
    cout << "Generated: " << currentDateTime() << "\n";
    cout << "------------------------------------------------\n";

    double grandTotal = 0;
    for (auto& b : bankNetwork) {
        double bal = b.calBalance(allCustomers);
        grandTotal += bal;
        int custCount = 0;
        for (Customer& c : allCustomers)
            if (c.getAccountNumber().find(b.getCode()) != string::npos)
                custCount++;

        cout << "Branch: " << b.getName() << " [" << b.getCode() << "]\n";
        cout << "Address: " << b.getAddress() << "\n";
        cout << "Customers: " << custCount << "\n";
        cout << "Total Funds: R" << fixed << setprecision(2) << bal << "\n";
        cout << "------------------------------------------------\n";

        b.lastReportBalance = bal;
        b.saveBranchBinary();
    }
    cout << "Network Total: R" << fixed << setprecision(2) << grandTotal << "\n";
    cout << "================================================\n";
}
//Managers only
void branchReportMenu(const string& branchCode) {
    int choice = 0;
    while (choice != 6) {
        cout << "\n===== Branch Reports =====" << endl;
        cout << "1. Daily Transaction Report" << endl;
        cout << "2. Customer Account Summary" << endl;
        cout << "3. Branch Performance Report" << endl;
        cout << "4. Inter-Branch Comparison" << endl;
        cout << "5. Export to CSV" << endl;
        cout << "6. Export to Text File" << endl;
        cout << "7. Back" << endl;
        cout << "Choice: ";
        choice = safeIntInput();

        if (choice == 1) reportDailyTransactions(branchCode);
        else if (choice == 2) reportCustomerSummary(branchCode);
        else if (choice == 3) reportBranchPerformance();
        else if (choice == 4) {
            cout << "\n===== Inter-Branch Comparison =====" << endl;
            for (auto& b : bankNetwork) {
                double bal = b.calBalance(allCustomers);
                cout << b.getName() << " [" << b.getCode() << "]: R";
                printMoney(bal); cout << endl;
            }
        }
        else if (choice == 5) exportToCSV();
        else if (choice == 6) exportToText();
        else if (choice == 7) { cout << "Returning..." << endl; break; }
    }
}
//Validates all inputs before creating the Account
void registerCustomer(const string& branchCode) {
    string fName, sName, idNum, email, contact, address, dob;
    double initialDep; int typeChoice;

    cout << "\n===== New Customer Registration =====" << endl;
    cout << "First Name : "; cin >> fName;
    cout << "Surname    : "; cin >> sName;

    do {//must be exactly 13 digits
        cout << "SA ID Number (13 digits): "; cin >> idNum;
        if (idNum.length() != 13 || idNum.find_first_not_of("0123456789") != string::npos)
            cout << "Invalid! Must be exactly 13 digits.\n";
    } while (idNum.length() != 13 || idNum.find_first_not_of("0123456789") != string::npos);

    do {//must be excatly 10 digits
        cout << "Contact Number (10 digits): "; cin >> contact;
        if (contact.length() != 10 || contact.find_first_not_of("0123456789") != string::npos)
            cout << "Invalid! Must be exactly 10 digits.\n";
    } while (contact.length() != 10 || contact.find_first_not_of("0123456789") != string::npos);

    do {//must contain @ or '.' for domain
        cout << "Email Address: "; cin >> email;
        if (email.find('@') == string::npos || email.find('.') == string::npos)
            cout << "Invalid! Must contain '@' and a domain ('.').\n";
    } while (email.find('@') == string::npos || email.find('.') == string::npos);

    do {//must match the format DD/MM/YYYY
        cout << "Date of Birth (DD/MM/YYYY): "; cin >> dob;
        if (dob.length() != 10 || dob[2] != '/' || dob[5] != '/')
            cout << "Invalid! Use DD/MM/YYYY format.\n";
    } while (dob.length() != 10 || dob[2] != '/' || dob[5] != '/');

    cin.ignore();
    do {//must be 5 or more characters
        cout << "Physical Address: "; getline(cin, address);
        if (address.length() < 5) cout << "Address too short.\n";
    } while (address.length() < 5);
//Account type selections with interest rates displayed
    cout << "\nAccount Types:\n";
    cout << "1. Savings (Min R100, Interest 4.25%)\n";
    cout << "2. Cheque (Min R100, No interest)\n";
    cout << "3. Fixed Deposit(Min R1000, Interest 8.05%)\n";
    cout << "4. Student (Min R50, Interest 4.75%, Zero Fees)\n";
    cout << "Choice: "; typeChoice = safeIntInput();
    //what is the minimum amount for each account type
    double minDep;
    if (typeChoice == 1) minDep = 100.0;
    else if (typeChoice == 2) minDep = 100.0;
    else if (typeChoice == 3) minDep = 1000.0;
    else minDep = 50.0;

    do {
        cout << "Initial Deposit (Min R" << fixed << setprecision(2) << minDep << "): R";
        cin >> initialDep;
        if (initialDep < minDep)
            cout << "Minimum deposit is R" << fixed << setprecision(2) << minDep << "\n";
    } while (initialDep < minDep);

    //Generate account number: ACC-BRANCHCODE-XXXXX
    string accNum = "ACC-" + branchCode + "-" + to_string((rand() % 90000) + 10000);
    //create the correct account suubtype
    Account* acc = nullptr;
    if (typeChoice == 1) acc = new SavingsAccount(accNum, initialDep);
    else if (typeChoice == 2) acc = new ChequeAccount(accNum, initialDep);
    else if (typeChoice == 3) acc = new FixedDepositAccount(accNum, initialDep);
    else acc = new StudentAccount(accNum, initialDep);
//PIN generated inside the constructor
    Customer newCust(fName + " " + sName, idNum, contact, email, address, dob, branchCode, acc);
    newCust.logTransaction("Initial Deposit", initialDep);
    allCustomers.push_back(newCust);
    //Display the generated account number and PIN
    cout << "\n===== Registration Complete! =====" << endl;
    cout << "Account Number: " << allCustomers.back().getAccountNumber() << endl;
    cout << "Temporary PIN: " << allCustomers.back().getPin()
         << " (Hand to customer securely)" << endl;
    cout << "Account Type: " << allCustomers.back().getAccountType() << endl;
    cout << "Initial Deposit: R"; printMoney(initialDep); cout << endl;
}

// Returns index or -1 if not found
int searchCustomer() {
    int sc;
    cout << "\nSearch by:\n1. Account Number\n2. Name\n3. SA ID Number\nChoice: ";
    sc = safeIntInput();

    // Linear search
    if (sc == 1) {
        string s; cout << "Account Number: "; cin >> s;
        for (int i = 0; i < (int)allCustomers.size(); i++)
            if (allCustomers[i].getAccountNumber() == s) return i;
    } else if (sc == 2) {//Case insensitive 
        string sn; cout << "Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, sn);
        string ls = toLower(sn);
        for (int i = 0; i < (int)allCustomers.size(); i++)
            if (toLower(allCustomers[i].getName()).find(ls) != string::npos) return i;
    } else if (sc == 3) {//Exact match on the ID no
        string sid; cout << "SA ID: "; cin >> sid;
        for (int i = 0; i < (int)allCustomers.size(); i++)
            if (allCustomers[i].getIDNumber() == sid) return i;
    }
    return -1; //means not found
}
//The main menu for tellers
void transactionMenu(Customer& ac) {
    int sub = 0;
    while (sub != 7) {
        cout << "\n===== Transaction Menu: " << ac.getName() << " =====" << endl;
        cout << "1. View Balance" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Fund Transfer" << endl;
        cout << "5. View Statement" << endl;
        cout << "6. Start Customer Self-Service Session" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "Choice: ";
        sub = safeIntInput();

        if (sub == 1) {
            cout << "Balance: R"; printMoney(ac.getBalance()); cout << endl;
        } else if (sub == 2) {
            double dep; cout << "Amount: R"; cin >> dep; ac.deposit(dep);
        } else if (sub == 3) {
            double wth; cout << "Amount: R"; cin >> wth; ac.withdraw(wth);
        } else if (sub == 4) {//Fund transfer requires a PIN re-verification as a security step
            int sPin; cout << "Re-enter customer PIN to authorize: "; sPin = safeIntInput();
            try {
                if (!ac.verifyCustomerLogin(ac.getAccountNumber(), sPin))
                    throw string("Incorrect PIN. Transfer cancelled.");
                Customer* r = ac.findTransferRecipient(ac.getAccountNumber());
                if (r) {
                    double a; cout << "Amount: R"; cin >> a;
                    if (ac.transfer(a, *r)) cout << "Transfer Successful!" << endl;
                }
            } catch (string err) { cout << err << endl; }
        } else if (sub == 5) {
            ac.printStatement();
        } else if (sub == 6) {//Hand terminal to customer for self-service
            int sp; cout << "Customer PIN: "; sp = safeIntInput();
            if (ac.verifyCustomerLogin(ac.getAccountNumber(), sp))
                ac.customerSession();
        } else if (sub == 7) {
            cout << "Returning to main menu..." << endl;
        }
    }
}
//Full access of the menu (manager + tellers + branch)
void managerMenu(const staffRecord& manager) {
    int choice = 0;
    while (choice != 7) {
        cout << "\n===== Manager Menu: " << manager.fullName
             << " [" << manager.branchCode << "] =====" << endl;
        cout << "1. Register New Customer" << endl;
        cout << "2. View/Search Customer" << endl;
        cout << "3. Process Transaction" << endl;
        cout << "4. Add New Teller" << endl;
        cout << "5. Remove a Teller" << endl;
        cout << "6. View Branch Tellers" << endl;
        cout << "7. Reset Teller Password" << endl;
        cout << "8. Branch Reports" << endl;
        cout << "9. Logout" << endl;
        cout << "Choice: ";
        choice = safeIntInput();

        if (choice == 1) {
            registerCustomer(string(manager.branchCode));

        } else if (choice == 2) {
            int idx = searchCustomer();
            if (idx != -1) {
                allCustomers[idx].displayInfo();
                char ul; cout << "Unlock account if locked? (y/n): "; cin >> ul;
                if (ul == 'y') { allCustomers[idx].setLock(false); cout << "Unlocked." << endl; }
            } else cout << "No matching record found." << endl;

        } else if (choice == 3) {
            int idx = searchCustomer();
            if (idx == -1) { cout << "Customer not found." << endl; continue; }
            Customer& ac = allCustomers[idx];
            int pin; cout << "Customer PIN: "; pin = safeIntInput();
            if (ac.verifyCustomerLogin(ac.getAccountNumber(), pin))
                transactionMenu(ac);
            else cout << "Authentication failed." << endl;

        } else if (choice == 4) {//add a new teller to this branch
            staffRecord nr; nr.isManager = false;
            strncpy(nr.branchCode, manager.branchCode, 9); nr.branchCode[9] = '\0';
            string id, name, pass;
            cout << "Teller ID (e.g. T011): "; cin >> id;
            //Check ID uniqueness
            vector<staffRecord> all = loadAllStaff();
            bool taken = false;
            for (auto& s : all) if (string(s.staffID) == id) { taken = true; break; }
            if (taken) { cout << "ID already in use." << endl; continue; }
            cout << "Full Name: "; cin.ignore(); getline(cin, name);
            cout << "Password: "; cin >> pass;
            strncpy(nr.staffID,  id.c_str(), 19); nr.staffID[19] = '\0';
            strncpy(nr.fullName, name.c_str(), 49); nr.fullName[49] = '\0';
            string h = encryptStr(pass);
            strncpy(nr.passwordHash, h.c_str(), 63); nr.passwordHash[63] = '\0';
            saveStaffRecord(nr);
            cout << "Teller " << name << " added." << endl;

        } else if (choice == 5) {//Remove a teller frfom this branch ony
            vector<staffRecord> all = loadAllStaff();
            cout << "\nTellers in branch " << manager.branchCode << ":\n";
            bool any = false;
            for (auto& s : all)
                if (!s.isManager && string(s.branchCode) == string(manager.branchCode)) {
                    cout << "  " << s.staffID << " - " << s.fullName << "\n"; any = true;
                }
            if (!any) { cout << "No tellers found.\n"; continue; }
            string rid; cout << "Enter Teller ID to remove: "; cin >> rid;
            vector<staffRecord> updated;
            bool found = false;
            for (auto& s : all) {//Only remove if teller belongs to thus managers branch
                if (string(s.staffID) == rid && !s.isManager &&
                    string(s.branchCode) == string(manager.branchCode))
                    found = true;
                else updated.push_back(s);
            }
            if (found) { rewriteStaffFile(updated); cout << "Teller removed." << endl; }
            else cout << "Teller not found in your branch." << endl;

        } else if (choice == 6) {//Reset a tellers password in this branch
            vector<staffRecord> all = loadAllStaff();
            cout << "\n===== Tellers in Branch " << manager.branchCode << " =====" << endl;
            bool any = false;
            for (auto& s : all)
                if (!s.isManager && string(s.branchCode) == string(manager.branchCode)) {
                    cout << "ID: " << s.staffID << " | Name: " << s.fullName << "\n"; any = true;
                }
            if (!any) cout << "No tellers assigned." << endl;

        } else if (choice == 7) {
            vector<staffRecord> all = loadAllStaff();
            cout << "\nTellers in branch " << manager.branchCode << ":\n";
            for (auto& s : all)
                if (!s.isManager && string(s.branchCode) == string(manager.branchCode))
                    cout << "  " << s.staffID << " - " << s.fullName << "\n";
            string tid, np; cout << "Teller ID to reset: "; cin >> tid;
            cout << "New Password: "; cin >> np;
            bool found = false;
            for (auto& s : all) {
                if (string(s.staffID) == tid && !s.isManager &&
                    string(s.branchCode) == string(manager.branchCode)) {
                    string h = encryptStr(np);
                    strncpy(s.passwordHash, h.c_str(), 63); s.passwordHash[63] = '\0';
                    found = true;
                }
            }
            if (found) { rewriteStaffFile(all); cout << "Password reset." << endl; }
            else cout << "Teller not found in your branch." << endl;

        } else if (choice == 8) {
            branchReportMenu(string(manager.branchCode));

        } else if (choice == 9) {
            cout << "Logging out..." << endl; break;
        }
    }
}
//Tellers Menu
void tellerMenu(const staffRecord& teller) {
    int choice = 0;
    while (choice != 4) {
        cout << "\n===== Teller Menu: " << teller.fullName
             << " [" << teller.branchCode << "] =====" << endl;
        cout << "1. Register New Customer" << endl;
        cout << "2. View/Search Customer" << endl;
        cout << "3. Process Transaction" << endl;
        cout << "4. Logout" << endl;
        cout << "Choice: ";
        choice = safeIntInput();

        if (choice == 1) {
            registerCustomer(string(teller.branchCode));

        } else if (choice == 2) {
            int idx = searchCustomer();
            if (idx != -1) {
                allCustomers[idx].displayInfo();
                char ul; cout << "Unlock if locked? (y/n): "; cin >> ul;
                if (ul == 'y') { allCustomers[idx].setLock(false); cout << "Unlocked." << endl; }
            } else cout << "No matching record found." << endl;

        } else if (choice == 3) {
            int idx = searchCustomer();
            if (idx == -1) { cout << "Customer not found." << endl; continue; }
            Customer& ac = allCustomers[idx];
            int pin; cout << "Customer PIN: "; pin = safeIntInput();
            if (ac.verifyCustomerLogin(ac.getAccountNumber(), pin))
                transactionMenu(ac);
            else cout << "Authentication failed." << endl;

        } else if (choice == 4) {
            cout << "Logging out..." << endl;
        }
    }
}
//Staff login and password
staffRecord attemptLogin(bool wantManager) {
    staffRecord blank{};//Empty record signals failed login
    string id, pass;
    int attempts = 0;
    cout << "\n---- Standard Bank "
         << (wantManager ? "Manager" : "Teller") << " Login ----" << endl;

    while (attempts < 3) {
        cout << "Staff ID: "; cin >> id;
        cout << "Password: "; cin >> pass;

        vector<staffRecord> all = loadAllStaff();
        for (auto& sr : all) {
            if (string(sr.staffID) == id &&
                sr.isManager == wantManager &&
                encryptStr(pass) == string(sr.passwordHash)) {
                cout << "\nWelcome, " << sr.fullName << "!" << endl;
                return sr; //Successful Login
            }
        }
        attempts++;
        cout << "Invalid credentials. Attempts remaining: " << (3 - attempts) << endl;
    }
    cout << "Too many failed attempts. Access denied." << endl;
    return blank;
}

int main() {
    //Seed random number generated for PIN and account number
    srand(time(0));

    // Initialise branch network
    bankNetwork.push_back(Branch("Sandton", "018105", "Sandton City, Johannesburg"));
    bankNetwork.push_back(Branch("Bloemfontein", "055034", "Mimosa Mall, Bloemfontein"));
    bankNetwork.push_back(Branch("Constantia", "025309", "Constantia Village, Cape Town"));
    //seed tellers.dat on the first run, then load all customer data
    seedStaffIfEmpty();
    loadAllData();
    saveSystemConfig("System Startup");

    cout << "\n================================================" << endl;
    cout << "     STANDARD BANK MULTI-BRANCH SYSTEM v1.0    " << endl;
    cout << "================================================" << endl;

    string lastUser = "N/A";
    int roleChoice = 0;
    //Main role selection loop
    while (roleChoice != 3) {
        cout << "\nPlease select your role:" << endl;
        cout << "1. Branch Manager" << endl;
        cout << "2. Teller" << endl;
        cout << "3. Exit System" << endl;
        cout << "Choice: ";
        roleChoice = safeIntInput();

        if (roleChoice == 1) {
            staffRecord mgr = attemptLogin(true);
            if (string(mgr.staffID) != "") {
                lastUser = string(mgr.fullName) + " (Manager)";
                managerMenu(mgr);//Enter teller workflow
            }
        } else if (roleChoice == 2) {
            staffRecord tel = attemptLogin(false);
            if (string(tel.staffID) != "") {
                lastUser = string(tel.fullName) + " (Teller)";
                tellerMenu(tel);//Enter teller Workflow
            }
        } else if (roleChoice == 3) {
            cout << "Shutting down system..." << endl;
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    // Save everything on exit
    saveAllData();//Persist customer data
    performBackup();//create backup copy
    saveSystemConfig(lastUser);//Update system_config.txt with recent

    cout << "\nAll data saved. Goodbye!" << endl;
    return 0;
}