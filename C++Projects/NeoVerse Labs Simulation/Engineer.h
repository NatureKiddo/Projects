#ifndef ENGINEER_H
#define ENGINEER_H
#include <string>
using namespace std;

class Engineer {
private:
    string engineerID;      
    string username;
    size_t passwordHash;
    string clearanceLevel;  

public:
    //Constructor: sets up a new Engineer with given details
    Engineer(string id, string uname, string plainPassword, string clearance);
    Engineer(string id, string uname, size_t existingHash, string clearance); 
    
    //Getters (return the private data safely)
    string getID() const;
    string getUsername() const;
    string getClearance() const;
    size_t getPasswordHash() const;

    //Checks if a given plain-text password matches this engineer's stored encrypted password
    bool checkPassword(string plainPassword) const;
};
#endif
