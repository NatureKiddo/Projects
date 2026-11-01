#include <iostream>
#include <vector>
#include "Engineer.h"
using namespace std;

//Constructor
Engineer::Engineer(string id, string uname, string plainPassword, string clearance)
   : engineerID(id), username(uname), clearanceLevel(clearance)
{
    passwordHash = hash<string>{}(plainPassword); //Hashing it only once. 
} 
//Constructor
Engineer::Engineer(string id, string uname, size_t existingHash, string clearance)
    : engineerID(id), username(uname), passwordHash(existingHash), clearanceLevel(clearance) {}

size_t Engineer::getPasswordHash() const {
    return passwordHash;
}

//Getters for the private values
string Engineer::getID() const{
    return engineerID;
}

string Engineer::getUsername() const {
    return username;
}

string Engineer::getClearance() const{
    return clearanceLevel;
}

//Password check and the hashes entered for password
bool Engineer::checkPassword(string plainPassword) const {
    size_t hashedInput = hash<string>{}(plainPassword);
    return hashedInput == passwordHash;
}
