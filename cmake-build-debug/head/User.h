#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class UserList;
class User;

class User {
public:
    string Name;
    string ID;
    string password;
};

class UserList {
public:
    vector<User> List;

    void AddUser(const string& name, const string& id,const string& password);
    void DeleteUser(const string& id);
    User* FindUserByName(const string& name);
    void DisplayUsers();
};
extern UserList userList;
extern User currentUser;

#endif