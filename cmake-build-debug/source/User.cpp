#ifndef USER
#define USER
#include <algorithm>
#include "../head/User.h"

void UserList::AddUser(const string& name, const string& id, const string& password) {
    User newUser;
    newUser.Name = name;
    newUser.ID = id;
    newUser.password = password;
    List.push_back(newUser);
}

void UserList::DeleteUser(const string& id) {
    List.erase(remove_if(List.begin(), List.end(), [&id](User& user) {
        return user.ID == id;
    }), List.end());
}

User* UserList::FindUserByName(const string& name) {
    for (auto& user : List) {
        if (user.Name == name) {
            return &user;
        }
    }
    return nullptr;
}

void UserList::DisplayUsers() {
    for (const auto& user : List) {
        cout << "Name: " << user.Name << ", ID: " << user.ID << endl;
    }
}
#endif