#ifndef _HW3_H
#define _HW3_H
#include <iostream>
#include <string>
#include <vector>
#include <utility>  // use just for std::pair< >
#include <fstream>

#define MAX_LOAD_FACTOR 0.65
#define EMPTY "EMPTY"
#define DELETED "DELETED"



class RegisteredHashTable {

public:

    RegisteredHashTable(int tableSize1);
    RegisteredHashTable() {};
    ~RegisteredHashTable();
    int getSize() { return registeredSize; }
    int insertToRegistered(const std::string username, const std::string password, int flag);
    int removeFromRegistered(const std::string username, std::vector<std::string>& oldPasswords);
    int isInRegistered(const std::string username, const std::string password);
    void rehashRegisteredTable();
    int isPrime(int n);
    std::string getUsername(int index) ;
    std::string getPassword(int index) ;


private:

    int hashFunction(std::string key, int tableSize, int i) {
        int length = key.length();
        int newK = 0;
        for (int i = 0; i < length; i++)
            newK += (int) key[i];

        // hash function 1 -> (newK % tableSize)
        // hash function 2 -> (newK * tableSize - 1) % tableSize)

        return ((newK % tableSize) + i * ((newK * tableSize - 1) % tableSize)) % tableSize;
    }
    std::vector< std::pair<std::string, std::string> > registeredTable ;
    int registeredSize;
    int currRegisteredSize ;

};

class ActiveHashTable {

public:

    ActiveHashTable(int tableSize2);
    ActiveHashTable() {};
    ~ActiveHashTable();
    int getSize() { return activeSize; }
    int insertToActive(const std::string username);
    int removeFromActive(const std::string username);
    int isInActive(const std::string username);
    int isPrime(int n);
    void rehashActiveTable();
    std::string getUsername(int index);

private:

    int hashFunction(std::string key, int tableSize, int i) {
        int length = key.length();
        int newK = 0;
        for (int i = 0; i < length; i++)
            newK += (int) key[i];

        // hash function 1 -> (newK % tableSize)
        // hash function 2 -> (newK * tableSize - 1) % tableSize)

        return ((newK % tableSize) + i * ((newK * tableSize - 1) % tableSize)) % tableSize;
    }
    std::vector< std::string >  activeTable;
    int activeSize;
    int currActiveSize;

};


class AccessControl
{
public:

    AccessControl(int table1Size, int table2Size);
    ~AccessControl();
    int addUser(std::string username, std::string pass);
    int addUsers(std::string filePath);
    int delUser(std::string username, std::vector<std::string>& oldPasswords);
    int changePass(std::string username, std::string oldpass, std::string newpass);

    int login(std::string username, std::string pass);
    int logout(std::string username);

    float printActiveUsers();
    float printPasswords();

private:

    ActiveHashTable activeUsers;	// hash(username) -> username
    RegisteredHashTable regUsers;  		// hash(username) -> username,password

    int hashFunction(std::string key, int tableSize, int i) {
        int length = key.length();
        int newK = 0;
        for (int i = 0; i < length; i++)
            newK += (int) key[i];

        // hash function 1 -> (newK % tableSize)
        // hash function 2 -> (newK * tableSize - 1) % tableSize)

        return ((newK % tableSize) + i * ((newK * tableSize - 1) % tableSize)) % tableSize;
    }
};

#endif
