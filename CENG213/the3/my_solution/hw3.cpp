#include "hw3.h"


RegisteredHashTable::RegisteredHashTable(int tableSize1) {
    registeredTable.resize(tableSize1);
    this->registeredSize = tableSize1;
    currRegisteredSize=0;

    for (int i=0; i < tableSize1 ; i++) {
        registeredTable[i].first = EMPTY;
        registeredTable[i].second = EMPTY;
    }
}

int RegisteredHashTable::insertToRegistered(const std::string username, const std::string password ,int flag) {
    int index = 0;
    int hashVal = hashFunction(username, registeredSize, index);

    while(registeredTable[hashVal].first != EMPTY && registeredTable[hashVal].first != DELETED) {
        if(index == registeredSize) return 0;
        if(flag == 1 && registeredTable[hashVal].first == username) return 0;
        index++;
        hashVal = hashFunction(username, registeredSize, index);
    }
    registeredTable[hashVal].first = username;
    registeredTable[hashVal].second = password;
    currRegisteredSize++;

    if(( (float) currRegisteredSize / registeredSize > MAX_LOAD_FACTOR )) {
        rehashRegisteredTable();
    }
    return 1;

}

int RegisteredHashTable::removeFromRegistered(const std::string username, std::vector<std::string>& oldPasswords) {
    int index = 0;
    int flag = 0;
    int hashVal = hashFunction(username, registeredSize, index);

    while(registeredTable[hashVal].first != EMPTY ) {
        if(registeredTable[hashVal].first == username) {
            oldPasswords.push_back(registeredTable[hashVal].second);
            registeredTable[hashVal] = std::make_pair(DELETED, DELETED);
            currRegisteredSize--;
            flag = 1;
        }
        index++;
        hashVal = hashFunction(username, registeredSize, index);
    }
    return flag;
}

int RegisteredHashTable::isInRegistered(const std::string username, const std::string password) {
    int index = 0;
    int hashVal = hashFunction(username, registeredSize, index);
    std::string temp = "";

    while( registeredTable[hashVal].first != EMPTY ) {
        if(index == registeredSize) break;
        if(registeredTable[hashVal].first == username) {
            temp = registeredTable[hashVal].second;
        }
        index++;
        hashVal =  hashFunction(username, registeredSize, index);
    }

    if(temp == "") {
        return 0;
    }

    if(temp == password){
        return 1;
    }
    return 0;
}

int RegisteredHashTable::isPrime(int n) {
    for(int i = 2; i*i <= n; i++){
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}


void RegisteredHashTable::rehashRegisteredTable() {

    std::vector<std::pair<std::string, std::string> > newRegisteredTable;
    std::string uniqueUser;
    int newSize = 2 * registeredSize + 1;

    while (isPrime(newSize) == 0) {newSize += 1;}

    newRegisteredTable.resize(newSize);

    for (int i = 0; i < newSize; i++) {
        newRegisteredTable[i].first = EMPTY;
        newRegisteredTable[i].second = EMPTY;
    }

    int index = 0;
    int hashVal = 0;
    for (int i = 0; i < registeredSize; i++) {
        index = 0;
        hashVal = hashFunction(registeredTable[i].first, registeredSize, index);
        uniqueUser = registeredTable[i].first;
        if (uniqueUser == EMPTY || uniqueUser == DELETED) continue;
        while (registeredTable[hashVal].first != EMPTY) {
            if (index == registeredSize) break;
            if (registeredTable[hashVal].first == uniqueUser) {
                int newIndex = 0;
                int newHashVal = hashFunction(registeredTable[hashVal].first, newSize, newIndex);

                while (newRegisteredTable[newHashVal].first != EMPTY ) {
                    if (newIndex == newSize) break;
                    newIndex++;
                    newHashVal = hashFunction(registeredTable[hashVal].first, newSize, newIndex);
                }
                newRegisteredTable[newHashVal].first = registeredTable[hashVal].first;
                newRegisteredTable[newHashVal].second = registeredTable[hashVal].second;

                registeredTable[hashVal].first = DELETED;
                registeredTable[hashVal].second = DELETED;

            }
            index++;
            hashVal = hashFunction(uniqueUser, registeredSize, index);
        }
    }
    registeredTable.resize(newSize);
    this->registeredSize = newSize;
    registeredTable = newRegisteredTable;
}
std::string RegisteredHashTable::getUsername(int index) {
    return registeredTable[index].first;
}

std::string RegisteredHashTable::getPassword(int index) {
    return registeredTable[index].second;
}

RegisteredHashTable::~RegisteredHashTable() {
}


ActiveHashTable::ActiveHashTable(int tableSize2) {
    activeTable.resize(tableSize2);
    activeSize=tableSize2;
    currActiveSize = 0;

    for (int i=0; i < tableSize2 ; i++) {
        activeTable[i] = EMPTY;
    }
}

int ActiveHashTable::isPrime(int n) {

    for(int i = 2; i*i <= n; i++){
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

int ActiveHashTable::removeFromActive(const std::string username) {

    int index = 0;
    int hashVal = hashFunction(username, activeSize, index);

    while(activeTable[hashVal] != EMPTY) {

        if(activeTable[hashVal] == username) {
            activeTable[hashVal] = DELETED;
            currActiveSize--;
            return 1;

        }
        index++;
        hashVal = hashFunction(username, activeSize, index);

    }

    return 0;

}

int ActiveHashTable::insertToActive(const std::string username) {

    int index = 0;
    int hashVal = hashFunction(username, activeSize, index);

    while(activeTable[hashVal] != EMPTY && activeTable[hashVal] != DELETED ) {

        if(index == activeSize) return 0;
        if(activeTable[hashVal] == username) return 0;

        index++;
        hashVal = hashFunction(username, activeSize, index);
    }

    activeTable[hashVal] = username;
    currActiveSize++;

    if((float) currActiveSize / activeSize > MAX_LOAD_FACTOR) {
        rehashActiveTable();
    }

    return 1;


}

int ActiveHashTable::isInActive(const std::string username) {

    int index = 0;
    int hashVal = hashFunction(username, activeSize, index);

    while ( activeTable[hashVal] != EMPTY && activeTable[hashVal] != DELETED) {

        if(index == activeSize) break;
        if(activeTable[hashVal] == username) return 1;

        index++;
        hashVal =  hashFunction(username, activeSize, index);
    }

    return 0;
}

void ActiveHashTable::rehashActiveTable(){

    std::vector< std::string >  newActiveTable;

    int newSize = 2*activeSize + 1;
    while (isPrime(newSize) == 0) {
        newSize += 1;
    }
    newActiveTable.resize(newSize);

    for (int i = 0; i < newSize; i++) {
        newActiveTable[i] = EMPTY;
    }
    for (int i = 0; i < activeSize; i++) {
        if (activeTable[i] == EMPTY || activeTable[i] == DELETED) continue;

        int newIndex = 0;
        int newHashVal = hashFunction(activeTable[i], newSize, newIndex);

        while (newActiveTable[newHashVal] != EMPTY) {
            if (newIndex == newSize) break;
            newIndex++;
            newHashVal = hashFunction(activeTable[i], newSize, newIndex);
        }
        newActiveTable[newHashVal] = activeTable[i];
        activeTable[i] = DELETED;
    }

    activeTable.resize(newSize);
    this->activeSize = newSize;
    activeTable=newActiveTable;

}

std::string ActiveHashTable::getUsername(int index) {
    return activeTable[index];
}

ActiveHashTable::~ActiveHashTable() {

}

AccessControl::AccessControl(int table1Size, int table2Size) {
    regUsers = RegisteredHashTable(table1Size);
    activeUsers = ActiveHashTable(table2Size);
}

AccessControl::~AccessControl() {

}

int AccessControl::addUser(std::string username, std::string pass) {
    return regUsers.insertToRegistered(username, pass, 1);

}

int AccessControl::addUsers(std::string filePath) {
    int count = 0;
    std::ifstream usersFile;
    usersFile.open(filePath.data());

    if (usersFile.is_open()) {

        std::string line, username, password;
        std::string delimeter=" ";
        std::string token;
        size_t pos=0;

        while (getline(usersFile, line)) {

            while((pos = line.find(delimeter)) != std::string::npos ) {

                token = line.substr(0, pos);
                username = token;
                line.erase(0, pos + delimeter.length());
            }
            password = line;
            if(addUser(username, password))
                count++;
        }
        usersFile.close();
    }
    return count;
}

int AccessControl::delUser(std::string username, std::vector<std::string> &oldPasswords) {
    return regUsers.removeFromRegistered(username, oldPasswords);
}

int AccessControl::changePass(std::string username, std::string oldpass, std::string newpass) {
    if(regUsers.isInRegistered(username, oldpass) == 1) {
        return regUsers.insertToRegistered(username, newpass, 0) ;
    }
    return 0;

}

int AccessControl::login(std::string username, std::string pass) {
    if(regUsers.isInRegistered(username, pass) == 1) {
        return activeUsers.insertToActive(username);
    }
    return 0;

}

int AccessControl::logout(std::string username) {
    if( activeUsers.isInActive(username) == 1) {
        return activeUsers.removeFromActive(username);

    }
    return 0;
}

float AccessControl::printActiveUsers() {
    int loadCount = 0;
    for(int i=0; i < activeUsers.getSize() ; i++) {

        if(activeUsers.getUsername(i) != EMPTY && activeUsers.getUsername(i) != DELETED) {
            loadCount++;
        }
        std::cout << activeUsers.getUsername(i) << std::endl;
    }
    return (float) loadCount / activeUsers.getSize();
}

float AccessControl::printPasswords() {

    int loadCount = 0;
    for(int i=0; i < regUsers.getSize(); i++) {
        if(regUsers.getUsername(i) != EMPTY && regUsers.getUsername(i) != DELETED ) {
            loadCount++;
        }
        std::cout << regUsers.getUsername(i) << " " << regUsers.getPassword(i) << std::endl;
    }
    return (float) loadCount / regUsers.getSize();

}



