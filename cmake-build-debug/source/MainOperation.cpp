//
// Created by JXY on 2024/5/27.
//
#ifndef MAINOPERATOR
#define MAINOPERATOR
#include"../head/MainOperation.h"
using namespace std;
int tester = 1; //用于主菜单循环测试
void MainOperation::Login() {
    system("cls");
    int choice;
    cout << "1. Select Account" << endl;
    cout << "2. Create Account" << endl;
    cout << "3. Quit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    User* user = nullptr;
    if (choice == 1) {
        // 显示账户列表并选择一个账户
        do {
            userList.DisplayUsers();
            string name;
            cout << "Enter the account name: ";
            cin >> name;
            user = userList.FindUserByName(name);
            if (user) {
                cout << "Login successful! Welcome, " << user->Name << "." << endl;
                currentUser = *user;
                system("pause");
            } else {
                cout << "Account not found." << endl;
                system("pause");
            }
        } while (!user);
    } else if (choice == 2) {
        // 创建账户
        string name, id, password;
        while (true) {
            cout << "Enter account name: ";
            cin >> name;
            if (userList.FindUserByName(name)) {
                cout << "Account name already exists. Please try again." << endl;
            } else {
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter password: ";
                cin >> password;
                userList.AddUser(name, id, password);
                cout << "Account created successfully!" << endl;
                break;
            }
        }
        system("pause");
    } else if (choice == 3) {
        exitRequested = true; // 请求退出系统
    } else {
        cout << "Invalid choice. Please try again." << endl;
        system("pause");
    }
}


void MainOperation::Logout() {
    system("cls");
    if (!currentUser.Name.empty()) {
        cout << "User " << currentUser.Name << " has been logged out successfully." << endl;
        currentUser = User();  // 清理当前用户信息
    } else {
        cout << "No user is currently logged in." << endl;
    }
    system("pause");
}

void MainOperation::ShowALL() {
    system("cls");
    if (g_nowfold) {
        g_nowfold->showAll("", true);
    } else {
        cout << "No directory structure found." << endl;
    }
    system("pause");
}

void MainOperation::ReadFile() {  //注意权限设置
    system("cls");
    system("cls");  // Windows 系统
    if (!SelectFile(TargetFile, "write")) {
        return;
    }
    cout << "File Name: " << TargetFile->name << endl;
    cout << "File Owner: " << TargetFile->owner << endl;
    cout << "Readable: " << (TargetFile->isotherread ? "yes" : "no") << endl;
    cout << "Writable: " << (TargetFile->isotherwrite ? "yes" : "no") << endl;
    cout << "father fold sign: " << TargetFile->fatherlogo << endl;
    cout << "file content: " <<endl<< TargetFile->content << endl;
    system("pause");
}


void MainOperation::WriteFile() {
    // 清屏
    system("cls");  // Windows 系统
    if (!SelectFile(TargetFile, "write")) {   //选择要修改的文件
        return;
    }

    // 提示用户输入新的内容
    cout << "Please write the new content: ";
    string newContent;
    cin.ignore();  // 忽略换行符
    // 当使用 cin >> 读取数据时，输入的内容会在遇到空白字符（如空格、换行符）时结束，
    // 但输入流中还会残留一个换行符。这个换行符在后续的输入操作中可能会被误读，
    // 从而导致输入操作失败或行为异常。
    getline(cin, newContent);

    // 保存原始内容
    string originalContent = TargetFile->content;

    try {
        // 修改文件内容
        TargetFile->content = newContent;

        // 更新文件块
        bool updateSuccess = Control.UpdateFileBlock(TargetFile);

        // 检查更新是否成功
        if (updateSuccess) {
            // 输出确认信息
            cout << "File " << TargetFile->name << " has been updated." << endl;
        } else {
            // 更新失败处理
            cout << "Failed to update file blocks for " << TargetFile->name << "." << endl;
            // 恢复原内容
            TargetFile->content = originalContent;
            cout << "File content has been restored to the original state." << endl;
        }
    } catch (const std::exception& e) {
        // 捕获异常并处理
        cout << "An error occurred while updating the file: " << e.what() << endl;
        // 恢复原内容
        TargetFile->content = originalContent;
        cout << "File content has been restored to the original state." << endl;
    }
    system("pause");
}




void MainOperation::DeleteFile() {
    // 清屏

    system("cls");  // Windows 系统

    if (!SelectFile(TargetFile, "delete")) {  // 选择要删除的文件
        return;
    }
    system("pause");
    g_nowfold->ifree(TargetFile);
}




void MainOperation::CreateFile(){
    int choice = 0;
    CFile RealNewFile;
    CFile *NewFile = &RealNewFile;
    while (true) {
        system("cls");  // Windows 系统
        cout << "Text your file name: ";
        cin >> NewFile->name;
        cout << "Text your file content: ";
        cin.ignore();  // 忽略换行符
        getline(cin, NewFile->content);

        cout << "Is this file readable? 0. no  1. yes" << endl;
        cin >> choice;
        if (choice != 0 && choice != 1) {
            cout<< " Invalid choice! try again"<<endl;
            system("pause");
            continue;
        }
        NewFile->isotherread = choice;

        cout << "Is this file writeable? 0. no  1. yes" << endl;
        cin >> choice;
        if (choice != 0 && choice != 1) {
            cout<< " Invalid choice! try again"<<endl;
            system("pause");
            continue;
        }
        NewFile->isotherwrite = choice;

        break;  // 输入正确，跳出循环
    }

    bool CreateSuccess = Control.Balloc(NewFile);

    if (CreateSuccess) {
        g_nowfold->ialloc(&disk[NewFile->inode].File);
    } else {
        // 输出错误信息
        cout << "Failed to Create file blocks for " << NewFile->name << "." << endl;
    }
    system("pause");

}


void MainOperation::DeleteFold() {
    while(true){
        system("cls");
        for(int i=0;i<g_nowfold->childfold.size();i++){
            cout<<i+1<<". "<<g_nowfold->childfold[i]->name<<endl;
        }
        cout<<"Select the fold you want to delete:";
        int temp;
        cin>>temp;
        if(temp<=0||temp>g_nowfold->childfold.size()){
            cout<<"error input,press any key to continue";
            system("pause");
            continue;
        }
        else {
            delfold(g_nowfold->childfold[temp-1]);
            g_nowfold->childfold.erase(g_nowfold->childfold.begin()+temp-1);
            break;
        }
    }
}

void MainOperation::CreateFold() {
    while(true){
        system("cls");
        cout<<"input fold name:\n";
        string name;
        cin>>name;
        int samename = 0;
        for(CFold *temp:g_nowfold->childfold){
            if(name==temp->name)samename = true;
        }
        if(samename){
            cout<<"The fold mustn't have the same name!\n";
            system("pause");
            continue;
        }
        else {
            CFold newfold;
            newfold.name = name;
            bool flag = Control.Bfreefold(&newfold);
            if(flag == false){
                cout<<"The disk is full!\n";
                system("pause");
                return;
            }

        }
    }
}


void MainOperation::Menu() {
    int choice;
    do {
        cout << "current fold: " << g_nowfold->name << endl;
        cout << "father fold: " << g_nowfold->fatherlogo << endl;
        cout << "son folds: " << endl;
        OutputAllFolds();
        OutputAllFiles();

        cout << "1. Logout" << endl;
        cout << "2. ShowALL" << endl;
        cout << "3. CreateFile" << endl;
        cout << "4. ReadFile" << endl;
        cout << "5. WriteFile" << endl;
        cout << "6. DeleteFile" << endl;
        cout << "7. DeleteFold" << endl;
        cout << "8. DrawBitmap" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "ok" << endl;
        cout << "your choice is:" << choice << endl;

        switch (choice) {
            case 1:
                Logout();
                break;
            case 2:
                ShowALL();
                break;
            case 3:
                CreateFile();
                break;
            case 4:
                ReadFile();
                break;
            case 5:
                WriteFile();
                break;
            case 6:
                DeleteFile();
                break;
            case 7:
                DeleteFold();
                break;
            case 8:
                BitMap();
                break;
            case 9:
                cout << "Exiting the system." << endl;
                exitRequested = true;
                system("pause");
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        tester++;  //用于测试
    } while (choice != 1 && choice != 9);
}



bool MainOperation::SelectFile(CFile* &file, const string& operation) {
    TargetFile = nullptr;
    cout << "Which file do you want to " << operation << ": " << endl;
    OutputAllFiles();
    cout << "Your choice: ";
    int choice = 0;
    cin >> choice;
    //cin.ignore(); // 忽略换行符
    //system("pause");
    if (choice < 1 || choice > g_nowfold->file.size()) {
        cout << "Invalid choice." << endl;
        return false;
    }

    file = g_nowfold->file[choice - 1];

    if (CurrentUser.ID != file->owner) {
        if (!file->isotherread) {
            cout << "You don't have the access to " << operation << " this file." << endl;
            return false;
        }
    }
    return true;
}

void MainOperation::OutputAllFolds(){
    //cout<<"son folds: "<<endl;
    for(int i = 0; i<g_nowfold->childfold.size(); i++){  //输出当前文件夹内的所有文件夹
        cout<<i+1<<"."<<g_nowfold->childfold[i]->name<<endl;
    }
}

void MainOperation::OutputAllFiles(){
    cout<<"files: "<<endl;
    for(int i = 0; i<g_nowfold->file.size(); i++){  //输出当前文件夹内的所有文件
        cout<<i+1<<"."<<g_nowfold->file[i]->name<<endl;   //错误，file[i]->name没有东西 推测：g_nowfold不在当前文件夹
    }
}

void MainOperation::BitMap() {
    BlockBitmap.DrawBitmap();
}

bool MainOperation::isExitRequested() const {
    return false;
}

#endif