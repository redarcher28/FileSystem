
#include <iostream>
#include<vector>

#include "../head/MainOperation.h"
#include "../head/init.h"
CDisk disk[MAXDISK];
int disk_pointer;
CFold* g_nowfold = &disk[1].Fold;
bool g_usedlogo[MAXDISK];
CFilePhysicalControl Control;
UserList userList;
User currentUser;
Bitmap BlockBitmap;
MainOperation mainOperation;

int main() {
    init();
    while (true) {
        mainOperation.Login();
        if (mainOperation.isExitRequested()) {
            break; // 退出系统
        }
        mainOperation.Menu();
    }
    return 0;
}
